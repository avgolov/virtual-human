#pragma once

#ifndef IEXPERIMENT_H_
#define IEXPERIMENT_H_

#include <string>

namespace Experiments
{
	class IExperiment
	{
	public:
		virtual ~IExperiment() { };
		virtual void Run() = 0;
	
	protected:
		std::string name_;
	};
}
#endif /*IEXPERIMENT_H_*/
