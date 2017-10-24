#pragma once
#ifndef BLOODRTISSUECNFACTORY_H_
#define BLOODRTISSUECNFACTORY_H_

#include "../IConnectionFactory.h"
#include "../../Systems/BloodSystem/IBloodSys.h"
#include "IBloodTissueCn.h"

namespace SystemModels
{
	class BloodTissueCnFactory :
		IConnectionFactory<IBloodSys, ITissueSys, IBloodTissueCn>
	{

	public:
		BloodTissueCnFactory() {}

		std::shared_ptr<IBloodTissueCn> Create(boost::shared_ptr<IBloodSys> system1, boost::shared_ptr<ITissueSys> system2) override;
	};
}

#endif /*BLOODRTISSUECNFACTORY_H_*/

