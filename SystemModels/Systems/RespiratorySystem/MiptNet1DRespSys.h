#pragma once
#ifndef MIPTNET1DRespSYS_H_
#define MIPTNET1DRespSYS_H_

#include <vector>
#include "IRespiratorySys.h"
#include "../../../MiptNet1D.Model/NetTopology/Model/BronchTree.h"
#include "../../../MiptNet1D.Model/BlockEquations/MassTransfer/LungsMtEquation.h"
#include "../../../MiptNet1D.Model/BlockEquations/Motion/LungsFluidEquation.h"
#include "../../../MiptNet1D.Model/BlockEquations/Motion/Breath/StandartBreath.h"
#include "../../../MiptNet1D.Model/NetTopology/Results/ResultHandlerUtility.h"

using namespace MiptNet1DModel;

namespace SystemModels
{
	//1D dynamical model of the flow of incompressible fluid in the network of elastic tubes.
	//The flow in tube is state by mass and momentum balance equation
	//The single-component representation of the Respiratory zone of lungs, 
	//in which the total mass of the lungs is considered as distributed over 
	//the surface of the variable volume reservoirs. Each reservoir is connected to a tube 
	//of the latest generation of the trachea-bronchial tree. 
	//The mechanical properties of these reservoirs are determined by integral characteristics
	class MiptNet1DRespSys : public IRespiratorySys
	{
	public:
		MiptNet1DRespSys() {};

		void Compute(TimeSpan* timeSpan) override;
		double GetTimeStep() override;
		void SetParam(RespiratorySysParam param, double value) override;
		double GetParam(RespiratorySysParam param) const override;
		std::vector<Alveole*>* GetAlveoles();
		BronchTree* GetTree() const { return tree_; }
		void SetTree(BronchTree* tree);

		void SetResultutility(ResultHandlerUtility resultUtility) {	resultUtility_ = resultUtility;	}
		void WriteResult(TimeSpan* timeSpan) const { resultUtility_.WriteResult(timeSpan->timeCurr); }
		LungsFluidEquation* GetMotionEquation() { return &motionEquation_; }
		LungsMtEquation* GetMassTransfEquation() { return &massTransferEquation_; }

	private:
		BronchTree* tree_ = nullptr;
		StandartBreath breathModel_;
		LungsFluidEquation motionEquation_;
		LungsMtEquation massTransferEquation_;
		ResultHandlerUtility resultUtility_;
		double timeStep_ = 0;
		int nIter_ = 0;
	};
}

#endif /*MIPTNET1DRespSYS_H_*/
