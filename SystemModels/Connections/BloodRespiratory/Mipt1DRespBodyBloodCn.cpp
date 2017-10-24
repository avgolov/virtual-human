#include "../../Systems/RespiratorySystem/MiptNet1DRespSys.h"
#include "Mipt1DRespBodyBloodCn.h"

namespace SystemModels
{
	void Mipt1DRespBodyBloodCn::Interact(TimeSpan* timeSpan)
	{
		auto blood_o2 = bloodSys_->GetParam(O2Art);
		auto blood_co2 = bloodSys_->GetParam(Co2Art);
		auto lungs = dynamic_cast<MiptNet1DRespSys*>(RespSys_.get());
		auto alveoles = lungs->GetAlveoles();

		auto o2_exchange = 0.;
		auto co2_exchange = 0.;

		for (size_t i = 0; i < alveoles->size(); i++)
		{
			auto alveole = (*alveoles)[i];

			auto alv_o2 = alveole->SubstancePrev.Conc[0];
			auto alv_co2 = alveole->SubstancePrev.Conc[1];

			double s = 1770 / alveoles->size();//4 * M_PI*pow(0.75*alveole->Volume/M_PI, 2./3.);
			double d_o2 = dO2Cf_ * timeSpan->dt*s*(alv_o2 - blood_o2);

			if (d_o2 > 0. && blood_o2 < 0.20) {
				alveole->Substance.Conc[0] -= d_o2 / alveole->Volume;
				o2_exchange += d_o2;
			}

			double d_co2 = dCo2Cf_ * timeSpan->dt*s*(blood_co2 - alv_co2);

			if (d_co2 > 0. && blood_co2 > 0.48) {
				alveole->Substance.Conc[1] += d_co2 / alveole->Volume;
				co2_exchange += d_co2;
			}
		}
		auto vol = bloodSys_->GetParam(AllVol);
		bloodSys_->SetParam(O2Art, blood_o2 + o2_exchange / vol);
		bloodSys_->SetParam(Co2Art, blood_co2 - co2_exchange / vol);
	}
}