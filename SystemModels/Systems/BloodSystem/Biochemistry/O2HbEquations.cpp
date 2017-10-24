#include "O2HbEquations.h"
#include "BioChemConst.h"

namespace SystemModels
{
	Vector O2HbEquations::GetTissueF(const Vector& u)
	{
		auto k = bcm_.KO2Hb;
		auto hbTot = bcm_.HbTot;
		auto m = bcm_.HillConst;
		auto lMolCoef = bcm_.LitMolCoef;
		
		Eigen::VectorXd result = Eigen::VectorXd::Zero(5);
		auto val = u.coeffRef(3);
		auto kVal = k + pow(val, m);
		auto resval = -(BrainUptake/(lMolCoef*Volumes[3])) * pow(kVal, 2.) /
			(pow(kVal, 2.) + pow(m, 2)*hbTot*k*pow(val, m - 1.));
		result.coeffRef(3) = resval;

		val = u.coeffRef(4);
		kVal = k + pow(val, m);
		resval = -(TissueUptake / (lMolCoef*Volumes[4])) * pow(kVal, 2.) /
			(pow(kVal, 2.) + pow(m, 2)*hbTot*k*pow(val, m - 1.));
		result.coeffRef(4) = resval;

		return result;
	}

	Matrix O2HbEquations::GetTissueDf(const Vector& u)
	{
		auto k = bcm_.KO2Hb;
		auto hbTot = bcm_.HbTot;
		auto m = bcm_.HillConst;
		auto m2 = m*m;
		auto lMolCoef = bcm_.LitMolCoef;

		Eigen::MatrixXd result = Eigen::MatrixXd::Zero(5, 5);

		auto val = u.coeffRef(3);
		auto kVal = k + pow(val, m);
		
		auto f1 = pow(kVal, 2.) + m2*hbTot*k*pow(val, m - 1.);
		auto df1 = 2.*kVal*m*pow(val, m - 1);
		df1 += (m - 1)*m2*hbTot*k*pow(val, m - 2);
		auto resval = 0.;
		resval -= 2.*kVal*m*pow(val, m - 1) / f1;
		resval += pow(kVal, 2.)*df1 / pow(f1, 2);
		resval *= (BrainUptake / (lMolCoef*Volumes[3]));
		result.coeffRef(3, 3) = resval;

		val = u.coeffRef(4);
		kVal = k + pow(val, m);
		
		f1 = pow(kVal, 2.) + m2*hbTot*k*pow(val, m - 1.);
		df1 = 2.*kVal*m*pow(val, m - 1);
		df1 += (m - 1)*m2*hbTot*k*pow(val, m - 2);
		resval = 0.;
		resval -= 2.*kVal*m*pow(val, m - 1) / f1;
		resval += pow(kVal, 2.)*df1 / pow(f1, 2);
		resval *= (TissueUptake / (lMolCoef*Volumes[4]));
		result.coeffRef(4, 4) = resval;

		return result;
	}

	Vector O2HbEquations::GetLungsF(const Vector& u)
	{
		auto k = bcm_.KO2Hb;
		auto hbTot = bcm_.HbTot;
		auto m = bcm_.HillConst;
		auto val = u.coeffRef(0);
		auto kVal = k + pow(val, m);
		auto dP = (O2AlvLung + O2AlvBlood*val/bcm_.SolCoefO2)/Volumes[0];
		auto resval = dP * pow(kVal, 2.) /
			(pow(kVal, 2.) + pow(m, 2)*hbTot*k*pow(val, m - 1.));
		
		Eigen::VectorXd result = Eigen::VectorXd::Zero(5);
		result.coeffRef(0) = resval;
		return result;
	}

	Matrix O2HbEquations::GetLungsDf(const Vector& u)
	{
		auto k = bcm_.KO2Hb;
		auto hbTot = bcm_.HbTot;
		auto m = bcm_.HillConst;
		auto m2 = m*m;

		auto val = u.coeffRef(0);
		auto kVal = k + pow(val, m);

		auto f1 = pow(kVal, 2.) + m2*hbTot*k*pow(val, m - 1.);
		auto df1 = 2.*kVal*m*pow(val, m - 1);
		df1 += (m - 1)*m2*hbTot*k*pow(val, m - 2);
		auto dPl = O2AlvLung / Volumes[0];
		auto dPb = O2AlvBlood / (bcm_.SolCoefO2*Volumes[0]);
		auto resval = 0.;
		resval += 2.*kVal*m*pow(val, m - 1) / f1;
		resval -= pow(kVal, 2.)*df1 / pow(f1, 2);
		resval *= dPl+val*dPb;
		resval += dPb*pow(kVal, 2.) / f1;

		Eigen::MatrixXd result = Eigen::MatrixXd::Zero(5, 5);
		result.coeffRef(0, 0) = resval;
		return result;
	}

	Vector O2HbEquations::GetBloodF(const Vector& u)
	{
		auto m = bcm_.HillConst;
		auto k = bcm_.KO2Hb;
		auto thb = bcm_.HbTot;
		//J – {PA, PV, SV, BA, TA} = {1, 2, 3, 4, 5}
		std::vector<double> s(5);
		std::vector<double> n(5);
		for (auto i = 0; i < 5; i++) {
			auto val = u.coeffRef(i);
			s[i] = 1 + m*m*thb*k*pow(val, m - 1) / pow(k + pow(val, m), 2);
			n[i] = val + m*thb*pow(val, m)/(k+ pow(val, m));
		}

		Eigen::VectorXd result = Eigen::VectorXd::Zero(5);
		result.coeffRef(0) = CardiacOutput*(-n[0] + n[2]) / (Volumes[0]* s[0]);
		result.coeffRef(1) = CardiacOutput*(n[0] - n[1]) / (Volumes[1] * s[1]);
		result.coeffRef(2) = (-CardiacOutput*n[2] + BrainFlow*n[3] +
			(CardiacOutput - BrainFlow)*n[4]) / (Volumes[2]* s[2]);
		result.coeffRef(3) = BrainFlow*(n[1] - n[3]) / (Volumes[3] * s[3]);
		result.coeffRef(4) = (CardiacOutput - BrainFlow)*(n[1] - n[4]) / (Volumes[4] * s[4]);
		/*result.coeffRef(0) = CardiacOutput*(n[2] - n[0]) / (s[0] * Volumes[0]);
		result.coeffRef(1) = CardiacOutput*(n[0] - n[1]) / (s[1] * Volumes[1]);
		result.coeffRef(2) = CardiacOutput*(n[1] - n[2]) / (s[2] * Volumes[2]);
		auto c1 = result.coeffRef(0);
		auto c2 = result.coeffRef(1);
		auto c3 = result.coeffRef(2);
		auto c4 = result.coeffRef(3);
		auto c5 = result.coeffRef(4);*/
		return result;
	}

	Matrix O2HbEquations::GetBloodDf(const Vector& u)
	{
		auto m = bcm_.HillConst;
		auto m2 = m*m;
		auto k = bcm_.KO2Hb;
		auto thb = bcm_.HbTot;
		//J – {PA, PV, SV, BA, TA} = {1, 2, 3, 4, 5}
		std::vector<double> s(5);
		std::vector<double> ds(5);
		std::vector<double> n(5);
		std::vector<double> dn(5);
		for (auto i = 0; i < 5; i++) {
			auto val = u.coeffRef(i);
			auto kx = k + pow(val, m);
			s[i] = 1 + m2*thb*k*pow(val, m - 1) / pow(kx, 2);
			ds[i] = m2*(m - 1)*thb*k*pow(val, m - 2) / pow(kx, 2) -
				2 * m2*thb*k*pow(val, m - 1)*m*pow(val, m - 1) / pow(kx, 3);
			n[i] = val + m*thb*pow(val, m) / (k + pow(val, m));
			dn[i] = 1 + m2*thb*pow(val, 2 * m - 1) / pow(kx, 2) + m2*thb*pow(val, m - 1) / kx;
		}
		Eigen::MatrixXd result = Eigen::MatrixXd::Zero(5, 5);
		result.coeffRef(0, 0) = CardiacOutput*(-n[2] * ds[0] / pow(s[0], 2) -
			dn[0] / s[0] + n[0] * ds[0] / pow(s[0], 2)) / Volumes[0];
		result.coeffRef(0, 2) = CardiacOutput*dn[2] / (s[0] * Volumes[0]);

		result.coeffRef(1, 1) = CardiacOutput*(-n[0] * ds[1] / pow(s[1], 2) -
			dn[1] / s[1] + n[1] * ds[1] / pow(s[1], 2)) / Volumes[1];
		result.coeffRef(1, 0) = CardiacOutput*dn[0] / (s[1] * Volumes[1]);

		result.coeffRef(2, 2) = (-(BrainFlow*n[3] + (CardiacOutput - BrainFlow)*n[4])*ds[2] / pow(s[2], 2)+
			CardiacOutput*(-dn[2]/s[2] + n[2]/pow(s[2], 2)))/ Volumes[2];
			
		result.coeffRef(2, 3) = BrainFlow*dn[3] / (s[2] * Volumes[2]);
		result.coeffRef(2, 4) = (CardiacOutput - BrainFlow)*dn[4] / (s[2] * Volumes[2]);

		result.coeffRef(3, 3) = BrainFlow*(-n[1] * ds[3] / pow(s[3], 2) -
			dn[3]/s[3] + n[3]*ds[3]/pow(s[3], 2)) / Volumes[3];
		result.coeffRef(3, 1) = BrainFlow*dn[1] / (s[3] * Volumes[3]);

		result.coeffRef(4, 4) = (CardiacOutput - BrainFlow)*(-n[1] * ds[4] / pow(s[4], 2) -
			dn[4] / s[4] + n[4] * ds[4] / pow(s[4], 2)) / Volumes[4];
		result.coeffRef(4, 1) = (CardiacOutput - BrainFlow)*dn[1] / (s[4] * Volumes[4]);

		return result;
	}
}