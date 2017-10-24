#pragma once

#ifndef FLUIDSTATEEQUATION_H_
#define FLUIDSTATEEQUATION_H_

#include "../../NetTopology/Model/BranchPoint.h"

namespace MiptNet1DModel {

	//Уравнение состояния в сосуде
	class FluidStateEquation {

	public:
		double Pressure(BranchPoint* point) const
		{
			return std::pow(point->Cwall, 2)* point->Substance.Density * 
				   (point->Square / point->SquareInit - 1);
		}
		
		// Давление/Плотность
		double P_ro(BranchPoint* point, double square) const
		{
			return std::pow(point->Cwall, 2) * (square / point->SquareInit - 1);
		}

		// d(Давление/Плотность)/d(Площадь)
		double Ds_p_ro(BranchPoint* point) const
		{
			return std::pow(point->Cwall, 2) / point->SquareInit;
		}
	};
}

#endif /*FLUIDSTATEEQUATION_H_*/