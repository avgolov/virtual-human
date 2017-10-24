#include <boost/smart_ptr/make_shared.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "../../../tools/csv.h"

#include "../../../SystemModels/Systems/RespiratorySystem/NormAirEnv.h"
#include "../../../SystemModels/Systems/RespiratorySystem/LumpRespSys.h"
#include "../../../SystemModels/Systems/BloodSystem/CompartmentsBloodSys.h"
#include "../../../SystemModels/Systems/TissueSystem/BaseTissueSys.h"

#include "../../../SystemModels/Systems/NervousSystem/WorkLoadNervousSys.h"
#include "LlFbHipoxiaSysBuilder.h"
#include "../../../SystemModels/Systems/StandartHumanSys.h"

namespace Experiments
{
	void LlFbHipoxiaSysBuilder::CreateNewSystem() {
		auto weight = config_->get<double>("Model.Anthropometry.Weight");
		auto height = config_->get<double>("Model.Anthropometry.Height");
		auto air = boost::make_shared<NormAirEnv>();
		auto lVol = 2.5*height;
		auto RespSys = boost::make_shared<LumpRespSys>(lVol, 0.2, 2., air);
		auto ve = config_->get<double>("Model.Lungs.BreathVolume");
		auto vt = ve / config_->get<double>("Model.Lungs.BreathRate");
		RespSys->SetParam(Rate, ve / vt);
		RespSys->SetParam(RateSS, ve / vt);
		RespSys->SetParam(TidalVolumeSS, vt);
		RespSys->SetParam(TidalVolume, vt);

		auto bloodSys = boost::make_shared<CompartmentsBloodSys>();
		//0-O2; 1-CO2; 2-Hb; 3-HCO3-; 4-H+, 5-La
		bloodSys->SetSubstances({
			config_->get<double>("Model.Blood.Biochemistry.O2"),
			config_->get<double>("Model.Blood.Biochemistry.CO2"),
			config_->get<double>("Model.Blood.Biochemistry.HB"),
			config_->get<double>("Model.Blood.Biochemistry.HCO3"),
			config_->get<double>("Model.Blood.Biochemistry.H"),
			config_->get<double>("Model.Blood.Biochemistry.LA")
		});
		auto bloodVolume = 0.077*weight;
		bloodSys->SetParam(PulmArtVol, bloodVolume*0.048);
		bloodSys->SetParam(PulmVenVol, bloodVolume*0.04);
		bloodSys->SetParam(BrainArtVol, bloodVolume*0.02);
		bloodSys->SetParam(SysArtVol, bloodVolume*0.252);
		bloodSys->SetParam(SysVenVol, bloodVolume*0.64);

		auto co = 0.187*pow(weight, 0.81);
		co *= 10;
		bloodSys->SetParam(CardicOutput, co / 60.);
		bloodSys->SetParam(BrainFlow, 0.13*co / 60.);
		bloodSys->SetParam(TissueFlow, 0.87*co / 60.);
		bloodSys->SetParam(CardicOutputSS, co / 60.);
		bloodSys->SetParam(BrainFlowSS, 0.13*co / 60.);
		bloodSys->SetParam(TissueFlowSS, 0.87*co / 60.);

		auto tissueSys = boost::make_shared<BaseTissueSys>();
		auto o2Uptake = config_->get<double>("Model.Lungs.O2Uptake")/8;
		tissueSys->SetParam(BrainO2Uptake, o2Uptake*0.2 / 60.);
		tissueSys->SetParam(BrainCO2Dep, 0.85 * o2Uptake*0.2 / 60.);
		tissueSys->SetParam(OtherO2Uptake, o2Uptake*0.8 / 60.);
		tissueSys->SetParam(OtherCO2Dep, 0.85*o2Uptake*0.8 / 60.);
		auto system = boost::make_shared<StandartHumanSys>();
		system->SetRespiratorySys(RespSys);
		system->SetBloodSys(bloodSys);
		system->SetTissueSys(tissueSys);
		system->SetMethod();

		//Regulation model
		auto nervous = boost::make_shared<WorkLoadNervousSys>();
		nervous->SetLungsSys(RespSys);
		nervous->SetBloodSys(bloodSys);
		nervous->SetSteadyStateThreshold(config_->get<double>("NumericalMethod.SteadyStateTime"));
		nervous->SetUseExperiment(false);
		nervous->SetLungsNco2bs(config_->get<double>("Model.Regulation.Lungs.Nco2bs"));
		nervous->SetLungsTco2bs(config_->get<double>("Model.Regulation.Lungs.Tco2bs"));
		nervous->SetLungsNco2ps(config_->get<double>("Model.Regulation.Lungs.Nco2ps"));
		nervous->SetLungsTco2ps(config_->get<double>("Model.Regulation.Lungs.Tco2ps"));
		nervous->SetLungsPco20(config_->get<double>("Model.Regulation.Lungs.Pco20"));
		nervous->SetLungsPo20(config_->get<double>("Model.Regulation.Lungs.Po20"));
		system->SetNervousSys(nervous);

		system_ = system;
	}
}