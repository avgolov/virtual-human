#include <boost/smart_ptr/make_shared.hpp>

#include "EmphysemaHumanSysBuilder.h"
#include "../../SystemModels/Systems/RespiratorySystem/MiptNet1DRespSys.h"
#include "../../SystemModels/Systems/TissueSystem/BaseTissueSys.h"
#include "../../SystemModels/Systems/HumanSys.h"
#include "../../SystemModels/Systems/BloodSystem/BodyBloodSys.h"

using namespace SystemModels;

namespace Experiments
{
	void EmphysemaHumanSysBuilder::CreateNewSystem()
	{
		auto RespSys = boost::make_shared<MiptNet1DRespSys>();
		RespSys->SetTree(tree_);
		RespSys->SetParam(RateSS, 10);
		RespSys->SetParam(TidalVolumeSS, 500.);
		RespSys->SetParam(Rate, 10);
		RespSys->SetParam(TidalVolume, 500.);
		RespSys->SetParam(Pg, 2.3);
		//RespSys->SetParam(Pg, 3.15e5); //(Pg, 3.15e5);

		auto bloodSys = boost::make_shared<BodyBloodSys>();
		bloodSys->SetSubstances({ 0.,0. });
		bloodSys->SetParam(AllVol, 4000.);
		bloodSys->SetParam(O2Art, 0.19);
		bloodSys->SetParam(Co2Art, 0.5);

		auto tissueSys = boost::make_shared<BaseTissueSys>();
		tissueSys->SetParam(OtherO2Uptake, 150. / 60.);
		tissueSys->SetParam(OtherCO2Dep, 100. / 60.);

		auto system = boost::make_shared<HumanSys>();
		system->SetRespiratorySys(RespSys);
		system->SetBloodSys(bloodSys);
		system->SetTissueSys(tissueSys);
		system_ = system;
	}
	
}