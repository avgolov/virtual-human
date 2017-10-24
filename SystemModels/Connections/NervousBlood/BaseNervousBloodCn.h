#pragma once
#ifndef BASENERVOUSRespiratoryCN_H_
#define BASENERVOUSRespiratoryCN_H_

#include "INervousBloodCn.h"

namespace SystemModels
{
	class BaseNervousBloodCn : public INervousBloodCn
	{
	public:
		BaseNervousBloodCn(boost::shared_ptr<IBloodSys> bloodSys,
			               boost::shared_ptr<INervousSys> nervousSys) :
			INervousBloodCn(bloodSys, nervousSys) {}

		void Interact(TimeSpan* timeSpan) override
		{
			nervousSys_->ControlBloodSys(timeSpan);
		}
	};
}

#endif /*BASENERVOUSRespiratoryCN_H_*/
