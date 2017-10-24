#pragma once
#ifndef AVGTANKSBLOOD_H_
#define AVGTANKSBLOOD_H_

#include <valarray>
#include "IBloodSys.h"
#include "Biochemistry/BaseBiochemistryModel.h"

namespace SystemModels
{
	class AvgTankBlood : ISystem
	{
	public:
		void Compute(TimeSpan* timeSpan) override;
		double GetVolume() const { return volume_; }
		void SetVolume(double volume) { volume_ = volume; }
		void SetSubstances(std::valarray<double> substances); 
		void SetOldValues() { substancesOld_ = substances_; }
		void Transfer(AvgTankBlood* tank, double volume);
		double GetCo2() const;
		double GetO2() const;
		double GetPco2() const;
		double GetPo2() const;
		double GetCo2Old() const;
		double GetO2Old() const;
		double GetSatO2() const;
		void SetCo2(double value) { substances_[1] = value; }
		void SetO2(double value) { substances_[0] = value; }
		void SetPco2(double value) { bioChemModel_->SetPco2(value); }
		void SetPo2(double value) { bioChemModel_->SetPo2(value); }

	private:
		double volume_;
		std::valarray<double> substances_;
		std::valarray<double> substancesOld_;
		BaseBiochemistryModel* bioChemModel_;
	};
}

#endif /*AVGTANKSBLOOD_H_*/