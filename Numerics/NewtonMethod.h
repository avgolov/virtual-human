#pragma once

#ifndef NUMERICSNEWTONMETHOD_H_
#define NUMERICSNEWTONMETHOD_H_

#include <Eigen\Dense>
#include <functional>

namespace Numerics {

	template<class Vector, class Matrix>
	class ISetEquations {

	public:
		virtual ~ISetEquations() {	}

		virtual Vector f(const Vector& u) = 0;
		virtual Matrix yacobian_f(const Vector& u) = 0;
	};

	template<class Vector, class Matrix>
	class BaseSetEquations : public ISetEquations<Vector, Matrix> {

	public:
		BaseSetEquations(
			std::function<Vector(const Vector&)>* f, 
			std::function<Matrix(const Vector&)>* df): f_(f), df_(df) { }

		Vector f(const Vector& u) override { return (*f_)(u); }
		Matrix yacobian_f(const Vector& u) override { return (*df_)(u); }

	private:
		std::function<Vector(const Vector&)>* f_;
		std::function<Matrix(const Vector&)>* df_;
		
	};

	template<class Vector, class Matrix>
	class NewtonMethod {

	public:
		NewtonMethod(ISetEquations<Vector, Matrix>* set_equations) : set_eq_(set_equations) { }

		bool Solve(Vector& u_0, double epsilon) {
			//auto cOld = u_0.coeffRef(4);
			int iteration = 0;
			Vector u_s(u_0.size());
			auto  norm0 = u_0.norm();
			do {
				if (iteration == max_iteration_) break;
				/*auto f_norm = set_eq_->f(u_0).norm();
				auto df = set_eq_->yacobian_f(u_0).norm();
				auto df_inv = set_eq_->yacobian_f(u_0).inverse().norm();*/
				u_s = set_eq_->yacobian_f(u_0).inverse() * set_eq_->f(u_0);
				auto c1 = u_0.coeffRef(0);
				auto c2 = u_0.coeffRef(1);
				auto c3 = u_0.coeffRef(2);
				auto c4 = u_0.coeffRef(3);
				auto c5 = u_0.coeffRef(4);
				auto us_norm = u_s.norm();
				u_0 -= u_s;
				/*c1 = u_0.coeffRef(0);
				c2 = u_0.coeffRef(1);
				c3 = u_0.coeffRef(2);
				c4 = u_0.coeffRef(3);
				c5 = u_0.coeffRef(4);
				for(auto i=0;i<5;i++)
				{
					u_0.coeffRef(i) = abs(u_0.coeffRef(i));
				}*/
				auto u0_norm = u_0.norm();
				iteration++;

			} while (u_s.norm() / norm0 > epsilon);

			//auto c = u_0.coeffRef(0);
			if (iteration == max_iteration_) {
				return false;
			}

			return true;
		}

	private:
		ISetEquations<Vector, Matrix>* set_eq_;
		const int max_iteration_ = 1000;
	};
}

#endif /*NUMERICSNEWTONMETHOD_H_*/