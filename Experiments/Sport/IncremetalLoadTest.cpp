#include <windows.h>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "../../SystemModels/Postprocessing/BasePostprocess.h"
#include "../../SystemModels/Postprocessing/TimeSavecondition.h"
#include "../../SystemModels/Postprocessing/PostprocessUtility.h"
#include "../../SystemModels/Systems/RespiratorySystem/MiptNet1DRespSys.h"
#include "../../SystemModels/Systems/BloodSystem/CompartmentsBloodSys.h"
#include "IncremetalLoadTest.h"
#include "IncremetalLoadSysBuilder.h"
#include "../../SystemModels/Postprocessing/OxyConsPostprocess.h"
#include "../../SystemModels/Postprocessing/DioxProdPostprocess.h"
#include "../../SystemModels/Postprocessing/HipoxiaPostprocess.h"


namespace Experiments
{
	void IncremetalLoadTest::Run()
	{
		boost::property_tree::ptree config;
		read_json("config.json", config);
		auto result_path = config.get<std::string>("Postprocessing.SavePath");
		CreateDirectory(result_path.c_str(), nullptr);

		IncremetalLoadSysBuilder builder(&config);
		builder.CreateNewSystem();
		auto human_sys = builder.GetHumanSystem();

		TimeSpan timeSpan;

		//Log description
		auto save_period = config.get<double>("Postprocessing.SavePeriod");
		auto bs = dynamic_cast<CompartmentsBloodSys*>(human_sys->GetBloodSys().get());
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
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), LaArt, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), CardicOutput, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), BrainFlow, result_path));
		postprocess.AddHandler(new BasePostprocess<IRespiratorySys, RespiratorySysParam>(new TimeSaveCondition(save_period), human_sys->GetRespiratorySys(), Ve, result_path));
		postprocess.AddHandler(new OxyConsPostprocess(new TimeSaveCondition(save_period), boost::shared_ptr<CompartmentsBloodSys>(bs), result_path));
		postprocess.AddHandler(new DioxProdPostprocess(new TimeSaveCondition(save_period), boost::shared_ptr<CompartmentsBloodSys>(bs), result_path));
		postprocess.AddHandler(new HipoxiaPostprocess(new TimeSaveCondition(save_period), boost::shared_ptr<CompartmentsBloodSys>(bs), result_path));

		//Solution
		int qty = 0;
		/*timeSpan.SetDt(0.01);
		human_sys->Compute(&timeSpan);
		auto bs = dynamic_cast<CompartmentsBloodSys*>(human_sys->GetBloodSys().get());
		auto massold = bs->Mass;*/
		auto computeTime = config.get<double>("NumericalMethod.Time") +
			               config.get<double>("NumericalMethod.SteadyStateTime");
		
		auto dt = 0.01;// config.get<double>("NumericalMethod.TimeStep");
		
		while (timeSpan.timeCurr <= computeTime)
		{
			timeSpan.SetDt(dt);
			human_sys->Compute(&timeSpan);
			 
			auto val = (1 + 1.e-6)*timeSpan.timeCurr / save_period;
			if (val-int(val) < 0.001) {
				std::cout << timeSpan.timeCurr << " " << qty << std::endl;
			}
			qty++;
			postprocess.Handle(timeSpan.timeCurr);
		}
		//auto massnw = massold-bs->Mass;
		auto aaa = 0;
	}
}