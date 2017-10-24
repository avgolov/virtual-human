#pragma once
#ifndef BASENERVOUSTISSUECN_H_
#define BASENERVOUSTISSUECN_H_

#include "INervousTissueCn.h"

namespace SystemModels
{
	class BaseNervousTissueCn : public INervousTissueCn
	{
	public:
		BaseNervousTissueCn(boost::shared_ptr<ITissueSys> tissueSys,
			                boost::shared_ptr<INervousSys> nervousSys) :
			INervousTissueCn(tissueSys, nervousSys) {}

		void Interact(TimeSpan* timeSpan) override
		{
			nervousSys_->ControlTissueSys(timeSpan);
		}
	};
}

#endif /*BASENERVOUSTISSUECN_H_*/

