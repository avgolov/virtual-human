#pragma once
#ifndef BODYBLOODTISSUECN_H_
#define BODYBLOODTISSUECN_H_

#include <memory>
#include "IBloodTissueCn.h"

namespace SystemModels
{
	class BodyBloodBaseTissueCn : public IBloodTissueCn
	{
	public:
		BodyBloodBaseTissueCn(boost::shared_ptr<IBloodSys> bloodSys,
							  boost::shared_ptr<ITissueSys> tissueSys) :
							  IBloodTissueCn(bloodSys, tissueSys) {}

		void Interact(TimeSpan* timeSpan) override;
	};
}

#endif /*BODYBLOODTISSUECN_H_*/

