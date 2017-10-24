#ifndef STOREWORKLOAD_H_
#define STOREWORKLOAD_H_

#include <vector>
#include "IWorkLoad.h"

namespace SystemModels
{
	class StoreWorkLoad : public IWorkLoad
	{
	public:
		StoreWorkLoad() {}

		double Get(TimeSpan* timeSpan) override { 
			if (timeSpan->timeCurr < steadyStateThreshold_) return 0;

			if (timeSpan->timeCurr - steadyStateThreshold_ < 5)
			{
				return velStorage_[0];
			}
			else if (timeSpan->timeCurr - steadyStateThreshold_ > 5 * velStorage_.size())
			{
				return velStorage_.back();
			}
			else {
				auto val = timeSpan->timeCurr - steadyStateThreshold_ - 5;
				auto index = static_cast<int>(val);
				auto l = (val - index) / 5.;
				index /= 5;
				return (1 - l)*velStorage_[index] + l*velStorage_[index + 1];
			}
		}
		double GetOld(TimeSpan* timeSpan) override { 
			
			if (timeSpan->timeCurr < steadyStateThreshold_) return 0;

			auto index = static_cast<int>(timeSpan->timeCurr - steadyStateThreshold_) / 5;
			auto l = (index * 5 - timeSpan->timeCurr + steadyStateThreshold_) / 5;
			return (1 - l)*velStorage_[index] + l*velStorage_[index + 1];
		}
		void SetStorage(std::vector<double> value)
		{
			velStorage_ = value;
		}
		void SetSteadyStateThreshold(double value) { steadyStateThreshold_ = value; }

	private:
		double steadyStateThreshold_ = 0;
		std::vector<double> velStorage_ = {};
	};
}

#endif /*STOREWORKLOAD_H_*/


