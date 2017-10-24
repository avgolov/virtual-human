#pragma once
#ifndef WLLAEQUETION_H_
#define WLLAEQUETION_H_

#include <Eigen/Dense>
#include <vector>
#include "BioChemConst.h"
#include "../../../../Numerics/Ode/IOde.h"

typedef Eigen::VectorXd Vector;
typedef Eigen::MatrixXd Matrix;

namespace SystemModels
{
	class WlLaEquations : public Numerics::IOde {

	public:
		WlLaEquations() {}
		
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
		double UtLa = 0.;
		double Po2 = 0.;
		double LaWsource = 0;
		double LaMin = 0;

	private:
		BioChemConst bcm_;
	};

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//class WlLaEquations : public Numerics::ISetEquations<Vector, Matrix> {

	//public:
	//	WlLaEquations() {}

	//	Vector GetBloodF(const Vector& u);
	//	Matrix GetBloodDf(const Vector& u);

	//	Vector f(const Vector& u) override {
	//		f_ = OldValues - u;
	//		f_ += GetBloodF(u)*Dt;
	//		return f_;
	//	};

	//	Matrix yacobian_f(const Vector& u) override {
	//		yacobian_ = -Matrix::Identity(5, 5);
	//		yacobian_ += GetBloodDf(u)*Dt;
	//		return yacobian_;
	//	}

	//	//{PA, PV, SV, BA, TA}
	//	Eigen::VectorXd OldValues = Eigen::VectorXd::Zero(5);
	//	std::vector<double> Volumes = std::vector<double>(5);
	//	double CardiacOutput = 0.;
	//	double BrainFlow = 0.;
	//	double Dt = 0.;
	//	double UtLa = 0.;
	//	double Po2 = 0.;
	//	double LaWsource = 0;
	//	double LaMin = 0;

	//private:
	//	BioChemConst bcm_;
	//	Vector f_;
	//	Matrix yacobian_;
	//};
}

#endif /*WLLAEQUETION_H_*/
