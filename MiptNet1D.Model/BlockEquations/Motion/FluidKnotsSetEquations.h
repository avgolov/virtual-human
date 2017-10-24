#pragma once

#ifndef FLUIDKNOTSSETEQUATIONS_H_
#define FLUIDKNOTSSETEQUATIONS_H_

#include <vector>

#include "FluidStateEquation.h"
#include "../../NetTopology/Model/Knot.h"
#include "../../BlockEquations/Numerics/NewtonMethod.h"

namespace MiptNet1DModel {
	
	//Система нелинейных уравнений в мультиузле для модели несжимаемой жидкости
	//Сумма(Sk*Uk) = 0 - сохранение массы; 
	//0.5*Uk^2 + Pk/ro = const - уравнение Бернулли
	//U = alpha * S + beta - из условия на характеристике
	class FluidKnotsSetEquations : public ISetEquations {

	public:
		FluidKnotsSetEquations(std::vector<double>* alpha,
			                   std::vector<double>* beta,
							   Knot* knot) : 
							   knot_(knot), 
							   alpha_(alpha), 
							   beta_(beta) { 
			
			e_ = std::vector<double>(alpha->size());
			int index = 0;
			for (auto branch : knot_->BranchesIn)
			{
				e_[index] = 1.0;
			}
			for (auto branch : knot_->BranchesOut)
			{
				e_[index] = -1.0;
			}
		}

		Eigen::VectorXd f(const Eigen::VectorXd& u) override {
			double alpha_prod = 1;
			for (size_t i = 0; i < alpha_->size(); i++){
				alpha_prod *= (*alpha_)[i];
			}

			int index = 0;
			double numerator = 0.0;
			double denominator = 0.0;
			
			//Расчет значения bernulli = 0.5*u_i^2 + P_i/ro_i 
			for (auto branch : knot_->BranchesIn)
			{
				BranchPoint* point = branch->get_last_point(); 
				double alpha = (*alpha_)[index];
				double beta = (*beta_)[index];
				double a_coef = alpha_prod / alpha;
				denominator += a_coef;
				numerator += a_coef * (alpha * beta * u[index] +
					std::pow(beta, 2) + 2 * state_.P_ro(point, u[index]));
				index++;
			}

			for (auto branch : knot_->BranchesOut)
			{
				BranchPoint* point = branch->get_first_point();
				double alpha = (*alpha_)[index];
				double beta = (*beta_)[index];
				double a_coef = alpha_prod / alpha;
				denominator -= a_coef;
				numerator -= a_coef * (alpha * beta * u[index] +
					std::pow(beta, 2) + 2*state_.P_ro(point, u[index]));
				index++;
			}
			denominator *= 2.0;

			//Задание системы 0.5u^2 + P/ro - bernulli = 0
			Eigen::VectorXd result(u.size());
			double num_den = numerator / denominator;
			index = 0;
			for (auto branch : knot_->BranchesIn)
			{
				BranchPoint* point = branch->get_last_point();
				double alpha = (*alpha_)[index];
				double beta = (*beta_)[index];
				result.coeffRef(index) = state_.P_ro(point, u[index]) +
					0.5 * std::pow(alpha * u[index] + beta, 2) - num_den;
				index++;
			}
			for (auto branch : knot_->BranchesOut)
			{
				BranchPoint* point = branch->get_first_point();
				double alpha = (*alpha_)[index];
				double beta = (*beta_)[index];
				result.coeffRef(index) = state_.P_ro(point, u[index]) +
					0.5 * std::pow(alpha * u[index] + beta, 2) - num_den;
				index++;
			}
			return result;
		};

		Eigen::MatrixXd yacobian_f(const Eigen::VectorXd& u) override {
			double alpha_prod = 1.;
			for (size_t i = 0; i < alpha_->size(); i++){
				alpha_prod *= (*alpha_)[i];
			}
			
			double denominator = 0.0;

			for (size_t i = 0; i < e_.size(); i++) {
				denominator += e_[i] * alpha_prod / (*alpha_)[i];
			}
			denominator *= 2.0;

			Eigen::MatrixXd yacobian = Eigen::MatrixXd::Zero(u.size(), u.size());
			double a_den = alpha_prod / denominator;
			int index = 0;
			for (auto branch : knot_->BranchesIn)
			{
				BranchPoint* point = branch->get_last_point();
				double alpha = (*alpha_)[index];
				double beta = (*beta_)[index];
				double value = -(alpha * beta + 2 * state_.Ds_p_ro(point)) * a_den / alpha;

				for (int i = 0; i < u.size(); i++) {
					yacobian.coeffRef(index, i) += value;
				}
				yacobian.coeffRef(index, index) += u[index] * std::pow(alpha, 2) +
					                               alpha * beta + state_.Ds_p_ro(point);
				index++;
			}

			for (auto branch : knot_->BranchesOut)
			{
				BranchPoint* point = branch->get_first_point();
				double alpha = (*alpha_)[index];
				double beta = (*beta_)[index];
				double value = (alpha * beta + 2 * state_.Ds_p_ro(point)) * a_den / alpha;

				for (int i = 0; i < u.size(); i++) {
					yacobian.coeffRef(index, i) += value;
				}

				yacobian.coeffRef(index, index) += u[index] * std::pow(alpha, 2) +
					                               alpha * beta + state_.Ds_p_ro(point);
				index++;
			}

			return yacobian;
		}

	private:
		Knot* knot_;
		FluidStateEquation state_;

		//U = alpha*S + beta 
		std::vector<double>* alpha_;
		std::vector<double>* beta_;
		std::vector<double> e_;
	};
}
#endif /*FLUIDKNOTSSETEQUATIONS_H_*/