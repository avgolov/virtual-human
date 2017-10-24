#include "ZoneWorkLoad.h"

namespace SystemModels {

	double ZoneWorkLoad::GetZoneLoad(int n)	const {
		switch(n)
		{
		case 1:
			return wmin_ + (lt_ - wmin_)*(1. / 6.);
		case 2:
			return wmin_ + (lt_ - wmin_)*(3. / 6.);
		case 3:
			return wmin_ + (lt_ - wmin_)*(5. / 6.);
		case 4:
			return lt_ + (wmax_ - lt_)*0.25;
		case 5:
			return lt_ + (wmax_ - lt_)*0.75;
		}
		return 0;
	}

	double ZoneWorkLoad::Get(TimeSpan* timeSpan) {
		if (timeSpan->timeCurr < steadyStateThreshold_) return 0;

		auto time = timeSpan->timeCurr - steadyStateThreshold_;
		auto t = 0;
		for (auto i =0; i<time_.size();i++) {
			t += time_[i];
			if(time <= t) {
				return GetZoneLoad(zones_[i]);
			}
		}
		
		return 0;
	}

	double ZoneWorkLoad::GetOld(TimeSpan* timeSpan) {
		if (timeSpan->timeCurr-timeSpan->dt < steadyStateThreshold_) return 0;

		auto time = timeSpan->timeCurr - timeSpan->dt - steadyStateThreshold_;
		auto t = 0;
		for (auto i = 0; i < time_.size(); i++) {
			t += time_[i];
			if (time <= t) {
				return GetZoneLoad(zones_[i]);
			}
		}

		return 0;
	}
}