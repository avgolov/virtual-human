#pragma once

#ifndef TIMESTORECONDITION_H_
#define TIMESTORECONDITION_H_

#include "IStoreCondition.h"

namespace MiptNet1DModel {

	//Запись результатов через промежуток времени
	class TimeStoreCondition : public IStoreCondition {
	public:
		TimeStoreCondition(double save_period): 
			save_period_(save_period) { }

		bool Check(double curr_time) override {
			if(curr_time >= last_save_ + save_period_) {
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

#endif /*TIMESTORECONDITION_H_*/