#pragma once
#ifndef MIPT1DRespAVGTANKBLOODCN_H_
#define MIPT1DRespAVGTANKBLOODCN_H_

#include <memory>
#include "IBloodRespiratoryCn.h"

namespace SystemModels
{
	class Mipt1DRespAvgTankBloodCn : public IBloodRespiratoryCn
	{
	public:
		Mipt1DRespAvgTankBloodCn(boost::shared_ptr<IBloodSys> bloodSys,
			                     boost::shared_ptr<IRespiratorySys> RespSys) :
		IBloodRespiratoryCn(bloodSys, RespSys){}

		void Interact(TimeSpan* timeSpan) override;

	private:
		//Коэффициент диффузии через легочную мембрану для О2
		const double dO2Cf_ = 250.*(98.1-40)/(60.*5000.);

		//Коэффициент диффузии через легочную мембрану для СО2
		const double dCo2Cf_ = 207.*(46-40)/ (60.*5000.);
	};
}

#endif /*MIPT1DRespAVGTANKBLOODCN_H_*/

