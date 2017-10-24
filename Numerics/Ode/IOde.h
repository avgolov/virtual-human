#pragma once

#ifndef IODE_H_
#define IODE_H_

#include <Eigen/Dense>

typedef Eigen::VectorXd Vector;
typedef Eigen::MatrixXd Matrix;

namespace Numerics {

	class IOde  {

	public:
		virtual ~IOde(){};
		virtual Vector f(const Vector& u) = 0;
		virtual Matrix yacobian(const Vector& u) = 0;
		virtual Matrix yacobianDx(const Vector& u, const int& i) = 0;
	};

	
}

#endif /*IODE_H_*/