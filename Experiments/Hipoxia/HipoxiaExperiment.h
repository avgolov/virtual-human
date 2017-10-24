#pragma once

#ifndef HIPOXIAEXPERIMENT_H_
#define HIPOXIAEXPERIMENT_H_

#include <string>
#include "../IExperiment.h"

namespace Experiments
{
	class HipoxiaExperiment : IExperiment
	{
	public:
		HipoxiaExperiment() { name_ = "hipoxia"; }
		void Run() override;
	};
}
#endif /*HipoxiaExperiment*/

