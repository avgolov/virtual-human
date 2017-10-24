#pragma once
#ifndef BODYBIOCHEMBLOODSYS_H_
#define BODYBIOCHEMBLOODSYS_H_

#include <valarray>
#include "IBloodSys.h"

namespace SystemModels
{
	//Simple model 1-Compartment and biochemistry
	class BodyBioChemBloodSys : public IBloodSys
	{
	public:
		void Compute(TimeSpan* timeSpan) override;
		void SetParam(BloodSysParam param, double value) override;
		double GetParam(BloodSysParam param) const override;
		double GetVolume() const { return volume_; }
		void SetVolume(double volume) { volume_ = volume; }
		void SetSubstances(std::valarray<double> substances);
		void SetOldValues() { substancesOld_ = substances_;}
		double GetCo2() const { return substances_[1]; }
		double GetO2() const { return substances_[0];}
		double GetCo2Old() const { return substancesOld_[1]; }
		double GetO2Old() const { return substancesOld_[0]; }
		double GetH() const { return substances_[4]; }
		double GetHCo3() const { return substances_[3]; }
		double GetHb() const { return substances_[2]; }
		double GetHOld() const { return substancesOld_[4]; }
		double GetHCo3Old() const { return substancesOld_[3]; }
		double GetHbOld() const { return substancesOld_[2]; }
		
		void SetCo2(double value) { substances_[1] = value; }
		void SetO2(double value) { substances_[0] = value; }
		void SetH(double value) { substances_[4] = value; }
		void SetHb(double value) { substances_[2] = value; }

		double GetReactHbO2K() const { return kO2Hb_; }
		double GetReactBbCo2K() const { return kCo2Bb_; }
		double GetHillConst() const { return hillConst_; }
		double GetHbTot() const { return hbTot_; }
		double GetSolCoefO2() const { return solCoefO2_; }
		double GetSolCoefCo2() const { return solCoefCo2_; }

		double GetCo2FreeOld() const;
		double GetO2FreeOld() const;
		double GetCo2Free() const;
		double GetO2Free() const;
		
		void SetO2Free(double value);
		void SetHco3(double value);

	private:
		const double kO2Hb_ = 2.09752986e-16;
		const double kCo2Bb_ = 7.425e-7;
		const double hillConst_ = 3.6;
		const double hbTot_ = 2.5839449e-3;
		const double solCoefO2_ = 1.1826e-6;
		const double solCoefCo2_ = 2.6487e-5;
		const double litMolCoef_ = 22.4;
		double volume_;

		/*0-O2; 1-CO2; 2-Hb; 3-HCO3-; 4-H+*/
		std::valarray<double> substances_;
		std::valarray<double> substancesOld_;
	};
}

#endif /*BODYBIOCHEMBLOODSYS_H_*/