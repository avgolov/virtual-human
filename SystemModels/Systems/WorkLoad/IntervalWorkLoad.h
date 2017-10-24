#ifndef INTERVALWLOAD_H_
#define INTERVALWLOAD_H_

#include "IWorkLoad.h"

namespace SystemModels
{
	class IntervalWorkLoad : public IWorkLoad
	{
	public:
		IntervalWorkLoad(double repeats, 
						 double work, 
			             double exDuration,
			             double restDuration):
				repeats_(repeats),
				work_(work),
				exDuration_(exDuration),
				restDuration_(restDuration){}

		double Get(TimeSpan* timeSpan)  {
			if (timeSpan->timeCurr < steadyStateThreshold_) return 0;
			
			/*if (timeSpan->timeCurr < 10 * 60) return 13 / 3.6;
			return 0;*/
			auto time = timeSpan->timeCurr - 
				int(timeSpan->timeCurr / (exDuration_+ restDuration_))*(exDuration_ + restDuration_);
			if(time > exDuration_) {
				return 0.;
			}
			auto load = work_ / (3.6*repeats_*exDuration_);
			return load;
		}

		double GetOld(TimeSpan* timeSpan)  {
			if (timeSpan->timeCurr < steadyStateThreshold_) return 0;

			auto time = (timeSpan->timeCurr-timeSpan->dt) - 
				int((timeSpan->timeCurr - timeSpan->dt) / (exDuration_ + restDuration_))*(exDuration_ + restDuration_);

			if (time > exDuration_) {
				return 0.;
			}
		    
			auto load = work_ / (3.6*repeats_*exDuration_);
			return load;
		}

		void SetSteadyStateThreshold(double value) { steadyStateThreshold_ = value; }

	private:
		double repeats_;
		double work_;
		double exDuration_;
		double restDuration_;
		double steadyStateThreshold_ = 0;
	};
}

#endif /*INTERVALWLOAD_H_*/