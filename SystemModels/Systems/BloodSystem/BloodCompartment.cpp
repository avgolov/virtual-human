#include "BloodCompartment.h"
#include "Biochemistry/BioChemConst.h"

namespace SystemModels
{
	void BloodCompartment::SetParam(BloodSysParam param, double value)
	{
		switch (param)
		{

		case AllVol: {
			SetVolume(value);
			break;
		}

		 //fractions
		case O2Art: {
			SetO2(value);
			break;
		}
		case Co2Art: {
			SetCo2(value);
			break;
		}

		default:
			throw std::exception("Wrong for setting blood system's parameter");
		}
	}
	double BloodCompartment::GetParam(BloodSysParam param) const
	{
		switch (param)
		{
		case O2Art: {
			return GetO2();
		}
		case Co2Art: {
			return GetCo2();
		}
		case O2ArtOld: {
			return GetO2Old();
		}
		case Co2ArtOld: {
			return GetCo2Old();
		}
		case HArt: {
			return GetH();
		}
		case Hco3Art: {
			return GetHCo3();
		}
		case HbArt: {
			return GetHb();
		}
		case PhArt: {
			return -log10(GetH());
		}
		case SysArtO2Sat: {
			return 100.*GetHb() / GetHbTot();
		}
		case Po2Art: {
			return GetO2Free() / GetSolCoefO2();
		}
		case Pco2Art: {
			return GetCo2Free() / GetSolCoefCo2();
		}
		case AllVol: {
			return GetVolume();
		}

		default:
			throw std::exception("Bad blood system's parameter!");
		}
	}
	void BloodCompartment::SetSubstances(std::valarray<double> substances)
	{
		if (substances.size() == 6) {
			substances_ = substances;
		}
		else {
			throw std::exception("Wrong substances number");
		}

	};
	double BloodCompartment::GetPo2() const	{
		return GetO2Free() / GetSolCoefO2();
	}
	double BloodCompartment::GetPco2() const {
		return GetCo2Free() / GetSolCoefCo2();
	}
	void BloodCompartment::SetPo2(double value)	{
		SetO2Free(value / bcm_.SolCoefO2);
	}
	void BloodCompartment::SetPco2(double value) {
		BioChemConst bcm;
		auto co2Free = value / solCoefCo2_;
		auto m = GetHCo3() - GetH();
		auto hco3 = 0.5*(m+sqrt(pow(m,2)+4*value*solCoefCo2_));
		substances_[3] = hco3;
		SetH(hco3 - m);
		SetCo2((co2Free + hco3)*bcm.LitMolCoef);
	}
	double BloodCompartment::GetCo2FreeOld() const {
		/*[CO2] = [CO2free]+[HCO3-] in mols*/
		return  GetCo2Old() / bcm_.LitMolCoef - GetHCo3Old();
	}
	double BloodCompartment::GetO2FreeOld() const {
		/*[O2] = [O2free]+m[Hb] in mols*/
		return  GetO2Old() / bcm_.LitMolCoef - bcm_.HillConst*GetHbOld();
	}

	double BloodCompartment::GetCo2Free() const {
		return  GetCo2() / bcm_.LitMolCoef- GetHCo3();
	}
	double BloodCompartment::GetO2Free() const {
		return  GetO2() / bcm_.LitMolCoef - bcm_.HillConst*GetHb();
	}

	void BloodCompartment::SetO2Free(double value) {
		auto hb = bcm_.HbTot*pow(value, bcm_.HillConst) / (bcm_.KO2Hb + pow(value, bcm_.HillConst));
		SetHb(hb);
		auto o2 = (value + bcm_.HillConst*hb)*bcm_.LitMolCoef;
		SetO2(o2);
	}
	void BloodCompartment::SetHco3(double value, double m) {
		substances_[3] = value;
		auto h = value - m;
		SetH(h);
		auto co2 = (value*h / kCo2Bb_);
		SetCo2((co2 + value)*bcm_.LitMolCoef);
	}
}