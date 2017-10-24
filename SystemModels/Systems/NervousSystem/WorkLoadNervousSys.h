#pragma once
#ifndef WORKLOADNERVOUSSYSTEM_H_
#define WORKLOADNERVOUSSYSTEM_H_

#include <vector>
#include "INervousSys.h"
#include "../WorkLoad/IWorkLoad.h"

namespace SystemModels
{
	class WorkLoadNervousSys : public INervousSys
	{
	public:
		WorkLoadNervousSys() {}

		void Compute(TimeSpan* timeSpan) override {
			ControlBloodSys(timeSpan);
			ControlRespiratorySys(timeSpan);
			ControlTissueSys(timeSpan);
		}

		void SetWorkLoad(boost::shared_ptr<IWorkLoad> workLoad) { workLoad_ = workLoad; }

		//Controling state of human's subsystems; 
		void ControlBloodSys(TimeSpan* timeSpan) override;
		void ControlRespiratorySys(TimeSpan* timeSpan) override;
		void ControlTissueSys(TimeSpan* timeSpan) override;

		void SetVeStorage(std::vector<double> value) { veStorage_ = value; }
		void SetBrStorage(std::vector<double> value) { brStorage_ = value; }
		void SetQStorage(std::vector<double> value) { qStorage_ = value; }
		void SetUseExperiment(bool value) { useExperiment_ = value; }
		void SetSteadyStateThreshold(double value) { steadyStateThreshold_ = value; }
		void SetLungsNco2bs(double value) { nco2bs = value; }
		void SetLungsTco2bs(double value) { tco2bs = value; }
		void SetLungsTco2ps(double value) { tco2ps = value; }
		void SetLungsNco2ps(double value) { nco2ps = value; }
		void SetLungsPco20(double value) { pco20 = value; }
		void SetLungsPo20(double value) { po20 = value; }
			
	private:
		boost::shared_ptr<IWorkLoad> workLoad_;
		bool useExperiment_ = false;
		double steadyStateThreshold_ = 0;

		std::vector<double> veStorage_ = {};
		std::vector<double> brStorage_ = {};
		std::vector<double> qStorage_ = {};

		//Lungs const
		//Threashold constant of the central part of controller
		double nco2bs = 0.54 / 1.5;
		double tco2bs = 45;

		//Gain constant of the peripheral part of controller
		double nco2ps = 1.08 / 4.1;

		//Threashold constant of the peripheral part of controller
		double tco2ps = 57.5;

		double pco20 = 61.7;
		double po20 = 45.55;

		//The lungs ventilation volume threashold
		double ves_ = 20;
	};
}

#endif /*WORKLOADNERVOUSSYSTEM_H_*/