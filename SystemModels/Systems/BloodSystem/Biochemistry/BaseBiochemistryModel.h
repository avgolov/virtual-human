#pragma once
#ifndef BLOODBIOCHEMESTRYMODEL_H_
#define BLOODBIOCHEMESTRYMODEL_H_

#include <valarray>
#include "../../ISystem.h"

namespace SystemModels
{
	class TimeSpan;

	class BaseBiochemistryModel : ISystem
	{
	public:
		BaseBiochemistryModel(std::valarray<double>* values,
			                  std::valarray<double>* valuesOld) : 
			values_(values), 
			valuesOld_(valuesOld) {}

		void Compute(TimeSpan* timeSpan) override;
		double GetPo2() const;
		double GetPco2() const;
		void SetPo2(double value);
		void SetPco2(double value);
		double GetSo2() const;
		
	private:
		std::valarray<double>* values_;
		std::valarray<double>* valuesOld_;

		const double k1_ = 5.e-4;
		const double k11_ = 0.89;//5.e-4 * 177916;
		const double k2_ = 3.71e-2;
		const double k22_ = 1.48e-2;
		const double m_ = 3.6;
		const double hb_tot_ = 2.33e-3;

		//Solubility coefficient of O2 in water (Mol/mmHg)
		const double a_o2_ = 1.27e-6;

		//Solubility coefficient of CO2 in water (Mol/mmHg)
		const double a_co2_ = 2.84e-5;

		double b_o2_ = 0.;
		
		double b_co2_ = 0.;

		//Blood O2 concentration
		double& o2() const { return (*values_)[0]; }
		
		//Blood CO2 concentration
		double& co2() const { return (*values_)[1]; }
		
		//Blood Hb full saturated concentration
		double& hb4() const { return (*values_)[2]; }

		//Blood HCO3- concentration
		double& hco3() const { return (*values_)[3]; }

		//Blood H+ concentration
		double& h() const { return (*values_)[4]; }
	};
}

#endif /*BLOODBIOCHEMESTRYMODEL_H_*/