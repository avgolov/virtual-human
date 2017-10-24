#include "BloodRespiratoryCnFactory.h"
#include "Mipt1DRespAvgTankBloodCn.h"
#include "Mipt1DRespBodyBloodCn.h"
#include "../../Systems/RespiratorySystem/MiptNet1DRespSys.h"
#include "../../Systems/BloodSystem/BodyBloodSys.h"
#include "../../Systems/BloodSystem/BodyBioChemBloodSys.h"
#include "Mipt1DRespBodyBioChem.h"


namespace SystemModels
{
	std::shared_ptr<IBloodRespiratoryCn> BloodRespiratoryCnFactory::Create(boost::shared_ptr<IBloodSys> system1,
		                                                                 boost::shared_ptr<IRespiratorySys> system2)
	{
		if (dynamic_cast<BodyBloodSys*>(system1.get()) &&
			dynamic_cast<MiptNet1DRespSys*>(system2.get()))
		{
			return std::static_pointer_cast<IBloodRespiratoryCn>(std::make_shared<Mipt1DRespBodyBloodCn>(system1, system2));
		}

		if (dynamic_cast<BodyBioChemBloodSys*>(system1.get()) &&
			dynamic_cast<MiptNet1DRespSys*>(system2.get()))
		{
			return std::static_pointer_cast<IBloodRespiratoryCn>(std::make_shared<Mipt1DRespBodyBioChemCn>(system1, system2));
		}
		
		return std::shared_ptr<IBloodRespiratoryCn>(nullptr);
	}
}
