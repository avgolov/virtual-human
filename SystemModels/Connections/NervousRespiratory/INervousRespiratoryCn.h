#pragma once
#ifndef INERVOUSRespiratoryCN_H_
#define INERVOUSRespiratoryCN_H_

#include "../IConnection.h"
#include "../../Systems/NervousSystem/INervousSys.h"
#include "../../Systems/RespiratorySystem/IRespiratorySys.h"

namespace SystemModels
{
	class INervousRespiratoryCn : public IConnection
	{
	public:
		INervousRespiratoryCn(boost::shared_ptr<IRespiratorySys> RespSys,
			                  boost::shared_ptr<INervousSys> nervousSys) :
			RespSys_(RespSys),
			nervousSys_(nervousSys) {}
		virtual ~INervousRespiratoryCn() {};

	protected:
		boost::shared_ptr<IRespiratorySys> RespSys_;
		boost::shared_ptr<INervousSys> nervousSys_;
	};
}

#endif /*INERVOUSRespiratoryCN_H_*/


