#include <boost/smart_ptr/make_shared.hpp>

#include "HipoxiaHumanSysBuilder.h"
#include "../../SystemModels/Systems/RespiratorySystem/MiptNet1DRespSys.h"
#include "../../SystemModels/Systems/TissueSystem/BaseTissueSys.h"
#include "../../SystemModels/Systems/BloodSystem/BodyBioChemBloodSys.h"
#include "../../SystemModels/Systems/ImplicitHumanSys.h"
#include "../../SystemModels/Systems/BloodSystem/Biochemistry/BioChemConst.h"
#include "../../SystemModels/Systems/NervousSystem/BloodDivNervousSys.h"

namespace Experiments
{
	void HipoxiaHumanSysBuilder::CreateNewSystem()
	{
		auto RespSys = boost::make_shared<MiptNet1DRespSys>();
		RespSys->SetTree(tree_);
		RespSys->SetParam(Pg, 2.015);
		RespSys->SetParam(RateSS, 10);
		RespSys->SetParam(TidalVolumeSS, 500.);
		auto ve = 5000;
		auto vt = 152 * pow(ve*0.001, 0.683);
		RespSys->SetParam(Rate, ve / vt);
		RespSys->SetParam(TidalVolume, vt);

		auto bloodSys = boost::make_shared<CompartmentsBloodSys>();
		//0-O2; 1-CO2; 2-Hb; 3-HCO3-; 4-H+
		bloodSys->SetSubstances({ 0.00916*22.4, 0.02145*22.4, 2.51159444e-3, 2.039052e-2, 3.857988418e-8 });
		bloodSys->SetParam(PulmArtVol, 156.);
		bloodSys->SetParam(PulmVenVol, 808.);
		bloodSys->SetParam(BrainArtVol, 70.);
		bloodSys->SetParam(SysArtVol, 880.);
		bloodSys->SetParam(SysVenVol, 3100.);
		bloodSys->SetParam(CardicOutput, 6000. / 60.);
		bloodSys->SetParam(BrainFlow, 850. / 60.);
		bloodSys->SetParam(TissueFlow, (6000. - 850.) / 60.);
		bloodSys->SetParam(CardicOutputSS, 6000. / 60.);
		bloodSys->SetParam(BrainFlowSS, 850. / 60.);
		bloodSys->SetParam(TissueFlowSS, (6000. - 850.) / 60.);
		
		auto tissueSys = boost::make_shared<BaseTissueSys>();
		tissueSys->SetParam(BrainO2Uptake, 35. / 60.);
		tissueSys->SetParam(BrainCO2Dep, 28. / 60.);
		tissueSys->SetParam(OtherO2Uptake, 175. / 60.);
		tissueSys->SetParam(OtherCO2Dep, 140. / 60.);

		auto nervousSys = boost::make_shared<BloodDivNervousSys>();
		nervousSys->SetBloodSys(bloodSys);
		nervousSys->SetLungsSys(RespSys);

		auto system = boost::make_shared<ImplicitHumanSys>();
		system->SetRespiratorySys(RespSys);
		system->SetBloodSys(bloodSys);
		system->SetTissueSys(tissueSys);
		system->SetNervousSys(nervousSys);
		system->SetMethod();
		system_ = system;
	}
}