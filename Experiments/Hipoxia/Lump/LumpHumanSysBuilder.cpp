#include <boost/smart_ptr/make_shared.hpp>
#include "LumpHumanSysBuilder.h"
#include "../../../SystemModels/Systems/RespiratorySystem/LumpRespSys.h"
#include "../../../SystemModels/Systems/RespiratorySystem/NormAirEnv.h"
#include "../../../SystemModels/Systems/BloodSystem/BodyBioChemBloodSys.h"
#include "../../../SystemModels/Systems/TissueSystem/BaseTissueSys.h"
#include "../../../SystemModels/Systems/LumpImplicitHumanSys.h"


namespace Experiments
{
	void LumpHumanSysBuilder::CreateNewSystem()
	{
		auto air = boost::make_shared<NormAirEnv>();
		auto RespSys = boost::make_shared<LumpRespSys>(6000, 0.2, 2., air);
		auto ve = 5000;
		auto vt = 152*pow(ve*0.001, 0.683);
		RespSys->SetParam(Rate, ve/vt);
		RespSys->SetParam(RateSS, ve / vt);
		RespSys->SetParam(TidalVolumeSS, vt);
		RespSys->SetParam(TidalVolume, vt);

		auto bloodSys = boost::make_shared<BodyBioChemBloodSys>();
		//0-O2; 1-CO2; 2-Hb; 3-HCO3-; 4-H+
		bloodSys->SetSubstances({ 0.00916*22.4, 0.02145*22.4, 2.51159444e-3, 2.039052e-2, 3.857988418e-8 });
		bloodSys->SetParam(AllVol, 5000.);
		auto tissueSys = boost::make_shared<BaseTissueSys>();


		tissueSys->SetParam(OtherO2Uptake, 280. / 60.);
		tissueSys->SetParam(OtherCO2Dep, 230. / 60.);

		auto system = boost::make_shared<LumpImplicitHumanSys>();
		system->SetRespiratorySys(RespSys);
		system->SetBloodSys(bloodSys);
		system->SetTissueSys(tissueSys);
		system_ = system;
	}
}