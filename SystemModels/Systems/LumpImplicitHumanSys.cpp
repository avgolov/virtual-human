#include <vector>
#include <array>

#include "LumpImplicitHumanSys.h"

namespace SystemModels
{
	void LumpImplicitHumanSys::SetTissueSys(boost::shared_ptr<ITissueSys> tissueSys)
	{
		tissueSys_ = tissueSys;
	}

	void LumpImplicitHumanSys::SetRespiratorySys(boost::shared_ptr<IRespiratorySys> RespSys)
	{
		RespSys_ = boost::dynamic_pointer_cast<LumpRespSys>(RespSys);
	}

	void LumpImplicitHumanSys::SetNervousSys(boost::shared_ptr<INervousSys> nervousSys)
	{
		nervousSys_ = nervousSys;
	}

	void LumpImplicitHumanSys::SetBloodSys(boost::shared_ptr<IBloodSys> bloodSys)
	{
		bloodSys_ = boost::dynamic_pointer_cast<BodyBioChemBloodSys>(bloodSys);
	}

	void LumpImplicitHumanSys::Compute(TimeSpan* timeSpan) {
		RespSys_->Compute(timeSpan);
		bloodSys_->Compute(timeSpan);

		//Tissue values
		auto bloodVol = bloodSys_->GetParam(AllVol);
		auto o2_uptake = tissueSys_->GetParam(OtherO2Uptake) / (bloodVol*bcm_.LitMolCoef);
		auto co2_dep = tissueSys_->GetParam(OtherCO2Dep) / (bloodVol*bcm_.LitMolCoef);
		auto o2 = bloodSys_->GetO2Free();

		//O2-Hb equations
		auto o2Lung = dO2Cf_*bcm_.AtmPres*RespSys_->GetO2() / (bloodVol*bcm_.LitMolCoef);
		auto o2Blood = -dO2Cf_ / (bcm_.SolCoefO2*bloodVol*bcm_.LitMolCoef);
		O2HbLumpImplicit setEqO2(o2_uptake, timeSpan->dt, o2, o2Lung, o2Blood);
		Numerics::NewtonMethod<Eigen::VectorXd, Eigen::MatrixXd> mO2(&setEqO2);
		Eigen::VectorXd sln(1);
		sln.coeffRef(0) = o2;
		mO2.Solve(sln, 1.e-6);
		bloodSys_->SetO2Free(sln.coeffRef(0));
		LungO2Transfer(timeSpan);
		auto o2Old = o2;
		while (abs(sln.coeffRef(0) - o2Old) > o2*1e-5)
		{
			o2Old = sln.coeffRef(0);
			o2Lung = dO2Cf_*bcm_.AtmPres*RespSys_->GetO2() / (bloodVol*bcm_.LitMolCoef);
			setEqO2 = O2HbLumpImplicit(o2_uptake, timeSpan->dt, o2, o2Lung, o2Blood);
			sln.coeffRef(0) = bloodSys_->GetO2Free();
			mO2.Solve(sln, 1.e-6);
			bloodSys_->SetO2Free(sln.coeffRef(0));
			LungO2Transfer(timeSpan);
		}
		
		//Co2 bicarbonate buffer equations
		auto h = bloodSys_->GetH();
		auto hco3 = bloodSys_->GetHCo3();
		auto m = hco3 - h;
		auto co2Lung = dCo2Cf_*bcm_.AtmPres*RespSys_->GetCo2() / (bloodVol*bcm_.LitMolCoef);
		auto co2Blood = -dCo2Cf_ / (bcm_.SolCoefCo2*bloodVol*bcm_.LitMolCoef);
		Co2BbLumpImplicit setEqCo2(m, co2_dep, timeSpan->dt, hco3, co2Lung, co2Blood);
		Numerics::NewtonMethod<Eigen::VectorXd, Eigen::MatrixXd> mCo2(&setEqCo2);
		sln.coeffRef(0) = hco3;
		mCo2.Solve(sln, 1.e-8);
		bloodSys_->SetHco3(sln.coeffRef(0));
		LungCo2Transfer(timeSpan);

		auto hco3Old = hco3;
		while (abs(sln.coeffRef(0) - hco3Old) > hco3*1e-5)
		{
			hco3Old = sln.coeffRef(0);
			co2Lung = dCo2Cf_*bcm_.AtmPres*RespSys_->GetCo2() / (bloodVol*bcm_.LitMolCoef);
			setEqCo2 = Co2BbLumpImplicit(m, co2_dep, timeSpan->dt, hco3, co2Lung, co2Blood);
			sln.coeffRef(0) = bloodSys_->GetHCo3();
			mO2.Solve(sln, 1.e-6);
			bloodSys_->SetHco3(sln.coeffRef(0));
			LungCo2Transfer(timeSpan);
		}

		//if(timeSpan->timeCurr>150)
			LungRegulate(timeSpan);
	}

	double LumpImplicitHumanSys::GetTimeStep() {
		auto dt = 1.e10;
		if (bloodSys_) {
			auto bloodDt = bloodSys_->GetTimeStep();
			dt = dt < bloodDt ? dt : bloodDt;
		}
		if (RespSys_) {
			auto lungsDt = RespSys_->GetTimeStep();
			dt = dt < lungsDt ? dt : lungsDt;
		}
		if (tissueSys_) {
			auto tissueDt = tissueSys_->GetTimeStep();
			dt = dt < tissueDt ? dt : tissueDt;
		}
		return (dt == 1.e10) ? 1. : dt;

	}

	void LumpImplicitHumanSys::LungO2Transfer(TimeSpan* timeSpan)
	{
		auto vol = RespSys_->GetVol();
		auto volOld = RespSys_->GetVolOld();
		auto concOld = RespSys_->GetO2old();
		auto bloodO2 = bloodSys_->GetParam(Po2Art);
		if(vol > volOld)
		{
			auto conc = (concOld*volOld + (vol - volOld)*RespSys_->GetAirO2(timeSpan)+
				dO2Cf_*bloodO2*timeSpan->dt)/
				(vol+ dO2Cf_*bcm_.AtmPres*timeSpan->dt);
			RespSys_->SetO2(conc);
		}
		else
		{
			auto conc = (concOld*volOld + dO2Cf_*bloodO2*timeSpan->dt) /
				(volOld + dO2Cf_*bcm_.AtmPres*timeSpan->dt);
			RespSys_->SetO2(conc);
		}
	}

	void LumpImplicitHumanSys::LungCo2Transfer(TimeSpan* timeSpan)
	{
		auto vol = RespSys_->GetVol();
		auto volOld = RespSys_->GetVolOld();
		auto concOld = RespSys_->GetCo2();
		auto bloodCo2 = bloodSys_->GetParam(Pco2Art);
		if (vol > volOld)
		{
			auto conc = (concOld*volOld + (vol - volOld)*RespSys_->GetAirCo2(timeSpan) + dCo2Cf_*bloodCo2*timeSpan->dt) /
				(vol + dCo2Cf_*bcm_.AtmPres*timeSpan->dt);
			RespSys_->SetCo2(conc);
		}
		else
		{
			auto conc = (concOld*volOld + dCo2Cf_*bloodCo2*timeSpan->dt) /
				(dCo2Cf_*bcm_.AtmPres*timeSpan->dt + volOld);
			RespSys_->SetCo2(conc);
		}
	}

	void LumpImplicitHumanSys::LungRegulate(TimeSpan* timeSpan)
	{
		auto po20 = 94.1;
		auto pco20 = 28.5; 
		auto kbco2 = 2.08/7.9;
		auto tbco2 = 48.4;
		auto kcbco2 = 1.04/7.3;
		auto tcbco2 = 38.7;
		auto tau = 210;
		//0.07
		/*auto kbco2 = 2.08 / 4.9;
		auto kcbco2 = 1.04 / 3.0;
		auto tau = 240;*/
		//0.06
		/*auto kbco2 = 2.08 / 5.9;
		auto kcbco2 = 1.04 / 4.3;
		auto tau = 220;*/
		//0.03
		/*auto kbco2 = 2.08 / 7.9;
		auto kcbco2 = 1.04 / 7.3;
		auto tau = 210;*/

		auto po2 = bloodSys_->GetParam(Po2Art);
		auto pco2 = bloodSys_->GetParam(Pco2Art);
		auto vc = kbco2*(47.4*pco2/pco20 -tbco2);
		if (vc < 0.) 
			vc = 0.;

		auto vp = kcbco2*(37.5*pco2/pco20 - tcbco2);
		auto fco2 = (pco2 <= pco20) ? 1 / (5 - 4 * pow(pco2 / pco20, 4)) :
			                               pow(pco2 / pco20, 3);
		auto vpO2 = (570 / (97.*po2/po20 -26.2)-8.05)*fco2;
		if(po20 > po2)
			vp += vpO2;

		if (vp < 0.) 
			vp = 0.;

		auto ve_ss = RespSys_->GetParam(TidalVolumeSS)*RespSys_->GetParam(RateSS)*0.001;
		auto ve_old = RespSys_->GetParam(TidalVolume)*RespSys_->GetParam(Rate)*0.001;
		
		auto ve = ve_ss+vc+vp;
		if (vc + vp + ve_ss > ve_old)
		{
			auto t1 = -tau*log((vc + vp + ve_ss - ve_old) / (vc + vp));
			ve = ve_ss+(vc + vp)*(1-exp(-(timeSpan->dt+t1)/tau));
		}
		
		if (ve > 50) ve = 50;
		auto vt = 0.152*pow(ve, 0.683);
		auto rate = ve / vt;
		
		if(ve<15.)
		{
			rate = RespSys_->GetParam(RateSS);
			vt = ve / rate;
		}
		RespSys_->SetParam(Rate, rate);
		RespSys_->SetParam(TidalVolume, vt*1000);
	}
}