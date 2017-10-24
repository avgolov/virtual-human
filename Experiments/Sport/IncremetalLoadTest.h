#pragma once

#ifndef INCREMENTALLOADTEST_H_
#define INCREMENTALLOADTEST_H_

#include <string>
#include "../IExperiment.h"

namespace Experiments
{
	class IncremetalLoadTest : IExperiment
	{
	public:
		IncremetalLoadTest() { name_ = "incremLoadTest"; }
		void Run() override;

	};
}
#endif /*INCREMENTALLOADTEST_H_*/


