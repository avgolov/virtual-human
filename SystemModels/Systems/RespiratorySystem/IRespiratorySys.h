#pragma once
#ifndef IRespiratorySYSTEM_H_
#define IRespiratorySYSTEM_H_

#include "../ISystem.h"

namespace SystemModels
{
	enum RespiratorySysParam
	{
		TidalVolume = 100,
		Rate,
		Ve,
		TidalVolumeOld,
		RateOld,
		TidalVolumeSS,
		RateSS,
		Pg,
		O2,
		CO2,
		Vol
	};
	
	//The model of Respiratory system
	class IRespiratorySys : public ISystem
	{
	public:
		IRespiratorySys() {}
		virtual ~IRespiratorySys() {};

		virtual void SetParam(RespiratorySysParam param, double value) = 0;
		virtual double GetParam(RespiratorySysParam param) const = 0;
	};
}

#endif /*IRespiratorySYSTEM_H_*/
