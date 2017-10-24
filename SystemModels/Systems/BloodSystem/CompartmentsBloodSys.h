#pragma once
#ifndef COMPARTMENTSBLOODSYSTEM_H_
#define COMPARTMENTSBLOODSYSTEM_H_

#include <Eigen/Dense>
#include <array>
#include "IBloodSys.h"
#include "BloodCompartment.h"

namespace SystemModels
{
	//The system of connected blood tanks
	class CompartmentsBloodSys : public IBloodSys
	{
	public:
		CompartmentsBloodSys() {}

		void Compute(TimeSpan* timeSpan) override;
		void SetParam(BloodSysParam param, double value) override;
		double GetParam(BloodSysParam param) const override;

		void SetPulmArtTankVol(double volume) { pulmArtTank_.SetVolume(volume); }
		void SetPulmVenTankVol(double volume) { pulmVenTank_.SetVolume(volume); }
		void SetBrainArtTankVol(double volume) { brainArtTank_.SetVolume(volume); }
		void SetTissueArtTankVol(double volume) { tissueArtTank_.SetVolume(volume); }
		void SetSysVenTankVol(double volume) { sysVenTank_.SetVolume(volume); }
		void SetSubstances(std::valarray<double> substances);
		Eigen::VectorXd GetO2FreeOldValues();
		Eigen::VectorXd GetHco3OldValues();
		Eigen::VectorXd GetHOldValues();
		Eigen::VectorXd GetLaOldValues();
		void SetO2Free(const Eigen::VectorXd& u);
		void SetHco3(const Eigen::VectorXd& u, const Eigen::VectorXd& m);
		void SetLa(const Eigen::VectorXd& u);

		double O2Cons = 0.;
		double CO2Prod = 0.;
		double Hipoxia = 0;
	private:

		BloodCompartment pulmArtTank_;
		BloodCompartment pulmVenTank_;
		BloodCompartment brainArtTank_;
		BloodCompartment tissueArtTank_;
		BloodCompartment sysVenTank_;

		double cardiacOutput_ = 0.;
		double cardiacOutputOld_ = 0.;
		double cardiacOutputSS_ = 0.;

		double brainFlow_ = 0.;
		double brainFlowOld_ = 0.;
		double brainFlowSS_ = 0.;

		double tissueFlow_ = 0.;
		double tissueFlowOld_ = 0.;
		double tissueFlowSS_ = 0.;


	};
}

#endif /*COMPARTMENTSBLOODSYSTEM_H_*/