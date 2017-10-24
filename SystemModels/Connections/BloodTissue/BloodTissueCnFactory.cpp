#include "BloodTissueCnFactory.h"
#include "BaseBloodTissueCn.h"
#include "BodyBloodBaseTissueCn.h"
#include "../../Systems/TissueSystem/BaseTissueSys.h"
#include "../../Systems/BloodSystem/BodyBloodSys.h"
#include "../../Systems/BloodSystem/BodyBioChemBloodSys.h"
#include "BodyBioChemTissueCn.h"


namespace SystemModels
{
	std::shared_ptr<IBloodTissueCn> BloodTissueCnFactory::Create(boost::shared_ptr<IBloodSys> system1, boost::shared_ptr<ITissueSys> system2)
	{
		if (dynamic_cast<BodyBloodSys*>(system1.get()) &&
			dynamic_cast<BaseTissueSys*>(system2.get()))
		{
			return std::static_pointer_cast<IBloodTissueCn>(std::make_shared<BodyBloodBaseTissueCn>(system1, system2));
		}

		if (dynamic_cast<BodyBioChemBloodSys*>(system1.get()) &&
			dynamic_cast<BaseTissueSys*>(system2.get()))
		{
			return std::static_pointer_cast<IBloodTissueCn>(std::make_shared<BodyBioChemTissueCn>(system1, system2));
		}
		
		return std::shared_ptr<IBloodTissueCn>(nullptr);
	}
}
