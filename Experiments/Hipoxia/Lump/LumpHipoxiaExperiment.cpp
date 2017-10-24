#include <windows.h>
#include <stdio.h>

#include "LumpHipoxiaExperiment.h"
#include "LumpHumanSysBuilder.h"
#include "../../../SystemModels/Postprocessing/PostprocessUtility.h"
#include "../../../SystemModels/Postprocessing/BasePostprocess.h"
#include "../../../SystemModels/Postprocessing/TimeSavecondition.h"


namespace Experiments
{
	void LumpHipoxiaExperiment::Run()
	{
		auto result_path = "./results_" + name_ + "_08112016_";
		CreateDirectory(result_path.c_str(), nullptr);

		LumpHumanSysBuilder builder;
		builder.CreateNewSystem();
		auto human_sys = builder.GetHumanSystem();

		TimeSpan timeSpan;

		//Log description
		auto save_period = 10;
		PostprocessUtility postprocess;
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), O2Art, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), Co2Art, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), Po2Art, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), Pco2Art, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), HArt, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), Hco3Art, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), HbArt, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), PhArt, result_path));
		postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(save_period), human_sys->GetBloodSys(), SysArtO2Sat, result_path));
		postprocess.AddHandler(new BasePostprocess<IRespiratorySys, RespiratorySysParam>(new TimeSaveCondition(save_period), human_sys->GetRespiratorySys(), O2, result_path));
		postprocess.AddHandler(new BasePostprocess<IRespiratorySys, RespiratorySysParam>(new TimeSaveCondition(save_period), human_sys->GetRespiratorySys(), CO2, result_path));
		postprocess.AddHandler(new BasePostprocess<IRespiratorySys, RespiratorySysParam>(new TimeSaveCondition(save_period), human_sys->GetRespiratorySys(), Ve, result_path));

		//Solution
		double save_time = 0.0;
		int qty = 0;
		while (timeSpan.timeCurr <= 500 + 40*60)
		{
			auto dt = human_sys->GetTimeStep();
			timeSpan.SetDt(dt);
			human_sys->Compute(&timeSpan);
			save_time += timeSpan.dt;
			if (save_time >= 2.5) {
				std::cout << timeSpan.timeCurr << " " << qty << std::endl;
				save_time = 0.0;
			}
			qty++;
			postprocess.Handle(timeSpan.timeCurr);
		}
	}
}