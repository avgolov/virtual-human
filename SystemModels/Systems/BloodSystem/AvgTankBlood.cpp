#include "AvgTankBlood.h"

namespace SystemModels
{
	void AvgTankBlood::Compute(TimeSpan* timeSpan)
	{
		if (bioChemModel_) bioChemModel_->Compute(timeSpan);
	}
	void AvgTankBlood::Transfer(AvgTankBlood* tank, double volume)
	{
		auto vals = tank->substancesOld_*volume ;
		substances_ += vals/volume_;
		tank->substances_ -= vals/tank->GetVolume();
	}
	void AvgTankBlood::SetSubstances(std::valarray<double> substances)
	{
		substances_ = substances;
		substancesOld_ = substances;
		bioChemModel_ = new BaseBiochemistryModel(&substances_, &substancesOld_);
	}
	double AvgTankBlood::GetCo2() const
	{
		return substances_[1];
	}
	double AvgTankBlood::GetO2() const
	{
		return substances_[0];
	}
	double AvgTankBlood::GetPco2() const
	{
		return bioChemModel_->GetPco2();
	}
	double AvgTankBlood::GetPo2() const
	{
		return bioChemModel_->GetPo2();
	}
	double AvgTankBlood::GetCo2Old() const
	{
		return substancesOld_[1];
	}
	double AvgTankBlood::GetO2Old() const
	{
		return substancesOld_[0];
	}
	double AvgTankBlood::GetSatO2() const
	{
		return bioChemModel_->GetSo2();
	}
}