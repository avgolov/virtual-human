#pragma once
#ifndef IHUMANSYSBUILDER_H_
#define IHUMANSYSBUILDER_H_

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/pointer_cast.hpp>

#include "ISysBuilder.h"
#include "../Systems/IHumanSys.h"


namespace SystemModels
{
	class IHumanSysBuilder : public ISysBuilder
	{
	public:
		IHumanSysBuilder() {}
		virtual ~IHumanSysBuilder() {};

		boost::shared_ptr<IHumanSys> GetHumanSystem() const 
		{
			return boost::dynamic_pointer_cast<IHumanSys>(system_);
		}
	};
}

#endif /*IHUMANSYSBUILDER_H_*/