#ifndef ZONEWLOAD_H_
#define ZONEWLOAD_H_

#include <vector>
#include "IWorkLoad.h"

namespace SystemModels
{
	class ZoneWorkLoad : public IWorkLoad
	{
	public:
		ZoneWorkLoad(double wmax, double wmin, double lt, 
			         std::vector<double> time,
		             std::vector<int> zones) :
			wmax_(wmax),
			wmin_(wmin),
			lt_(lt),
			time_(time),
			zones_(zones) {}

		double Get(TimeSpan* timeSpan) override;
		double GetOld(TimeSpan* timeSpan) override;

		void SetSteadyStateThreshold(double value) { steadyStateThreshold_ = value; }

	private:
		double wmax_;
		double wmin_;
		double lt_;
		double steadyStateThreshold_ = 0;
		std::vector<double> time_;
		std::vector<int> zones_;

		double GetZoneLoad(int n) const;
	};
}

#endif /*ZONEWLOAD_H_*/
