#pragma once

#ifndef BRONCHTREERESULTHANDLER_H_
#define BRONCHTREERESULTHANDLER_H_

#include "AbstractResultHandler.h"
#include "../Model/BranchPoint.h"
#include "../Model/Alveole.h"

namespace MiptNet1DModel {

	//Параметры в бронхиальном дереве
	class BronchTreeResultHandler : public AbstractResultHandler {
	public:
		BronchTreeResultHandler(IStoreCondition* condition, 
								BranchPoint* point, 
								VariableType variable_type,
								std::string path) :
			AbstractResultHandler(condition, path),
			point_(point),
			variable_type_(variable_type) { }

		BronchTreeResultHandler(IStoreCondition* condition,
								Alveole* alveole , 
								VariableType variable_type,
								std::string path) :
			AbstractResultHandler(condition, path),
			alveole_(alveole),
			variable_type_(variable_type) { }

	protected:
		double GetValue() override {
			if (point_) {
				if (variable_type_ == Velocity) {
					return point_->Substance.Velocity;
				}
				if (variable_type_ == Conc_O2) {
					return point_->Substance.Conc[0];
				}
				if (variable_type_ == Conc_CO2) {
					return point_->Substance.Conc[1];
				}
				if (variable_type_ == Square) {
					return point_->Square;
				}
				if (variable_type_ == Pressure) {
					return point_->Substance.Pressure;
				}
				throw std::exception("Wrong variable type!");
			}
			if(alveole_)
			{
				if (variable_type_ == Velocity) {
					return alveole_->Substance.Velocity;
				}
				if (variable_type_ == Conc_O2) {
					return alveole_->Substance.Conc[0];
				}
				if (variable_type_ == Conc_CO2) {
					return alveole_->Substance.Conc[1];
				}
				if(variable_type_ == Volume) {
					return alveole_->Volume;
				}
				if (variable_type_ == Pressure) {
					return alveole_->Substance.Pressure;
				}
				if(variable_type_ == P_O2) {
					return alveole_->Substance.Conc[0]* alveole_->Substance.Pressure;
				}
				if (variable_type_ == P_CO2) {
					return alveole_->Substance.Conc[1] * alveole_->Substance.Pressure;
				}
				throw std::exception("Wrong variable type!");
			}
			return 0.;
		};
		
		std::string GetFileName() const override {
			if (point_) {
				return path_ + "/branch_" +
					std::to_string(point_->get_branch_id()) + "_" +
					std::to_string(point_->get_id()) + "_" +
					GetVariableName() + ".txt";
			}
			if(alveole_) {
				return path_ + "/alveole_" +
					std::to_string(alveole_->get_id()) + "_" +
					GetVariableName() + ".txt";
			}
			throw new std::exception("Wrong state!");
		};

	private:
		BranchPoint* point_ = nullptr;
		Alveole* alveole_ = nullptr;
		VariableType variable_type_;

		std::string GetVariableName() const	{
			switch(variable_type_) {
				case Velocity: return "vel";
				case Conc_O2: return "conc_o2";
				case Conc_CO2: return "conc_co2";
				case Square: return "square";
				case Volume: return "volume";
				case P_CO2: return "p_co2";
				case P_O2: return "p_o2";
				case Pressure: return "pressure";
				default: return "";
			}
		}
	};
}

#endif /*BRONCHTREERESULTHANDLER_H_*/