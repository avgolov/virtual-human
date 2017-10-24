#include <iostream>
#include "Builders/HumanSysBuilder.h"
#include "Postprocessing/BasePostprocess.h"
#include "Postprocessing/PostprocessUtility.h"
#include "Postprocessing/TimeSavecondition.h"

using namespace SystemModels;

//int main()
//{
//	HumanSysBuilder builder;
//	builder.CreateNewSystem();
//	auto humanSys = builder.GetHumanSystem();
//	TimeSpan timeSpan;
//
//	PostprocessUtility postprocess;
//	auto result_path = "./results";
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1),humanSys->GetBloodSys(),  O2Art, result_path));
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1), humanSys->GetBloodSys(), O2Ven, result_path));
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1), humanSys->GetBloodSys(), Co2Art, result_path));
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1), humanSys->GetBloodSys(), Co2Ven, result_path));
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1), humanSys->GetBloodSys(), O2Brain, result_path));
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1), humanSys->GetBloodSys(), Co2Brain, result_path));
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1), humanSys->GetBloodSys(), O2AlvArt, result_path));
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1), humanSys->GetBloodSys(), Co2AlvArt, result_path));
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1), humanSys->GetBloodSys(), O2AlvVen, result_path));
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1), humanSys->GetBloodSys(), Co2AlvVen, result_path));
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1), humanSys->GetBloodSys(), Po2Art, result_path));
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1), humanSys->GetBloodSys(), Po2Ven, result_path));
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1), humanSys->GetBloodSys(), Pco2Art, result_path));
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1), humanSys->GetBloodSys(), Pco2Ven, result_path));
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1), humanSys->GetBloodSys(), Po2Brain, result_path));
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1), humanSys->GetBloodSys(), Pco2Brain, result_path));
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1), humanSys->GetBloodSys(), Po2AlvArt, result_path));
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1), humanSys->GetBloodSys(), Pco2AlvArt, result_path));
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1), humanSys->GetBloodSys(), Po2AlvVen, result_path));
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1), humanSys->GetBloodSys(), Pco2AlvVen, result_path));
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1), humanSys->GetBloodSys(), CardicOutput, result_path));
//	postprocess.AddHandler(new BasePostprocess<IBloodSys, BloodSysParam>(new TimeSaveCondition(0.1), humanSys->GetBloodSys(), BrainFlow, result_path));
//	postprocess.AddHandler(new BasePostprocess<IRespiratorySys, RespiratorySysParam>(new TimeSaveCondition(0.1), humanSys->GetRespiratorySys(), Rate, result_path));
//	postprocess.AddHandler(new BasePostprocess<IRespiratorySys, RespiratorySysParam>(new TimeSaveCondition(0.1), humanSys->GetRespiratorySys(), TidalVolume, result_path));
//	double save_time = 0.0;
//	int qty = 0;
//	while (timeSpan.timeCurr <= 500.0)
//	{
//		auto dt = humanSys->GetTimeStep();
//		timeSpan.SetDt(dt);
//		humanSys->Compute(&timeSpan);
//		save_time += timeSpan.dt;
//		if (save_time >= 0.5) {
//			std::cout << timeSpan.timeCurr << " " << qty << std::endl;
//			save_time = 0.0;
//		}
//		qty++;
//		postprocess.Handle(timeSpan.timeCurr);
//	}
//
//	return 0;
//}

