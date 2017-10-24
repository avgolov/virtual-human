#pragma once
#ifndef NERVOUSRespiratoryCNFACTORY_H_
#define NERVOUSRespiratoryCNFACTORY_H_

#include "../IConnectionFactory.h"
#include "INervousRespiratoryCn.h"

namespace SystemModels
{
	class NervousRespiratoryCnFactory :
		IConnectionFactory<IRespiratorySys, INervousSys, INervousRespiratoryCn>
	{

	public:
		NervousRespiratoryCnFactory() {}

		std::shared_ptr<INervousRespiratoryCn> Create(boost::shared_ptr<IRespiratorySys> system1, boost::shared_ptr<INervousSys> system2) override;
	};
}

#endif /*NERVOUSRespiratoryCNFACTORY_H_*/


