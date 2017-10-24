#pragma once

#ifndef ABSTRACTRESULTHANDLER_H_
#define ABSTRACTRESULTHANDLER_H_

#include <iostream>
#include <fstream>
#include "IStoreCondition.h"

namespace MiptNet1DModel {

	enum VariableType {
		Velocity,
		Square,
		Pressure,
		Volume,
		Conc_O2,
		Conc_CO2,
		P_CO2,
		P_O2
	};

	//Логирование результатов
	class AbstractResultHandler {
	public:
		AbstractResultHandler(IStoreCondition* condition, std::string path) :
			condition_(condition), path_(path) { }

		virtual ~AbstractResultHandler() {}

		virtual void WriteResult(double curr_time) {
			last_time_ = curr_time_;
			curr_time_ = curr_time;
			Process();
			if (!condition_->Check(curr_time)) return;
			
			auto value = GetValue();
			if(!stream_.is_open()) {
				auto path = GetFileName();
				stream_.open(path, std::ofstream::out | std::ofstream::trunc);
			}
			stream_ << value << " " << std::endl;
		}

	protected:
		IStoreCondition* condition_;
		std::ofstream stream_;
		std::string path_;
		double curr_time_ = 0.;
		double last_time_ = 0.;
		
		virtual double GetValue() = 0;
		virtual std::string GetFileName() const	= 0;
		virtual void Process() { };
	};
}

#endif /*ABSTRACTRESULTHANDLER_H_*/