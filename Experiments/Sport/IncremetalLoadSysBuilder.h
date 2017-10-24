#pragma once
#ifndef INCREMENTALSYSBUILDER_H_
#define INCREMENTALSYSBUILDER_H_

#include <boost/property_tree/ptree.hpp>
#include "../HumanSysBuilders/IHumanSysBuilder.h"

using namespace SystemModels;

namespace Experiments
{
	class IncremetalLoadSysBuilder : public IHumanSysBuilder
	{
	public:
		IncremetalLoadSysBuilder(boost::property_tree::ptree* config) : config_(config){}

		void CreateNewSystem() override;
	
	private:
		boost::property_tree::ptree* config_;
	};
}

#endif /*INCREMENTALSYSBUILDER_H_*/
