#include <vector>

//#include "SweepMethod.h"
#include "MassTransferEquation.h"

namespace MiptNet1DModel {
	
	void MassTransferEquation::ComputeMultiKnots(double dt) {
		std::vector<Knot*>* multi_knots = tree_->GetMultiKnots();
		tbb::parallel_for(size_t(0), multi_knots->size(), [&](size_t i)
		{
			auto knot = (*multi_knots)[i];
			//объем, входящего в узел потока
			double su = 0.0;
			const double su_min_vol = 1.e-10;

			for (auto branch : knot->BranchesIn)
			{
				BranchPoint* last = branch->get_last_point();
				double vel = last->Substance.Velocity;
				if (vel > 0.0) {
					su += last->Square * vel;
				}
			} 
			for (auto branch : knot->BranchesOut)
			{
				BranchPoint* first = branch->get_first_point();
				double vel = first->Substance.Velocity;
				if (vel < 0.0) {
					su -= first->Square * vel;
				}
			}
			
			//маленький поток
			if (su < su_min_vol) return;
			
			//Вычисление притока концентрации в узел
			auto csu = std::vector<double>(conc_count_, 0.);

			//Если есть прямой поток на входящей ветви
			for (auto branch : knot->BranchesIn)
			{
				BranchPoint* last = branch->get_last_point();
				double vel = last->Substance.Velocity;
				if (vel > 0.) {
					BranchPoint* second2last = branch->get_second2last_point(); 
					double dx = branch->get_dx();
					for (size_t m = 0; m < conc_count_; m++){
						double grad_conc = (last->SubstancePrev.Conc[m] - second2last->SubstancePrev.Conc[m]) / dx;

						//конвективный поток 
						last->Substance.Conc[m] -= vel * dt * grad_conc;
						csu[m] += last->Square * vel * last->SubstancePrev.Conc[m];

						////диффузионный поток
						//last->Substance.Conc += last->Substance.D_conc * time_step_->dt * grad_conc;
						//csu -= last->Square*last->Substance.D_conc* grad_conc;
					}
				}
			}

			//Если есть обратный поток на исходящей ветви
			for (auto branch : knot->BranchesOut)
			{
				BranchPoint* first = branch->get_first_point();
				double vel = first->Substance.Velocity;
				if (vel < 0.) {
					BranchPoint* second = branch->get_second_point();
					double dx = branch->get_dx();
					for (size_t m = 0; m < conc_count_; m++) {
						double grad_conc = (second->SubstancePrev.Conc[m] - first->SubstancePrev.Conc[m]) / dx;

						//конвективный поток 
						first->Substance.Conc[m] -= vel * dt * grad_conc;
						csu[m] -= first->Square * vel * first->SubstancePrev.Conc[m];

						////диффузионный поток
						//first->Substance.Conc += first->Substance.D_conc * time_step_->dt * grad_conc;
						//csu -= first->Square*first->Substance.D_conc* grad_conc;
					}
				}
			}

			//Вычисление концентрации на ветвях, которые уносят вещество из узла
			for (auto branch : knot->BranchesIn)
			{
				BranchPoint* last = branch->get_last_point();
				if (last->Substance.Velocity < 0.0) {
					for (size_t m = 0; m < conc_count_; m++) {
						last->Substance.Conc[m] = csu[m] / su;
					}
				}
			}
			for (auto branch : knot->BranchesOut)
			{
				BranchPoint* first = branch->get_first_point();
				if (first->Substance.Velocity > 0.0) {
					for (size_t m = 0; m < conc_count_; m++) {
						first->Substance.Conc[m] = csu[m] / su;
					}
				}
			}
		});
	}

	void MassTransferEquation::ComputeInternalPoints(double dt) const {
		auto branches = tree_->get_branches();
		tbb::parallel_for(size_t(0), branches->size(), [&](size_t i)
		{
			auto branch = (*branches)[i];
			double dx = branch->get_dx();
			auto points = branch->get_points();

			////Расчет коэф. для алгоритма прогонки
			//std::vector<double> a(points->size());
			//std::vector<double> b(points->size());
			//std::vector<double> c(points->size());
			//std::vector<double> f(points->size());
			//std::vector<double> solution(points->size());

			//double dtdx2 = time_step_->dt / (dx*dx);
			double dtdx = dt / dx;
			//for (size_t pos = 0; pos < points->size(); pos++) {
			//	BranchPoint* point = (*points)[pos];
			//	a[pos] = -point->Substance.D_conc[m_numb] * dtdx2 - 0.5 * point->Substance.Velocity * dtdx;
			//	b[pos] = 1 + 2 * point->Substance.D_conc[m_numb] * dtdx2;
			//	c[pos] = -point->Substance.D_conc[m_numb] * dtdx2 + 0.5 * point->Substance.Velocity * dtdx;
			//	f[pos] = point->Substance.Conc[m_numb];
			//	solution[pos] = point->Substance.Conc[m_numb];
			//}

			////Учет граничных условий
			//a[0] = 0.0;
			//b[0] = -1.0;
			//c[0] = 0.0;

			//Расчет новых значений концентрации методом прогонки
			//SweepMethod method(&a, &b, &c, &f);
			//method.Execute(&solution);

			//Установка новых значений концентрации
			for (size_t m_numb = 0; m_numb < conc_count_; m_numb++) {
				for (size_t pos = 1; pos < points->size() - 1; pos++) {
					double vel = (*points)[pos]->Substance.Velocity;
					if (vel > 0.0) {
						(*points)[pos]->Substance.Conc[m_numb] -= vel * dtdx * ((*points)[pos]->SubstancePrev.Conc[m_numb] - (*points)[pos-1]->SubstancePrev.Conc[m_numb]);
					}
					else {
						(*points)[pos]->Substance.Conc[m_numb] -= vel * dtdx * ((*points)[pos+1]->SubstancePrev.Conc[m_numb] - (*points)[pos]->SubstancePrev.Conc[m_numb]);
					}
				}
			}
			/*for (size_t pos = 1; pos < points->size() - 1; pos++) {
				auto vel = (*points)[pos]->Substance.Velocity;
				auto vel_nei1 = 0.5*((*points)[pos - 1]->Substance.Velocity + vel);
				auto vel_nei2 = 0.5*((*points)[pos + 1]->Substance.Velocity + vel);
				auto s = (*points)[pos]->Square;
				auto s_nei1 = (*points)[pos - 1]->Square;
				auto s_nei2 = (*points)[pos + 1]->Square;
				auto s_prev = (*points)[pos]->SquarePrev;
				auto s_nei1_prev = (*points)[pos - 1]->SquarePrev;
				auto s_nei2_prev = (*points)[pos + 1]->SquarePrev;
				
				for (size_t m_numb = 0; m_numb < conc_count_; m_numb++) {
					auto c = (*points)[pos]->SubstancePrev.Conc[m_numb]*s_prev / s;
					auto c_nei1 = (*points)[pos-1]->SubstancePrev.Conc[m_numb] * s_nei1_prev / s_nei1;
					auto c_nei2 = (*points)[pos+1]->SubstancePrev.Conc[m_numb] * s_nei2_prev / s_nei2;
					if (vel_nei1 >= 0.) {
						(*points)[pos]->Substance.Conc[m_numb] += c_nei1*vel_nei1*dtdx*s_nei1 / s;
						(*points)[pos]->Substance.Conc[m_numb] -= c*vel_nei2*dtdx;
					}
					else if (vel_nei2 <= 0.) {
						(*points)[pos]->Substance.Conc[m_numb] -= c_nei2*vel_nei2*dtdx*s_nei2 / s;
						(*points)[pos]->Substance.Conc[m_numb] += c*vel_nei1*dtdx;
					}
					if ((*points)[pos]->Substance.Conc[m_numb] < 0.)
						(*points)[pos]->Substance.Conc[m_numb] = 0.;
					if ((*points)[pos]->Substance.Conc[m_numb] > 1.)
						(*points)[pos]->Substance.Conc[m_numb] = 1.;
				}
			}*/
		});
	}

	void MassTransferEquation::ModifyConcentration() {
		auto branches = tree_->get_branches();
		tbb::parallel_for(size_t(0), branches->size(), [&](size_t i)
		{
			auto branch = (*branches)[i];
			auto points = branch->get_points(); 
			for (size_t pos = 0; pos < points->size(); pos++)
			{
				auto coef = (*points)[pos]->SquarePrev / (*points)[pos]->Square;
				for (size_t m_numb = 0; m_numb < conc_count_; m_numb++)
				{
					(*points)[pos]->Substance.Conc[m_numb] *= coef;
					(*points)[pos]->SubstancePrev.Conc[m_numb] *= coef;

					if ((*points)[pos]->Substance.Conc[m_numb] < 0.)
						(*points)[pos]->Substance.Conc[m_numb] = 0.;
					else if ((*points)[pos]->Substance.Conc[m_numb] > 1.)
						(*points)[pos]->Substance.Conc[m_numb] = 1.;

					if ((*points)[pos]->SubstancePrev.Conc[m_numb] < 0.)
						(*points)[pos]->SubstancePrev.Conc[m_numb] = 0.;
					else if ((*points)[pos]->SubstancePrev.Conc[m_numb] > 1.)
						(*points)[pos]->SubstancePrev.Conc[m_numb] = 1.;
				}
			}
		});
	}
}