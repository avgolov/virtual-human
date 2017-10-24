#include "MiptNet1DRespSys.h"
#include "../../../MiptNet1D.Model/NetTopology/Loaders/TreeDataLoader.h"

namespace SystemModels
{
	void MiptNet1DRespSys::SetTree(BronchTree* tree)
	{
		tree_ = tree;
		motionEquation_ = LungsFluidEquation(tree_);
		motionEquation_.set_breath_param(&breathModel_);
		massTransferEquation_ = LungsMtEquation(tree_);
	}

	void MiptNet1DRespSys::Compute(TimeSpan* timeSpan)
	{
		nIter_++;
		timeStep_ += timeSpan->dt;
		resultUtility_.WriteResult(timeSpan->timeCurr);
		auto tree = motionEquation_.GetTree();
		tree->SetPrev();
		motionEquation_.Solve(timeSpan->dt);
		if (nIter_ % 1 == 0) {
			massTransferEquation_.Solve(timeStep_);
			timeStep_ = 0;
			nIter_ = 0;
		}
	}

	double MiptNet1DRespSys::GetTimeStep()
	{
		return motionEquation_.ComputeTimeStep();
	}

	void MiptNet1DRespSys::SetParam(RespiratorySysParam param, double value)
	{
		switch(param)
		{
		case Rate: {
			motionEquation_.SetRate(value);
			break;
		}
		case TidalVolume: {
			motionEquation_.SetTidalVolume(value);
			break;
		}
		case RateSS: {
			motionEquation_.SetRateSS(value);
			break;
		}
		case TidalVolumeSS: {
			motionEquation_.SetTidalVolumeSS(value);
			break;
		}
		case Pg: {
			breathModel_.SetPg(value);
			break;
		}
		default:
			throw std::exception("Wrong for setting parameter!");
		}
	}

	double MiptNet1DRespSys::GetParam(RespiratorySysParam param) const
	{
		switch (param)
		{
		case  Rate: {
			return motionEquation_.GetRate();
		}
		case TidalVolume: {
			return motionEquation_.GetTidalVolume();
		}
		case RateOld: {
			return motionEquation_.GetRateOld();
		}
		case TidalVolumeOld: {
			return motionEquation_.GetTidalVolumeOld();
		}
		case RateSS: {
			return motionEquation_.GetRateSS();
		}
		case TidalVolumeSS: {
			return motionEquation_.GetTidalVolumeSS();
		}
		case Ve: {
			return motionEquation_.GetTidalVolume()*
				   motionEquation_.GetRate();
		}
		default:
			throw std::exception("Wrong lungs parameter!");
		}
	}

	std::vector<Alveole*>* MiptNet1DRespSys::GetAlveoles()
	{
		return motionEquation_.GetAlveoles();
	}
}