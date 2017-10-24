#pragma once
#ifndef LUMPHUMANSYSBUILDER_H_
#define LUMPHUMANSYSBUILDER_H_

#include "../../HumanSysBuilders/IHumanSysBuilder.h"

using namespace SystemModels;

namespace Experiments
{
	class LumpHumanSysBuilder : public IHumanSysBuilder
	{
	public:
		LumpHumanSysBuilder() {}

		void CreateNewSystem() override;

	};
}

#endif /*LUMPHUMANSYSBUILDER_H_*/
