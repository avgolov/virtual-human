#include "WlCo2BbEquations.h"
#include "BioChemConst.h"

namespace SystemModels
{
	Vector WlCo2BbEquations::GetTissueF(const Vector& u)
	{
		auto k = bcm_.KCO2Bb;

		Eigen::VectorXd result = Eigen::VectorXd::Zero(5);
		auto val = u.coeffRef(3);
		auto resval = (BrainProd / (bcm_.LitMolCoef*Volumes[3]))*k / (k - Hhco3[3] + 2 * val);
		result.coeffRef(3) = resval;

		val = u.coeffRef(4);
		resval = (TissueProd / (bcm_.LitMolCoef*Volumes[4]))*k / (k - Hhco3[4] + 2 * val);
		result.coeffRef(4) = resval;
		return result;
	}
	Matrix WlCo2BbEquations::GetTissueDf(const Vector& u)
	{
		auto k = bcm_.KCO2Bb;

		Eigen::MatrixXd result = Eigen::MatrixXd::Zero(5, 5);
		auto val = u.coeffRef(3);
		auto resval = -(BrainProd / (bcm_.LitMolCoef*Volumes[3])) * 2 * k / pow(k - Hhco3[3] + 2 * val, 2);
		result.coeffRef(3, 3) = resval;

		val = u.coeffRef(4);
		resval = -(TissueProd / (bcm_.LitMolCoef*Volumes[4])) * 2 * k / pow(k - Hhco3[4] + 2 * val, 2);
		result.coeffRef(4, 4) = resval;
		return result;
	}
	Matrix WlCo2BbEquations::GetTissueDfdx(const Vector& u, const int& i)
	{
		Eigen::MatrixXd result = Eigen::MatrixXd::Zero(5, 5);
		if (i != 3 && i != 4) return result;

		auto k = bcm_.KCO2Bb;
		if(i == 3) {
			auto val = u.coeffRef(3);
			auto resval = (BrainProd / (bcm_.LitMolCoef*Volumes[3])) * 8 * k / pow(k - Hhco3[3] + 2 * val, 3);
			result.coeffRef(3, 3) = resval;
		}
		else if(i == 4) {
			auto val = u.coeffRef(4);
			auto resval = (TissueProd / (bcm_.LitMolCoef*Volumes[4])) * 8 * k / pow(k - Hhco3[4] + 2 * val, 3);
			result.coeffRef(4, 4) = resval;
		}

		return result;
	}

	Vector WlCo2BbEquations::GetLungsF(const Vector& u)
	{
		auto k = bcm_.KCO2Bb;
		auto val = u.coeffRef(0);
		auto dP = bcm_.Dco2Cf*(Pco2AlvLung -(val*val - Hhco3[0] * val) / (bcm_.SolCoefCo2*k)) / (Volumes[0]*bcm_.LitMolCoef);
		auto resval = dP*k / (k - Hhco3[0] + 2 * val);

		Eigen::VectorXd result = Eigen::VectorXd::Zero(5);
		result.coeffRef(0) = resval;
		return result;
	}
	Matrix WlCo2BbEquations::GetLungsDf(const Vector& u)
	{
		auto k = bcm_.KCO2Bb;
		auto val = u.coeffRef(0);
		auto resval = -(bcm_.Dco2Cf*Pco2AlvLung / Volumes[0]) * 2 * k / pow(k - Hhco3[0] + 2 * val, 2);

		auto subres = (2 * val - Hhco3[0]) / (k - Hhco3[0] + 2 * val);
		subres += -val*(val - Hhco3[0]) * 2 / pow(k - Hhco3[0] + 2 * val, 2);
		subres *= -bcm_.Dco2Cf / (bcm_.SolCoefCo2*Volumes[0]);
		resval += subres;
		resval /= bcm_.LitMolCoef;

		Eigen::MatrixXd result = Eigen::MatrixXd::Zero(5, 5);
		result.coeffRef(0, 0) = resval;
		return result;
	}
	Matrix WlCo2BbEquations::GetLungsDfdx(const Vector& u, const int& i)
	{
		Eigen::MatrixXd result = Eigen::MatrixXd::Zero(5, 5);
		if (i != 0) return result;

		auto k = bcm_.KCO2Bb;
		auto val = u.coeffRef(0);
		auto g = k - Hhco3[0] + 2 * val;
		auto resval = (bcm_.Dco2Cf*Pco2AlvLung / Volumes[0])*8.*k/pow(g,3);

		auto subres = 2. / g - 4 * (2 * val - Hhco3[0]) / pow(g, 2) + 
			8 * val*(val - Hhco3[0]) / pow(g, 3);
		subres *= -bcm_.Dco2Cf / (bcm_.SolCoefCo2*Volumes[0]);
		resval += subres;
		resval /= bcm_.LitMolCoef;
		
		result.coeffRef(0, 0) = resval;
		return result;
	}

	Vector WlCo2BbEquations::GetBloodF(const Vector& u)
	{
		auto k = bcm_.KCO2Bb;
		//J – {PA, PV, SV, BA, TA} = {1, 2, 3, 4, 5}
		for (auto i = 0; i < 5; i++) {
			auto val = u.coeffRef(i);
			s_[i] = (k - Hhco3[i] + 2 * val) / k;
			n_[i] = val*(val - Hhco3[i]) / k;
		}

		Eigen::VectorXd result = Eigen::VectorXd::Zero(5);
		result.coeffRef(0) = CardiacOutput*(-n_[0] + n_[2]) / (Volumes[0] * s_[0]);
		result.coeffRef(1) = CardiacOutput*(n_[0] - n_[1]) / (Volumes[1] * s_[1]);
		result.coeffRef(2) = (-CardiacOutput*n_[2] + BrainFlow*n_[3] +
			                 (CardiacOutput - BrainFlow)*n_[4]) / (Volumes[2] * s_[2]);
		result.coeffRef(3) = BrainFlow*(n_[1] - n_[3]) / (Volumes[3] * s_[3]);
		result.coeffRef(4) = (CardiacOutput - BrainFlow)*(n_[1] - n_[4]) / (Volumes[4] * s_[4]);
		return result;
	}

	Matrix WlCo2BbEquations::GetBloodDf(const Vector& u)
	{
		auto k = bcm_.KCO2Bb;
		for (auto i = 0; i < 5; i++) {
			auto val = u.coeffRef(i);
			s_[i] = (k - Hhco3[i] + 2. * val) / k;
			n_[i] = val*(val - Hhco3[i]) / k;
			ds_[i] = 2. / k;
			dn_[i] = (2.*val - Hhco3[i]) / k;
		}
		Eigen::MatrixXd result = Eigen::MatrixXd::Zero(5, 5);
		result.coeffRef(0, 0) = CardiacOutput*(-n_[2] * ds_[0] / pow(s_[0], 2) -
			dn_[0] / s_[0] + n_[0] * ds_[0] / pow(s_[0], 2)) / Volumes[0];
		result.coeffRef(0, 2) = CardiacOutput*dn_[2] / (s_[0] * Volumes[0]);

		result.coeffRef(1, 1) = CardiacOutput*(-n_[0] * ds_[1] / pow(s_[1], 2) -
			dn_[1] / s_[1] + n_[1] * ds_[1] / pow(s_[1], 2)) / Volumes[1];
		result.coeffRef(1, 0) = CardiacOutput*dn_[0] / (s_[1] * Volumes[1]);

		result.coeffRef(2, 2) = (-(BrainFlow*n_[3] + (CardiacOutput - BrainFlow)*n_[4])*ds_[2] / pow(s_[2], 2) +
			CardiacOutput*(-dn_[2] / s_[2] + n_[2] / pow(s_[2], 2))) / Volumes[2];

		result.coeffRef(2, 3) = BrainFlow*dn_[3] / (s_[2] * Volumes[2]);
		result.coeffRef(2, 4) = (CardiacOutput - BrainFlow)*dn_[4] / (s_[2] * Volumes[2]);

		result.coeffRef(3, 3) = BrainFlow*(-n_[1] * ds_[3] / pow(s_[3], 2) -
			dn_[3] / s_[3] + n_[3] * ds_[3] / pow(s_[3], 2)) / Volumes[3];
		result.coeffRef(3, 1) = BrainFlow*dn_[1] / (s_[3] * Volumes[3]);

		result.coeffRef(4, 4) = (CardiacOutput - BrainFlow)*(-n_[1] * ds_[4] / pow(s_[4], 2) -
			dn_[4] / s_[4] + n_[4] * ds_[4] / pow(s_[4], 2)) / Volumes[4];
		result.coeffRef(4, 1) = (CardiacOutput - BrainFlow)*dn_[1] / (s_[4] * Volumes[4]);

		return result;
	}

	Matrix WlCo2BbEquations::GetBloodDfdx(const Vector& u, const int& i) {
		auto k = bcm_.KCO2Bb;
		for (auto j = 0; j < 5; j++) {
			auto val = u.coeffRef(j);
			s_[j] = (k - Hhco3[j] + 2. * val) / k;
			n_[j] = val*(val - Hhco3[j]) / k;
			ds_[j] = 2. / k;
			dn_[j] = (2.*val - Hhco3[j]) / k;
		}
		Eigen::MatrixXd result = Eigen::MatrixXd::Zero(5, 5);

		if(i == 0) {
			result.coeffRef(0, 0) = CardiacOutput*(
				8.*(n_[2]+n_[0])/(k*k*pow(s_[0],3))
				-2./(k*s_[0])
				) / Volumes[0];

			result.coeffRef(1, 1) = CardiacOutput*(-dn_[0] * ds_[1] / pow(s_[1], 2)) / Volumes[1];
			result.coeffRef(1, 0) = 2.*CardiacOutput*dn_[0] / (k*s_[1] * Volumes[1]);
		}
		else if(i == 1) {
			result.coeffRef(1, 1) = CardiacOutput*(
				8.*(n_[0] + n_[1]) / (k*k*pow(s_[1], 3))
				- 2. / (k*s_[1])
				) / Volumes[1];
			
			result.coeffRef(3, 1) = 2.*BrainFlow/(k*s_[3] * Volumes[3]);
			result.coeffRef(3, 3) = BrainFlow*(-dn_[1] * ds_[3] / pow(s_[3], 2)) / Volumes[3];
			result.coeffRef(4, 1) = 2.*(CardiacOutput - BrainFlow)/(k*s_[4] * Volumes[4]);
			result.coeffRef(4, 4) = (CardiacOutput - BrainFlow)*(-dn_[1] * ds_[4] / pow(s_[4], 2)) / Volumes[4];
		}
		else if (i == 2) {
			result.coeffRef(2, 2) = (
				8.*((BrainFlow*n_[3] + (CardiacOutput - BrainFlow)*n_[4]) +
					n_[2]*CardiacOutput) / (k*k*pow(s_[2], 3))
				-2.*CardiacOutput/(k*s_[2])
				) / Volumes[2];

			result.coeffRef(0, 0) = CardiacOutput*(-dn_[2] * ds_[0] / pow(s_[0], 2)) / Volumes[0];
			result.coeffRef(0, 2) = 2.*CardiacOutput/(k*s_[0] * Volumes[0]);
		}
		else if (i == 3) {
			result.coeffRef(3, 3) = BrainFlow*(
				8.*(n_[1] + n_[3]) / (k*k*pow(s_[3], 3))
				- 2. / (k*s_[3])
				) / Volumes[3];

			result.coeffRef(2, 2) = -(BrainFlow*dn_[3]*ds_[2] / pow(s_[2], 2)) / Volumes[2];
			result.coeffRef(2, 3) = 2.*BrainFlow/(k*s_[2] * Volumes[2]);
		}
		else if (i == 4) {
			result.coeffRef(4, 4) = (CardiacOutput - BrainFlow)*(
				8.*(n_[1] + n_[4]) / (k*k*pow(s_[4], 3))
				- 2. / (k*s_[4])
				) / Volumes[4];

			result.coeffRef(2, 2) = -(((CardiacOutput - BrainFlow)*dn_[4])*ds_[2] / pow(s_[2], 2))/ Volumes[2];
			result.coeffRef(2, 4) = 2.*(CardiacOutput - BrainFlow)/(k*s_[2] * Volumes[2]);
		}
		return result;
	}

	Vector WlCo2BbEquations::GetLaBb(const Vector& u)
	{
		Eigen::VectorXd result(5);
	    /*for(auto i=0; i<5;i++)
		{
			result.coeffRef(i) = bcm_.KLaBb*La.coeffRef(i)*Bb.coeffRef(i);
		}*/
		return result;
	}
}