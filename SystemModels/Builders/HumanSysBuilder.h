#pragma once
#ifndef HUMANSYSBUILDER_H_
#define HUMANSYSBUILDER_H_

#include "IHumanSysBuilder.h"


namespace SystemModels
{
	class HumanSysBuilder : public IHumanSysBuilder
	{
	public:
		HumanSysBuilder() {}

		void CreateNewSystem() override;
		
	};
}

#endif /*HUMANSYSBUILDER_H_*/