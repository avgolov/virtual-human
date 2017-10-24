#pragma once

#ifndef HYBRIDHYPEQMETHOD_H_
#define HYBRIDHYPEQMETHOD_H_

#include "HyperbolicEquation.h"

namespace MiptNet1DModel {

	//Двухшаговая гибридная схема
	class HybridHypEquation : public HyperbolicEquation {

	public:
		HybridHypEquation() { }

		void set_dx(double dx) { dx_ = dx; }

		std::vector<Eigen::VectorXd> Solve() override {

			std::vector<Eigen::VectorXd> u_new(u_->size());
			std::vector<Eigen::VectorXd> u_tilda(u_->size());
			Predictor(u_tilda);
			Corrector(u_new, u_tilda);

			return u_new;
		};

	private:
		double dx_ = 0.;

		//Наиболее точная монотонная схема первого порядка точности
		void Predictor(std::vector<Eigen::VectorXd>& u_tilda) const {
			double th = 0.5 * time_step_ / dx_;
			for (size_t i = 1; i < u_->size() - 1; i++) {
				u_tilda[i] = (*u_)[i] - th * ((*fu_)[i + 1] - (*fu_)[i - 1]) + (*right_part_)[i-1]*time_step_;
			}
		}

		//Наименне осцилирующая схема второго порядка точности
		void Corrector(std::vector<Eigen::VectorXd>& u_new,
			std::vector<Eigen::VectorXd>& u_tilda) const {

			double th = time_step_ / dx_;
			std::vector<Eigen::MatrixXd> mat;
			for (size_t i = 0; i < u_->size() - 1; i++) {
				mat.push_back((*omega_inverse_)[i] * 0.5 * th *
					(*lambda_)[i].cwiseAbs().asDiagonal() * (*omega_)[i]);
			}

			for (size_t i = 1; i < u_->size() - 1; i++) {
				u_new[i] = u_tilda[i] +
					mat[i] * ((*u_)[i + 1] - (*u_)[i]) -
					mat[i - 1] * ((*u_)[i] - (*u_)[i - 1]);
			}
		}
	};
}

#endif /*HYBRIDHYPEQMETHOD_H_*/
