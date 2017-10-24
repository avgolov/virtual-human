#pragma once
#ifndef BIOCHEMCONST_H_
#define BIOCHEMCONST_H_

namespace SystemModels
{
	
	class BioChemConst
	{
	public:
		static double KO2Hb;
		static double KCO2Bb;
		static double HillConst;
		static double HbTot;
		static double SolCoefO2;
		static double SolCoefCo2;
		static double LitMolCoef;
		
		//The blood-lungs O2 diffusion coef
		static  double Do2Cf;
		//The blood-lungs CO2 diffusion coef
		static  double Dco2Cf;
		
		static double AtmPres;
		static double Co2M;
	};
}

#endif /*BIOCHEMCONST_H_*/