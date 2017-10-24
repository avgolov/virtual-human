#pragma once

#ifndef BREATHSTORECONDITION_H_
#define BREATHSTORECONDITION_H_

#include "IStoreCondition.h"
#include "../Model/BronchTree.h"

namespace MiptNet1DModel {

	//Запись результатов за цикл дыхания
	class BreathStoreCondition : public IStoreCondition {
	public:
		BreathStoreCondition(BronchTree* tree) {
			point_ = tree->get_nasopharynx()->get_first_point();
		}

		bool Check(double curr_time) override {
			double vel = point_->Substance.Velocity;
			if (vel*vel_old_ <= 0) change_++;
			vel_old_ = vel;

			if(change_ == 2) {
				change_ = 0;
				return true;
			}
			return false;
		};

	private:
		BranchPoint* point_;
		int change_ = 0;
		double vel_old_ = 0.;
	};
}

#endif /*BREATHSTORECONDITION_H_*/