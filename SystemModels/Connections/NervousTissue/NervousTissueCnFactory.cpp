#include "NervousTissueCnFactory.h"
#include "../../Systems/TissueSystem/BaseTissueSys.h"
#include "../../Systems/NervousSystem/BloodDivNervousSys.h"
#include "BaseNervousTissueCn.h"

namespace SystemModels
{
	std::shared_ptr<INervousTissueCn> NervousTissueCnFactory::Create(boost::shared_ptr<ITissueSys> system1, boost::shared_ptr<INervousSys> system2)
	{
		if (dynamic_cast<BaseTissueSys*>(system1.get()) &&
			dynamic_cast<BloodDivNervousSys*>(system2.get()))
		{
			return std::shared_ptr<INervousTissueCn>(new BaseNervousTissueCn(system1, system2));
		}
		return std::shared_ptr<INervousTissueCn>(nullptr);
	}
}