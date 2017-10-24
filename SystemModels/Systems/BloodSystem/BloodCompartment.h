#pragma once
#ifndef BLOODCOMPARTMENT_H_
#define BLOODCOMPARTMENT_H_

#include <valarray>
#include "IBloodSys.h"
#include "Biochemistry/BioChemConst.h"

namespace SystemModels
{
	class BloodCompartment : public IBloodSys
	{
	public:
		void Compute(TimeSpan* timeSpan) override {}
		void SetParam(BloodSysParam param, double value) override;
		double GetParam(BloodSysParam param) const override;
		double GetVolume() const { return volume_; }
		void SetVolume(double volume) { volume_ = volume; }
		void SetSubstances(std::valarray<double> substances);
		void SetOldValues() { substancesOld_ = substances_; }
		double GetPo2() const;
		double GetPco2() const;
		void SetPo2(double value);
		void SetPco2(double value);
		double GetCo2() const { return substances_[1]; }
		double GetO2() const { return substances_[0]; }
		double GetCo2Old() const { return substancesOld_[1]; }
		double GetO2Old() const { return substancesOld_[0]; }
		double GetH() const { return substances_[4]; }
		double GetHCo3() const { return substances_[3]; }
		double GetHb() const { return substances_[2]; }
		double GetLa() const { return substances_[5]; }
		double GetHOld() const { return substancesOld_[4]; }
		double GetHCo3Old() const { return substancesOld_[3]; }
		double GetHbOld() const { return substancesOld_[2]; }
		double GetLaOld() const { return substancesOld_[5]; }

		void SetCo2(double value) { substances_[1] = value; }
		void SetO2(double value) { substances_[0] = value; }
		void SetH(double value) { substances_[4] = value; }
		void SetHb(double value) { substances_[2] = value; }
		void SetLa(double value) { substances_[5] = value; }

		double GetReactHbO2K() const { return bcm_.KO2Hb; }
		double GetReactBbCo2K() const { return kCo2Bb_; }
		double GetHillConst() const { return bcm_.HillConst; }
		double GetHbTot() const { return bcm_.HbTot; }
		double GetSolCoefO2() const { return bcm_.SolCoefO2; }
		double GetSolCoefCo2() const { return solCoefCo2_; }

		double GetCo2FreeOld() const;
		double GetO2FreeOld() const;
		double GetCo2Free() const;
		double GetO2Free() const;

		void SetO2Free(double value);
		void SetHco3(double value, double m);

	private:
		
		const double kCo2Bb_ = 7.425e-7;
		const double solCoefCo2_ = 2.6487e-5;
		double volume_;

		/*0-O2; 1-CO2; 2-Hb; 3-HCO3-; 4-H+; 5-La; 6-NaHCO3*/
		std::valarray<double> substances_;
		std::valarray<double> substancesOld_;
		BioChemConst bcm_;
	};
}

#endif /*BLOODCOMPARTMENT_H_*/