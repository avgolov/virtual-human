#include "NervousBloodCnFactory.h"
#include "../../Systems/NervousSystem/BloodDivNervousSys.h"
#include "BaseNervousBloodCn.h"

namespace SystemModels
{
	std::shared_ptr<INervousBloodCn> NervousBloodCnFactory::Create(boost::shared_ptr<IBloodSys> system1, boost::shared_ptr<INervousSys> system2)
	{
		
		return std::shared_ptr<INervousBloodCn>(nullptr);
	}
}