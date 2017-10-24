#pragma once

#ifndef EMPHYSEMAEXPERIMENT_H_
#define EMPHYSEMAEXPERIMENT_H_

#include <string>
#include "../IExperiment.h"
#include "../../MiptNet1D.Model/NetTopology/Model/BronchTree.h"

namespace Experiments
{
	class EmphysemaExperiment : IExperiment
	{
	public:
		EmphysemaExperiment()	{ name_ = "emphysema"; }
		void Run() override;
	
	private:
		void ModifyBronchTree(MiptNet1DModel::BronchTree* tree, double s) const;
	};
}
#endif /*EMPHYSEMAEXPERIMENT_H_*/

