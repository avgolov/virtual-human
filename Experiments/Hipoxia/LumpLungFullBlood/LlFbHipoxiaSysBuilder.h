#pragma once
#ifndef LLFBHIPOXIASYSBUILDER_H_
#define LLFBHIPOXIASYSBUILDER_H_

#include <boost/property_tree/ptree.hpp>
#include "../../HumanSysBuilders/IHumanSysBuilder.h"

using namespace SystemModels;

namespace Experiments
{
	class LlFbHipoxiaSysBuilder : public IHumanSysBuilder
	{
	public:
		LlFbHipoxiaSysBuilder(boost::property_tree::ptree* config) : config_(config) {}

		void CreateNewSystem() override;

	private:
		boost::property_tree::ptree* config_;
	};
}

#endif /*LLFBHIPOXIASYSBUILDER_H_*/
