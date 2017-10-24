#pragma once
#ifndef STANDARTIMPLICITHUMANSYSTEM_H_
#define STANDARTIMPLICITHUMANSYSTEM_H_

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
	//The model of whole body
	class StandartHumanSys : public IHumanSys
	{
	public:
		StandartHumanSys() {}

		void SetTissueSys(boost::shared_ptr<ITissueSys> tissueSys) override;
		void SetRespiratorySys(boost::shared_ptr<IRespiratorySys> RespSys) override;
		void SetBloodSys(boost::shared_ptr<IBloodSys> bloodSys) override;
		void SetNervousSys(boost::shared_ptr<INervousSys> nervousSys) override;
		

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
		BioChemConst bcm_;
		WlO2HbEquations eqO2Hb_;
		WlCo2BbEquations eqCo2Bb_;
		BbEquations eqBb_;

		Matrix FastInv(const Matrix& m) const;
		void SolveNonLinEq(
			Numerics::ISetEquations<Vector, Matrix>& eq,
			Vector& u_0, double epsilon);

		void LungO2Transfer(TimeSpan* timeSpan);
		void LungCo2Transfer(TimeSpan* timeSpan);
	};
}

#endif /*STANDARTIMPLICITHUMANSYSTEM_H_*/