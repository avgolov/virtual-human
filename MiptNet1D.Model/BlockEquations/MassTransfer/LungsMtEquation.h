#pragma once

#ifndef LUNGSMTEQUATION_H_
#define LUNGSMTEQUATION_H_

#include <exception>
#include "MassTransferEquation.h"
#include "../../NetTopology/Model/BronchTree.h"

namespace MiptNet1DModel {

	//Расчет массопереноса в легких
	class LungsMtEquation : public MassTransferEquation {

	public:
		LungsMtEquation(BaseTree* tree) : 
			MassTransferEquation(tree) {
			
			if (!dynamic_cast<BronchTree*>(tree)) {
				throw std::exception("Wrong tree type!");
			}
		}
		LungsMtEquation() {}

		void ComputeNasopharynx(double dt) 
		{
			time_ += dt;
			auto tree = dynamic_cast<BronchTree*>(tree_);

			//Носоглотка
			auto nasopharynx = tree->get_nasopharynx();
			auto point_first = nasopharynx->get_first_point();
			auto point_second = nasopharynx->get_second_point();
			double dx = nasopharynx->get_dx();

			//Конвективный поток
			if (point_first->Substance.Velocity > 0.) {

				std::vector<double> conc_air = { 0., 0. };
				conc_air[0] = 0.209;
				conc_air[1] = 0.07;//(time_ > 250.) ? 0.07 : 2.8e-4;
				//point_first->Substance.Conc = conc_air;
				for (size_t m = 0; m < conc_count_; m++) {
					point_first->Substance.Conc[m] += point_first->Substance.Velocity * (dt / dx) *
						(conc_air[m] - point_first->SubstancePrev.Conc[m]);
				}
			}
			else {
				for (size_t m = 0; m < conc_count_; m++) {
					point_first->Substance.Conc[m] -= point_first->Substance.Velocity * (dt / dx) *
						(point_second->SubstancePrev.Conc[m] - point_first->SubstancePrev.Conc[m]);
				}
			}

			/*std::vector<double> conc_air = { 0., 0. };
			conc_air[0] = 0.209;
			conc_air[1] = 2.8e-4;

			auto vel = point_first->Substance.Velocity;
			auto vel_nei2 = 0.5*(point_second->Substance.Velocity+vel);
			auto s = point_first->Square;
			auto s_nei2 = point_second->Square;
			auto s_prev = point_first->SquarePrev;
			auto s_nei2_prev = point_second->SquarePrev;

			for (size_t m_numb = 0; m_numb < conc_count_; m_numb++) {
			auto c = point_first->SubstancePrev.Conc[m_numb] * s_prev / s;
			auto c_nei2 = point_second->SubstancePrev.Conc[m_numb] * s_nei2_prev / s_nei2;
			if (vel >= 0.) {
			point_first->Substance.Conc[m_numb] += conc_air[m_numb]*vel*dt/dx;
			point_first->Substance.Conc[m_numb] -= c*vel_nei2*dt/dx;
			}
			else if (vel_nei2 <= 0.) {
			point_first->Substance.Conc[m_numb] -= c_nei2*vel_nei2*dt*s_nei2 / (s*dx);
			point_first->Substance.Conc[m_numb] += c*vel*dt/dx;
			}
			if (point_first->Substance.Conc[m_numb] < 0.)
			point_first->Substance.Conc[m_numb] = 0.;
			if (point_first->Substance.Conc[m_numb] > 1.)
			point_first->Substance.Conc[m_numb] = 1.;
			}*/
		}
		void ComputeBoundaryKnots(double dt) override {
			auto tree = dynamic_cast<BronchTree*>(tree_);
			ComputeNasopharynx(dt);

			//Альвеолярный объем
			auto alveoles = tree->get_alveoles();
			tbb::parallel_for(size_t(0), alveoles->size(), [&](size_t i)
			{
				ComputeAlveole((*alveoles)[i], dt);
			});
		}
	
	private:
		void ComputeAlveole(Alveole* alveole, double dt) const	{

			Branch* branch = alveole->get_branch();
			BranchPoint* point = branch->get_last_point();
			BranchPoint* pointPrev = branch->get_second2last_point();
			double dx = branch->get_dx();
			/*auto s = point->Square;
			auto s_prev = point->SquarePrev;
			auto s_nei = pointPrev->Square;
			auto s_nei_prev = pointPrev->SquarePrev;
			auto vel = point->Substance.Velocity;
			auto vel_nei = 0.5*(pointPrev->Substance.Velocity+vel);*/
			/*double vol = point->Substance.Velocity * point->Square * dt;
			for (size_t m = 0; m < conc_count_; m++) {
				auto c = point->SubstancePrev.Conc[m] * s_prev / s;
				auto c_nei = pointPrev->SubstancePrev.Conc[m] * s_nei_prev / s_nei;
				
				if (vol > 0.) {
					point->Substance.Conc[m] -= c*vel*dt/dx;
					point->Substance.Conc[m] += c_nei*vel_nei*s_nei*dt / (s*dx);

					alveole->Substance.Conc[m] = (vol * c +
						alveole->VolumePrev * alveole->SubstancePrev.Conc[m]) /
						alveole->Volume;
				}
				else {
					point->Substance.Conc[m] -= alveole->SubstancePrev.Conc[m] * vol / (s*dx);
					point->Substance.Conc[m] += c*vel_nei*dt / dx;
				}
				if (point->Substance.Conc[m] < 0.)
					point->Substance.Conc[m] = 0.;
				if (point->Substance.Conc[m] > 1.)
					point->Substance.Conc[m] = 1.;
			}*/
			double vol = point->Substance.Velocity * point->Square * dt;
			for (size_t m = 0; m < conc_count_; m++) {
				if (vol > 0.) {
					
					point->Substance.Conc[m] += point->Substance.Velocity * (dt / dx) *
						(pointPrev->SubstancePrev.Conc[m] - point->SubstancePrev.Conc[m]);

					alveole->Substance.Conc[m] = (vol * point->SubstancePrev.Conc[m] +
						alveole->VolumePrev * alveole->SubstancePrev.Conc[m]) /
						alveole->Volume;
				}
				else {
					point->Substance.Conc[m] -= point->Substance.Velocity * (dt / dx) *
						(alveole->SubstancePrev.Conc[m] - point->SubstancePrev.Conc[m]);
				}
			}
		}
	};
}

#endif /*LUNGSMTEQUATION_H_*/