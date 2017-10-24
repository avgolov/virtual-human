#pragma once
#ifndef IAIRENV_H_
#define IAIRENV_H_

#include "../../TimeSpan.h"

namespace SystemModels
{
	//The model of Respiratory system
	class IAirEnv 
	{
	public:
		IAirEnv() {}
		virtual ~IAirEnv() {};

		virtual double GetO2(TimeSpan* timeSpan) = 0;
		virtual double GetCO2(TimeSpan* timeSpan) = 0;
	};
}

#endif /*IAIRENV_H_*/