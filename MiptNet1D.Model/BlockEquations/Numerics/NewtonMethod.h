#pragma once

#ifndef NEWTONMETHOD_H_
#define NEWTONMETHOD_H_

#include <Eigen\Dense>

namespace MiptNet1DModel {

	//Система нелинейных уравнений
	class ISetEquations {

	public:
		virtual ~ISetEquations() {	}

		virtual Eigen::VectorXd f(const Eigen::VectorXd& u) = 0; 
		virtual Eigen::MatrixXd yacobian_f(const Eigen::VectorXd& u) = 0;
	};

	//Решение нелинейной системы уравнений методом Ньютона
	class NewtonMethod {

	public:
		NewtonMethod(ISetEquations* set_equations) : set_eq_(set_equations) { }

		bool Solve(Eigen::VectorXd& u_0, double epsilon) {
			const int max_iteration = 1000;

			int iteration = 0;
			Eigen::VectorXd u_s(u_0.size());
			auto  norm0 = u_0.norm();
			do {
				if (iteration == max_iteration) break;
				u_s = set_eq_->yacobian_f(u_0).inverse() * set_eq_->f(u_0);
				u_0 -= u_s;
				iteration++;

			} while (u_s.norm()/ norm0 > epsilon);


			if (iteration == max_iteration) {
				return false;
			}

			return true;
		}

	private:
		ISetEquations* set_eq_;
	};
}

#endif /*NEWTONMETHOD_H_*/