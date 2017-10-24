#include "BaseBloodTissueCn.h"

namespace SystemModels
{
	void BaseBloodTissueCn::Interact(TimeSpan* timeSpan)
	{
		//Tissue values
		auto brainVol = bloodSys_->GetParam(BrainArtVol);
		auto brainO2Uptake = tissueSys_->GetParam(BrainO2Uptake)*timeSpan->dt / brainVol;
		auto brainCO2Dep = tissueSys_->GetParam(BrainCO2Dep)*timeSpan->dt / brainVol;
		auto otherVol = bloodSys_->GetParam(SysArtVol);
		auto otherO2Uptake = tissueSys_->GetParam(OtherO2Uptake)*timeSpan->dt / otherVol;
		auto otherCO2Dep = tissueSys_->GetParam(OtherCO2Dep)*timeSpan->dt / otherVol;
		
		auto brainO2 = bloodSys_->GetParam(O2Brain);
		auto brainCO2 = bloodSys_->GetParam(Co2Brain);
		auto otherO2 = bloodSys_->GetParam(O2Art);
		auto otherCO2 = bloodSys_->GetParam(Co2Art);

		auto brainPo2 = bloodSys_->GetParam(Po2Brain);
		auto brainPco2 = bloodSys_->GetParam(Pco2Brain);
		auto artPo2 = bloodSys_->GetParam(Po2Art);
		auto artPco2 = bloodSys_->GetParam(Pco2Art);

		if(brainO2Uptake > brainO2)
		{
			bloodSys_->SetParam(O2Brain, 0.);
		}
		else
		{
			bloodSys_->SetParam(O2Brain, brainO2-brainO2Uptake);
		}
		
		if(otherO2Uptake > otherO2)
		{
			bloodSys_->SetParam(O2Art, 0.);
		}
		else
		{
			bloodSys_->SetParam(O2Art, otherO2-otherO2Uptake);
		}
		bloodSys_->SetParam(Co2Brain, brainCO2+brainCO2Dep);
		bloodSys_->SetParam(Co2Art, otherCO2+otherCO2Dep);
	}
}