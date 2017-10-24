#pragma once
#ifndef ISYSBUILDER_H_
#define ISYSBUILDER_H_

#include <memory>
#include "../Systems/ISystem.h"

namespace SystemModels
{
	//The builder of any human's systems
	class ISysBuilder 
	{
	public:
		ISysBuilder() {}
		virtual ~ISysBuilder() {};

		boost::shared_ptr<ISystem> GetSystem() const { return system_; }
		virtual void CreateNewSystem() = 0;

	protected:
		boost::shared_ptr<ISystem> system_;
	};
}

#endif /*ISYSBUILDER_H_*/