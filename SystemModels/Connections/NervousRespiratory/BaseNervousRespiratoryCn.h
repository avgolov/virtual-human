#pragma once
#ifndef BASENERVOUSRespiratoryCN_H_
#define BASENERVOUSRespiratoryCN_H_

#include "INervousRespiratoryCn.h"

namespace SystemModels
{
	class BaseNervousRespiratoryCn : public INervousRespiratoryCn
	{
	public:
		BaseNervousRespiratoryCn(boost::shared_ptr<IRespiratorySys> RespSys,
			                     boost::shared_ptr<INervousSys> nervousSys) :
			INervousRespiratoryCn(RespSys, nervousSys){}
		
		void Interact(TimeSpan* timeSpan) override
		{
			nervousSys_->ControlRespiratorySys(timeSpan);
		}
	};
}

#endif /*BASENERVOUSRespiratoryCN_H_*/
