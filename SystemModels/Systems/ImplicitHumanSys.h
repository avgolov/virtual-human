#pragma once
#ifndef IMPLICITHUMANSYSTEM_H_
#define IMPLICITHUMANSYSTEM_H_

#include <Eigen/Dense>

#include "BloodSystem/IBloodSys.h"
#include "../Connections/NervousTissue/INervousTissueCn.h"
#include "IHumanSys.h"
#include "BloodSystem/CompartmentsBloodSys.h"
#include "BloodSystem/Biochemistry/O2HbEquations.h"
#include "BloodSystem/Biochemistry/Co2BbEquations.h"
#include "RespiratorySystem/MiptNet1DRespSys.h"
#include "BloodSystem/Biochemistry/BbEquations.h"

typedef Eigen::VectorXd Vector;
typedef Eigen::MatrixXd Matrix;

namespace SystemModels
{
	//The model of whole body
	class ImplicitHumanSys : public IHumanSys
	{
	public:
		ImplicitHumanSys() {}

		void SetTissueSys(boost::shared_ptr<ITissueSys> tissueSys) override;
		void SetRespiratorySys(boost::shared_ptr<IRespiratorySys> RespSys) override;
		void SetBloodSys(boost::shared_ptr<IBloodSys> bloodSys) override;
		void SetNervousSys(boost::shared_ptr<INervousSys> nervousSys) override;
		boost::shared_ptr<ITissueSys> GetTissueSys() override { return tissueSys_; }
		boost::shared_ptr<IRespiratorySys> GetRespiratorySys() override { return RespSys_; }
		boost::shared_ptr<IBloodSys> GetBloodSys() override { return bloodSys_; }
		boost::shared_ptr<INervousSys> GetNervousSys() override { return nervousSys_; }
		void SetMethod();
		void Compute(TimeSpan* timeSpan) override;
		double GetTimeStep() override;

	protected:

		//Systems
		boost::shared_ptr<CompartmentsBloodSys> bloodSys_;
		boost::shared_ptr<MiptNet1DRespSys> RespSys_;
		boost::shared_ptr<ITissueSys> tissueSys_;
		boost::shared_ptr<INervousSys> nervousSys_;

	private:
		O2HbEquations eqO2Hb_;
		Co2BbEquations eqCo2Bb_;
		BbEquations eqBb_;
		BioChemConst bcm_;

		Matrix FastInv(const Matrix& m) const;
		void ImplicitHumanSys::SolveNonLinEq(
			Numerics::ISetEquations<Vector, Matrix>& eq, 
			Vector& u_0, double epsilon);
		std::array<double, 2> ImplicitHumanSys::AlvBloodTransferO2(double dt);
		std::array<double, 2> ImplicitHumanSys::AlvBloodTransferCo2(double dt);
		void  Alv1dNetTransfer(double dt) const;
	};
}

#endif /*IMPLICITHUMANSYSTEM_H_*/