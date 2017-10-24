#pragma once
#ifndef IBLOODTISSUECN_H_
#define IBLOODTISSUECN_H_

#include <boost/smart_ptr/shared_ptr.hpp>
#include "../IConnection.h"
#include "../../Systems/BloodSystem/IBloodSys.h"
#include "../../Systems/TissueSystem/ITissueSys.h"

namespace SystemModels
{
	class IBloodTissueCn : public IConnection
	{
	public:
		IBloodTissueCn(boost::shared_ptr<IBloodSys> bloodSys,
					   boost::shared_ptr<ITissueSys> tissueSys) :
			bloodSys_(bloodSys),
			tissueSys_(tissueSys) {}
		virtual ~IBloodTissueCn() {};

	protected:
		boost::shared_ptr<IBloodSys> bloodSys_;
		boost::shared_ptr<ITissueSys> tissueSys_;
	};
}

#endif /*IBLOODTISSUECN_H_*/

