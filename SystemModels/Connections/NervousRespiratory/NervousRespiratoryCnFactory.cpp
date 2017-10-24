#include "NervousRespiratoryCnFactory.h"
#include "../../Systems/RespiratorySystem/MiptNet1DRespSys.h"
#include "../../Systems/NervousSystem/BloodDivNervousSys.h"
#include "BaseNervousRespiratoryCn.h"

namespace SystemModels
{
	std::shared_ptr<INervousRespiratoryCn> NervousRespiratoryCnFactory::Create(boost::shared_ptr<IRespiratorySys> system1, boost::shared_ptr<INervousSys> system2)
	{
		if (dynamic_cast<MiptNet1DRespSys*>(system1.get()) &&
			dynamic_cast<BloodDivNervousSys*>(system2.get()))
		{
			return std::shared_ptr<INervousRespiratoryCn>(new BaseNervousRespiratoryCn(system1, system2));
		}
		return std::shared_ptr<INervousRespiratoryCn>(nullptr);
	}
}