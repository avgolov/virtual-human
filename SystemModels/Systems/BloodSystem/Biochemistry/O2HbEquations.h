#pragma once
#ifndef O2HBEQUETION_H_
#define O2HBEQUETION_H_

#include <Eigen/Dense>
#include "../../../../Numerics/NewtonMethod.h"
#include <vector>
#include "BioChemConst.h"

typedef Eigen::VectorXd Vector;
typedef Eigen::MatrixXd Matrix;

namespace SystemModels
{
	class O2HbEquations : public Numerics::ISetEquations<Vector, Matrix> {

	public:
		O2HbEquations() {}

		Vector GetTissueF(const Vector& u);
		Matrix GetTissueDf(const Vector& u);
		Vector GetLungsF(const Vector& u);
		Matrix GetLungsDf(const Vector& u);
		Vector GetBloodF(const Vector& u);
		Matrix GetBloodDf(const Vector& u);

		Vector f(const Vector& u) override {
			Vector result = OldValues - u;
			result += (GetTissueF(u)+GetBloodF(u) + GetLungsF(u))*Dt;
			return result;
		};

		Matrix yacobian_f(const Vector& u) override {
			Matrix result = -Matrix::Identity(5, 5);
			result += (GetTissueDf(u)+GetBloodDf(u)+GetLungsDf(u))*Dt;
			return result;
		}

		//{PA, PV, SV, BA, TA}
		Eigen::VectorXd OldValues = Eigen::VectorXd::Zero(5);
		std::vector<double> Volumes = std::vector<double>(5);
		double TissueUptake = 0.;
		double BrainUptake = 0.;
		double CardiacOutput = 0.;
		double BrainFlow = 0.;
		double O2AlvLung = 0.;
		double O2AlvBlood = 0.;
		double Dt = 0.;

	private:
		BioChemConst bcm_;
	};
}

#endif /*O2HBEQUETION_H_*/