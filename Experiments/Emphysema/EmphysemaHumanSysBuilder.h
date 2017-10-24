#pragma once
#ifndef EMPHYSEMAHUMANSYSBUILDER_H_
#define EMPHYSEMAHUMANSYSBUILDER_H_

#include "../HumanSysBuilders/IHumanSysBuilder.h"
#include "../../MiptNet1D.Model/NetTopology/Model/BronchTree.h"

using namespace SystemModels;

namespace Experiments
{
	class EmphysemaHumanSysBuilder : public IHumanSysBuilder
	{
	public:
		EmphysemaHumanSysBuilder(MiptNet1DModel::BronchTree* tree) : tree_(tree) {}

		void CreateNewSystem() override;
	
	private:
		MiptNet1DModel::BronchTree* tree_;
	};
}

#endif /*EPHYSEMAHUMANSYSBUILDER_H_*/
