#ifndef IWORKLOAD_H_
#define IWORKLOAD_H_

#include "../../TimeSpan.h"

namespace SystemModels
{
	//The model of Work load
	class IWorkLoad
	{
	public:
		IWorkLoad() {}
		virtual ~IWorkLoad() {};

		virtual double Get(TimeSpan* timeSpan) = 0;
		virtual double GetOld(TimeSpan* timeSpan) = 0;
	};
}

#endif /*IWORKLOAD_H_*/
