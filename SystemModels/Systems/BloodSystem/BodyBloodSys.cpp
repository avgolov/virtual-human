#include "BodyBloodSys.h"

namespace SystemModels
{
	void BodyBloodSys::SetParam(BloodSysParam param, double value)
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

	double BodyBloodSys::GetParam(BloodSysParam param) const
	{
		switch (param)
		{
		//fractions
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
		case AllVol: {
			return GetVolume();
		}

		default:
			throw std::exception("Bad blood system's parameter!");
		}
	}
}