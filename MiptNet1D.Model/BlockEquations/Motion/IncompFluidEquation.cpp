#include <math.h>
#include <iostream>

#include "IncompFluidEquation.h"
#include "../Numerics/NewtonMethod.h"
#include "../Numerics/HybridHypEquation.h"

namespace MiptNet1DModel {

	double IncompFluidEquation::ComputeTimeStep() {

		auto branches = tree_->get_branches();
		std::vector<double> s(branches->size());
		tbb::parallel_for(size_t(0), branches->size(), [&](size_t i)
		{
			auto branch = (*branches)[i];
			double dx = branch->get_dx();
			double s_max_temp = 0;
			for (auto point : *branch->get_points()) {
				double val1 = std::abs(point->Substance.Velocity + point->Cwall);
				double val2 = std::abs(point->Substance.Velocity - point->Cwall);
				double value = (val1 > val2) ? val1 : val2;
				value /= dx;
				s_max_temp = (s_max_temp > value) ? s_max_temp : value;
			}
			s[i] = s_max_temp;
		});
		double s_max = *max_element(s.begin(), s.end());
		return 0.9 / s_max;
		/*time_step_->dtPrev = time_step_->dt;
		time_step_->dt = time_step_->cfl / s_max;
		time_step_->time_curr += time_step_->dt;
		time_step_->step++;*/
	}
	
	void IncompFluidEquation::ComputeInternalPoints(double dt) {
		
		auto branches = tree_->get_branches();
		tbb::parallel_for(size_t(0), branches->size(), [&](size_t i) {
			auto branch = (*branches)[i];
			HybridHypEquation method;
			method.set_dx(branch->get_dx());
			method.set_time_step(dt);

			auto points = branch->get_points();
			std::vector<Eigen::VectorXd> fu;
			std::vector<Eigen::VectorXd> lambda;
			std::vector<Eigen::VectorXd> u;
			std::vector<Eigen::VectorXd> right_part;
			std::vector<Eigen::MatrixXd> omega;
			std::vector<Eigen::MatrixXd> omega_inverse;
			double s0 = points->front()->SquareInit;
			double c_wall = points->front()->Cwall;
			int index = 0;
			for (size_t k = 0; k < points->size();k++)
			{
				auto point = (*points)[k];
				double s = point->SquarePrev;
				double v = point->SubstancePrev.Velocity;
				Eigen::VectorXd u_k(2);
				u_k << s, v;
				u.push_back(u_k);
				if(s!=s || s<0.)
				{
					std::cout << "BAD S = " << s << std::endl;
					//std::cout << "Time = " << time_step_->time_curr << std::endl;
					std::cout << "i = " << i << std::endl;
					std::cout << "k = " << k << std::endl;
					std::cout << "size_k = " << points->size() << std::endl;
				}
				Eigen::VectorXd fu_k(2);
				fu_k << s * v, 0.5 * std::pow(v, 2) + state_.P_ro(point, s);
				fu.push_back(fu_k);

				index++;
			}
			
			for (size_t i1 = 1; i1 < u.size(); i1++)
			{
				Eigen::VectorXd u_k = 0.5 * (u[i1 - 1] + u[i1]);

				double sonic_speed = c_wall * std::sqrt(u_k.coeff(0) / s0);
				Eigen::VectorXd lambda_k(2);
				lambda_k << u_k.coeff(1) - sonic_speed, u_k.coeff(1) + sonic_speed;
				lambda.push_back(lambda_k);

				Eigen::MatrixXd omega_k(2, 2);
				omega_k << sonic_speed / u_k.coeff(0), -1.0,
					       sonic_speed / u_k.coeff(0), 1.0;
				omega.push_back(omega_k);

				Eigen::MatrixXd omega_k_inverse(2, 2);
				omega_k_inverse << 0.5*u_k.coeff(0) / sonic_speed, 0.5*u_k.coeff(0) / sonic_speed,
					                              -0.5           ,                      0.5;
				omega_inverse.push_back(omega_k_inverse);
				
				auto point = (*points)[i1];
				right_part.push_back(GetRightPart(point, &u_k.data()[0], &u_k.data()[1]));
			}
			method.set_u(&u);
			method.set_fu(&fu);
			method.set_lambda(&lambda);
			method.set_omega(&omega);
			method.set_omega_inverse(&omega_inverse);
			method.set_right_part(&right_part);
			auto solution = method.Solve();
			for (size_t pos = 1; pos < points->size() - 1; pos++)
			{
				(*points)[pos]->Square = solution[pos][0];
				/*(*points)[pos]->Square = ((*points)[pos]->Square < 1.05*(*points)[pos]->SquareInit) ? (*points)[pos]->Square : 1.05*(*points)[pos]->SquareInit;
				(*points)[pos]->Square = ((*points)[pos]->Square > 0.95*(*points)[pos]->SquareInit) ? (*points)[pos]->Square : 0.95*(*points)[pos]->SquareInit;*/
				(*points)[pos]->Substance.Velocity = solution[pos][1];
				if((*points)[pos]->Square < 0.)
				{
					std::cout << "Bad solution s=" << solution[pos][0] << std::endl;
					std::cout << "Size = " << points->size() << std::endl;
					std::cout << "Pos = " << pos << std::endl;
					int n;
					std::cin >> n;
				}
			}
		});
	}

	void IncompFluidEquation::ComputeMultiKnots(double dt) {
		auto multi_knots = tree_->GetMultiKnots();
		tbb::parallel_for(size_t(0), multi_knots->size(), [&](size_t i)
		{
			auto knot = (*multi_knots)[i];
			//Расчет коэф. U = alpha*S + beta
			std::vector<double> alpha(knot->BranchesIn.size() + knot->BranchesOut.size());
			std::vector<double> beta(alpha.size());
			Eigen::VectorXd s(alpha.size());
			int index = 0;
			for (auto branch : knot->BranchesIn)
			{
				InCompatibilityCoef(branch, alpha[index], beta[index], dt);
				s.coeffRef(index) = branch->get_last_point()->SquarePrev;
				index++;
			}
			for (auto branch : knot->BranchesOut)
			{
				OutCompatibilityCoef(branch, alpha[index], beta[index], dt);
				s.coeffRef(index) = branch->get_first_point()->SquarePrev;
				index++;
			}

			//Система нелинейных уравнений относительно площади сечения
			FluidKnotsSetEquations set_eq(&alpha, &beta, knot);
			NewtonMethod method(&set_eq);
			//bool good_solution = method.Solve(s, 1.e-6);
			method.Solve(s, 1.e-6);
			/*if (!good_solution) {
				SetBadMultiKnot(knot);
				return;
			}*/

			index = 0;
			for (auto branch : knot->BranchesIn)
			{
				BranchPoint* point = branch->get_last_point();
				point->Square = s.coeff(index);
				point->Substance.Velocity = alpha[index] * s.coeff(index) + beta[index];
				point->Substance.Pressure = state_.Pressure(point);
				if (point->Square != point->Square || point->Square<0.)
				{
					int n;
					std::cout << "Square = " << point->Square << std::endl;
					std::cout << "Vel = " << point->Substance.Velocity << std::endl;
					std::cin >> n;
				}
				index++;
			}
			for (auto branch : knot->BranchesOut)
			{
				BranchPoint* point = branch->get_first_point();
				point->Square = s.coeff(index);
				point->Substance.Velocity = alpha[index] * s.coeff(index) + beta[index];
				point->Substance.Pressure = state_.Pressure(point);
				if (point->Square != point->Square || point->Square<0.)
				{
					int n;
					std::cout <<"Square = "<< point->Square << std::endl;
					std::cout << "Vel = " << point->Substance.Velocity << std::endl;
					std::cin >> n;
				}
				index++;
			}

			//CheckMultiKnot(knot, alpha, beta, s);
		});
	}

	void IncompFluidEquation::InCompatibilityCoef(
		Branch* branch, double& alpha, double& beta, double dt) const {

		BranchPoint* last = branch->get_last_point();
		BranchPoint* second2last = branch->get_second2last_point();
		double s = last->SquarePrev;
		if(s < 0.)
		{
			std::cout << s << " last_id" << std::endl;
		}
		auto right_part = GetRightPart(last, &last->SquarePrev, &last->SubstancePrev.Velocity);
		double sigma = (last->SubstancePrev.Velocity + last->Cwall * 
			std::sqrt(s / last->SquareInit)) * dt / branch->get_dx();
		
		alpha = -last->Cwall / std::sqrt(s * last->SquareInit);
		beta = (last->SubstancePrev.Velocity + sigma*second2last->Substance.Velocity - 
			alpha*(last->SquarePrev + sigma*second2last->Square)+
			dt*(right_part.data()[1]-alpha*right_part.data()[0])) / (1 + sigma);
		if(beta != beta)
		{
			std::cout <<"sigma ="<<sigma << std::endl;
			std::cout << "alpha =" << alpha << std::endl;
			std::cout << "last->SquarePrev =" << last->SquarePrev << std::endl;
			std::cout << "second2last->Square =" << second2last->Square << std::endl;
			std::cout << "second2last->Substance.Velocity =" << second2last->Substance.Velocity << std::endl;
			std::cout << "last->SubstancePrev.Velocity =" << last->SubstancePrev.Velocity << std::endl;
			int n;
			std::cin >> n;
		}
	}

	void  IncompFluidEquation::OutCompatibilityCoef(
		Branch* branch, double& alpha, double& beta, double dt) const	{
		
		BranchPoint* first = branch->get_first_point();
		BranchPoint* second = branch->get_second_point();
		double xi = -first->Cwall / std::sqrt(first->SquarePrev * first->SquareInit);
		double s = first->SquarePrev;
		auto right_part = GetRightPart(first, &first->SquarePrev, &first->SubstancePrev.Velocity);
		double sigma = (first->SubstancePrev.Velocity - first->Cwall *
			std::sqrt(s / first->SquareInit)) * dt / branch->get_dx();
		alpha = -xi;
		beta = (alpha*(sigma*second->Square - first->SquarePrev) + 
			    first->SubstancePrev.Velocity - sigma*second->Substance.Velocity+
			    dt*(right_part.data()[1] - alpha*right_part.data()[0])) / (1 - sigma);

	}

	void IncompFluidEquation::SetBadMultiKnot(Knot* knot) const	{
		double volume = 0.0;
		for (auto branch : knot->BranchesIn)
		{
			BranchPoint* point = branch->get_last_point();
			BranchPoint* point_second = branch->get_second2last_point();
			
			point->Square = 0.5*(point_second->Square + point->SquarePrev);
			point->Substance.Velocity = point_second->Substance.Velocity;
			volume += point->Square * point->Substance.Velocity;
		}

		for (auto branch : knot->BranchesOut)
		{
			BranchPoint* point = branch->get_first_point();
			BranchPoint* point_second = branch->get_second_point();
			point->Square = 0.5*(point_second->Square + point->SquarePrev);
			point->Substance.Velocity = 0.5*volume / point->Square;
		}
	}

	void IncompFluidEquation::CheckMultiKnot(Knot* knot,
		std::vector<double>& alpha, std::vector<double>& beta, Eigen::VectorXd& s) {
		
		bool bad_solution = false;
		double vel_in = 0.0;
		double vel_out = 0.0;

		for (auto branch : knot->BranchesIn)
		{
			BranchPoint* point = branch->get_last_point();
			vel_in += point->Substance.Velocity;
		}
		for (auto branch : knot->BranchesOut)
		{
			BranchPoint* point = branch->get_first_point();
			vel_out += point->Substance.Velocity;
		}
			
		if (vel_in * vel_out < 0) {
			bad_solution = true;
		}

		if (!bad_solution) {
			for (int i = 0; i < s.size(); i++) {
				if (s.coeff(i) <= 0.0){
					bad_solution = true;
					break;
				}
			}
		}

		if (bad_solution) {
			SetBadMultiKnot(knot);
		}
	}

	Eigen::VectorXd IncompFluidEquation::GetRightPart(BranchPoint* point, double* s, double* v) const
	{
		auto s0 = point->SquareInit;
		auto rp = Eigen::VectorXd(2);
		rp.data()[0] = 0.;
		rp.data()[1] = 0.;
		//rp.data()[1] = -4*M_PI*point->Substance.Viscosity*(*v)*(*s/s0 + s0/(*s))/pow(*s,2);
		//rp.data()[1] = -4 * M_PI*point->Substance.Viscosity*(*v);
		return rp;
	}
}