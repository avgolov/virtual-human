#include "LumpRespSys.h"
#include <Eigen/Geometry>

namespace SystemModels
{
	void LumpRespSys::Compute(TimeSpan* timeSpan)
	{
		_vol_old = _vol;
		_rate_old = _rate;
		_pg_old = _pg;
		_c_o2_old = _c_o2;
		_c_co2_old = _c_co2;
		
		auto l = 1 / (_r*_c);
		auto w = 2 * M_PI*_rate / 60.;
		_vol = _vol_ss + _pg*(1+sin(w*timeSpan->timeCurr)) / (_r*sqrt(l*l + w*w));

		//if(_vol > _vol_old)
		//{
		//	_c_o2 = (_vol_old*_c_o2_old + (_vol - _vol_old)*_air->GetO2(timeSpan)) / _vol;
		//	_c_co2 = (_vol_old*_c_co2_old + (_vol - _vol_old)*_air->GetCO2(timeSpan)) / _vol;
		//}

	}

	double LumpRespSys::GetSquare() const
	{
		return pow(36 * M_PI*pow(_vol, 2), 0.3333);
	}

	double LumpRespSys::GetSquareOld() const
	{
		return pow(36 * M_PI*pow(_vol_old, 2), 0.3333);
	}

	double LumpRespSys::GetTimeStep()
	{
		return 0.1;
	};

	void LumpRespSys::SetParam(RespiratorySysParam param, double value)
	{
		switch (param)
		{
		case Pg:
			_pg = value;
			break;
		case Rate:
			_rate = value;
			break;
		case RateSS:
			_rate_ss = value;
			break;
		case TidalVolumeSS: {
			auto l_ss = 1 / (_r*_c);
			auto w_ss = 2 * M_PI*_rate_ss / 60.;
			_pg_ss = 0.5*value*_r*sqrt(l_ss*l_ss + w_ss*w_ss);
			break; 
		}
		case TidalVolume: {
			auto l = 1 / (_r*_c);
			auto w = 2 * M_PI*_rate / 60.;
			_pg = 0.5*_r*sqrt(l*l + w*w)*value;
			break; 
		}
		default:
			throw std::exception("Wrong parameter!");
		}
	};

	double LumpRespSys::GetParam(RespiratorySysParam param) const
	{
		switch(param)
		{
		case Pg:
			return _pg;
		case Rate:
			return _rate;
		case RateOld:
			return _rate_old;
		case RateSS:
			return _rate_ss;
		case TidalVolume: {
			auto l = 1 / (_r*_c);
			auto w = 2 * M_PI*_rate / 60.;
			return 2.*_pg / (_r*sqrt(l*l + w*w)); 
		}
		case TidalVolumeOld: {
			auto l_old = 1 / (_r*_c);
			auto w_old = 2 * M_PI*_rate_old / 60.;
			return 2.*_pg_old / (_r*sqrt(l_old*l_old + w_old*w_old)); 
		}
		case TidalVolumeSS: {
			auto l_ss = 1 / (_r*_c);
			auto w_ss = 2 * M_PI*_rate_ss / 60.;
			return 2.*_pg_ss / (_r*sqrt(l_ss*l_ss + w_ss*w_ss)); 
		}
		case Ve:
			return GetParam(Rate)*GetParam(TidalVolume);
		case O2:
			return _c_o2;
		case CO2:
			return _c_co2;
		case Vol:
			return _vol;
		default:
			throw std::exception("Wrong parameter!");
		}
	};
}

