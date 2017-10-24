#pragma once
#ifndef INERVOUSTISSUECN_H_
#define INERVOUSTISSUECN_H_

#include "../IConnection.h"
#include "../../Systems/NervousSystem/INervousSys.h"
#include "../../Systems/TissueSystem/ITissueSys.h"

namespace SystemModels
{
	class INervousTissueCn : public IConnection
	{
	public:
		INervousTissueCn(boost::shared_ptr<ITissueSys> tissueSys,
						 boost::shared_ptr<INervousSys> nervousSys) :
			tissueSys_(tissueSys),
			nervousSys_(nervousSys) {}
		virtual ~INervousTissueCn() {};

	protected:
		boost::shared_ptr<ITissueSys> tissueSys_;
		boost::shared_ptr<INervousSys> nervousSys_;
	};
}

#endif /*INERVOUSTISSUECN_H_*/

