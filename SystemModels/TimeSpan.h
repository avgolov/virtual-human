#pragma once

#ifndef TIMESPAN_H_
#define TIMESPAN_H_

namespace SystemModels
{
	class TimeSpan
	{
	public:
		double dt = 0.;
		double dtPrev = 0.;
		double timeCurr = 0.;
		int step = 0;

		void SetDt(double value);
	};
}
#endif /*TIMESPAN_H_*/