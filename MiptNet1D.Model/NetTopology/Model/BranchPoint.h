#pragma once

#ifndef BRANCHPOINT_H_
#define BRANCHPOINT_H_

#include "BaseSubstance.h"

namespace MiptNet1DModel {

	class BranchPoint {

	public:
		BranchPoint(){}
		BranchPoint(unsigned int id, unsigned int branch_id,
			        double square, double c_wall, 
					BaseSubstance substance) :
			Square(square), 
			SquarePrev(square), 
			SquareInit(square),
			Cwall(c_wall),
			Substance(substance),
			SubstancePrev(substance),
			id_(id),
			branch_id_(branch_id) { }

		//Площадь сосуда
		double Square = 0.;
		
		//Площадь сосуда на предыдущем шаге
		double SquarePrev = 0.;

		//Начальная площадь сосуда
		double SquareInit = 0.;

		//Скорость распространения малых 
		//возмущений в стенке сосуда
		double Cwall = 0.;

		//Параметры вещества
		BaseSubstance Substance;

		//параметры вещества на предыдущем шаге
		BaseSubstance SubstancePrev;

		unsigned int get_id() const { return id_; }
		unsigned int get_branch_id() const { return branch_id_; }

	private:
		unsigned int id_ = 0;
		unsigned int branch_id_ = 0;
	};
}

#endif /*BRANCHPOINT_H_*/