#pragma once

#ifndef STANDARTBREATH_H_
#define STANDARTBREATH_H_

#include "AbstractBreath.h"

namespace MiptNet1DModel {

	//Ќормальное синусоидальное дыхание 
	class StandartBreath: public AbstractBreath {

	public:
		StandartBreath() : AbstractBreath() { }

		double GetVentVolume(double time_curr) override;
		double GetRateCur() const override;
		double GetTidalVolumeCur() const override;
		double GetRateOld() const override;
		double GetTidalVolumeOld() const override;
		void SetRate(double value) override;
		void SetTidalVolume(double value) override;
		double GetPg() override;
		void SetPg(double value) override;
		double GetTidalVolumeSS() const override;
		double GetRateSS() const override;
		void SetRateSS(double value) override;
		void SetTidalVolumeSS(double value) override;

	private:
		double tidalVolume_ = 1400.;
		double tidalVolumeSS_ = 1400.;
		double tidalVolumeOld_ = 1400;
		double rate_ = 5.;
		double rateSS_ = 5.;
		double rateOld_ = 5.;
		double pg_ = 0.;
		double pgOld_ = 0.;
		double pgSS_ = 0.;
	};
}

#endif /*STANDARTBREATH_H_*/