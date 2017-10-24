#pragma once
#ifndef IBLOODRespiratoryCN_H_
#define IBLOODRespiratoryCN_H_

#include "../IConnection.h"
#include "../../Systems/BloodSystem/IBloodSys.h"
#include "../../Systems/RespiratorySystem/IRespiratorySys.h"
#include <boost/smart_ptr/shared_ptr.hpp>

namespace SystemModels
{
	class IBloodRespiratoryCn : public IConnection
	{
	public:
		IBloodRespiratoryCn(boost::shared_ptr<IBloodSys> bloodSys, 
			                boost::shared_ptr<IRespiratorySys> RespSys):
		    bloodSys_(bloodSys),
			RespSys_(RespSys){}
		virtual ~IBloodRespiratoryCn() {};

	protected:
		boost::shared_ptr<IBloodSys> bloodSys_;
		boost::shared_ptr<IRespiratorySys> RespSys_;
	};
}

#endif /*IBLOODRespiratoryCN_H_*/
