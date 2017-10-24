#include <boost/smart_ptr/make_shared.hpp>
#include "HumanSysBuilder.h"
#include "../Systems/HumanSys.h"
#include "../Systems/RespiratorySystem/MiptNet1DRespSys.h"
#include "../Systems/TissueSystem/BaseTissueSys.h"
#include "../Systems/NervousSystem/BloodDivNervousSys.h"
#include "../../MiptNet1D.Model/NetTopology/Results/BronchTreeResultHandler.h"
#include "../../MiptNet1D.Model/NetTopology/Results/TimeStoreCondition.h"
#include "../../MiptNet1D.Model/NetTopology/Results/TidalVolumeHandler.h"
#include "../../MiptNet1D.Model/NetTopology/Results/BreathStoreCondition.h"
#include "../../MiptNet1D.Model/TreeBuilders/SimpleBronchTreeBuilder.h"


namespace SystemModels
{
	void HumanSysBuilder::CreateNewSystem()
	{
		//auto RespSys = boost::make_shared<MiptNet1DRespSys>();
		//SimpleBronchTreeBuilder tree_builber(0.2, 1., 5000.);
		//RespSys->SetTree(tree_builber.Build());
		//RespSys->SetParam(RateSS, 10);
		//RespSys->SetParam(TidalVolumeSS, 500.);
		//RespSys->SetParam(Rate, 10);
		//RespSys->SetParam(TidalVolume, 500.);
		//ResultHandlerUtility resultsUtility;
		//auto alveoles = RespSys->GetAlveoles();
		//auto tree = RespSys->GetTree();
		//auto result_path = "./results";
		//resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(0.1), (*alveoles)[0], Conc_O2, result_path));
		//resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(0.1), (*alveoles)[0], Conc_CO2, result_path));
		//resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(0.1), tree->get_nasopharynx()->get_first_point(), Velocity, result_path));
		//resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(0.1), tree->get_branches()->back()->get_last_point(), Velocity, result_path));
		//resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(0.1), tree->get_branches()->back()->get_last_point(), Square, result_path));
		//resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(0.1), (*tree->get_branches())[2]->get_last_point(), Velocity, result_path));
		//resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(0.1), (*tree->get_branches())[2]->get_last_point(), Square, result_path));
		//resultsUtility.add_handler(new TidalVolumeHandler(new BreathStoreCondition(tree), tree, result_path));
		//RespSys->SetResultutility(resultsUtility);
		//
		//auto bloodSys = boost::make_shared<AvgTanksBloodSys>();
		//bloodSys->SetParam(PulmArtVol, 156.);
		//bloodSys->SetParam(PulmVenVol, 808.);
		//bloodSys->SetParam(BrainArtVol, 70.);
		//bloodSys->SetParam(SysArtVol, 880.);
		//bloodSys->SetParam(SysVenVol, 3100.);
		//bloodSys->SetParam(CardicOutput, 6000. / 60.);
		//bloodSys->SetParam(BrainFlow, 750. / 60.);
		//bloodSys->SetParam(TissueFlow, (6000. - 750.) / 60.);
		//bloodSys->SetParam(CardicOutputSS, 6000. / 60.);
		//bloodSys->SetParam(BrainFlowSS, 750. / 60.);
		//bloodSys->SetParam(TissueFlowSS, (6000. - 750.) / 60.);
		//bloodSys->SetSubstances({ 0., 0.,2.27e-3, 21.45e-3, 21.45e-3 });
		//bloodSys->SetParam(O2AlvArt, 0.15);
		//bloodSys->SetParam(Co2AlvArt, 0.52);
		//bloodSys->SetParam(O2AlvVen, 0.2);
		//bloodSys->SetParam(Co2AlvVen, 0.48);
		//bloodSys->SetParam(O2Brain, 0.2);
		//bloodSys->SetParam(Co2Brain, 0.48);
		//bloodSys->SetParam(O2Art, 0.2);
		//bloodSys->SetParam(Co2Art, 0.48);
		//bloodSys->SetParam(O2Ven, 0.15);
		//bloodSys->SetParam(Co2Ven, 0.52);
		//
		//bloodSys->SetParam(Po2Art, 98.1);
		//bloodSys->SetParam(Pco2Art,40);
		//bloodSys->SetParam(Po2Ven,40);
		//bloodSys->SetParam(Pco2Ven,46);
		//bloodSys->SetParam(Po2Brain,37);
		//bloodSys->SetParam(Pco2Brain,49);
		//bloodSys->SetParam(Po2AlvArt,40.0);
		//bloodSys->SetParam(Pco2AlvArt,46);
		//bloodSys->SetParam(Po2AlvVen, 98.1);
		//bloodSys->SetParam(Pco2AlvVen,40);
		//auto tissueSys = boost::make_shared<BaseTissueSys>();
		//tissueSys->SetParam(BrainO2Uptake, 1.0*45/60.);
		//tissueSys->SetParam(BrainCO2Dep, 1.0 * 37/60.);
		//tissueSys->SetParam(OtherO2Uptake, 1.0*205./60.);
		//tissueSys->SetParam(OtherCO2Dep, 1.0*170./60.);
		//
		//auto nervousSys = boost::make_shared<BloodDivNervousSys>();

		//auto system = boost::make_shared<HumanSys>();
		//system->SetRespiratorySys(RespSys);
		//system->SetBloodSys(bloodSys);
		//system->SetTissueSys(tissueSys);
		////system->SetNervousSys(nervousSys);
		//system_ = system;
	};
}