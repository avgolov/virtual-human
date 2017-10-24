#pragma once
#ifndef IHUMANSYSTEM_H_
#define IHUMANSYSTEM_H_

#include <memory>
#include "ISystem.h"
#include "RespiratorySystem/IRespiratorySys.h"
#include "TissueSystem/ITissueSys.h"
#include "NervousSystem/INervousSys.h"
#include "BloodSystem/IBloodSys.h"

namespace SystemModels
{
	//The model of whole body
	class IHumanSys : public ISystem
	{
	public:
		IHumanSys(){}
		
		virtual ~IHumanSys() {};

		virtual void SetTissueSys(boost::shared_ptr<ITissueSys> tissueSys) = 0;
		virtual void SetRespiratorySys(boost::shared_ptr<IRespiratorySys> RespSys) = 0;
		virtual void SetBloodSys(boost::shared_ptr<IBloodSys> bloodSys) = 0;
		virtual void SetNervousSys(boost::shared_ptr<INervousSys> nervousSys) = 0;
		virtual boost::shared_ptr<ITissueSys> GetTissueSys() = 0;
		virtual boost::shared_ptr<IRespiratorySys> GetRespiratorySys() = 0;
		virtual boost::shared_ptr<IBloodSys> GetBloodSys() = 0;
		virtual boost::shared_ptr<INervousSys> GetNervousSys() = 0;
	};
}

#endif /*IHUMANSYSTEM_H_*/