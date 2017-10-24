#include "BodyBloodBaseTissueCn.h"

namespace SystemModels
{
	void BodyBloodBaseTissueCn::Interact(TimeSpan* timeSpan)
	{
		//Tissue values
		auto bloodVol = bloodSys_->GetParam(AllVol);
		auto o2_uptake = tissueSys_->GetParam(OtherO2Uptake)*timeSpan->dt / bloodVol;
		auto co2_dep = tissueSys_->GetParam(OtherCO2Dep)*timeSpan->dt / bloodVol;

		auto co2 = bloodSys_->GetParam(Co2Art);
		auto o2 = bloodSys_->GetParam(O2Art);

		if (o2_uptake > o2)
		{
			bloodSys_->SetParam(O2Art, 0.);
		}
		else
		{
			bloodSys_->SetParam(O2Art, o2 - o2_uptake);
		}

		bloodSys_->SetParam(Co2Art, co2 + co2_dep);
	}
}