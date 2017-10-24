#pragma once

#ifndef RUNGEKUTTAMETHOD_H_
#define RUNGEKUTTAMETHOD_H_

#include "CauchyProblemMethod.h"

namespace MiptNet1DModel {

	class RungeKutta23Method : public CauchyProblemMethod {
	public:
		RungeKutta23Method(FuncCauchy func) : CauchyProblemMethod(func) { }

		double Solve(const double t, const double u, const double dt) override {
			auto k1 = dt * func_(t, u);
			auto k2 = dt * func_(t + dt, u + k1);
			auto k3 = dt * func_(t + 0.5*dt, u + 0.25*k1 + 0.25*k2);
			auto k4 = dt * func_(t, u + 0.5*k1 + 0.5*k2);
			return u + dt*(k2 + k3 + 4.0*k4) / 6.0;
		}
	};
}

#endif /*RUNGEKUTTAMETHOD_H_*/
