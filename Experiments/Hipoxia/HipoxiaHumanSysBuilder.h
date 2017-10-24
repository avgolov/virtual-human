#pragma once
#ifndef HIPOXIAUMANSYSBUILDER_H_
#define HIPOXIAUMANSYSBUILDER_H_

#include "../HumanSysBuilders/IHumanSysBuilder.h"
#include "../../MiptNet1D.Model/NetTopology/Model/BronchTree.h"

using namespace SystemModels;

namespace Experiments
{
	class HipoxiaHumanSysBuilder : public IHumanSysBuilder
	{
	public:
		HipoxiaHumanSysBuilder(MiptNet1DModel::BronchTree* tree) : tree_(tree) {}

		void CreateNewSystem() override;

	private:
		MiptNet1DModel::BronchTree* tree_;
	};
}

#endif /*HIPOXIAUMANSYSBUILDER_H_*/
