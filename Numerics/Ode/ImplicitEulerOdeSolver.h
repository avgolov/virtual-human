#pragma once

#ifndef IMPLICITEULERODESOLVER_H_
#define IMPLICITEULERODESOLVER_H_

#include "ImplicitOdeSolver.h"

namespace Numerics {

	class ImplicitEulerOdeSolver: public ImplicitOdeSolver {

	public:
		Vector ImplicitEulerOdeSolver::R(IOde& ode, Vector& x0, Vector& xn, double t) override{
			return -xn + x0 + t*ode.f(xn);
		}

		Matrix ImplicitEulerOdeSolver::YacobianR(IOde& ode, Vector& x0, Vector& xn, double t) override{
			return -Matrix::Identity(x0.size(), x0.size()) + t*ode.yacobian(xn);
		}
	};

}

#endif /*IMPLICITEULERODESOLVER_H_*/