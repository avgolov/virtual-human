#pragma once

#ifndef ODESOLVER_H_
#define ODESOLVER_H_

#include "IOde.h"

namespace Numerics {
	
	class OdeSolver {

	public:
		virtual ~OdeSolver(){}
		virtual void Solve(IOde& ode, Vector& x0, Vector& xn, double t) = 0;
	};

}

#endif /*ODESOLVER_H_*/