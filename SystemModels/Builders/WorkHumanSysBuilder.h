#pragma once
#ifndef WORKHUMANSYSBUILDER_H_
#define WORKHUMANSYSBUILDER_H_

#include "IHumanSysBuilder.h"


namespace SystemModels
{
	//Model human system under work load
	class WorkHumanSysBuilder : public IHumanSysBuilder
	{
	public:
		WorkHumanSysBuilder() {}

		void CreateNewSystem() override;

	};
}

#endif /*WORKHUMANSYSBUILDER_H_*/