#pragma once

#ifndef BASESUBSTANCE_H_
#define BASESUBSTANCE_H_

#include <Eigen\Dense>
#include <vector>

namespace MiptNet1DModel {

	
	class BaseSubstance {

	public:
		//Скорость
		double Velocity;

		//Давление
		double Pressure;

		//плотность
		double Density;

		double Viscosity;

		//Концентрация вещ-в
		std::vector<double> Conc;

		//Коэф. диффузии вещ-в
		std::vector<double> D_conc;

	};
}

#endif /*BASESUBSTANCE_H_*/
