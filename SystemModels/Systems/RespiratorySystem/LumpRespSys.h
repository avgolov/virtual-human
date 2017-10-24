#pragma once
#ifndef LUMPRespSYS_H_
#define LUMPRespSYS_H_

#include <memory>

#include "IRespiratorySys.h"
#include "IAirEnv.h"
#include <boost/smart_ptr/shared_ptr.hpp>

namespace SystemModels
{
	//One-component lungs representation
	class LumpRespSys : public IRespiratorySys
	{
	public:
		LumpRespSys(double vol, double r, double c, boost::shared_ptr<IAirEnv> air):
			_vol(vol), _vol_old(vol), _vol_ss(vol), _r(r), _c(c), _air(air) {};

		void Compute(TimeSpan* timeSpan) override;
		double GetTimeStep() override;
		void SetParam(RespiratorySysParam param, double value) override;
		double GetParam(RespiratorySysParam param) const override;
		double GetO2() const { return _c_o2; }
		double GetO2old() const { return _c_o2_old; }
		void SetO2(double value) { _c_o2 = value; }
		double GetCo2() const { return _c_co2; }
		double GetCo2old() const { return _c_co2_old; }
		void SetCo2(double value) { _c_co2 = value; }
		double GetVol() const { return _vol; }
		double GetVolSS() const { return _vol_ss; }
		double GetVolOld() const { return _vol_old; }
		double GetSquare() const;
		double GetSquareOld() const;
		double GetAirO2(TimeSpan* timeSpan)  { return _air->GetO2(timeSpan); }
		double GetAirCo2(TimeSpan* timeSpan) { return _air->GetCO2(timeSpan); }

	private:
		double _vol;
		double _vol_old;
		double _vol_ss;
		double _r;
		double _c;
		double _rate = 0;
		double _rate_old = 0;
		double _rate_ss = 0;
		double _pg = 0;
		double _pg_old = 0;
		double _pg_ss = 0;
		double _c_o2 = 0.209;
		double _c_co2 = 2.8e-4;
		double _c_o2_old = 0.209;
		double _c_co2_old = 2.8e-4;
		/*double _c_o2 = 0.144;
		double _c_co2 = 0.056;
		double _c_o2_old = 0.144;
		double _c_co2_old = 0.056;*/
		boost::shared_ptr<IAirEnv> _air;
	};
}

#endif /*LUMPRespSYS_H_*/
