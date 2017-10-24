#pragma once
#ifndef WORKHUMANSYSBUILDER_H_
#define WORKHUMANSYSBUILDER_H_

#include "Builders/IHumanSysBuilder.h"


namespace SystemModels
{
	//Model of human under work load 
	class WorkHumanSysBuilder : public IHumanSysBuilder
	{
	public:
		WorkHumanSysBuilder() {}

		void CreateNewSystem() override;

	};
}

#endif /*WORKHUMANSYSBUILDER_H_*/