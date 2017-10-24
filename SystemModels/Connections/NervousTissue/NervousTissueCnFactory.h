#pragma once
#ifndef NERVOUSTISSUECNFACTORY_H_
#define NERVOUSTISSUECNFACTORY_H_

#include "../IConnectionFactory.h"
#include "INervousTissueCn.h"

namespace SystemModels
{
	class NervousTissueCnFactory :
		IConnectionFactory<ITissueSys, INervousSys, INervousTissueCn>
	{

	public:
		NervousTissueCnFactory() {}

		std::shared_ptr<INervousTissueCn> Create(boost::shared_ptr<ITissueSys> system1, boost::shared_ptr<INervousSys> system2) override;
	};
}

#endif /*NERVOUSTISSUECNFACTORY_H_*/

