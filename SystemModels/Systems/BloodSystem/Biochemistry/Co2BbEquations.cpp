#include "Co2BbEquations.h"
#include "BioChemConst.h"

namespace SystemModels
{
	Vector Co2BbEquations::GetTissueF(const Vector& u)
	{
		auto k = bcm_.KCO2Bb;

		Eigen::VectorXd result = Eigen::VectorXd::Zero(5);
		auto val = u.coeffRef(3);
		auto resval = (BrainProd / (bcm_.LitMolCoef*Volumes[3]))*k/(k-Hhco3[3]+2*val);
		result.coeffRef(3) = resval;

		val = u.coeffRef(4);
		resval = (TissueProd/ (bcm_.LitMolCoef*Volumes[4]))*k/(k-Hhco3[4]+2*val);
		result.coeffRef(4) = resval;
		return result;
	}

	Matrix Co2BbEquations::GetTissueDf(const Vector& u)
	{
		auto k = bcm_.KCO2Bb;
		
		Eigen::MatrixXd result = Eigen::MatrixXd::Zero(5, 5);
		auto val = u.coeffRef(3);
		auto resval = -(BrainProd / (bcm_.LitMolCoef*Volumes[3])) * 2 * k / pow(k - Hhco3[3] + 2*val, 2);
		result.coeffRef(3,3) = resval;

		val = u.coeffRef(4);
		resval = -(TissueProd / (bcm_.LitMolCoef*Volumes[4])) * 2 * k / pow(k - Hhco3[4] + 2*val, 2);
		result.coeffRef(4,4) = resval;
		return result;
	}

	Vector Co2BbEquations::GetLungsF(const Vector& u)
	{
		auto k = bcm_.KCO2Bb;
		auto val = u.coeffRef(0);
		auto dP = (Co2AlvLung + Co2AlvBlood*(val*val-Hhco3[0]*val) / (bcm_.SolCoefCo2*k)) / Volumes[0];
		auto resval = dP*k / (k - Hhco3[0] + 2 * val);

		Eigen::VectorXd result = Eigen::VectorXd::Zero(5);
		result.coeffRef(0) = resval;
		return result;
	}

	Matrix Co2BbEquations::GetLungsDf(const Vector& u)
	{
		auto k = bcm_.KCO2Bb;
		auto val = u.coeffRef(3);
		auto resval = -(Co2AlvLung / (bcm_.LitMolCoef*Volumes[0])) * 2 * k / pow(k - Hhco3[0] + 2 * val, 2);

		auto subres = (2 * val - Hhco3[0]) / (k - Hhco3[0] + 2 * val);
		subres += -val*(val - Hhco3[0]) * 2 / pow(k - Hhco3[0] + 2 * val, 2);
		subres *= Co2AlvBlood / (bcm_.LitMolCoef*Volumes[0]);
		resval += subres;

		Eigen::MatrixXd result = Eigen::MatrixXd::Zero(5, 5);
		result.coeffRef(0, 0) = resval;
		return result;
	}

	Vector Co2BbEquations::GetBloodF(const Vector& u)
	{
		auto k = bcm_.KCO2Bb;
		//J – {PA, PV, SV, BA, TA} = {1, 2, 3, 4, 5}
		std::vector<double> s(5);
		std::vector<double> n(5);
		for (auto i = 0; i < 5; i++) {
			auto val = u.coeffRef(i);
			s[i] = (k-Hhco3[i]+2*val)/k;
			n[i] = val*(val-Hhco3[i])/k;
		}

		Eigen::VectorXd result = Eigen::VectorXd::Zero(5);
		result.coeffRef(0) = CardiacOutput*(-n[0] + n[2]) / (Volumes[0] * s[0]);
		result.coeffRef(1) = CardiacOutput*(n[0] - n[1]) / (Volumes[1] * s[1]);
		result.coeffRef(2) = (-CardiacOutput*n[2] + BrainFlow*n[3] +
			(CardiacOutput - BrainFlow)*n[4]) / (Volumes[2] * s[2]);
		result.coeffRef(3) = BrainFlow*(n[1] - n[3]) / (Volumes[3] * s[3]);
		result.coeffRef(4) = (CardiacOutput - BrainFlow)*(n[1] - n[4]) / (Volumes[4] * s[4]);
		return result;
	}

	Matrix Co2BbEquations::GetBloodDf(const Vector& u)
	{
		auto k = bcm_.KCO2Bb;
		std::vector<double> s(5);
		std::vector<double> ds(5);
		std::vector<double> n(5);
		std::vector<double> dn(5);
		for (auto i = 0; i < 5; i++) {
			auto val = u.coeffRef(i);
			s[i] = (k - Hhco3[i] + 2. * val) / k;
			n[i] = val*(val - Hhco3[i]) / k;
			ds[i] = 2./k;
			dn[i] = (2.*val-Hhco3[i]) / k;
		}
		Eigen::MatrixXd result = Eigen::MatrixXd::Zero(5, 5);
		result.coeffRef(0, 0) = CardiacOutput*(-n[2] * ds[0] / pow(s[0], 2) -
			dn[0] / s[0] + n[0] * ds[0] / pow(s[0], 2)) / Volumes[0];
		result.coeffRef(0, 2) = CardiacOutput*dn[2] / (s[0] * Volumes[0]);

		result.coeffRef(1, 1) = CardiacOutput*(-n[0] * ds[1] / pow(s[1], 2) -
			dn[1] / s[1] + n[1] * ds[1] / pow(s[1], 2)) / Volumes[1];
		result.coeffRef(1, 0) = CardiacOutput*dn[0] / (s[1] * Volumes[1]);

		result.coeffRef(2, 2) = (-(BrainFlow*n[3] + (CardiacOutput - BrainFlow)*n[4])*ds[2] / pow(s[2], 2) +
			CardiacOutput*(-dn[2] / s[2] + n[2] / pow(s[2], 2))) / Volumes[2];

		result.coeffRef(2, 3) = BrainFlow*dn[3] / (s[2] * Volumes[2]);
		result.coeffRef(2, 4) = (CardiacOutput - BrainFlow)*dn[4] / (s[2] * Volumes[2]);

		result.coeffRef(3, 3) = BrainFlow*(-n[1] * ds[3] / pow(s[3], 2) -
			dn[3] / s[3] + n[3] * ds[3] / pow(s[3], 2)) / Volumes[3];
		result.coeffRef(3, 1) = BrainFlow*dn[1] / (s[3] * Volumes[3]);

		result.coeffRef(4, 4) = (CardiacOutput - BrainFlow)*(-n[1] * ds[4] / pow(s[4], 2) -
			dn[4] / s[4] + n[4] * ds[4] / pow(s[4], 2)) / Volumes[4];
		result.coeffRef(4, 1) = (CardiacOutput - BrainFlow)*dn[1] / (s[4] * Volumes[4]);

		return result;
	}
}