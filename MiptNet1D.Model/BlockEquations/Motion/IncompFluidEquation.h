#pragma once

#ifndef INCOMPFLUIDEQUATION_H_
#define INCOMPFLUIDEQUATION_H_

#include <Eigen\Dense>
#include "tbb/tbb.h"
#include "FluidKnotsSetEquations.h"
#include "../AbstractEquation.h"

namespace MiptNet1DModel {

	//Модель движения несжимаемой жидкости
	class IncompFluidEquation : public AbstractEquation {

	public:
		FluidStateEquation state_;
		
		IncompFluidEquation(BaseTree* tree):AbstractEquation(tree) { }
		IncompFluidEquation(){ }
		IncompFluidEquation& IncompFluidEquation::operator=(const IncompFluidEquation& other) {
			tree_ = other.tree_;
			state_ = other.state_;
			return *this;
		}

		double ComputeTimeStep();

		void Solve(double dt) override{
			ComputeInternalPoints(dt);
			ComputeMultiKnots(dt);
			ComputeBoundaryKnots(dt);
		};

	protected:
		virtual void ComputeBoundaryKnots(double dt) = 0;
		void ComputeMultiKnots(double dt);
		void ComputeInternalPoints(double dt);
		
		void InCompatibilityCoef(Branch* branch, double& alpha, double& beta, double dt) const;
		void OutCompatibilityCoef(Branch* branch, double& alpha, double& beta, double dt) const;

	private:
		void CheckMultiKnot(Knot* knot, std::vector<double>& alpha, 
			                std::vector<double>& beta, Eigen::VectorXd& s);
		
		void SetBadMultiKnot(Knot* knot) const;
		Eigen::VectorXd GetRightPart(BranchPoint* point, double* s, double* v) const;
		
	};
}

#endif /*INCOMPFLUIDEQUATION_H_*/