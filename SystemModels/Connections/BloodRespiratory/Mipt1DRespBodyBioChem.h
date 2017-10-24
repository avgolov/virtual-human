#pragma once
#ifndef MIPT1DRespBODYBIOCHEMCN_H_
#define MIPT1DRespBODYBIOCHEMCN_H_

#include <memory>
#include "IBloodRespiratoryCn.h"
#include "../../Systems/BloodSystem/BodyBioChemBloodSys.h"

namespace SystemModels
{
	class Mipt1DRespBodyBioChemCn : public IBloodRespiratoryCn
	{
	public:
		Mipt1DRespBodyBioChemCn(
			boost::shared_ptr<IBloodSys> bloodSys,
			boost::shared_ptr<IRespiratorySys> RespSys) :
			IBloodRespiratoryCn(bloodSys, RespSys) {
			bodyBioChemBloodSys_ = boost::dynamic_pointer_cast<BodyBioChemBloodSys>(bloodSys);
			miptNet1DRespSys_ = boost::dynamic_pointer_cast<MiptNet1DRespSys>(RespSys);
		}

		void Interact(TimeSpan* timeSpan) override;

	private:
		/*O2 - 20ml/(min*mmHg); CO2- 500ml/(min*mmHg); V=4000 S=1218 */
		const double dO2Cf_ = 20. / (60.*1218.);
		const double dCo2Cf_ = 500. / (60.*1218.);
		const double atmPres_ = 714.;
		boost::shared_ptr<BodyBioChemBloodSys> bodyBioChemBloodSys_;
		boost::shared_ptr<MiptNet1DRespSys> miptNet1DRespSys_;
	};
}

#endif /*MIPT1DRespBODYBIOCHEMCN_H_*/
