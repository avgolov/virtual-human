#pragma once

#ifndef LUMPHIPOXIAEXPERIMENT_H_
#define LUMPHIPOXIAEXPERIMENT_H_

#include <string>
#include "../../IExperiment.h"

namespace Experiments
{
	class LumpHipoxiaExperiment : IExperiment
	{
	public:
		LumpHipoxiaExperiment() { name_ = "lump_hipoxia"; }
		void Run() override;
	};
}
#endif /*LUMPHIPOXIAEXPERIMENT_H_*/

