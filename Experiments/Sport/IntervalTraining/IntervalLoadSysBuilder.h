#pragma once
#ifndef INTERVALLOADSYSBUILDER_H_
#define INTERVALLOADSYSBUILDER_H_

#include <boost/property_tree/ptree.hpp>
#include "../../HumanSysBuilders/IHumanSysBuilder.h"

using namespace SystemModels;

namespace Experiments
{
	class IntervalLoadSysBuilder : public IHumanSysBuilder
	{
	public:
		IntervalLoadSysBuilder(boost::property_tree::ptree* config) : 
			config_(config){}

		void CreateNewSystem() override;

	private:
		boost::property_tree::ptree* config_;
	};
}

#endif /*INTERVALLOADSYSBUILDER_H_*/
