#define _USE_MATH_DEFINES
#include <exception>
#include <algorithm>
#include <boost/math/tools/roots.hpp>

#include "LungsFluidEquation.h"
#include "../../NetTopology/Model/Alveole.h"

namespace MiptNet1DModel {

	struct poly4_functor
	{
		poly4_functor(double a0, double a1, double a2, double a3, double a4) : 
			          a0_(a0), a1_(a1), a2_(a2), a3_(a3), a4_(a4) {}
		std::tr1::tuple<double, double> operator()(double const& z)
		{
			auto f = a4_*pow(z,4) + a3_*pow(z, 3) + a2_*pow(z, 2) + a1_*z + a0_;
			auto der_f = 4*a4_*pow(z, 3) + 3*a3_*pow(z, 2) + 2*a2_*z + a1_;
			return std::tr1::make_tuple(f, der_f);
		}
	private:
		double a4_;
		double a3_;
		double a2_;
		double a1_;
		double a0_;
	};

	void LungsFluidEquation::ComputeAlveole(Alveole* alveole, double dt) {
		double a, b;
		InCompatibilityCoef(alveole->get_branch(), a, b, dt);
		//auto t0 = 0.;//time_step_->time_curr - dt;
		//auto t1 = dt;//time_step_->time_curr;
		auto  point = alveole->get_branch()->get_last_point();
		auto point_nei = alveole->get_branch()->get_second2last_point();
		//V1 рабочая
		/*double press_nei = state_.Pressure(point_nei);
		double dv = point->SquarePrev * point->SubstancePrev.Velocity;

		Eigen::VectorXd y(2);
		y << alveole->VolumePrev, dv ;

		Eigen::VectorXd rp(2);
		GetAlvRightPart(y, rp, press_nei, time_step_->time_curr - dt);

		Eigen::VectorXd rm = y * alv_a0_ - rp * dt * alv_b0_;
		Eigen::MatrixXd e(2, 2);
		e << 1.0, 0.0, 0.0, 1.0;
		Eigen::MatrixXd b_inv = (e - yacobian_ * dt * alv_b1_ - yacobian2_ * dt * dt * alv_c1_).inverse();

		Eigen::VectorXd yn = y;
		Eigen::VectorXd yn_old(2);
		Eigen::VectorXd drp(2);
		Eigen::VectorXd rn(2);

		const int max_iteration = 1000;
		int iteration = 0;
		double eps;
		do {
			if (iteration == max_iteration) break;

			yn_old = yn;
			GetAlvRightPart(yn_old, rp, press_nei, time_step_->time_curr);
			GetAlvDiffRightPart(yn_old, drp, time_step_->time_curr);
			rn = yn_old * alv_a1_ - rp * dt *alv_b1_ - (drp + yacobian_*rp) * dt*dt*alv_c1_;
			yn -= b_inv * (rm + rn);
			eps = abs((yn.coeff(0) - yn_old.coeff(0)) / (yn.coeff(0) + yn_old.coeff(0))) +
				  abs((yn.coeff(1) - yn_old.coeff(1)) / (yn.coeff(1) + yn_old.coeff(1)));

		} while (eps > 1.e-5);

		if (iteration == max_iteration) {
			std::cout << "Max alveolar volume iteration reached!";
		}

		alveole->Volume = yn.coeff(0);
		double c = (alveole->Volume - alveole->VolumePrev) / dt;
		point->Square = 0.5 * (-b - sqrt(b*b + 4.*a*c)) / a;
		point->Substance.Velocity = a*point->Square + b;*/
		//V2
		
		//V3
		//auto sq = alveole->get_branch()->get_first_point()->Square;
		//auto max_val = 4000 * sq + 650*sq;
		//auto val_new = alveole->Volume + point->Square*point->Substance.Velocity*dt;
		//alveole->Volume = (val_new > max_val) ? max_val : val_new;

		//V4
		//auto l1 = 0.5*(-alv_r_ + sqrt(alv_r_*alv_r_ - 4 * alv_i_ / alv_c_)) / alv_i_;
		//auto l2 = 0.5*(-alv_r_ - sqrt(alv_r_*alv_r_ - 4 * alv_i_ / alv_c_)) / alv_i_;
		//auto alv_vol = alveole->Volume;
		//auto c1 = (point->Square*point->Substance.Velocity - l2*alv_vol)*exp(-l1*t0);
		//auto c2 = alv_vol*exp(-l2*t0);
		//auto c3 = exp(-l1*t0)*(point->SubstancePrev.Pressure + alveole->VolumeInit / alv_c_);
		//auto c_eq = (c1 - 0.5*t0*c3)*(exp(t1*(l1 - l2)) - exp(t0*(l1 - l2))) / (l1 - l2);
		//c_eq += 0.5*c3*( exp(t1*(l1-l2))*((l1-l2)*t1 - 1) - exp(t0*(l1-l2))*((l1-l2)*t0 - 1)) / pow(l1-l2,2);
		//c_eq += 0.25*pow(t1 - t0, 2)*exp(-l2*t1)*(-(point->Substance.Density*pow(point->Cwall,2))+alveole->VolumeInit / alv_c_);
		//c_eq += c2;
		//c_eq = -c_eq*exp(l2*t1) + alv_vol;
		//auto b_eq = b*dt - 0.25*pow(t1 - t0, 2)*exp(-l2*t1)*point->Substance.Density*pow(point->Cwall, 2) / point->SquareInit;
		//auto a_eq = a*dt;
		//auto s1 = 0.5*(-b_eq + sqrt(b_eq*b_eq - 4 * a_eq*c_eq))/a_eq;
		//auto s2 = 0.5*(-b_eq - sqrt(b_eq*b_eq - 4 * a_eq*c_eq)) / a_eq;
		//auto v1 = a*s1 + b;
		//auto v2 = a*s2 + b;
		//V5
		/*point->Square = point_nei->Square;
		point->Substance.Velocity = point_nei->Substance.Velocity;*/
		//alveole->Volume += point->Square*point->Substance.Velocity*dt;
		//V6
		/*auto l1 = 0.5*(-alv_r_ + sqrt(alv_r_*alv_r_ - 4 * alv_i_ / alv_c_)) / alv_i_;
		auto l2 = 0.5*(-alv_r_ - sqrt(alv_r_*alv_r_ - 4 * alv_i_ / alv_c_)) / alv_i_;
		auto alv_vol = alveole->Volume;
		auto c1 = (point->Square*point->Substance.Velocity - l2*alv_vol)*exp(-l1*t0);
		auto c2 = alv_vol*exp(-l2*t0);
		auto c3 = exp(-l1*t0)*(state_.Pressure(point)+ alveole->VolumeInit / alv_c_);
		auto v_t1 = (c1 - 0.5*t0*c3)*(exp(t1*(l1 - l2)) - exp(t0*(l1 - l2))) / (l1 - l2);
		v_t1 += 0.5*c3*(exp(t1*(l1 - l2))*((l1 - l2)*t1 - 1) - exp(t0*(l1 - l2))*((l1 - l2)*t0 - 1)) / pow(l1 - l2, 2);
		v_t1 += 0.25*pow(t1 - t0, 2)*exp(-l2*t1)*(state_.Pressure(point_nei) + alveole->VolumeInit / alv_c_);
		v_t1 += c2;
		v_t1 *= exp(l2*t1);
		point->Square = point_nei->Square;
		point->Substance.Velocity = (v_t1 - alv_vol) / (dt*point->Square);
		alveole->Volume = v_t1;
		if (abs(point->Substance.Velocity) >1000.)
		{
			int aaa = 0;
		}*/
		//V7
		//auto l = 1. / (alv_r_*alv_c_);
		//auto f_t1 = exp(l*t1)*(state_.Pressure(point_nei));
		//auto f_t0 = exp(l*t0)*(state_.Pressure(point));
		//auto v_t1 = exp(-l*t1)*0.5*(t1-t0)*(f_t1 + f_t0)/alv_r_;
		//v_t1 += alveole->VolumeInit + (alveole->Volume - alveole->VolumeInit)*exp(l*(t0 - t1));
		//point->Square = point_nei->Square;
		//point->Substance.Velocity = (v_t1 - alveole->Volume) / (dt*point->Square);
		//alveole->Volume = v_t1;
		
		//V8 рабочая
		/*auto l = 1. / (alv_r_*alv_c_);
		auto v_t1 = alveole->VolumeInit + (alveole->Volume - alveole->VolumeInit)*exp(l*(t0 - t1));
		v_t1 += exp(-l*t1)*0.5*(t1 - t0)* exp(l*t0)*state_.Pressure(point) / alv_r_;
		v_t1 -= 0.5*(t1 - t0)*point->Substance.Density*pow(point->Cwall, 2) / alv_r_;
		auto cc = alveole->Volume - v_t1;
		auto aa = dt*a;
		auto bb = dt*b - 0.5*(t1 - t0)*point->Substance.Density*pow(point->Cwall, 2) / (alv_r_*point->SquareInit);
		auto s2 = 0.5*(-bb - sqrt(bb*bb - 4 * aa*cc))/aa;
		auto v2 = a*s2 + b;
		point->Square = s2;
		point->Substance.Velocity = v2;
		alveole->Volume += point->Square*point->Substance.Velocity*dt;*/
	/*	const double alv_r_ = 2.0;
		const double alv_c_ = 10.0;*/
		/*if (alveole->Volume > 4*alveole->VolumeInit ||
			alveole->Volume < 0.1*alveole->VolumeInit) {
			alveole->Volume -= point->Square*point->Substance.Velocity*dt;
			point->Substance.Velocity = 0;
			point->Square = point->SquarePrev;
			alveole->Volume = (alveole->Volume < 4*alveole->VolumeInit) ? alveole->Volume : 4*alveole->VolumeInit;
			alveole->Volume = (alveole->Volume > 0.1*alveole->VolumeInit) ? alveole->Volume : 0.1*alveole->VolumeInit;
		}*/

		//V9 рабочая
		//const double alv_i_ = 0.0021;
		//const double alv_r_ = 2.0;
		//const double alv_c_ = 10.0;
		/*auto l1 = 0.5*(-alv_r_ + sqrt(alv_r_*alv_r_ - 4 * alv_i_ / alv_c_)) / alv_i_;
		auto l2 = 0.5*(-alv_r_ - sqrt(alv_r_*alv_r_ - 4 * alv_i_ / alv_c_)) / alv_i_;
		auto alv_vol = alveole->Volume;
		auto c1 = (point->Square*point->Substance.Velocity - l2*alv_vol)*exp(-l1*t0);
		auto c2 = alv_vol*exp(-l2*t0);
		auto c3 = exp(-l1*t0)*(state_.Pressure(point) + alveole->VolumeInit / alv_c_) / alv_i_;
		auto c_eq = (c1 - 0.5*t0*c3)*(exp(t1*(l1 - l2)) - exp(t0*(l1 - l2))) / (l1 - l2);
		c_eq += 0.5*c3*( exp(t1*(l1-l2))*((l1-l2)*t1 - 1) - exp(t0*(l1-l2))*((l1-l2)*t0 - 1)) / pow(l1-l2,2);
		c_eq += 0.25*pow(t1 - t0, 2)*exp(-l2*t1)*(-point->Substance.Density*pow(point->Cwall,2)+alveole->VolumeInit / alv_c_) / alv_i_;
		c_eq += c2;
		c_eq = -c_eq*exp(l2*t1) + alv_vol;
		auto b_eq = b*dt - 0.25*pow(t1 - t0, 2)*point->Substance.Density*pow(point->Cwall, 2) / (point->SquareInit*alv_i_);
		auto a_eq = a*dt;
		auto s1 = 0.5*(-b_eq + sqrt(b_eq*b_eq - 4 * a_eq*c_eq)) / a_eq;
		auto s2 = 0.5*(-b_eq - sqrt(b_eq*b_eq - 4 * a_eq*c_eq)) / a_eq;
		auto v1 = a*s1 + b;
		auto v2 = a*s2 + b;
		point->Square = s2;
		point->Substance.Velocity = v2;
		alveole->Volume += point->Square*point->Substance.Velocity*dt;*/
		//V10
		//auto rate = breath_param_->GetRateCur();
		//auto tidalVolume = breath_param_->GetTidalVolumeCur();
		//auto w = M_PI * rate / 30.;
		//alveole->Volume = alveole->VolumeInit + 0.5*0.25*tidalVolume*(1-sin(w*time_));
		//auto c = -0.5*0.25*tidalVolume*w*cos(w*time_);
		//auto s = 0.5*(-b - sqrt(b*b - 4 * a*c)) / a;
		//auto v = a*s + b;
		//point->Square = s;
		//point->Substance.Velocity = v;

		////Pressure
		//auto p_el = (alveole->Volume - alveole->VolumeInit) / alv_c_;
		//auto p_br = 0.;//state_.Pressure(point);
		//auto lambda = 1. / (alv_c_*alv_r_);
		//auto p0 = tidalVolume*alv_r_*sqrt(lambda*lambda + w*w) / w;
		//alveole->Substance.Pressure = p_el + p_br - p0*sin(w*time_);
		
		////V11
        /*auto p_g = breath_param_->GetPg(); 
		auto w = M_PI * breath_param_->GetRateCur() / 30.;
		auto k = alveole->R; 
		auto dens = point->Substance.Density;
		auto v_n = alveole->VolumePrev;
		auto v_0 = alveole->VolumeInit;
		auto c_alv = alveole->C;
		auto c_w = point->Cwall;
		auto s_0 = point->SquareInit;
		auto a4 = pow(dt*a, 2);
		auto a3 = 2 * a*b*dt*dt - pow(c_w, 2)*dt*a*(c_alv*dens / s_0);
		auto a2 = b*dt*dt;
		a2	-= pow(c_w, 2)*dt*b*c_alv*dens/s_0;
		a2 += a*(dt*(2 * v_n - v_0) + c_alv*k);
		a2 += a*c_alv*dt*(pow(c_w, 2)*dens + p_g*sin(w*time_));
		
		auto a1 = b*dt*(2 * v_n - v_0);
		a1 += b*dt*c_alv*(pow(c_w, 2)*dens + p_g*sin(w*time_));
		a1 += b*c_alv*k;
		a1 -= c_alv*pow(c_w, 2)*dens*v_n/s_0;

		auto a0 = v_n*(v_n - v_0);
		a0 += v_n*c_alv*(pow(c_w, 2)*dens + p_g*sin(w*time_));
		
		auto min = 0.1*point->SquareInit;
		auto max = 10.*point->SquareInit;
		auto guess = point->Square;
		auto digits = 15;
		point->Square = boost::math::tools::newton_raphson_iterate(
			poly4_functor(a0, a1, a2, a3, a4), guess, min, max, digits);
		point->Substance.Velocity = a*point->Square + b;
		alveole->Volume += dt*point->Square*point->Substance.Velocity;
		point->Substance.Pressure = state_.Pressure(point);*/
		
		//V12
		auto p_g = breath_param_->GetPg();
		auto w = M_PI * breath_param_->GetRateCur() / 30.;
		auto dens = point->Substance.Density;
		auto v_n = alveole->VolumePrev;
		auto v_0 = alveole->VolumeInit;
		auto e = 1/alveole->C;
		auto c_w = point->Cwall;
		auto s_0 = point->SquareInit;
		auto a1 = alveole->R*a + a*dt*e;
		auto a2 = alveole->R*b + b*dt *e - pow(c_w, 2)*dens/ s_0;
		auto a3 = (v_n - v_0)*e+p_g*sin(w*time_)+ pow(c_w, 2)*dens;
		auto s = 0.5*(-a2 - sqrt(a2*a2 - 4 * a1*a3)) / a1;
		auto v = a*s + b;
		point->Square = s;
		point->Substance.Velocity = v;
		alveole->Volume += point->Square*point->Substance.Velocity*dt;
		point->Substance.Pressure = state_.Pressure(point);
       

		
	}

	void LungsFluidEquation::SetRate(double value)
	{
		if(breath_param_ != nullptr)
		{
			breath_param_->SetRate(value);
			return;
		}
		throw std::exception("Breath pattern does not set!");
	}

	void LungsFluidEquation::SetTidalVolume(double value) 
	{
		if (breath_param_ != nullptr)
		{
			breath_param_->SetTidalVolume(value);
			return;
		}
		throw std::exception("Breath pattern does not set!");
	}
	void LungsFluidEquation::SetRateSS(double value)
	{
		breath_param_->SetRateSS(value);
	}
	void LungsFluidEquation::SetTidalVolumeSS(double value)
	{
		breath_param_->SetTidalVolumeSS(value);
	}
	double LungsFluidEquation::GetRate() const
	{
		if (breath_param_ != nullptr)
		{
			return breath_param_->GetRateCur();
		}
		throw std::exception("Breath pattern does not set!");
	}
	double LungsFluidEquation::GetTidalVolume() const 
	{
		if (breath_param_ != nullptr)
		{
			return breath_param_->GetTidalVolumeCur();
		}
		throw std::exception("Breath pattern does not set!");
	}
	double LungsFluidEquation::GetRateOld() const 
	{
		if (breath_param_ != nullptr)
		{
			return breath_param_->GetRateOld();
		}
		throw std::exception("Breath pattern does not set!");
	}
	double LungsFluidEquation::GetTidalVolumeOld() const 
	{
		if (breath_param_ != nullptr)
		{
			return breath_param_->GetTidalVolumeOld();
		}
		throw std::exception("Breath pattern does not set!");
	}
	double LungsFluidEquation::GetRateSS() const
	{
		return breath_param_->GetRateSS();
	}
	double LungsFluidEquation::GetTidalVolumeSS() const
	{
		return breath_param_->GetTidalVolumeSS();
	}

	std::vector<Alveole*>* LungsFluidEquation::GetAlveoles() const
	{
		return dynamic_cast<BronchTree*>(tree_)->get_alveoles();
	}
}