#pragma once
#ifndef BBEQUETION_H_
#define BBEQUETION_H_

#include <Eigen/Dense>
#include <vector>
#include "BioChemConst.h"
#include "../../../../Numerics/Ode/IOde.h"

typedef Eigen::VectorXd Vector;
typedef Eigen::MatrixXd Matrix;

namespace SystemModels
{
	class BbEquations : public Numerics::IOde {

	public:
		BbEquations() {}

		Vector GetBloodF(const Vector& u);
		Matrix GetBloodDf(const Vector& u);
		
		Vector f(const Vector& u) override {
			return GetBloodF(u);
		};

		Matrix yacobian(const Vector& u) override {
			return GetBloodDf(u);
		}

		Matrix yacobianDx(const Vector& u, const int& i) override {
			return Matrix::Zero(u.size(), u.size());
		}

		//{PA, PV, SV, BA, TA}
		Eigen::VectorXd OldValues = Eigen::VectorXd::Zero(5);
		std::vector<double> Volumes = std::vector<double>(5);
		double CardiacOutput = 0.;
		double BrainFlow = 0.;
		double Dt = 0.;

	private:
		BioChemConst bcm_;
	};

	//class BbEquations : public Numerics::ISetEquations<Vector, Matrix> {

	//public:
	//	BbEquations() {}

	//	Vector GetBloodF(const Vector& u);
	//	Matrix GetBloodDf(const Vector& u);

	//	Vector f(const Vector& u) override {
	//		Vector result = OldValues - u;
	//		result += GetBloodF(u)*Dt;
	//		return result;
	//	};

	//	Matrix yacobian_f(const Vector& u) override {
	//		Matrix result = -Matrix::Identity(5, 5);
	//		result += GetBloodDf(u)*Dt;
	//		return result;
	//	}

	//	//{PA, PV, SV, BA, TA}
	//	Eigen::VectorXd OldValues = Eigen::VectorXd::Zero(5);
	//	std::vector<double> Volumes = std::vector<double>(5);
	//	double CardiacOutput = 0.;
	//	double BrainFlow = 0.;
	//	double Dt = 0.;

	//private:
	//	BioChemConst bcm_;
	//};
}

#endif /*BBEQUETION_H_*/
