#pragma once
#ifndef ASTHMAHUMANSYSBUILDER_H_
#define ASTHMAHUMANSYSBUILDER_H_

#include "../HumanSysBuilders/IHumanSysBuilder.h"
#include "../../MiptNet1D.Model/NetTopology/Model/BronchTree.h"

using namespace SystemModels;

namespace Experiments
{
	class AsthmaHumanSysBuilder : public IHumanSysBuilder
	{
	public:
		AsthmaHumanSysBuilder(MiptNet1DModel::BronchTree* tree) : tree_(tree) {}

		void CreateNewSystem() override;
	
	private:
		MiptNet1DModel::BronchTree* tree_;
	};
}

#endif /*ASTHMAHUMANSYSBUILDER_H_*/
