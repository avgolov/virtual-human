#pragma once

#ifndef INTERVALLOAD_H_
#define INTERVALLOAD_H_

#include <string>
#include "../../IExperiment.h"

namespace Experiments
{
	class IntervalLoad : IExperiment
	{
	public:
		IntervalLoad() { name_ = "intervalLoad"; }
		void Run() override;

	};
}
#endif /*INTERVALLOAD_H_*/


