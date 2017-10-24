#pragma once
#ifndef LUMPIMPLICITHUMANSYSTEM_H_
#define LUMPIMPLICITHUMANSYSTEM_H_

#include <Eigen/Dense>

#include "BloodSystem/IBloodSys.h"
#include "../Connections/NervousTissue/INervousTissueCn.h"
#include "IHumanSys.h"
#include "BloodSystem/BodyBioChemBloodSys.h"
#include "RespiratorySystem/LumpRespSys.h"
#include "BloodSystem/Biochemistry/BioChemConst.h"
#include "../../Numerics/NewtonMethod.h"

typedef Eigen::VectorXd Vector;
typedef Eigen::MatrixXd Matrix;

namespace SystemModels
{
	//O2-Hb Implicit Euler
	class O2HbLumpImplicit : public Numerics::ISetEquations<Eigen::VectorXd, Eigen::MatrixXd> {

	public:
		O2HbLumpImplicit(double o2_uptake, double t, double o2Old, double o2Lung, double o2Blood) :
						o2_uptake_(o2_uptake), t_(t), o2Old_(o2Old), o2Lung_(o2Lung), o2Blood_(o2Blood)	{

			BioChemConst bcm;
			k_ = bcm.KO2Hb;
			m_ = bcm.HillConst;
			thb_ = bcm.HbTot;
		}

		Eigen::VectorXd f(const Eigen::VectorXd& u) override {
			auto val = u.coeffRef(0);
			auto kVal = k_ + pow(val, m_);
			auto resval = -o2_uptake_ * pow(kVal, 2.) /
				(pow(kVal, 2.) + pow(m_, 2)*thb_*k_*pow(val, m_ - 1.));
			resval *= t_;
			resval += o2Old_ - val;

			auto resvalLung = (o2Lung_ + o2Blood_*val) * pow(kVal, 2.) /
				(pow(kVal, 2.) + pow(m_, 2)*thb_*k_*pow(val, m_ - 1.));
			resvalLung *= t_;

			Eigen::VectorXd result(1);
			result.coeffRef(0) = resval+resvalLung;
			return result;
			/*auto val = u.coeffRef(0);
			auto kVal = k_ + pow(val, m_);
			auto resval = -(o2_uptake_+ o2Lung_ + o2Blood_*o2Old_) * pow(kVal, 2.) /
				(pow(kVal, 2.) + pow(m_, 2)*thb_*k_*pow(val, m_ - 1.));
			resval *= t_;
			resval += o2Old_ - val;

			Eigen::VectorXd result(1);
			result.coeffRef(0) = resval;
			return result;*/
		};

		Eigen::MatrixXd yacobian_f(const Eigen::VectorXd& u) override {
			auto val = u.coeffRef(0);
			auto kVal = k_ + pow(val, m_);
			auto m2 = pow(m_, 2);
			auto resval = 0.;

			auto f1 = pow(kVal, 2.) + m2*thb_*k_*pow(val, m_ - 1.);
			auto df1 = 2.*kVal*m_*pow(val, m_ - 1);
			df1 += (m_ - 1)*m2*thb_*k_*pow(val, m_ - 2);
			resval -= 2.*kVal*m_*pow(val, m_ - 1) / f1;
			resval += pow(kVal, 2.)*df1 / pow(f1, 2);
			resval *= o2_uptake_*t_;
			resval -= 1.;

			auto df = 2.*kVal*m_*pow(val, m_ - 1) / f1;
			df -= pow(kVal, 2.)*df1 / pow(f1, 2);
			auto f = pow(kVal, 2.) / f1;

			auto resvalLung = 0.;
			resvalLung += o2Lung_* df;
			resvalLung += o2Blood_*(val*df + f);
			resvalLung *= t_;

			Eigen::MatrixXd result(1, 1);
			result.coeffRef(0, 0) = resval+resvalLung;
			return result;
			/*auto val = u.coeffRef(0);
			auto kVal = k_ + pow(val, m_);
			auto m2 = pow(m_, 2);
			auto resval = 0.;

			auto f1 = pow(kVal, 2.) + m2*thb_*k_*pow(val, m_ - 1.);
			auto df1 = 2.*kVal*m_*pow(val, m_ - 1);
			df1 += (m_ - 1)*m2*thb_*k_*pow(val, m_ - 2);
			resval -= 2.*kVal*m_*pow(val, m_ - 1) / f1;
			resval += pow(kVal, 2.)*df1 / pow(f1, 2);
			resval *= (o2_uptake_ + o2Lung_ + o2Blood_*o2Old_)*t_;
			resval -= 1.;

			Eigen::MatrixXd result(1, 1);
			result.coeffRef(0, 0) = resval;
			return result;*/
		}

	private:
		double k_;
		double m_;
		double thb_;
		double o2_uptake_;
		double t_;
		double o2Old_;
		double o2Lung_;
		double o2Blood_;
	};

	//CO2-B. Buffer Implicit Euler
	class Co2BbLumpImplicit : public Numerics::ISetEquations<Eigen::VectorXd, Eigen::MatrixXd> {

	public:
		Co2BbLumpImplicit(double m, double co2_dep, double t, 
			              double hco3Old, double co2Lung, double co2Blood) :
			m_(m), co2_dep_(co2_dep), t_(t), hco3Old_(hco3Old), 
			co2Lung_(co2Lung), co2Blood_(co2Blood)
		{
			BioChemConst bcm;
			k_ = bcm.KCO2Bb;
		}

		Eigen::VectorXd f(const Eigen::VectorXd& u) override {
			auto val = u.coeffRef(0);
			auto resval = hco3Old_ - val + co2_dep_*t_*k_ / (k_ - m_ + 2 * val);

			resval += 
				(co2Lung_ + val*(val - m_)*co2Blood_ / k_)*t_*k_ /
				(k_ - m_ + 2 * val);


			Eigen::VectorXd result(1);
			result.coeffRef(0) = resval;
			return result;
		};

		Eigen::MatrixXd yacobian_f(const Eigen::VectorXd& u) override {
			auto val = u.coeffRef(0);
			Eigen::MatrixXd result(1, 1);
			result.coeffRef(0, 0) = -2.*co2_dep_*t_*k_ / pow(k_ - m_ + 2 * val, 2.) - 1;

			auto df = -2.*k_ / pow(k_ - m_ + 2 * val, 2.);
			auto f = k_ / (k_ - m_ + 2 * val);
			result.coeffRef(0, 0) += (co2Lung_*df + co2Blood_*(f*(2 * val - m_) +
				df*val*(val - m_)) / k_)*t_;
			return result;
		}

	private:
		double k_;
		double m_;
		double co2_dep_;
		double t_;
		double hco3Old_;
		double co2Lung_;
		double co2Blood_;
	};
	
	//The model of whole body
	class LumpImplicitHumanSys : public IHumanSys
	{
	public:
		LumpImplicitHumanSys() {}

		void SetTissueSys(boost::shared_ptr<ITissueSys> tissueSys) override;
		void SetRespiratorySys(boost::shared_ptr<IRespiratorySys> RespSys) override;
		void SetBloodSys(boost::shared_ptr<IBloodSys> bloodSys) override;
		void SetNervousSys(boost::shared_ptr<INervousSys> nervousSys) override;
		boost::shared_ptr<ITissueSys> GetTissueSys() override { return tissueSys_; }
		boost::shared_ptr<IRespiratorySys> GetRespiratorySys() override { return RespSys_; }
		boost::shared_ptr<IBloodSys> GetBloodSys() override { return bloodSys_; }
		boost::shared_ptr<INervousSys> GetNervousSys() override { return nervousSys_; }
		void Compute(TimeSpan* timeSpan) override;
		double GetTimeStep() override;

	protected:

		//Systems
		boost::shared_ptr<BodyBioChemBloodSys> bloodSys_;
		boost::shared_ptr<LumpRespSys> RespSys_;
		boost::shared_ptr<ITissueSys> tissueSys_;
		boost::shared_ptr<INervousSys> nervousSys_;

	private:
		BioChemConst bcm_;
		const double dO2Cf_ = 20. / 60.;
		const double dCo2Cf_ = 500. / 60.;

		void LungO2Transfer(TimeSpan* timeSpan);
		void LungCo2Transfer(TimeSpan* timeSpan);
		void LungRegulate(TimeSpan* timeSpan);
	};
}

#endif /*LUMPIMPLICITHUMANSYSTEM_H_*/