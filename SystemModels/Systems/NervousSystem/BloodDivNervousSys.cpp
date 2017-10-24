#include "BloodDivNervousSys.h"

namespace SystemModels
{
	void BloodDivNervousSys::ControlBloodSys(TimeSpan* timeSpan)
	{
		//return;
		auto po2Art = blood_->GetParam(Po2Art);
		auto pco2Art = blood_->GetParam(Pco2Art);
		auto po2Brain = blood_->GetParam(Po2Brain);
		auto pco2Brain = blood_->GetParam(Pco2Brain);

		//Cardiac output
		auto lco2 = (pco2Art > pco2qs_) ? 0.03 : -0.025;
		auto qss = blood_->GetParam(CardicOutputSS);
		auto q = qss;
		//if(po2Art < po2qs_)
			q *=(0.937 + 0.0817 / (1 + pow(po2Art / po2qs_, 3.41)));
		
		//if(pco2Art > pco2qs_)
			q *= 1 + lco2*(pco2Art - pco2qs_);
		blood_->SetParam(CardicOutput,q);

		//Brain blood flow
		auto qbss = blood_->GetParam(BrainFlowSS);
		auto qb = qbss*(1.014 + 0.734 / (1 + pow(po2Brain /po2bs_, 16.6)));
		qb *= 0.43 + 1.91 / (1 + 1.6*exp(-5.25*log10(pco2Brain)));
		blood_->SetParam(BrainFlow, qb);

		//Tissue blood flow
		blood_->SetParam(TissueFlow, q - qb);
	};

	void BloodDivNervousSys::ControlRespiratorySys(TimeSpan* timeSpan)
	{
		auto po2 = blood_->GetParam(Po2Art);
		auto pco2 = blood_->GetParam(Pco2Art);
		auto pbco2 = blood_->GetParam(Pco2Brain);
		auto vc = nco2bs_*(pbco2 - tco2bs_);
		if (vc < 0.)
			vc = 0.;

		auto vp = nco2ps_*(pco2- tco2ps_);
		auto fco2 = (pco2 <= pco20_) ? 1 / (5 - 4 * pow(pco2 / pco20_, 4)) :
			pow(pco2 / pco20_, 3);
		auto vpO2 = (570 / (97.*po2 / po20_ - 26.2) - 8.05)*fco2;
		if (po20_ > po2)
			vp += vpO2;

		if (vp < 0.)
			vp = 0.;

		auto ve_ss = lungs_->GetParam(TidalVolumeSS)*lungs_->GetParam(RateSS)*0.001;
		auto ve = ve_ss + vc + vp;

		if (ve > 50) ve = 50;
		auto vt = 0.152*pow(ve, 0.683);
		auto rate = ve / vt;

		if (ve < ves_)
		{
			rate = lungs_->GetParam(RateSS);
			vt = ve / rate;
		}
		lungs_->SetParam(Rate, rate);
		lungs_->SetParam(TidalVolume, vt * 1000);
	};

	void BloodDivNervousSys::ControlTissueSys(TimeSpan* timeSpan) { };
}