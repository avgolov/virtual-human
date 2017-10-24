#pragma once
#ifndef ISYSTEM_H_
#define ISYSTEM_H_

#include "../TimeSpan.h"

namespace SystemModels 
{
	class ISystem
	{
	public:
		ISystem() {}
		virtual ~ISystem() {}
		virtual void Compute(TimeSpan* timeSpan) = 0;
		virtual double GetTimeStep() { return 1.e10; }
	};
}

#endif /*ISYSTEM_H_*/
