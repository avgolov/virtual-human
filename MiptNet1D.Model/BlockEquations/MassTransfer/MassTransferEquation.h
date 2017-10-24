#pragma once

#ifndef MASSTRANSFERBLOCKEQ_H_
#define MASSTRANSFERBLOCKEQ_H_

#include "../AbstractEquation.h"
#include "tbb/tbb.h"

namespace MiptNet1DModel {

	class MassTransferEquation : public AbstractEquation {

	public:
		MassTransferEquation(BaseTree* tree) : 
			AbstractEquation(tree) { }
		MassTransferEquation(){}

		MassTransferEquation& MassTransferEquation::operator=(const MassTransferEquation& other) {
			tree_ = other.tree_;
			return *this;
		}

		virtual void ComputeBoundaryKnots(double dt) = 0;
		void ComputeMultiKnots(double dt);
		void ComputeInternalPoints(double dt) const;
		void ModifyConcentration();

		void Solve(double dt) override {
			ModifyConcentration();
			ComputeMultiKnots(dt);//?????
			ComputeInternalPoints(dt);
			ComputeBoundaryKnots(dt);
		}

	protected:
		//количество переносимых веществ
		const int conc_count_ = 2;
		double time_ = 0.;

	};
}

#endif /*MASSTRANSFERBLOCKEQ_H_*/