#pragma once
#ifndef MIPT1DRespBODYBLOODCN_H_
#define MIPT1DRespBODYBLOODCN_H_

#include <memory>
#include "IBloodRespiratoryCn.h"

namespace SystemModels
{
	class Mipt1DRespBodyBloodCn : public IBloodRespiratoryCn
	{
	public:
		Mipt1DRespBodyBloodCn(boost::shared_ptr<IBloodSys> bloodSys,
							  boost::shared_ptr<IRespiratorySys> RespSys) :
			IBloodRespiratoryCn(bloodSys, RespSys) {}

		void Interact(TimeSpan* timeSpan) override;

	private:
		const double dO2Cf_ = 280. / (6.*400.);
		const double dCo2Cf_ = 230. / (6.*400.);
	};
}

#endif /*MIPT1DRespBODYBLOODCN_H_*/

