#pragma once
#ifndef CAUCHYPROBLEMMETHOD_H_
#define CAUCHYPROBLEMMETHOD_H_

#include <functional>
typedef std::function <double(const double&, const double&)> FuncCauchy;

namespace MiptNet1DModel {
	class CauchyProblemMethod {
	public:
		CauchyProblemMethod(FuncCauchy func) : func_(func) { }
		virtual ~CauchyProblemMethod() { }

		virtual double Solve(const double t, const double u, const double dt) = 0;

	protected:
		FuncCauchy func_;
	};
}

#endif /*CAUCHYPROBLEMMETHOD_H_*/