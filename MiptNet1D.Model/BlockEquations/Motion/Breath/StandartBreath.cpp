#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include "StandartBreath.h"

namespace MiptNet1DModel
{
	double StandartBreath::GetVentVolume(double time_curr)
	{
		return (0.5*tidalVolume_*rate_*  M_PI) *
			    sin(M_PI * time_curr * rate_ / 30.);
	}

	double StandartBreath::GetRateCur() const
	{
		return rate_;
	}
	double StandartBreath::GetTidalVolumeCur() const
	{
		return tidalVolume_;
	}
	
	double StandartBreath::GetRateOld() const
	{
		return rateOld_;
	}

	double StandartBreath::GetTidalVolumeOld() const
	{
		return tidalVolumeOld_;
	}

	void StandartBreath::SetRate(double value)
	{
		rateOld_ = rate_;
		rate_ = value;
	}

	void StandartBreath::SetTidalVolume(double value)
	{
		tidalVolumeOld_ = tidalVolume_;
		tidalVolume_ = value;
		pg_ = pgSS_*value / tidalVolumeSS_;
	}

	double StandartBreath::GetPg()
	{
		return pg_;
	}
	void StandartBreath::SetPg(double value)
	{
		pgOld_ = pg_;
		pg_ = value;
	}

	double StandartBreath::GetTidalVolumeSS() const
	{
		return tidalVolumeSS_;
	}
	double StandartBreath::GetRateSS() const
	{
		return rateSS_;
	}
	void StandartBreath::SetRateSS(double value)
	{
		rateSS_ = value;
	}
	void StandartBreath::SetTidalVolumeSS(double value)
	{
		tidalVolumeSS_ = value;

		//Initial value of pg - 
		//empirically from numerical experiment
		pgSS_ = pg_;
	}
}