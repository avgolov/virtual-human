#pragma once

#ifndef BREATHRESULTHANDLER_H_
#define BREATHRESULTHANDLER_H_

#include "AbstractResultHandler.h"
#include "../../BlockEquations/Motion/Breath/AbstractBreath.h"

namespace MiptNet1DModel {

	enum BreathVariableType
	{
		Frequency,
		Ve
	};

	class BreathResultHandler : public AbstractResultHandler {
	public:
		BreathResultHandler(
			IStoreCondition* condition,
			AbstractBreath* breath_model,
			BreathVariableType variable_type,
			std::string path) :
			AbstractResultHandler(condition, path),
			breath_model_(breath_model),
			variable_type_(variable_type) { }

	protected:
		double GetValue() override {
			if(variable_type_ == Frequency) {
				return breath_model_->GetRateCur();
			}
			if (variable_type_ == Ve) {
				return breath_model_->GetTidalVolumeCur();
			}
			throw std::exception("Wrong variable type!");
		}

		std::string GetFileName() const override {
			return path_ + "/breath_" +
				(variable_type_ == Frequency ? 
					std::string("frequency") : 
					std::string("ve")) + ".txt";
		};

	private:
		AbstractBreath* breath_model_;
		BreathVariableType variable_type_;
	};
}

#endif /*BREATHRESULTHANDLER_H_*/
