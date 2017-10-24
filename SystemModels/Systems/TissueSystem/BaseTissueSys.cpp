#include "BaseTissueSys.h"
#include <exception>

namespace SystemModels
{
	void BaseTissueSys::SetParam(TissueSysParam param, double value)
	{
		switch (param)
		{
		case BrainO2Uptake: {
			brainO2Uptake_ = value;
			break;
		}
		case BrainCO2Dep: {
			brainCO2Dep_ = value;
			break;
		}
		case OtherO2Uptake: {
			otherO2Uptake_ = value;
			break;
		}
		case OtherCO2Dep: {
			otherCO2Dep_ = value;
			break;
		}
		default:
			throw std::exception("Wrong tissue's parameter!");
		};
	}
	double BaseTissueSys::GetParam(TissueSysParam param) const
	{
		switch (param)
		{
		case O2Uptake: {
			return brainO2Uptake_ + otherO2Uptake_;
		}
		case CO2Dep: {
			return brainCO2Dep_ + otherCO2Dep_;
		}
		case BrainO2Uptake: {
			return brainO2Uptake_;
		}
		case BrainCO2Dep: {
			return brainCO2Dep_;
		}
		case OtherO2Uptake: {
			return otherO2Uptake_;
		}
		case OtherCO2Dep: {
			return otherCO2Dep_;
		}
		default:
			throw std::exception("Wrong tissue's parameter!");
		};
	}
}