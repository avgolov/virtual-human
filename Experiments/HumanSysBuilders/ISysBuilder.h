#pragma once
#ifndef ISYSBUILDER_H_
#define ISYSBUILDER_H_

#include <memory>
#include "../../SystemModels/Systems/ISystem.h"

namespace Experiments
{
	//The builder of any human's systems
	class ISysBuilder
	{
	public:
		ISysBuilder() {}
		virtual ~ISysBuilder() {};

		boost::shared_ptr<SystemModels::ISystem> GetSystem() const { return system_; }
		virtual void CreateNewSystem() = 0;

	protected:
		boost::shared_ptr<SystemModels::ISystem> system_;
	};
}

#endif /*ISYSBUILDER_H_*/