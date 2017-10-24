#pragma once
#ifndef HUMANSYSTEM_H_
#define HUMANSYSTEM_H_

#include "BloodSystem/IBloodSys.h"
#include "../Connections/BloodRespiratory/IBloodRespiratoryCn.h"
#include "../Connections/BloodTissue/IBloodTissueCn.h"
#include "../Connections/NervousBlood/INervousBloodCn.h"
#include "../Connections/NervousRespiratory/INervousRespiratoryCn.h"
#include "../Connections/NervousTissue/INervousTissueCn.h"
#include "IHumanSys.h"

namespace SystemModels
{
	//The model of whole body
	class HumanSys : public IHumanSys
	{
	public:
		HumanSys() {}

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
		boost::shared_ptr<IBloodSys> bloodSys_;
		boost::shared_ptr<IRespiratorySys> RespSys_;
		boost::shared_ptr<ITissueSys> tissueSys_;
		boost::shared_ptr<INervousSys> nervousSys_;

		//Connections
		std::shared_ptr<IBloodRespiratoryCn> bloodRespCn_;
		std::shared_ptr<IBloodTissueCn> bloodTissueCn_;
		std::shared_ptr<INervousBloodCn> nervousBloodCn_;
		std::shared_ptr<INervousRespiratoryCn> nervousRespCn_;
		std::shared_ptr<INervousTissueCn> nervousTissueCn_;
	};
}

#endif /*HUMANSYSTEM_H_*/