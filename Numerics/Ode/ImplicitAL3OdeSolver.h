#pragma once

#ifndef IMPLICITAL3ODESOLVER_H_
#define IMPLICITAL3ODESOLVER_H_

#include "ImplicitOdeSolver.h"

namespace Numerics {

	class ImplicitAL3OdeSolver : public ImplicitOdeSolver {

	public:
		Vector ImplicitAL3OdeSolver::R(IOde& ode, Vector& x0, Vector& xn, double t) override {
			auto rn = -x0-t*ode.f(x0)/3.;
			return rn + xn - 2.*t*ode.f(xn) / 3. + t*t*ode.yacobian(xn)*ode.f(xn) / 6;
		}

		Matrix ImplicitAL3OdeSolver::YacobianR(IOde& ode, Vector& x0, Vector& xn, double t) override {
			auto fx = ode.yacobian(xn);
			auto size = x0.size();
			auto E = Matrix::Identity(size, size);
			Matrix C = Matrix(size, size);
			Matrix f = ode.f(xn).transpose();
			for(size_t i=0; i<size; i++) {
				C.row(i) = f*ode.yacobianDx(xn, i);
			}
			return E-2.*t*fx/3.+t*t*(fx*fx+C.transpose())/6.;
		}
	};

}

#endif /*IMPLICITEULERODESOLVER_H_*/
