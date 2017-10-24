#pragma once

#ifndef OXYCONSRESULTHANDLER_H_
#define OXYCONSRESULTHANDLER_H_

#include "AbstractResultHandler.h"
#include "../Model/BranchPoint.h"

namespace MiptNet1DModel {

	//Потребление кислорода
	class OxyConsResultHandler : public AbstractResultHandler {
	public:
		OxyConsResultHandler(IStoreCondition* condition,
							 BranchPoint* point,
			                 std::string path) :
			AbstractResultHandler(condition, path),
			point_(point) { }

	protected:
		double GetValue() override { 
			auto value = o2_vol_; 
			o2_vol_ = 0.;
			return value;
		};

		std::string GetFileName() const override {
			return path_ + "/oxygen_consumption.txt";
		};

		void Process() override {
			double vel = point_->Substance.Velocity;
			if (vel > 0.0) {
				o2_vol_ += point_->Square * vel* 0.209*(curr_time_-last_time_);
			}
			else {
				o2_vol_ += point_->Square * vel * point_->Substance.Conc[0]*(curr_time_ - last_time_);
			}
		};

	private:
		BranchPoint* point_;
		double o2_vol_ = 0.;
	};
}

#endif /*OXYCONSRESULTHANDLER_H_*/