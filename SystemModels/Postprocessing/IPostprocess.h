#pragma once

#ifndef IPOSTPROCESS_H_
#define IPOSTPROCESS_H_

#include <iostream>
#include <fstream>
#include "ISaveCondition.h"

namespace SystemModels {

	class IPostprocess{
	public:
		IPostprocess(ISaveCondition* condition) :
			condition_(condition) { }

		virtual ~IPostprocess() {}

		virtual void Handle(double curr_time) {
			Process();
			if (!condition_->Check(curr_time)) return;

			auto value = GetValue();
			if (!stream_.is_open()) {
				auto path = GetFileName();
				stream_.open(path, std::ofstream::out | std::ofstream::trunc);
			}
			stream_ << value << " " << std::endl;
		}

	protected:
		ISaveCondition* condition_;
		std::ofstream stream_;

		virtual double GetValue() = 0;
		virtual std::string GetFileName() const = 0;
		virtual void Process() { };
	};
}

#endif /*IPOSTPROCESS_H_*/