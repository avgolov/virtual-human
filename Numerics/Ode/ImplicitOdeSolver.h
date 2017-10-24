#pragma once

#ifndef IMPLICITODESOLVER_H_
#define IMPLICITODESOLVER_H_

#include "OdeSolver.h"

namespace Numerics {

	class ImplicitOdeSolver : public OdeSolver {

	public:
		void Solve(IOde& ode, Vector& x0, Vector& xn, double t) override {
			auto maxIter = 100;
			auto eps = 10e-6;
			int iter = 0;
			xn = x0;
			auto diff = x0;
			auto  norm = x0.norm();
			do {
				if (iter == maxIter) break;
				diff = YacobianR(ode, x0, xn, t).inverse() * R(ode, x0, xn, t);
				xn -= diff;
				iter++;

			} while (diff.norm() / norm > eps);
		};
		

		virtual Vector R(IOde& ode, Vector& x0, Vector& xn, double t) { return Vector(); }
		virtual Matrix YacobianR(IOde& ode, Vector& x0, Vector& xn, double t) { return Matrix(); }
	};

}

#endif /*IMPLICITODESOLVER_H_*/
