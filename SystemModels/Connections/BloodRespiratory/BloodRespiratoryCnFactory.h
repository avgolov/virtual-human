#pragma once
#ifndef BLOODRespiratoryCNFACTORY_H_
#define BLOODRespiratoryCNFACTORY_H_

#include "../IConnectionFactory.h"
#include "../../Systems/BloodSystem/IBloodSys.h"
#include "../../Systems/RespiratorySystem/IRespiratorySys.h"
#include "IBloodRespiratoryCn.h"

namespace SystemModels
{
	class BloodRespiratoryCnFactory: 
		IConnectionFactory<IBloodSys,IRespiratorySys,IBloodRespiratoryCn>
	{
	
	public:
		//BloodRespiratoryCnFactory() {}

		std::shared_ptr<IBloodRespiratoryCn> Create(boost::shared_ptr<IBloodSys> system1, boost::shared_ptr<IRespiratorySys> system2) override;
	};
}

#endif /*BLOODRespiratoryCNFACTORY_H_*/
