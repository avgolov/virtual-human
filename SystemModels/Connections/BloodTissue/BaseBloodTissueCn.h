#pragma once
#ifndef BASEBLOODTISSUECN_H_
#define BASEBLOODTISSUECN_H_

#include <memory>
#include "IBloodTissueCn.h"

namespace SystemModels
{
	class BaseBloodTissueCn : public IBloodTissueCn
	{
	public:
		BaseBloodTissueCn(boost::shared_ptr<IBloodSys> bloodSys,
			              boost::shared_ptr<ITissueSys> tissueSys) :
			IBloodTissueCn(bloodSys, tissueSys) {}

		void Interact(TimeSpan* timeSpan) override;
	};
}

#endif /*BASEBLOODTISSUECN_H_*/
