#include "WlLaEquations.h"

namespace SystemModels
{
	Vector WlLaEquations::GetBloodF(const Vector& u)
	{
		//J – {PA, PV, SV, BA, TA} = {1, 2, 3, 4, 5}
		Eigen::VectorXd result = Eigen::VectorXd::Zero(5);

		//Lactate part
		//Convective flow
		result.coeffRef(0) = CardiacOutput*(-u.coeffRef(0) + u.coeffRef(2)) / Volumes[0];
		result.coeffRef(1) = CardiacOutput*(u.coeffRef(0) - u.coeffRef(1)) / Volumes[1];
		result.coeffRef(2) = (-CardiacOutput*u.coeffRef(2) + BrainFlow*u.coeffRef(3) +
			(CardiacOutput - BrainFlow)*u.coeffRef(4)) / Volumes[2];
		result.coeffRef(3) = BrainFlow*(u.coeffRef(1) - u.coeffRef(3)) / Volumes[3];
		result.coeffRef(4) = (CardiacOutput - BrainFlow)*(u.coeffRef(1) - u.coeffRef(4)) / Volumes[4];

		//Source part
		result.coeffRef(4) += (LaWsource - UtLa*(u.coeffRef(4) - LaMin)) / Volumes[4];
		return result;
	}

	Matrix WlLaEquations::GetBloodDf(const Vector& u)
	{
		//J – {PA, PV, SV, BA, TA} = {1, 2, 3, 4, 5}
		Eigen::MatrixXd result = Eigen::MatrixXd::Zero(5, 5);

		//Lactate part
		//Convective flow
		result.coeffRef(0, 0) = -CardiacOutput / Volumes[0];
		result.coeffRef(0, 2) = CardiacOutput / Volumes[0];
		result.coeffRef(1, 0) = CardiacOutput / Volumes[1];
		result.coeffRef(1, 1) = -CardiacOutput / Volumes[1];
		result.coeffRef(2, 2) = -CardiacOutput / Volumes[2];
		result.coeffRef(2, 3) = BrainFlow / Volumes[2];
		result.coeffRef(2, 4) = (CardiacOutput - BrainFlow) / Volumes[2];
		result.coeffRef(3, 1) = BrainFlow / Volumes[3];
		result.coeffRef(3, 3) = -BrainFlow / Volumes[3];
		result.coeffRef(4, 1) = (CardiacOutput - BrainFlow) / Volumes[4];
		result.coeffRef(4, 4) = -(CardiacOutput - BrainFlow) / Volumes[4];

		//Source part
		result.coeffRef(4, 4) -= UtLa / Volumes[4];

		return result;
	}

}