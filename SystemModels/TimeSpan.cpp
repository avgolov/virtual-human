#include "TimeSpan.h"

namespace SystemModels
{
	void TimeSpan::SetDt(double value)
	{
		dtPrev = dt;
		dt = value;
		timeCurr += value;
		step++;
	}
}