#ifndef STANDARTWLOAD_H_
#define STANDARTWLOAD_H_

#include "IWorkLoad.h"

namespace SystemModels
{
	class StandartWorkLoad : public IWorkLoad
	{
	public:
		StandartWorkLoad() {}

		double Get(TimeSpan* timeSpan)  { 
			double threshold = 50;
			if(timeSpan->timeCurr<threshold)
			{
				return 0;
			}
			return 150+(timeSpan->timeCurr- threshold)/5;
		}

		double GetOld(TimeSpan* timeSpan)  {
			double threshold = 50;
			if (timeSpan->timeCurr-timeSpan->dt-threshold)
			{
				return 0;
			}
			return 150 + (timeSpan->timeCurr - timeSpan->dt - threshold) / 5;
		}
	};
}

#endif /*STANDARTWLOAD_H_*/

