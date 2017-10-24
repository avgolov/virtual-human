#include "WlO2HbEquations.h"
#include "BioChemConst.h"

namespace SystemModels
{
	Vector WlO2HbEquations::GetTissueF(const Vector& u)
	{
		auto k = bcm_.KO2Hb;
		auto hbTot = bcm_.HbTot;
		auto m = bcm_.HillConst;

		Eigen::VectorXd result = Eigen::VectorXd::Zero(5);
		auto val = u.coeffRef(3);
		auto kVal = k + pow(val, m);
		auto resval = -(BrainUptake / (bcm_.LitMolCoef*Volumes[3])) * pow(kVal, 2.) /
			(pow(kVal, 2.) + pow(m, 2)*hbTot*k*pow(val, m - 1.));
		result.coeffRef(3) = resval;

		val = u.coeffRef(4);
		kVal = k + pow(val, m);
		resval = -(TissueUptake / (bcm_.LitMolCoef*Volumes[4])) * pow(kVal, 2.) /
			(pow(kVal, 2.) + pow(m, 2)*hbTot*k*pow(val, m - 1.));
		result.coeffRef(4) = resval;

		return result;
	}

	Matrix WlO2HbEquations::GetTissueDf(const Vector& u)
	{
		auto k = bcm_.KO2Hb;
		auto hbTot = bcm_.HbTot;
		auto m = bcm_.HillConst;
		auto m2 = m*m;

		Eigen::MatrixXd result = Eigen::MatrixXd::Zero(5, 5);

		auto val = u.coeffRef(3);
		auto kVal = k + pow(val, m);

		auto f1 = pow(kVal, 2.) + m2*hbTot*k*pow(val, m - 1.);
		auto df1 = 2.*kVal*m*pow(val, m - 1);
		df1 += (m - 1)*m2*hbTot*k*pow(val, m - 2);
		auto resval = 0.;
		resval -= 2.*kVal*m*pow(val, m - 1) / f1;
		resval += pow(kVal, 2.)*df1 / pow(f1, 2);
		resval *= (BrainUptake / (bcm_.LitMolCoef*Volumes[3]));
		result.coeffRef(3, 3) = resval;

		val = u.coeffRef(4);
		kVal = k + pow(val, m);

		f1 = pow(kVal, 2.) + m2*hbTot*k*pow(val, m - 1.);
		df1 = 2.*kVal*m*pow(val, m - 1);
		df1 += (m - 1)*m2*hbTot*k*pow(val, m - 2);
		resval = 0.;
		resval -= 2.*kVal*m*pow(val, m - 1) / f1;
		resval += pow(kVal, 2.)*df1 / pow(f1, 2);
		resval *= (TissueUptake / (bcm_.LitMolCoef*Volumes[4]));
		result.coeffRef(4, 4) = resval;

		return result;
	}

	Matrix WlO2HbEquations::GetTissueDfdx(const Vector& u, const int& i)
	{
		Eigen::MatrixXd result = Eigen::MatrixXd::Zero(5, 5);
		if (i != 3 && i != 4) return result;

		auto m = bcm_.HillConst;
		auto m2 = m*m;
		auto k = bcm_.KO2Hb;
		auto thb = bcm_.HbTot;

		if (i == 3) {
			auto val = u.coeffRef(3);
			auto kx = k + pow(val, m);
			s_[3] = 1 + m2*thb*k*pow(val, m - 1) / pow(kx, 2);
			ds_[3] = m2*(m - 1)*thb*k*pow(val, m - 2) / pow(kx, 2) -
				2 * m2*thb*k*pow(val, m - 1)*m*pow(val, m - 1) / pow(kx, 3);
			d2s_[3] = m2*(m - 1)*k*thb*((m - 2)*pow(val, m - 3) / pow(kx, 2) - 2 * m*pow(val, 2 * m - 3) / pow(kx, 3));
			d2s_[3] -= 2 * m2*m*thb*k*((2 * m - 2)*pow(val, 2 * m - 3) / pow(kx, 3) - 3 * m*pow(val, 3 * m - 3) / pow(kx, 4));

			result.coeffRef(3, 3) = -(BrainUptake / (bcm_.LitMolCoef*Volumes[3] * pow(s_[3], 2)))*(
				2 * pow(ds_[3], 2) / s_[3] - d2s_[3]);
		}
		else if (i == 4) {
			auto val = u.coeffRef(4);
			auto kx = k + pow(val, m);
			s_[4] = 1 + m2*thb*k*pow(val, m - 1) / pow(kx, 2);
			ds_[4] = m2*(m - 1)*thb*k*pow(val, m - 2) / pow(kx, 2) -
				2 * m2*thb*k*pow(val, m - 1)*m*pow(val, m - 1) / pow(kx, 3);
			d2s_[4] = m2*(m - 1)*k*thb*((m - 2)*pow(val, m - 3) / pow(kx, 2) - 2 * m*pow(val, 2 * m - 3) / pow(kx, 3));
			d2s_[4] -= 2 * m2*m*thb*k*((2 * m - 2)*pow(val, 2 * m - 3) / pow(kx, 3) - 3 * m*pow(val, 3 * m - 3) / pow(kx, 4));

			result.coeffRef(4, 4) = -(TissueUptake / (bcm_.LitMolCoef*Volumes[4] * pow(s_[4], 2)))*(
				2 * pow(ds_[4], 2) / s_[4] - d2s_[4]);
		}
		return result;
	}

	Vector WlO2HbEquations::GetLungsF(const Vector& u)
	{
		auto k = bcm_.KO2Hb;
		auto hbTot = bcm_.HbTot;
		auto m = bcm_.HillConst;
		auto val = u.coeffRef(0);
		auto kVal = k + pow(val, m);
		auto dP = bcm_.Do2Cf*(Po2AlvLung - val / bcm_.SolCoefO2) / (Volumes[0]*bcm_.LitMolCoef);
		auto resval = dP * pow(kVal, 2.) /
			(pow(kVal, 2.) + pow(m, 2)*hbTot*k*pow(val, m - 1.));

		Eigen::VectorXd result = Eigen::VectorXd::Zero(5);
		result.coeffRef(0) = resval;
		return result;
	}

	Matrix WlO2HbEquations::GetLungsDf(const Vector& u)
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
		auto dPl = bcm_.Do2Cf*Po2AlvLung / (Volumes[0]*bcm_.LitMolCoef);
		auto dPb = -bcm_.Do2Cf / (bcm_.SolCoefO2*Volumes[0]*bcm_.LitMolCoef);
		auto resval = 0.;
		resval += 2.*kVal*m*pow(val, m - 1) / f1;
		resval -= pow(kVal, 2.)*df1 / pow(f1, 2);
		resval *= dPl + val*dPb;
		resval += dPb*pow(kVal, 2.) / f1;

		Eigen::MatrixXd result = Eigen::MatrixXd::Zero(5, 5);
		result.coeffRef(0, 0) = resval;
		return result;
	}
	Matrix WlO2HbEquations::GetLungsDfdx(const Vector& u, const int& i)
	{
		Eigen::MatrixXd result = Eigen::MatrixXd::Zero(5, 5);
		if (i != 0) return result;

		auto m = bcm_.HillConst;
		auto m2 = m*m;
		auto k = bcm_.KO2Hb;
		auto thb = bcm_.HbTot;
		auto val = u.coeffRef(0);
		auto kx = k + pow(val, m);
		s_[0] = 1 + m2*thb*k*pow(val, m - 1) / pow(kx, 2);
		ds_[0] = m2*(m - 1)*thb*k*pow(val, m - 2) / pow(kx, 2) -
			2 * m2*thb*k*pow(val, m - 1)*m*pow(val, m - 1) / pow(kx, 3);
		d2s_[0] = m2*(m - 1)*k*thb*((m - 2)*pow(val, m - 3) / pow(kx, 2) - 2 * m*pow(val, 2 * m - 3) / pow(kx, 3));
		d2s_[0] -= 2 * m2*m*thb*k*((2 * m - 2)*pow(val, 2 * m - 3) / pow(kx, 3) - 3 * m*pow(val, 3 * m - 3) / pow(kx, 4));

		auto resval = (bcm_.Do2Cf*Po2AlvLung / (bcm_.LitMolCoef*Volumes[0] * pow(s_[0], 2)))*(
			2 * pow(ds_[0], 2) / s_[0] - d2s_[0]);

		resval -= (bcm_.Do2Cf/(bcm_.SolCoefO2*bcm_.LitMolCoef*Volumes[0] * pow(s_[0], 2)))*
			(-2*ds_[0]-d2s_[0]*val+2*pow(ds_[0],2)*val/s_[0]);
		result.coeffRef(0, 0) = resval;
		return result;
	}

	Vector WlO2HbEquations::GetBloodF(const Vector& u)
	{
		auto m = bcm_.HillConst;
		auto k = bcm_.KO2Hb;
		auto thb = bcm_.HbTot;
		//J – {PA, PV, SV, BA, TA} = {1, 2, 3, 4, 5}
		for (auto i = 0; i < 5; i++) {
			auto val = u.coeffRef(i);
			s_[i] = 1 + m*m*thb*k*pow(val, m - 1) / pow(k + pow(val, m), 2);
			n_[i] = val + m*thb*pow(val, m) / (k + pow(val, m));
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

	Matrix WlO2HbEquations::GetBloodDf(const Vector& u)
	{
		auto m = bcm_.HillConst;
		auto m2 = m*m;
		auto k = bcm_.KO2Hb;
		auto thb = bcm_.HbTot;
		//J – {PA, PV, SV, BA, TA} = {1, 2, 3, 4, 5}
		for (auto i = 0; i < 5; i++) {
			auto val = u.coeffRef(i);
			auto kx = k + pow(val, m);
			s_[i] = 1 + m2*thb*k*pow(val, m - 1) / pow(kx, 2);
			ds_[i] = m2*(m - 1)*thb*k*pow(val, m - 2) / pow(kx, 2) -
				2 * m2*thb*k*pow(val, m - 1)*m*pow(val, m - 1) / pow(kx, 3);
			n_[i] = val + m*thb*pow(val, m) / kx;
			dn_[i] = 1 - m2*thb*pow(val, 2 * m - 1) / pow(kx, 2) + m2*thb*pow(val, m - 1) / kx;
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

	Matrix WlO2HbEquations::GetBloodDfdx(const Vector& u, const int& i)
	{
		auto m = bcm_.HillConst;
		auto m2 = m*m;
		auto k = bcm_.KO2Hb;
		auto thb = bcm_.HbTot;
		//J – {PA, PV, SV, BA, TA} = {1, 2, 3, 4, 5}
		for (auto j = 0; j < 5; j++) {
			auto val = u.coeffRef(j);
			auto kx = k + pow(val, m);
			s_[j] = 1 + m2*thb*k*pow(val, m - 1) / pow(kx, 2);
			ds_[j] = m2*(m - 1)*thb*k*pow(val, m - 2) / pow(kx, 2) -
				2 * m2*thb*k*pow(val, m - 1)*m*pow(val, m - 1) / pow(kx, 3);
			d2s_[j] = m2*(m - 1)*k*thb*((m - 2)*pow(val, m - 3) / pow(kx, 2)-2*m*pow(val,2*m-3)/pow(kx,3));
			d2s_[j] -= 2*m2*m*thb*k*((2*m-2)*pow(val,2*m-3)/pow(kx,3)-3*m*pow(val,3*m-3)/pow(kx,4));

			n_[j] = val + m*thb*pow(val, m) / (k + pow(val, m));
			dn_[j] = 1 - m2*thb*(pow(val, 2 * m - 1) / pow(kx, 2) + pow(val, m - 1) / kx);
			d2n_[j] = m2*thb*(-(2*m-1)*pow(val,2*m-2)/pow(kx,2)+2*m*pow(val,3*m-2)/pow(kx,3)+
				(m-1)*pow(val,m-2)/kx-m*pow(val,2*m-2)/pow(kx,2));
		}
		Eigen::MatrixXd result = Eigen::MatrixXd::Zero(5, 5);
		
		if(i == 0)
		{
			result.coeffRef(0, 0) = -n_[2]*d2s_[0]/pow(s_[0], 2) + 2.*n_[2] * pow(ds_[0],2) / pow(s_[0], 3);
			result.coeffRef(0, 0) -= d2n_[0] / s_[0] - dn_[0] * ds_[0] / pow(s_[0],2);
			result.coeffRef(0, 0) += (dn_[0] * ds_[0]+ n_[0] * d2s_[0]) / pow(s_[0], 2)
				-2*n_[0]*pow(ds_[0],2)/pow(s_[0], 3);
			result.coeffRef(0, 0) *= CardiacOutput / Volumes[0];

			result.coeffRef(1, 1) = CardiacOutput*(-dn_[0] * ds_[1] / pow(s_[1], 2)) / Volumes[1];
			result.coeffRef(1, 0) = CardiacOutput*d2n_[0] / (s_[1] * Volumes[1]);

		}
		else if(i == 1)
		{
			result.coeffRef(1, 1) = -n_[0] * d2s_[1] / pow(s_[1], 2) + 2.*n_[0] * pow(ds_[1], 2) / pow(s_[1], 3);
			result.coeffRef(1, 1) -= d2n_[1] / s_[1] - dn_[1] * ds_[1] / pow(s_[1], 2);
			result.coeffRef(1, 1) += (dn_[1] * ds_[1] + n_[1] * d2s_[1]) / pow(s_[1], 2)
				- 2 * n_[1] * pow(ds_[1], 2) / pow(s_[1], 3);
			result.coeffRef(1, 1) *= CardiacOutput / Volumes[1];

			result.coeffRef(3, 1) = BrainFlow*d2n_[1] / (s_[3] * Volumes[3]);
			result.coeffRef(3, 3) = BrainFlow*(-dn_[1] * ds_[3] / pow(s_[3], 2)) / Volumes[3];
			result.coeffRef(4, 1) = (CardiacOutput - BrainFlow)*d2n_[1] / (s_[4] * Volumes[4]);
			result.coeffRef(4, 4) = (CardiacOutput - BrainFlow)*(-dn_[1] * ds_[4] / pow(s_[4], 2)) / Volumes[4];
		}
		else if (i == 2)
		{
			result.coeffRef(2, 2) = -BrainFlow*n_[3] * d2s_[2] / pow(s_[2], 2) +
				2.*(CardiacOutput - BrainFlow)*n_[4] * pow(ds_[2], 2) / pow(s_[2], 3);
			result.coeffRef(2, 2) -= (d2n_[2] / s_[2] - dn_[2] * ds_[2] / pow(s_[2], 2))*CardiacOutput;
			result.coeffRef(2, 2) += ((dn_[2] * ds_[2] + n_[2] * d2s_[2]) / pow(s_[2], 2)
				- 2 * n_[2] * pow(ds_[2], 2) / pow(s_[2], 3))*CardiacOutput;
			result.coeffRef(2, 2) /= Volumes[2];

			result.coeffRef(0, 2) = CardiacOutput*d2n_[2] / (s_[0] * Volumes[0]);
			result.coeffRef(0, 0) = CardiacOutput*(-dn_[2] * ds_[0] / pow(s_[0], 2)) / Volumes[0];
		}
		else if (i == 3)
		{
			result.coeffRef(3, 3) = -n_[1] * d2s_[3] / pow(s_[3], 2) + 2.*n_[1] * pow(ds_[3], 2) / pow(s_[3], 3);
			result.coeffRef(3, 3) -= d2n_[3] / s_[3] - dn_[3] * ds_[3] / pow(s_[3], 2);
			result.coeffRef(3, 3) += (dn_[3] * ds_[3] + n_[3] * d2s_[3]) / pow(s_[3], 2)
				- 2 * n_[3] * pow(ds_[3], 2) / pow(s_[3], 3);
			result.coeffRef(3, 3) *= BrainFlow / Volumes[3];
			
			result.coeffRef(2, 3) = BrainFlow*d2n_[3] / (s_[2] * Volumes[2]);
			result.coeffRef(2, 2) = (-BrainFlow*dn_[3]*ds_[2] / pow(s_[2], 2)) / Volumes[2];
		}
		else if (i == 4)
		{
			result.coeffRef(4, 4) = -n_[1] * d2s_[4] / pow(s_[4], 2) + 2.*n_[1] * pow(ds_[4], 2) / pow(s_[4], 3);
			result.coeffRef(4, 4) -= d2n_[4] / s_[4] - dn_[4] * ds_[4] / pow(s_[4], 2);
			result.coeffRef(4, 4) += (dn_[4] * ds_[4] + n_[4] * d2s_[4]) / pow(s_[4], 2)
				- 2 * n_[4] * pow(ds_[4], 2) / pow(s_[4], 3);
			result.coeffRef(4, 4) *= (CardiacOutput - BrainFlow) / Volumes[4];

			result.coeffRef(2, 4) = (CardiacOutput - BrainFlow)*d2n_[4] / (s_[2] * Volumes[2]);
			result.coeffRef(2, 2) = (-(CardiacOutput - BrainFlow)*dn_[4]*ds_[2] / pow(s_[2], 2)) / Volumes[2];
		}

		return result;
	}
}