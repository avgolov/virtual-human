#pragma once

#ifndef LLFBHIPOXIAEXPERIMENT_H_
#define LLFBHIPOXIAEXPERIMENT_H_

#include <string>
#include "../../IExperiment.h"

namespace Experiments
{
	class LlFbHipoxiaExperiment : IExperiment
	{
	public:
		LlFbHipoxiaExperiment() { name_ = "hipoxia"; }
		void Run() override;
	};
}
#endif /*LLFBHIPOXIAEXPERIMENT_H_*/

