#pragma once
#ifndef WORKO2HBEQUETION_H_
#define WORKO2HBEQUETION_H_

#include <Eigen/Dense>
#include <vector>
#include "BioChemConst.h"
#include "../../../../Numerics/Ode/IOde.h"

typedef Eigen::VectorXd Vector;
typedef Eigen::MatrixXd Matrix;

namespace SystemModels
{
	class WlO2HbEquations : public Numerics::IOde {

	public:
		WlO2HbEquations() {}

		Vector GetTissueF(const Vector& u);
		Matrix GetTissueDf(const Vector& u);
		Matrix GetTissueDfdx(const Vector& u, const int& i);
		Vector GetLungsF(const Vector& u);
		Matrix GetLungsDf(const Vector& u);
		Matrix GetLungsDfdx(const Vector& u, const int& i);
		Vector GetBloodF(const Vector& u);
		Matrix GetBloodDf(const Vector& u);
		Matrix GetBloodDfdx(const Vector& u, const int& i);

		Vector f(const Vector& u) override {
			return GetTissueF(u) + GetBloodF(u) + GetLungsF(u);
		};

		Matrix yacobian(const Vector& u) override {
			return GetTissueDf(u) + GetBloodDf(u) + GetLungsDf(u);
		}

		Matrix yacobianDx(const Vector& u, const int& i) override {
			return GetTissueDfdx(u, i)+GetLungsDfdx(u,i)+GetBloodDfdx(u,i);
		}

		//{PA, PV, SV, BA, TA}
		Vector f_;
		Matrix yacobian_;
		Eigen::VectorXd OldValues = Eigen::VectorXd::Zero(5);
		std::vector<double> Volumes = std::vector<double>(5);
		double TissueUptake = 0.;
		double BrainUptake = 0.;
		double CardiacOutput = 0.;
		double BrainFlow = 0.;
		double Po2AlvLung = 0.;
		double Dt = 0.;

	private:
		BioChemConst bcm_;
		std::vector<double> s_ = std::vector<double>(5);
		std::vector<double> ds_ = std::vector<double>(5);
		std::vector<double> d2s_ = std::vector<double>(5);
		std::vector<double> n_ = std::vector<double>(5);
		std::vector<double> dn_ = std::vector<double>(5);
		std::vector<double> d2n_ = std::vector<double>(5);
	};

	//class WlO2HbEquations : public Numerics::ISetEquations<Vector, Matrix> {

	//public:
	//	WlO2HbEquations() {}

	//	Vector GetTissueF(const Vector& u);
	//	Matrix GetTissueDf(const Vector& u);
	//	Vector GetLungsF(const Vector& u);
	//	Matrix GetLungsDf(const Vector& u);
	//	Vector GetBloodF(const Vector& u);
	//	Matrix GetBloodDf(const Vector& u);

	//	Vector f(const Vector& u) override {
	//		f_ = OldValues - u;
	//		f_ += (GetTissueF(u) + GetBloodF(u) + GetLungsF(u))*Dt;
	//		return f_;
	//	};

	//	Matrix yacobian_f(const Vector& u) override {
	//		yacobian_ = -Matrix::Identity(5, 5);
	//		yacobian_ += (GetTissueDf(u) + GetBloodDf(u) + GetLungsDf(u))*Dt;
	//		return yacobian_;
	//	}

	//	//{PA, PV, SV, BA, TA}
	//	Vector f_;
	//	Matrix yacobian_;
	//	Eigen::VectorXd OldValues = Eigen::VectorXd::Zero(5);
	//	std::vector<double> Volumes = std::vector<double>(5);
	//	double TissueUptake = 0.;
	//	double BrainUptake = 0.;
	//	double CardiacOutput = 0.;
	//	double BrainFlow = 0.;
	//	double Po2AlvLung = 0.;
	//	double Dt = 0.;

	//private:
	//	BioChemConst bcm_;
	//	std::vector<double> s_ = std::vector<double>(5);
	//	std::vector<double> ds_ = std::vector<double>(5);
	//	std::vector<double> n_ = std::vector<double>(5);
	//	std::vector<double> dn_ = std::vector<double>(5);
	//};
}

#endif /*WORKO2HBEQUETION_H_*/
