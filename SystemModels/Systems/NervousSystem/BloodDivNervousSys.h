#pragma once
#ifndef BLOODDIVNERVOUSSYSTEM_H_
#define BLOODDIVNERVOUSSYSTEM_H_

#include "INervousSys.h"

namespace SystemModels
{
	//The model of the nervous system of controling 
	//by blood subnormality composition diviation 
	class BloodDivNervousSys : public INervousSys
	{
	public:
		BloodDivNervousSys() {}
		
		void Compute(TimeSpan* timeSpan) override {
			ControlBloodSys(timeSpan);
			ControlRespiratorySys(timeSpan);
			ControlTissueSys(timeSpan);
		}

		//Controling state of human's subsystems; 
		void ControlBloodSys(TimeSpan* timeSpan) override;
		void ControlRespiratorySys(TimeSpan* timeSpan) override;
		void ControlTissueSys(TimeSpan* timeSpan) override;

	private:
		//Cardiac system
		//Constant of cardiac output response to O2 
		const double po2qs_ = 63.5;//47.2;

		//Constant of cardiac output response to CO2
		const double pco2qs_ = 40.;

		//Constant of brain blood flow response to O2
		const double po2bs_ = 70.3;//41.4;

		//Res
		//Gain constant of the central part of controller
		const double nco2bs_ = 0.54 / 1.5;

		//Threashold constant of the central part of controller
		const double tco2bs_ = 45;//38.7;

		//Gain constant of the peripheral part of controller
		const double nco2ps_ = 1.08 / 1.5;

		//Threashold constant of the peripheral part of controller
		const double tco2ps_ = 57.5;// 48.4;

		const double pco20_ = 61.7;//89.65;
		const double po20_ = 63.5;//53.55;

		//The lungs ventilation volume threashold
		const double ves_ = 15;
	};
}

#endif /*BLOODDIVNERVOUSSYSTEM_H_*/