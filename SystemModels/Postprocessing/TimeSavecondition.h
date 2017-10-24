#pragma once

#ifndef TIMESAVECONDITION_H_
#define TIMESAVECONDITION_H_

#include "ISaveCondition.h"

namespace SystemModels {

	//Запись результатов через промежуток времени
	class TimeSaveCondition : public ISaveCondition {
	public:
		TimeSaveCondition(double save_period) :
			save_period_(save_period) { }

		bool Check(double curr_time) override {
			auto val = (1 + 1.e-6)*curr_time / save_period_;
			if (val - int(val) < 0.001) {
				last_save_ = curr_time;
				return true;
			}
			return false;
		};

	private:
		double save_period_;
		double last_save_ = 0.;
	};
}

#endif /*TIMESAVECONDITION_H_*/