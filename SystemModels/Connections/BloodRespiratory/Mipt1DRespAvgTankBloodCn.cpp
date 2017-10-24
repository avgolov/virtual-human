#include "Mipt1DRespAvgTankBloodCn.h"
#include "../../Systems/RespiratorySystem/MiptNet1DRespSys.h"

namespace SystemModels
{
	void Mipt1DRespAvgTankBloodCn::Interact(TimeSpan* timeSpan)
	{
		auto bloodPo2 = bloodSys_->GetParam(Po2AlvArt);//40.*bloodSys_->GetParam(Co2AlvArtOld)/0.15;//
		auto bloodPco2 = bloodSys_->GetParam(Pco2AlvArt);//46.*bloodSys_->GetParam(Co2AlvArtOld)/0.52;
		auto lungs = dynamic_cast<MiptNet1DRespSys*>(RespSys_.get());
		auto alveoles = lungs->GetAlveoles();

		auto o2_exchange = 0.;
		auto co2_exchange = 0.;

		for (size_t i = 0; i < alveoles->size(); i++)
		{
			auto alveole = (*alveoles)[i];
			
			auto alv_p = 714;//alveole->Substance.Pressure*7.5006e-3;
			auto alv_po2 = alv_p*alveole->SubstancePrev.Conc[0];
			auto alv_pco2 = alv_p*alveole->SubstancePrev.Conc[1];

			double d_o2 = dO2Cf_ * timeSpan->dt *
				alveole->VolumeInit*(alv_po2 - bloodPo2);

			if (d_o2 > 0. && bloodSys_->GetParam(O2AlvArt) < 0.20) {
				alveole->Substance.Conc[0] -= d_o2/alveole->Volume;
				o2_exchange += d_o2 ;
			}

			double d_co2 = dCo2Cf_ * timeSpan->dt *
				alveole->VolumeInit*(bloodPco2 - alv_pco2);

			if (d_co2 > 0. && bloodSys_->GetParam(Co2AlvArt) > 0.48) {
				alveole->Substance.Conc[1] += d_co2 / alveole->Volume;
				co2_exchange += d_co2;
			}
		}
		auto vol = bloodSys_->GetParam(PulmArtVol);
		auto bloodCo2 = bloodSys_->GetParam(Co2AlvArt);
		auto bloodO2 = bloodSys_->GetParam(O2AlvArt);
		bloodSys_->SetParam(O2AlvArt, bloodO2 + o2_exchange / vol);
		bloodSys_->SetParam(Co2AlvArt, bloodCo2 - co2_exchange / vol);
	}
}