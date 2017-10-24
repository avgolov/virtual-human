#pragma once
#ifndef INERVOUSBLOODCN_H_
#define INERVOUSBLOODCN_H_

#include "../IConnection.h"
#include "../../Systems/BloodSystem/IBloodSys.h"
#include "../../Systems/NervousSystem/INervousSys.h"

namespace SystemModels
{
	class INervousBloodCn : public IConnection
	{
	public:
		INervousBloodCn(boost::shared_ptr<IBloodSys> bloodSys,
						boost::shared_ptr<INervousSys> nervousSys) :
			bloodSys_(bloodSys),
			nervousSys_(nervousSys) {}
		virtual ~INervousBloodCn() {};

	protected:
		boost::shared_ptr<IBloodSys> bloodSys_;
		boost::shared_ptr<INervousSys> nervousSys_;
	};
}

#endif /*INERVOUSBLOODCN_H_*/


