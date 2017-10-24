#include <windows.h>
#include <stdio.h>

#include "AsthmaExperiment.h"
#include "AsthmaHumanSysBuilder.h"
#include "../../SystemModels/Postprocessing/BasePostprocess.h"
#include "../../SystemModels/Postprocessing/TimeSavecondition.h"
#include "../../SystemModels/Postprocessing/PostprocessUtility.h"
#include "../../SystemModels/Systems/RespiratorySystem/MiptNet1DRespSys.h"
#include "../../MiptNet1D.Model/NetTopology/Results/TimeStoreCondition.h"
#include "../../MiptNet1D.Model/NetTopology/Results/BronchTreeResultHandler.h"
#include "../../MiptNet1D.Model/TreeBuilders/SimpleBronchTreeBuilder.h"
#include "../../MiptNet1D.Model/NetTopology/Results/BreathStoreCondition.h"
#include "../../MiptNet1D.Model/NetTopology/Results/TidalVolumeHandler.h"
#include "../../MiptNet1D.Model/NetTopology/Results/OxyConsResultHandler.h"
#include "../../MiptNet1D.Model/TreeBuilders/Lungs8TreeBuilder.h"
#include "../../MiptNet1D.Model/TreeBuilders/LungsNTreeBuilder.h"

namespace Experiments
{
	void AsthmaExperiment::Run()
	{
		for (auto i = 0; i < 1; i++)
		{
			auto s = 1 - i*0.1*0.5;
			auto result_path = "./results_" + name_ +  std::to_string(i);
			CreateDirectory(result_path.c_str(), nullptr);

			Lungs8TreeBuilder tree_builber(2e2, 3.95e-2, 5000.);
			//LungsNTreeBuilder tree_builber(2e2, 3.95e-2, 5000., 5);
			auto tree = tree_builber.Build();
			ModifyBronchTree(&tree, s);

			AsthmaHumanSysBuilder builder(&tree);
			builder.CreateNewSystem();
			auto human_sys = builder.GetHumanSystem();
			
			TimeSpan timeSpan;

			//Log description
			auto save_period = 0.1;
			PostprocessUtility postprocess;
			postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), O2Art, result_path));
			postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), Co2Art, result_path));
			
			//Special bronch tree handler
			auto Resp_sys = dynamic_cast<MiptNet1DRespSys*>(human_sys->GetRespiratorySys().get());
			auto alveoles = Resp_sys->GetAlveoles();
			ResultHandlerUtility resultsUtility;
			resultsUtility.add_handler(new OxyConsResultHandler(new BreathStoreCondition(&tree), tree.get_nasopharynx()->get_first_point(), result_path));
			for(int a=0; a < alveoles->size(); a++)
			{
				resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), (*alveoles)[a], Conc_O2, result_path));
				resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), (*alveoles)[a], Conc_CO2, result_path));
				resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), (*alveoles)[a], Volume, result_path));
			}
			
			resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), tree.get_nasopharynx()->get_first_point(), Velocity, result_path));
			resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), tree.get_nasopharynx()->get_first_point(), Square, result_path));
			resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), tree.get_branches()->back()->get_last_point(), Velocity, result_path));
			resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), tree.get_branches()->back()->get_last_point(), Square, result_path));
			resultsUtility.add_handler(new BronchTreeResultHandler(new TimeStoreCondition(save_period), tree.get_branches()->back()->get_last_point(), Pressure, result_path));
					
			resultsUtility.add_handler(new TidalVolumeHandler(new BreathStoreCondition(&tree), &tree, result_path));
			Resp_sys->SetResultutility(resultsUtility);

			//Solution
			double save_time = 0.0;
			int qty = 0;
			time_t begin, end;
			time(&begin);
			while (timeSpan.timeCurr <= 60.0)
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
			time(&end);
			double difference = difftime(end, begin);
			int aaa = 0;
		}
	}

	void AsthmaExperiment::ModifyBronchTree(BronchTree* tree, double s) const
	{
		auto alveoles = tree->get_alveoles();
		for(auto alveole: *alveoles)
		{
			auto points = alveole->get_branch()->get_points();
			for(auto i =0;i<points->size();i++)
			{
				auto point = (*points)[i];
				auto coef = 0.;
				if (i==0|| i==4)
				{
					coef = (1 - (1. - s)*0.10) / s;
				}
				else if(i == 1 || i == 3)
				{
					coef = (1-(1.-s)*0.90)/s;
				}
				else//15,90-10.3 -o2!!!! 0.5,0.5-co214,5
				{
					coef = 1.;
				}
				point->Square = coef*s*point->Square;
				point->SquareInit = coef*s*point->SquareInit;
				point->SquarePrev = coef*s*point->SquarePrev;
			}
			
			/*for(auto point : *points)
			{
				point->Square = s*point->Square;
				point->SquareInit = s*point->SquareInit;
				point->SquarePrev = s*point->SquarePrev;
			}*/
			/*auto branch = alveole->get_branch();
			auto last_point = branch->get_last_point();
			auto second2last_point = branch->get_last_point();
			last_point->Square *= s;
			last_point->SquareInit *= s;
			last_point->SquarePrev *= s;
			second2last_point->Square *= s;
			second2last_point->SquareInit *= s;
			second2last_point->SquarePrev *= s;*/
		}
		auto branches = tree->get_branches();
		for(auto branch : *branches)
		{
			auto points = branch->get_points();
			for(auto point : *points)
			{
				point->Cwall *= 0.1;
			}
		}
	}
}