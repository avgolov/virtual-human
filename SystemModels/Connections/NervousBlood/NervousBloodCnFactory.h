#pragma once
#ifndef NERVOUSBLOODCNFACTORY_H_
#define NERVOUSBLOODCNFACTORY_H_

#include "../IConnectionFactory.h"
#include "../../Systems/BloodSystem/IBloodSys.h"
#include "../NervousBlood/INervousBloodCn.h"

namespace SystemModels
{
	class NervousBloodCnFactory :
		IConnectionFactory<IBloodSys, INervousSys, INervousBloodCn>
	{

	public:
		NervousBloodCnFactory() {}

		std::shared_ptr<INervousBloodCn> Create(boost::shared_ptr<IBloodSys> system1, boost::shared_ptr<INervousSys> system2) override;
	};
}

#endif /*NERVOUSBLOODCNFACTORY_H_*/

