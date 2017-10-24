#include "WorkLoadNervousSys.h"

namespace SystemModels
{
	void WorkLoadNervousSys::ControlBloodSys(TimeSpan* timeSpan)
	{
		//if (timeSpan->timeCurr < steadyStateThreshold_) return;
		if (false) {
			auto index = 0;
			auto l = 0;
			
			if (timeSpan->timeCurr - steadyStateThreshold_ > 5 * veStorage_.size())
			{
				l = 1;
				index = qStorage_.size() - 2;
			}
			else if (timeSpan->timeCurr - steadyStateThreshold_ >= 5)
			{
				auto val = timeSpan->timeCurr - steadyStateThreshold_ - 5;
				index = static_cast<int>(val);
				l = (val - index) / 5.;
				index /= 5;
			}
			auto q = (1 - l)*qStorage_[index] + l*qStorage_[index + 1];
			q *= 0.23 / 60;
			auto qOld = blood_->GetParam(CardicOutput);
			auto tOld = blood_->GetParam(TissueFlow);
			auto bOld = blood_->GetParam(BrainFlow);
			blood_->SetParam(CardicOutput, q);
			blood_->SetParam(TissueFlow, q*tOld / qOld);
			blood_->SetParam(BrainFlow, q*bOld / qOld);
		}
		else{
			//Constant of cardiac output response to O2 
			const double po2qs = 47.2;
			//Constant of cardiac output response to CO2
			const double pco2qs = 40.;
			//Constant of brain blood flow response to O2
			const double po2bs = 41.4;

			auto po2Art = blood_->GetParam(Po2Art);
			auto pco2Art = blood_->GetParam(Pco2Art);

			//Cardiac output
			auto lco2 = (pco2Art > pco2qs) ? 0.03 : -0.025;
			auto qss = blood_->GetParam(CardicOutputSS);
			auto q = qss;
			//if(po2Art < po2qs_)
			q *= (0.937 + 0.0817 / (1 + pow(po2Art / po2qs, 3.41)));

			//if(pco2Art > pco2qs_)
			q *= 1 + lco2*(pco2Art - pco2qs);
			blood_->SetParam(CardicOutput, q);

			//Brain blood flow
			auto qbss = blood_->GetParam(BrainFlowSS);
			auto qb = qbss*(1.014 + 0.734 / (1 + pow(po2Art / po2bs, 16.6)));
			qb *= 0.43 + 1.91 / (1 + 1.6*exp(-5.25*log10(pco2Art)));
			blood_->SetParam(BrainFlow, qb);

			//Tissue blood flow
			blood_->SetParam(TissueFlow, q - qb);
		}
	};

	void WorkLoadNervousSys::ControlRespiratorySys(TimeSpan* timeSpan)
	{
		//if (timeSpan->timeCurr < steadyStateThreshold_) return;

		if (useExperiment_) {
			if (timeSpan->timeCurr - steadyStateThreshold_ < 5)
			{
				auto ve = veStorage_[0];
				auto rate = brStorage_[0];
				lungs_->SetParam(Rate, rate);
				lungs_->SetParam(TidalVolume, ve / rate);
			}
			else if(timeSpan->timeCurr - steadyStateThreshold_ > 5* veStorage_.size())
			{
				auto ve = veStorage_.back();
				auto rate = brStorage_.back();
				lungs_->SetParam(Rate, rate);
				lungs_->SetParam(TidalVolume, ve / rate);
			}
			else {
				auto val = timeSpan->timeCurr - steadyStateThreshold_ - 5;
				auto index = static_cast<int>(val);
				auto l = (val-index) / 5.;// (index * 5. + 5. - timeSpan->timeCurr + steadyStateThreshold_) / 5.;
				index /= 5;
				auto ve = (1 - l)*veStorage_[index] + l*veStorage_[index + 1];
				auto rate = (1 - l)*brStorage_[index] + l*brStorage_[index + 1];
				lungs_->SetParam(Rate, rate);
				lungs_->SetParam(TidalVolume, ve / rate);
			}
		}
		else
		{
			auto po2 = blood_->GetParam(Po2Art);
			auto pco2 = blood_->GetParam(Pco2Art);
			auto pbco2 = blood_->GetParam(Pco2Brain);
			auto vc = nco2bs*(pbco2 - tco2bs);
			if (vc < 0.)
				vc = 0.;

			auto vp = nco2ps*(pco2 - tco2ps);
			auto fco2 = (pco2 <= pco20) ? 1 / (5 - 4 * pow(pco2 / pco20, 4)) :
				pow(pco2 / pco20, 3);
			auto vpO2 = (570 / (97.*po2 / po20 - 26.2) - 8.05)*fco2;
			//vpO2 = 0;
			if (po20 > po2)
				vp += vpO2;

			auto ve_ss = lungs_->GetParam(TidalVolumeSS)*lungs_->GetParam(RateSS);
			auto ve = ve_ss + vc + vp;

			if (ve > 200) ve = 200;
			auto vt = 0.152*pow(ve, 0.683);
			auto rate = ve / vt;

			if (ve < ves_)
			{
				rate = lungs_->GetParam(RateSS);
				vt = ve / rate;
			}

			lungs_->SetParam(Rate, rate);
			lungs_->SetParam(TidalVolume, vt);
		}
	};

	void WorkLoadNervousSys::ControlTissueSys(TimeSpan* timeSpan) { };
}