#pragma once

#ifndef FLUIDSTATEEQUATION_H_
#define FLUIDSTATEEQUATION_H_

#include "../../NetTopology/Model/BranchPoint.h"

namespace MiptNet1DModel {

	//��������� ��������� � ������
	class FluidStateEquation {

	public:
		double Pressure(BranchPoint* point) const
		{
			return std::pow(point->Cwall, 2)* point->Substance.Density * 
				   (point->Square / point->SquareInit - 1);
		}
		
		// ��������/���������
		double P_ro(BranchPoint* point, double square) const
		{
			return std::pow(point->Cwall, 2) * (square / point->SquareInit - 1);
		}

		// d(��������/���������)/d(�������)
		double Ds_p_ro(BranchPoint* point) const
		{
			return std::pow(point->Cwall, 2) / point->SquareInit;
		}
	};
}

#endif /*FLUIDSTATEEQUATION_H_*/