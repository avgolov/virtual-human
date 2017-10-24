#pragma once

#ifndef HYPERBOLICEQMETHOD_H_
#define HYPERBOLICEQMETHOD_H_

#include <vector>
#include <Eigen\Dense>

namespace MiptNet1DModel {

	class HyperbolicEquation {

	public:
		HyperbolicEquation() { }
		virtual ~HyperbolicEquation() {	}

		void set_time_step(double time_step) { time_step_ = time_step; }
		void set_fu(std::vector<Eigen::VectorXd>* fu) { fu_ = fu; }
		void set_u(std::vector<Eigen::VectorXd>* u) { u_ = u; }
		void set_lambda(std::vector<Eigen::VectorXd>* lambda) { lambda_ = lambda; }
		void set_omega(std::vector<Eigen::MatrixXd>* omega) { omega_ = omega; }
		void set_omega_inverse(std::vector<Eigen::MatrixXd>* omega) { omega_inverse_ = omega; }
		void set_right_part(std::vector<Eigen::VectorXd>* right_part) { right_part_ = right_part; }

		//Поиск решения на новом шаге по времени 
		virtual std::vector<Eigen::VectorXd> Solve() = 0;

	protected:
		//Вектор функции F для уравнения в консервативной форме
		std::vector<Eigen::VectorXd>* fu_;

		//Вектор собственных значений
		std::vector<Eigen::VectorXd>* lambda_;

		//Решение на текущем шаге по времени
		std::vector<Eigen::VectorXd>* u_;

		//Матрица левых собственных векторов
		std::vector<Eigen::MatrixXd>* omega_;

		//Обратная матрица левых собственных векторов
		std::vector<Eigen::MatrixXd>* omega_inverse_;

		std::vector<Eigen::VectorXd>* right_part_;

		//Шаг по времени
		double time_step_;
	};
}

#endif /*HYPERBOLICEQMETHOD_H_*/
