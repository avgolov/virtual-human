#pragma once
#ifndef BODYBLOODSYS_H_
#define BODYBLOODSYS_H_

#include <valarray>
#include "IBloodSys.h"

namespace SystemModels
{
	//Simple model 1-Compartment, no biochemistry
	class BodyBloodSys : public IBloodSys
	{
	public:
		void Compute(TimeSpan* timeSpan) override {}
		void SetParam(BloodSysParam param, double value) override;
		double GetParam(BloodSysParam param) const override;

		double GetVolume() const { return volume_; }
		void SetVolume(double volume) { volume_ = volume; }
		void SetSubstances(std::valarray<double> substances) { substances_ = substances; };
		void SetOldValues() { substancesOld_ = substances_; }
		double GetCo2() const { return substances_[1]; }
		double GetO2() const { return substances_[0]; }
		double GetCo2Old() const { return substancesOld_[1]; }
		double GetO2Old() const { return substancesOld_[0];	}
		void SetCo2(double value) { substances_[1] = value; }
		void SetO2(double value) { substances_[0] = value; }

	private:
		double volume_;
		std::valarray<double> substances_;
		std::valarray<double> substancesOld_;
	};
}

#endif /*BODYBLOODSYS_H_*/