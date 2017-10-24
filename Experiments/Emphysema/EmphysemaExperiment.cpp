#include <windows.h>
#include <stdio.h>

#include "EmphysemaExperiment.h"
#include "EmphysemaHumanSysBuilder.h"
#include "../../SystemModels/Postprocessing/BasePostprocess.h"
#include "../../SystemModels/Postprocessing/TimeSavecondition.h"
#include "../../SystemModels/Postprocessing/PostprocessUtility.h"
#include "../../SystemModels/Systems/RespiratorySystem/MiptNet1DRespSys.h"
#include "../../MiptNet1D.Model/NetTopology/Results/TimeStoreCondition.h"
#include "../../MiptNet1D.Model/NetTopology/Results/BronchTreeResultHandler.h"
#include "../../MiptNet1D.Model/NetTopology/Results/BreathStoreCondition.h"
#include "../../MiptNet1D.Model/NetTopology/Results/TidalVolumeHandler.h"
#include "../../MiptNet1D.Model/TreeBuilders/Lungs8TreeBuilder.h"
#include "../../MiptNet1D.Model/TreeBuilders/SimpleBronchTreeBuilder.h"

namespace Experiments
{
	void EmphysemaExperiment::Run()
	{
		for (auto i = 0; i < 15; i++)
		{
			auto s = 1 - i*0.1*0.5;
			auto result_path = "./results_" + name_ + "_01092015_" + std::to_string(i);
			CreateDirectory(result_path.c_str(), nullptr);

			Lungs8TreeBuilder tree_builber(1.8e2, 2.0e-2, 5000.);// (20, 2.85e-3, 5000.);//(2e-4, 0.0026, 5000.);//(2e2, 3.95e-2, 5000.);
			auto tree = tree_builber.Build();
			ModifyBronchTree(&tree, s);

			EmphysemaHumanSysBuilder builder(&tree);
			builder.CreateNewSystem();
			auto human_sys = builder.GetHumanSystem();
			
			TimeSpan timeSpan;

			//Log description
			auto save_period = 0.01;
			PostprocessUtility postprocess;
			postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), O2Art, result_path));
			postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), Co2Art, result_path));
			
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
			resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), tree.get_branches()->front()->get_last_point(), Velocity, result_path));
			resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), tree.get_branches()->front()->get_last_point(), Square, result_path));
			resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), tree.get_branches()->front()->get_last_point(), Pressure, result_path));
			resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), tree.get_branches()->front()->get_first_point(), Velocity, result_path));
			resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), tree.get_branches()->front()->get_first_point(), Square, result_path));
			resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), tree.get_branches()->front()->get_first_point(), Pressure, result_path));
			resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), tree.get_branches()->front()->get_second_point(), Velocity, result_path));
			resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), tree.get_branches()->front()->get_second_point(), Square, result_path));
			resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), tree.get_branches()->front()->get_second_point(), Pressure, result_path));
			resultsUtility.add_handler(new TidalVolumeHandler(new BreathStoreCondition(&tree), &tree, result_path));
			Resp_sys->SetResultutility(resultsUtility);

			//Solution
			double save_time = 0.0;
			int qty = 0;
			while (timeSpan.timeCurr <= 250.0)
			{
				auto dt = human_sys->GetTimeStep();
				timeSpan.SetDt(dt);
				human_sys->Compute(&timeSpan);
				save_time += timeSpan.dt;
				if (save_time >= 0.5) {
					std::cout << timeSpan.timeCurr << " " << qty << " " << s << std::endl;
					save_time = 0.0;
				}
				qty++;
				postprocess.Handle(timeSpan.timeCurr);
			}
		}
	}

	void EmphysemaExperiment::ModifyBronchTree(BronchTree* tree, double s) const
	{
		auto alveoles = tree->get_alveoles();
		for(auto alveole: *alveoles)
		{
			alveole->C *= s;
		}
		auto branches = tree->get_branches();
		for(auto branch : std::vector<Branch*>(branches->begin(), branches->end()))
		{
			auto points = branch->get_points();
			for(auto point : *points)
			{
				point->Cwall *= 0.8;
			}
		}
	}
}