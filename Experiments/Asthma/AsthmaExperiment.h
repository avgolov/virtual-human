#pragma once

#ifndef ASTHMAEXPERIMENT_H_
#define ASTHMAEXPERIMENT_H_

#include <string>
#include "../IExperiment.h"
#include "../../MiptNet1D.Model/NetTopology/Model/BronchTree.h"

namespace Experiments
{
	class AsthmaExperiment : IExperiment
	{
	public:
		AsthmaExperiment()	{ name_ = "asthma"; }
		void Run() override;
	
	private:
		void ModifyBronchTree(MiptNet1DModel::BronchTree* tree, double s) const;
	};
}
#endif /*ASTHMAEXPERIMENT_H_*/

