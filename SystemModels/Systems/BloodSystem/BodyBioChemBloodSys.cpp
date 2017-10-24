#include "BodyBioChemBloodSys.h"

namespace SystemModels
{
	void BodyBioChemBloodSys::Compute(TimeSpan* timeSpan) {
		SetOldValues();
	}

	void BodyBioChemBloodSys::SetParam(BloodSysParam param, double value)
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
	double BodyBioChemBloodSys::GetParam(BloodSysParam param) const
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
		case Po2ArtOld: {
			return GetO2FreeOld() / GetSolCoefO2();
		}
		case Pco2ArtOld: {
			return GetCo2FreeOld() / GetSolCoefCo2();
		}
		case AllVol: {
			return GetVolume();
		}

		default:
			throw std::exception("Bad blood system's parameter!");
		}
	}
	void BodyBioChemBloodSys::SetSubstances(std::valarray<double> substances)
	{
		if (substances.size() == 5) {
			substances_ = substances;
		}
		else {
			throw std::exception("Wrong substances number");
		}

	};
	double BodyBioChemBloodSys::GetCo2FreeOld() const {
		/*[CO2] = [CO2free]+[HCO3-] in mols*/
		return  GetCo2Old() / litMolCoef_ - GetHCo3Old();
	}
	double BodyBioChemBloodSys::GetO2FreeOld() const {
		/*[O2] = [O2free]+m[Hb] in mols*/
		return  GetO2Old() / litMolCoef_ - hillConst_*GetHbOld();
	}
	
	double BodyBioChemBloodSys::GetCo2Free() const {
		return  GetCo2() / litMolCoef_ - GetHCo3();
	}
	double BodyBioChemBloodSys::GetO2Free() const {
		return  GetO2() / litMolCoef_ - hillConst_*GetHb();
	}
	
	void BodyBioChemBloodSys::SetO2Free(double value) {
		auto hb = hbTot_*pow(value, hillConst_) / (kO2Hb_ + pow(value, hillConst_));
		SetHb(hb);
		auto o2 = (value + hillConst_*hb)*litMolCoef_;
		SetO2(o2);
	}
	void BodyBioChemBloodSys::SetHco3(double value) {
		auto m = GetHCo3() - GetH();
		substances_[3] = value;
		auto h = value - m;
		SetH(h);
		auto co2 = (value*h/kCo2Bb_);
		SetCo2((co2 + value)*litMolCoef_);
	}
}