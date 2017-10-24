#pragma once

#ifndef BASESUBSTANCE_H_
#define BASESUBSTANCE_H_

#include <Eigen\Dense>
#include <vector>

namespace MiptNet1DModel {

	
	class BaseSubstance {

	public:
		//��������
		double Velocity;

		//��������
		double Pressure;

		//���������
		double Density;

		double Viscosity;

		//������������ ���-�
		std::vector<double> Conc;

		//����. �������� ���-�
		std::vector<double> D_conc;

	};
}

#endif /*BASESUBSTANCE_H_*/
