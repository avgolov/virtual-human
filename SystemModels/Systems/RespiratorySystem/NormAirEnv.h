#pragma once
#ifndef NORMAIRENV_H_
#define NORMAIRENV_H_
#include "IAirEnv.h"
#include "../../TimeSpan.h"

namespace SystemModels
{
	//The model of Respiratory system
	class NormAirEnv : public IAirEnv
	{
	public:
		NormAirEnv() {}

		double GetO2(TimeSpan* timeSpan) override {
			//if (timeSpan->timeCurr<300)
				return 0.209;
			
			//return 0.09;
		};
		
		double GetCO2(TimeSpan* timeSpan) override {
			//if (timeSpan->timeCurr<500)
				return 2.8e-4;

			//return 0.03;
		};
	};
}

#endif /*NORMAIRENV_H_*/
