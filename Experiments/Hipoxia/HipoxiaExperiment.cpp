#include <windows.h>
#include <stdio.h>

#include "HipoxiaExperiment.h"
#include "HipoxiaHumanSysBuilder.h"
#include "../../SystemModels/Postprocessing/BasePostprocess.h"
#include "../../SystemModels/Postprocessing/TimeSavecondition.h"
#include "../../SystemModels/Postprocessing/PostprocessUtility.h"
#include "../../SystemModels/Systems/RespiratorySystem/MiptNet1DRespSys.h"
#include "../../MiptNet1D.Model/NetTopology/Results/TimeStoreCondition.h"
#include "../../MiptNet1D.Model/NetTopology/Results/BronchTreeResultHandler.h"
#include "../../MiptNet1D.Model/TreeBuilders/SimpleBronchTreeBuilder.h"
#include "../../MiptNet1D.Model/NetTopology/Results/BreathStoreCondition.h"
#include "../../MiptNet1D.Model/NetTopology/Results/TidalVolumeHandler.h"
#include "../../SystemModels/Systems/BloodSystem/CompartmentsBloodSys.h"
#include "../../MiptNet1D.Model/TreeBuilders/Lungs8TreeBuilder.h"


namespace Experiments
{
	void HipoxiaExperiment::Run()
	{
		auto result_path = "./results_" + name_ + "_16102016_";
		CreateDirectory(result_path.c_str(), nullptr);

		Lungs8TreeBuilder tree_builber(2e2, 3.95e-2, 5000.);
		auto tree = tree_builber.Build();
		HipoxiaHumanSysBuilder builder(&tree);
		builder.CreateNewSystem();
		auto human_sys = builder.GetHumanSystem();

		TimeSpan timeSpan;

		//Log description
		auto save_period = 0.1;
		PostprocessUtility postprocess;
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), O2Art, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), Co2Art, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), Po2Art, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), Po2AlvArt, result_path)); 
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), Po2AlvVen, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), Po2Ven, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), Po2Brain, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), Pco2Art, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), Pco2AlvArt, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), Pco2AlvVen, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), Pco2Ven, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), Pco2Brain, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), HArt, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), Hco3Art, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), HbArt, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), PhArt, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), SysArtO2Sat, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), CardicOutput, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), BrainFlow, result_path));
		postprocess.AddHandler(new BasePostprocess<IRespiratorySys, RespiratorySysParam>(new TimeSaveCondition(save_period), human_sys->GetRespiratorySys(), Ve, result_path));
		//Special bronch tree handler
		auto Resp_sys = dynamic_cast<MiptNet1DRespSys*>(human_sys->GetRespiratorySys().get());
		auto alveoles = Resp_sys->GetAlveoles();
		ResultHandlerUtility resultsUtility;
		resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), (*alveoles)[0], Conc_O2, result_path));
		resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), (*alveoles)[0], Conc_CO2, result_path));
		resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), (*alveoles)[0], Volume, result_path));
		resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), tree.get_nasopharynx()->get_first_point(), Velocity, result_path));
		resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), tree.get_branches()->back()->get_last_point(), Velocity, result_path));
		resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), tree.get_branches()->back()->get_last_point(), Square, result_path));
		resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), tree.get_branches()->back()->get_last_point(), Pressure, result_path));
		resultsUtility.add_handler(new TidalVolumeHandler(new BreathStoreCondition(&tree), &tree, result_path));
		Resp_sys->SetResultutility(resultsUtility);

		//Solution
		double save_time = 0.0;
		int qty = 0;
		auto bs = dynamic_cast<CompartmentsBloodSys*>(human_sys->GetBloodSys().get());
		bs->Compute(&timeSpan);
		while (timeSpan.timeCurr <= 1000.0)
		{
			auto dt = human_sys->GetTimeStep();
			timeSpan.SetDt(dt);
			human_sys->Compute(&timeSpan);
			save_time += timeSpan.dt;
			if (save_time >= 0.5) {
				std::cout << timeSpan.timeCurr << " " << qty << std::endl;
				save_time = 0.0;
			}
			qty++;
			postprocess.Handle(timeSpan.timeCurr);
		}
		auto aaa = 0;
	}
}