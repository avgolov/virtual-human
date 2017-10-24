#include <vector>
#include "../../../Numerics/NewtonMethod.h"
#include "../../Systems/RespiratorySystem/MiptNet1DRespSys.h"
#include "Mipt1DRespBodyBioChem.h"

namespace SystemModels
{
	//O2-Hb Implicit Euler
	class O2HbLungSetEquations : public Numerics::ISetEquations<Eigen::VectorXd, Eigen::MatrixXd> {

	public:
		O2HbLungSetEquations(
			double k, double m, double thb, double o2lung, 
			double o2Blood, double t, double o2Old) :
			k_(k), m_(m), thb_(thb), o2Lung_(o2lung),
			o2Blood_(o2Blood), t_(t), o2Old_(o2Old) {}

		Eigen::VectorXd f(const Eigen::VectorXd& u) override {
			auto val = u.coeffRef(0);
			auto kVal = k_ + pow(val, m_);
			auto resval = (o2Lung_ + o2Blood_*val) * pow(kVal, 2.) /
				(pow(kVal, 2.) + pow(m_, 2)*thb_*k_*pow(val, m_ - 1.));
			resval *= t_;
			resval += o2Old_ - val;

			Eigen::VectorXd result(1);
			result.coeffRef(0) = resval;
			return result;
		};

		Eigen::MatrixXd yacobian_f(const Eigen::VectorXd& u) override {
			auto val = u.coeffRef(0);
			auto kVal = k_ + pow(val, m_);
			auto m2 = pow(m_, 2);

			auto f1 = pow(kVal, 2.) + m2*thb_*k_*pow(val, m_ - 1.);
			auto df1 = 2.*kVal*m_*pow(val, m_ - 1);
			df1 += (m_ - 1)*m2*thb_*k_*pow(val, m_ - 2);
			auto df = 2.*kVal*m_*pow(val, m_ - 1) / f1;
			df -= pow(kVal, 2.)*df1 / pow(f1, 2);
			auto f = pow(kVal, 2.) / f1;
			
			auto resval = 0.;
			resval += o2Lung_* df;
			resval += o2Blood_*(val*df + f);
			resval *= t_;
			resval -= 1.;
			Eigen::MatrixXd result(1, 1);
			result.coeffRef(0, 0) = resval;
			return result;
		}

	private:
		double k_;
		double m_;
		double thb_;
		double o2Lung_;
		double o2Blood_;
		double t_;
		double o2Old_;
	};

	//CO2-B. Buffer Implicit Euler
	class Co2BbLungSetEquations : public Numerics::ISetEquations<Eigen::VectorXd, Eigen::MatrixXd> {

	public:
		Co2BbLungSetEquations(
			double k, double m, double co2Lung,
			double co2Blood, double t, double hco3Old) :
			k_(k), m_(m), co2Lung_(co2Lung),
			co2Blood_(co2Blood), t_(t), hco3Old_(hco3Old) {}

		Eigen::VectorXd f(const Eigen::VectorXd& u) override {
			auto val = u.coeffRef(0);
			Eigen::VectorXd result(1);
			result.coeffRef(0) = hco3Old_ - val +
				(co2Lung_+val*(val-m_)*co2Blood_/k_)*t_*k_ / 
				(k_ - m_ + 2 * val);
			return result;
		};

		Eigen::MatrixXd yacobian_f(const Eigen::VectorXd& u) override {
			auto val = u.coeffRef(0);
			Eigen::MatrixXd result(1, 1);
			auto df = -2.*k_ / pow(k_ - m_ + 2 * val, 2.);
			auto f = k_ / (k_ - m_ + 2 * val);
			result.coeffRef(0, 0) = (co2Lung_*df + co2Blood_*(f*(2*val-m_)+
				df*val*(val - m_))/k_)*t_ - 1;
			return result;
		}

	private:
		double k_;
		double m_;
		double co2Lung_;
		double co2Blood_;
		double t_;
		double hco3Old_;
	};

	void Mipt1DRespBodyBioChemCn::Interact(TimeSpan* timeSpan)
	{
		auto vol = bloodSys_->GetParam(AllVol);
		auto o2Old = bodyBioChemBloodSys_->GetO2();
		auto co2Old = bodyBioChemBloodSys_->GetCo2();
		auto blood_o2 = bodyBioChemBloodSys_->GetO2Free();
		auto alveoles = miptNet1DRespSys_->GetAlveoles();
		auto o2SolCoef = bodyBioChemBloodSys_->GetSolCoefO2();
		auto co2SolCoef = bodyBioChemBloodSys_->GetSolCoefCo2();

		auto o2_exchange = 0.;
		auto co2_exchange = 0.;
		auto squareSum = 0.;
		for (size_t i = 0; i < alveoles->size(); i++)
		{
			auto alveole = (*alveoles)[i];
			auto alvPo2 = atmPres_*alveole->SubstancePrev.Conc[0];
			auto alvPco2 = atmPres_*alveole->SubstancePrev.Conc[1];
			double square = 4 * M_PI*pow(0.75*alveole->Volume / M_PI, 2. / 3.);
			o2_exchange += dO2Cf_*square*alvPo2;
			co2_exchange -= dCo2Cf_*square*alvPco2;
			squareSum += square;
		}
		o2_exchange /= vol;
		co2_exchange /= vol;
		auto o2Coef = -squareSum*dO2Cf_ / (o2SolCoef*vol);
		auto co2Coef = squareSum*dCo2Cf_ / (co2SolCoef*vol);

		//O2 equations
		auto hill = bodyBioChemBloodSys_->GetHillConst();
		auto thb = bodyBioChemBloodSys_->GetHbTot();
		auto kHbO2 = bodyBioChemBloodSys_->GetReactHbO2K();
		O2HbLungSetEquations setEqO2(kHbO2, hill, thb, o2_exchange, o2Coef, timeSpan->dt, blood_o2);
		Numerics::NewtonMethod<Eigen::VectorXd, Eigen::MatrixXd> mO2(&setEqO2);
		Eigen::VectorXd slnO2(1);
		slnO2.coeffRef(0) = blood_o2;
		mO2.Solve(slnO2, 1.e-6);
		bodyBioChemBloodSys_->SetO2Free(slnO2.coeffRef(0));

		//CO2 equations
		auto h = bodyBioChemBloodSys_->GetH();
		auto hco3 = bodyBioChemBloodSys_->GetHCo3();
		auto m = hco3 - h;
		auto kCo2Bb = bodyBioChemBloodSys_->GetReactBbCo2K();
		Co2BbLungSetEquations setEqCo2(kCo2Bb, m, co2_exchange, co2Coef, timeSpan->dt, hco3);
		Numerics::NewtonMethod<Eigen::VectorXd, Eigen::MatrixXd> mCo2(&setEqCo2);
		Eigen::VectorXd slnCo2(1);
		slnCo2.coeffRef(0) = hco3;
		mCo2.Solve(slnCo2, 1.e-6);
		bodyBioChemBloodSys_->SetHco3(slnCo2.coeffRef(0));
	
		auto dO2Vol = (o2Old - bodyBioChemBloodSys_->GetO2());
		auto dCo2Vol = (bodyBioChemBloodSys_->GetCo2() - co2Old);
		for (size_t i = 0; i < alveoles->size(); i++)
		{
			auto alveole = (*alveoles)[i];
			auto alvPo2 = atmPres_*alveole->SubstancePrev.Conc[0];
			auto alvPco2 = atmPres_*alveole->SubstancePrev.Conc[1];
			double square = 4 * M_PI*pow(0.75*alveole->Volume / M_PI, 2. / 3.);
			double c_o2 = dO2Cf_ * square*alvPo2 / o2_exchange;
			alveole->Substance.Conc[0] += c_o2*dO2Vol / alveole->Volume;
			double c_co2 = dCo2Cf_*square*alvPco2 / co2_exchange;
			alveole->Substance.Conc[1] += c_co2*dCo2Vol / alveole->Volume;
		}
	}
}