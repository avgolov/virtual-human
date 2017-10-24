#pragma once
#ifndef ITISSUESYSTEM_H_
#define ITISSUESYSTEM_H_

#include "../ISystem.h"

namespace SystemModels
{
	enum TissueSysParam
	{
		O2Uptake,
		CO2Dep,
		BrainO2Uptake,
		BrainCO2Dep,
		OtherO2Uptake,
		OtherCO2Dep
	};

	//The model of tissue system
	class ITissueSys : public ISystem
	{
	public:
		ITissueSys() {}
		virtual ~ITissueSys() {};

		virtual void SetParam(TissueSysParam param, double value) = 0;
		virtual double GetParam(TissueSysParam param) const = 0;
	};
}

#endif /*ITISSUESYSTEM_H_*/
