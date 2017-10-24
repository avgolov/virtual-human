#pragma once
#ifndef WORKIMPLICITHUMANSYSTEM_H_
#define WORKIMPLICITHUMANSYSTEM_H_

#include <Eigen/Dense>

#include "BloodSystem/IBloodSys.h"
#include "../Connections/NervousTissue/INervousTissueCn.h"
#include "IHumanSys.h"
#include "BloodSystem/CompartmentsBloodSys.h"
#include "BloodSystem/Biochemistry/WlO2HbEquations.h"
#include "BloodSystem/Biochemistry/WlCo2BbEquations.h"
#include "RespiratorySystem/LumpRespSys.h"
#include "BloodSystem/Biochemistry/BbEquations.h"
#include "./WorkLoad/IWorkLoad.h"
#include "BloodSystem/Biochemistry/WlLaEquations.h"
#include "../../Numerics/NewtonMethod.h"

typedef Eigen::VectorXd Vector;
typedef Eigen::MatrixXd Matrix;

namespace SystemModels
{
	class WorkLoadMuscle {
	public:
		WorkLoadMuscle(){}
		WorkLoadMuscle(double vo2Max,
					   double enEqvO2Alpha,
		               double workLoadAlpha,
		               double lactateTreshold, 
			           double enEqvO2) :
			vo2Max_(vo2Max),
			enEqvO2Alpha_(enEqvO2Alpha),
			workLoadAlpha_(workLoadAlpha),
			lactateTreshold_(lactateTreshold),
			enEqvO2_(enEqvO2)		 {}

		void Compute(TimeSpan* timeSpan, double workLoad) {
			/*vo2SlowPrev_ = vo2Slow_;
			vo2FastPrev_ = vo2Fast_;
			vo2Fast_ = (vo2FastPrev_-workLoad/enEqvO2Fast_)*exp(-timeSpan->dt/tauFast_) + workLoad/enEqvO2Fast_;
			
			if(workLoad < lactateTreshold_) {
				vo2Slow_ = 0;
			}
			else {
				vo2Slow_ = (vo2SlowPrev_ - workLoad / enEqvO2Slow_)*exp(-timeSpan->dt / tauSlow_) + workLoad / enEqvO2Slow_;
			}*/
			vo2Old_ = vo2_;
			vo2_ = workLoadAlpha_*workLoad;
			workLoadOld_ = workLoadNew_;
			auto eqv = enEqvO2_;
			if (workLoad > lactateTreshold_)
				eqv = enEqvO2_*(1.5-1/(1+exp(-enEqvO2Alpha_*(workLoad-lactateTreshold_))));
			
			if(vo2_*eqv > workLoad) {
				workLoadNew_ = workLoad;
			}
			else {
				workLoadNew_ = eqv*vo2_;
			}
		}

		double GetVo2() const {
			//return vo2Fast_ + vo2Slow_;
			return vo2_;
		}

		double GetWorkLoad() const {
			//return vo2Fast_*enEqvO2Fast_ + vo2Slow_*enEqvO2Slow_;
			return workLoadNew_;
		}

	private:
		/*double enEqvO2Fast_ = 0.;
		double enEqvO2Slow_ = 0.;
		double tauSlow_ = 0;
		double tauFast_ = 0;
		double tdSlow_ = 0;*/
		double vo2Max_ = 0.;
		double enEqvO2Alpha_ = 0.;
		double workLoadAlpha_ = 0;
		double lactateTreshold_ = 0;

		/*double vo2SlowPrev_ = 0;
		double vo2FastPrev_ = 0;
		double vo2Slow_ = 0;
		double vo2Fast_ = 0;*/
		double vo2_ = 0.;
		double vo2Old_ = 0;
		double enEqvO2_ = 0.;
		double workLoadNew_ = 0.;
		double workLoadOld_ = 0.;
	};
	
	//The model of whole body
	class WorkLaodHumanSys : public IHumanSys
	{
	public:
		WorkLaodHumanSys() {}

		void SetTissueSys(boost::shared_ptr<ITissueSys> tissueSys) override;
		void SetRespiratorySys(boost::shared_ptr<IRespiratorySys> RespSys) override;
		void SetBloodSys(boost::shared_ptr<IBloodSys> bloodSys) override;
		void SetNervousSys(boost::shared_ptr<INervousSys> nervousSys) override;
		void SetWork(boost::shared_ptr<IWorkLoad> work_);
		/*void SetVo2Max(double value) { vo2Max_ = value; }
		void SetEnEqvO2Alpha(double value) { enEqvO2Alpha_ = value; }
		void SetWorkLoadAlpha(double value) { workLoadAlpha_ = value; }*/
		void SetCAirWork(double value) { cAirWork_ = value; }
		void SetBAirWork(double value) { bAirWork_ = value; }
		void SetEnEqvO2(double value) { enEqvO2_ = value; }
		void SetLactateTreshold(double value) { lactateTreshold_ = value; }
		void SetEnEqvOLaCoef(double value) { enEqvOLa_ = value; }
		void SetOxyUtLaCoef(double value) { oxyUtLa_ = value; }
		void SetUtLaCoef(double value) { utLa_ = value; }
		void SetCo2UtLaCoef(double value) { co2UtLa_ = value; }
		void SetRer(double value) { rer_ = value; }
		
		boost::shared_ptr<ITissueSys> GetTissueSys() override { return tissueSys_; }
		boost::shared_ptr<IRespiratorySys> GetRespiratorySys() override { return respSys_; }
		boost::shared_ptr<IBloodSys> GetBloodSys() override { return bloodSys_; }
		boost::shared_ptr<INervousSys> GetNervousSys() override { return nervousSys_; }
		void SetMethod();
		void Compute(TimeSpan* timeSpan) override;
		double GetTimeStep() override;

	protected:

		//Systems
		boost::shared_ptr<CompartmentsBloodSys> bloodSys_;
		boost::shared_ptr<LumpRespSys> respSys_;
		boost::shared_ptr<ITissueSys> tissueSys_;
		boost::shared_ptr<INervousSys> nervousSys_;

	private:
		WlO2HbEquations eqO2Hb_;
		WlCo2BbEquations eqCo2Bb_;
		BbEquations eqBb_;
		WlLaEquations eqLa_;
		BioChemConst bcm_;
		boost::shared_ptr<IWorkLoad> work_;

		//The coef of O2 energy equivalent
		/*double enEqvO2Fast_ = 0.;
		double enEqvO2Slow_ = 0.;
		double tauSlow_ = 0;
		double tauFast_ = 0;
		double tdSlow_ = 0;*/
		/*double vo2Max_ = 0.;
		double enEqvO2Alpha_ = 0.;
		double workLoadAlpha_ = 0;*/
		double enEqvO2_ = 0.;
		double cAirWork_ = 0.;
		double bAirWork_ = 0.;

		//The coef of lactate energy equivalent
		double enEqvOLa_ = 0.;
		double lactateTreshold_ = 0;

		//The coef of lactate utilization by tissue
		double oxyUtLa_ = 0.;
		double co2UtLa_ = 0.;
		double utLa_ = 0.;

		double rer_ = 0.;
		//WorkLoadMuscle muscle_;

		double GetAirWorkPart(double& work);

		Matrix FastInv(const Matrix& m) const;
		void SolveNonLinEq(
			Numerics::ISetEquations<Vector, Matrix>& eq,
			Vector& u_0, double epsilon);

		void LungO2Transfer(TimeSpan* timeSpan);
		void LungCo2Transfer(TimeSpan* timeSpan);
		
	};
}

#endif /*WORKIMPLICITHUMANSYSTEM_H_*/