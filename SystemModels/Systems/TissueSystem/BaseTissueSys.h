#ifndef BASETISSUESYSTEM_H_
#define BASETISSUESYSTEM_H_

#include "ITissueSys.h"

namespace SystemModels
{
	class BaseTissueSys : public ITissueSys

	{
	public:
		BaseTissueSys() {}

		void Compute(TimeSpan* timeSpan) override {}
		void SetParam(TissueSysParam param, double value) override;
		double GetParam(TissueSysParam param) const override;

	private:
		double brainO2Uptake_ = 0.;
		double brainCO2Dep_ = 0;
		double otherO2Uptake_ = 0.;
		double otherCO2Dep_ = 0;
	};
}

#endif /*BASETISSUESYSTEM_H_*/
