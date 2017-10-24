#include <vector>
#include "BodyBioChemTissueCn.h"
#include "../../../Numerics/NewtonMethod.h"


namespace SystemModels
{
	//O2-Hb Implicit Euler
	class O2HbArtSetEquations : public Numerics::ISetEquations<Eigen::VectorXd, Eigen::MatrixXd> {

	public:
		O2HbArtSetEquations(
			double k,	double m, double thb, 
			double o2_uptake, double t, double o2Old) :
			k_(k), m_(m), thb_(thb), 
			o2_uptake_(o2_uptake), t_(t), o2Old_(o2Old) {}

		Eigen::VectorXd f(const Eigen::VectorXd& u) override {
			auto val = u.coeffRef(0);
			auto kVal = k_ + pow(val, m_);
			auto resval = -o2_uptake_ * pow(kVal, 2.) /
				(pow(kVal, 2.) + pow(m_,2)*thb_*k_*pow(val,m_-1.));
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
			auto resval = 0.;

			auto f1 = pow(kVal, 2.) + m2*thb_*k_*pow(val, m_ - 1.);
			auto df1 = 2.*kVal*m_*pow(val, m_ - 1);
			df1 += (m_ - 1)*m2*thb_*k_*pow(val, m_ - 2);
			resval -= 2.*kVal*m_*pow(val, m_-1) / f1;
			resval += pow(kVal, 2.)*df1 / pow(f1, 2);
			resval *= o2_uptake_*t_;
			resval -= 1.;
			Eigen::MatrixXd result(1, 1);
			result.coeffRef(0, 0) = resval;
			return result;
		}

	private:
		double k_;
		double m_;
		double thb_;
		double o2_uptake_;
		double t_;
		double o2Old_;
	};
	
	//CO2-B. Buffer Implicit Euler
	class Co2BbArtSetEquations : public Numerics::ISetEquations<Eigen::VectorXd, Eigen::MatrixXd> {

	public:
		Co2BbArtSetEquations(
			double k, double m, double co2_dep, double t, double hco3Old) :
			k_(k), m_(m), co2_dep_(co2_dep), t_(t), hco3Old_(hco3Old) {}

		Eigen::VectorXd f(const Eigen::VectorXd& u) override {
			auto val = u.coeffRef(0);
			auto resval = hco3Old_ - val + co2_dep_*t_*k_ / (k_ - m_ + 2 * val);
			Eigen::VectorXd result(1);
			result.coeffRef(0) = resval;
			return result;
		};

		Eigen::MatrixXd yacobian_f(const Eigen::VectorXd& u) override {
			auto val = u.coeffRef(0);
			Eigen::MatrixXd result(1, 1);
			result.coeffRef(0, 0) = -2.*co2_dep_*t_*k_ / pow(k_ - m_ + 2 * val, 2.) - 1;
			return result;
		}

	private:
		double k_;
		double m_;
		double co2_dep_;
		double t_;
		double hco3Old_;
	};

	void BodyBioChemTissueCn::Interact(TimeSpan* timeSpan)
	{
		//Tissue values
		auto bloodVol = bodyBioChemBloodSys_->GetParam(AllVol);
		auto o2_uptake = tissueSys_->GetParam(OtherO2Uptake) / (bloodVol*22.4);
		auto co2_dep = tissueSys_->GetParam(OtherCO2Dep) / (bloodVol*22.4);
		auto o2 = bodyBioChemBloodSys_->GetO2Free();

		//O2-Hb equations
		auto hill = bodyBioChemBloodSys_->GetHillConst();
		auto thb = bodyBioChemBloodSys_->GetHbTot();
		auto kHbO2 = bodyBioChemBloodSys_->GetReactHbO2K();
		O2HbArtSetEquations setEqO2(kHbO2, hill, thb, o2_uptake,timeSpan->dt, o2);
		Numerics::NewtonMethod<Eigen::VectorXd, Eigen::MatrixXd> mO2(&setEqO2);
		Eigen::VectorXd sln(1);
		sln.coeffRef(0) = o2;
		mO2.Solve(sln, 1.e-6);
		bodyBioChemBloodSys_->SetO2Free(sln.coeffRef(0));
		
		//Co2 bicarbonate buffer equations
		auto h = bodyBioChemBloodSys_->GetH();
		auto hco3 = bodyBioChemBloodSys_->GetHCo3();
		auto m = hco3 - h;
		auto kCo2Bb = bodyBioChemBloodSys_->GetReactBbCo2K();
		Co2BbArtSetEquations setEqCo2(kCo2Bb,m,co2_dep, timeSpan->dt, hco3);
		Numerics::NewtonMethod<Eigen::VectorXd, Eigen::MatrixXd> mCo2(&setEqCo2);
		sln.coeffRef(0) = hco3;
		mCo2.Solve(sln, 1.e-6);
		bodyBioChemBloodSys_->SetHco3(sln.coeffRef(0));
	}
}