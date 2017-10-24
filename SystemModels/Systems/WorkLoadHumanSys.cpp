#include <vector>

#include "WorkLoadHumanSys.h"
#include "../../Numerics/Ode/ImplicitEulerOdeSolver.h"
#include "../../Numerics/NewtonMethod.h"
#include "../../Numerics/Ode/ImplicitAL3OdeSolver.h"

namespace SystemModels
{
	void WorkLaodHumanSys::SetMethod()
	{
		//{PA, PV, SV, BA, TA}
		eqO2Hb_.Volumes[0] = bloodSys_->GetParam(PulmArtVol);
		eqO2Hb_.Volumes[1] = bloodSys_->GetParam(PulmVenVol);
		eqO2Hb_.Volumes[2] = bloodSys_->GetParam(SysVenVol);
		eqO2Hb_.Volumes[3] = bloodSys_->GetParam(BrainArtVol);
		eqO2Hb_.Volumes[4] = bloodSys_->GetParam(SysArtVol);

		eqCo2Bb_.Volumes[0] = bloodSys_->GetParam(PulmArtVol);
		eqCo2Bb_.Volumes[1] = bloodSys_->GetParam(PulmVenVol);
		eqCo2Bb_.Volumes[2] = bloodSys_->GetParam(SysVenVol);
		eqCo2Bb_.Volumes[3] = bloodSys_->GetParam(BrainArtVol);
		eqCo2Bb_.Volumes[4] = bloodSys_->GetParam(SysArtVol);

		eqBb_.Volumes[0] = bloodSys_->GetParam(PulmArtVol);
		eqBb_.Volumes[1] = bloodSys_->GetParam(PulmVenVol);
		eqBb_.Volumes[2] = bloodSys_->GetParam(SysVenVol);
		eqBb_.Volumes[3] = bloodSys_->GetParam(BrainArtVol);
		eqBb_.Volumes[4] = bloodSys_->GetParam(SysArtVol);

		eqLa_.Volumes[0] = bloodSys_->GetParam(PulmArtVol);
		eqLa_.Volumes[1] = bloodSys_->GetParam(PulmVenVol);
		eqLa_.Volumes[2] = bloodSys_->GetParam(SysVenVol);
		eqLa_.Volumes[3] = bloodSys_->GetParam(BrainArtVol);
		eqLa_.Volumes[4] = bloodSys_->GetParam(SysArtVol);

		eqLa_.UtLa = utLa_;
		eqLa_.LaMin = bloodSys_->GetParam(LaArt);
		//muscle_ = WorkLoadMuscle(vo2Max_, enEqvO2Alpha_, workLoadAlpha_, lactateTreshold_, enEqvO2_);
	}

	void WorkLaodHumanSys::SetTissueSys(boost::shared_ptr<ITissueSys> tissueSys)
	{
		tissueSys_ = tissueSys;
	}

	void WorkLaodHumanSys::SetRespiratorySys(boost::shared_ptr<IRespiratorySys> RespSys)
	{
		respSys_ = boost::dynamic_pointer_cast<LumpRespSys>(RespSys);
	}

	void WorkLaodHumanSys::SetNervousSys(boost::shared_ptr<INervousSys> nervousSys)
	{
		nervousSys_ = nervousSys;
	}

	void WorkLaodHumanSys::SetBloodSys(boost::shared_ptr<IBloodSys> bloodSys)
	{
		bloodSys_ = boost::dynamic_pointer_cast<CompartmentsBloodSys>(bloodSys);
	}

	void WorkLaodHumanSys::SetWork(boost::shared_ptr<IWorkLoad> work) { work_ = work; }

	double WorkLaodHumanSys::GetAirWorkPart(double& work)
	{
		auto h = 1 - cAirWork_*exp(bAirWork_*(work-lactateTreshold_));
		
		bloodSys_->Hipoxia = h;
		auto airWorkLoad = work*h;
		return airWorkLoad;
	}

	void WorkLaodHumanSys::Compute(TimeSpan* timeSpan) {

		auto workLoad = work_->Get(timeSpan);
		auto workLoadAir = GetAirWorkPart(workLoad);
		nervousSys_->ControlBloodSys(timeSpan);
		nervousSys_->ControlRespiratorySys(timeSpan);

		respSys_->Compute(timeSpan);
		bloodSys_->Compute(timeSpan);
		auto squareCf = respSys_->GetSquare() / respSys_->GetSquareOld();
		bcm_.Do2Cf *= squareCf;
		bcm_.Dco2Cf *= squareCf;

		//Set biochemistry equations
		eqO2Hb_.OldValues = bloodSys_->GetO2FreeOldValues();
		eqO2Hb_.Dt = timeSpan->dt;

		eqCo2Bb_.OldValues = bloodSys_->GetHco3OldValues();
		eqCo2Bb_.Dt = timeSpan->dt;

		eqBb_.OldValues = eqCo2Bb_.OldValues - bloodSys_->GetHOldValues();
		eqBb_.Dt = timeSpan->dt;

		eqLa_.OldValues = bloodSys_->GetLaOldValues();
		eqLa_.LaWsource = (workLoad - workLoadAir) / enEqvOLa_;
		eqLa_.Po2 = bloodSys_->GetParam(Po2Art);
		eqBb_.CardiacOutput = bloodSys_->GetParam(CardicOutput);
		eqBb_.BrainFlow = bloodSys_->GetParam(BrainFlow);

		Numerics::ImplicitEulerOdeSolver solver;
		Numerics::ImplicitAL3OdeSolver solverAL3;
		//Solve La-Bb equations
		auto slnLaBbOld = eqLa_.OldValues;
		//SolveNonLinEq(eqLa_, slnLaBbOld, 1e-6);
		solverAL3.Solve(eqLa_, eqLa_.OldValues, slnLaBbOld, timeSpan->dt);
		bloodSys_->SetLa(slnLaBbOld);

		//Solve O2 equations
		auto o2Uptake = tissueSys_->GetParam(OtherO2Uptake) + workLoadAir/enEqvO2_;
		if (workLoad==0)
			o2Uptake += oxyUtLa_*(slnLaBbOld.coeffRef(4) - eqLa_.LaMin);
		eqO2Hb_.TissueUptake = o2Uptake;
		eqO2Hb_.BrainUptake = tissueSys_->GetParam(BrainO2Uptake);
		eqO2Hb_.CardiacOutput = bloodSys_->GetParam(CardicOutput);
		eqO2Hb_.BrainFlow = bloodSys_->GetParam(BrainFlow);
		bloodSys_->O2Cons = (eqO2Hb_.TissueUptake+ eqO2Hb_.BrainUptake);

		auto slnO2Old = eqO2Hb_.OldValues;
		LungO2Transfer(timeSpan);
		eqO2Hb_.Po2AlvLung = respSys_->GetO2() * bcm_.AtmPres;
		auto  slnO2 = slnO2Old;
		//SolveNonLinEq(eqO2Hb_, slnO2, 1e-6);
		solverAL3.Solve(eqO2Hb_, eqO2Hb_.OldValues, slnO2, timeSpan->dt);

		/*while ((slnO2 - slnO2Old).norm() > slnO2Old.norm()*1e-5) {
			LungO2Transfer(timeSpan);
			eqO2Hb_.Po2AlvLung = respSys_->GetO2() * bcm_.AtmPres;
			slnO2Old = slnO2;
			SolveNonLinEq(eqO2Hb_, slnO2, 1e-6);
			bloodSys_->SetO2Free(slnO2);
		}*/
		bloodSys_->SetO2Free(slnO2);

		//Solve Co2 equations
		auto co2Prod = tissueSys_->GetParam(OtherCO2Dep) + rer_*workLoadAir / enEqvO2_;
		co2Prod += co2UtLa_*(slnLaBbOld.coeffRef(4) - eqLa_.LaMin);
		eqCo2Bb_.TissueProd = co2Prod;
		eqCo2Bb_.BrainProd = tissueSys_->GetParam(BrainCO2Dep);
		eqCo2Bb_.CardiacOutput = bloodSys_->GetParam(CardicOutput);
		eqCo2Bb_.BrainFlow = bloodSys_->GetParam(BrainFlow);
		bloodSys_->CO2Prod = (eqCo2Bb_.TissueProd + eqCo2Bb_.BrainProd);

		auto  slnBb = eqBb_.OldValues;
		//SolveNonLinEq(eqBb_, slnBb, 1e-6);
		solverAL3.Solve(eqBb_, eqBb_.OldValues, slnBb, timeSpan->dt);
		eqCo2Bb_.Hhco3 = std::vector<double>(slnBb.data(), slnBb.data() + slnBb.rows() * slnBb.cols());

		auto slnCo2Old = eqCo2Bb_.OldValues;
		LungCo2Transfer(timeSpan);
		eqCo2Bb_.Pco2AlvLung = respSys_->GetCo2()*bcm_.AtmPres;
		eqCo2Bb_.La = bloodSys_->GetLaOldValues();

		auto  slnCo2 = slnCo2Old;
		//SolveNonLinEq(eqCo2Bb_, slnCo2, 1e-6);
		solverAL3.Solve(eqCo2Bb_, eqCo2Bb_.OldValues, slnCo2, timeSpan->dt);

		/*while ((slnCo2 - slnCo2Old).norm() > slnCo2Old.norm()*1e-5) {
			LungCo2Transfer(timeSpan);
			eqCo2Bb_.Pco2AlvLung = respSys_->GetCo2()*bcm_.AtmPres;
			slnCo2Old = slnCo2;
			SolveNonLinEq(eqCo2Bb_, slnCo2, 1e-6);
			bloodSys_->SetHco3(slnCo2, slnBb);
		}*/
		bloodSys_->SetHco3(slnCo2, slnBb);
	}

	double WorkLaodHumanSys::GetTimeStep() {
		auto dt = 1.e10;
		if (bloodSys_) {
			auto bloodDt = bloodSys_->GetTimeStep();
			dt = dt < bloodDt ? dt : bloodDt;
		}
		if (respSys_) {
			auto lungsDt = respSys_->GetTimeStep();
			dt = dt < lungsDt ? dt : lungsDt;
		}
		if (tissueSys_) {
			auto tissueDt = tissueSys_->GetTimeStep();
			dt = dt < tissueDt ? dt : tissueDt;
		}
		return (dt == 1.e10) ? 1. : dt;

	}

	Matrix WorkLaodHumanSys::FastInv(const Matrix& m) const
	{
		auto detInv = 1. /
			(m.coeffRef(0, 2)*m.coeffRef(1, 0)*m.coeffRef(2, 3)*m.coeffRef(3, 1)*m.coeffRef(4, 4) -
				m.coeffRef(0, 0)*m.coeffRef(1, 1)*m.coeffRef(2, 2)*m.coeffRef(3, 3)*m.coeffRef(4, 4) +
				m.coeffRef(0, 2)*m.coeffRef(1, 0)*m.coeffRef(2, 4)*m.coeffRef(3, 3)*m.coeffRef(4, 1));

		Matrix result = Matrix::Zero(5, 5);
		result.coeffRef(0, 0) = -(m.coeffRef(1, 1)*m.coeffRef(2, 2)*m.coeffRef(3, 3)*m.coeffRef(4, 4))*detInv;
		result.coeffRef(1, 0) = (m.coeffRef(1, 0)*m.coeffRef(2, 2)*m.coeffRef(3, 3)*m.coeffRef(4, 4))*detInv;
		result.coeffRef(2, 0) = (m.coeffRef(1, 0)*m.coeffRef(2, 3)*m.coeffRef(3, 1)*m.coeffRef(4, 4) +
			m.coeffRef(1, 0)*m.coeffRef(2, 4)*m.coeffRef(3, 3)*m.coeffRef(4, 1))*detInv;
		result.coeffRef(3, 0) = -(m.coeffRef(1, 0)*m.coeffRef(2, 2)*m.coeffRef(3, 1)*m.coeffRef(4, 4))*detInv;
		result.coeffRef(4, 0) = -(m.coeffRef(1, 0)*m.coeffRef(2, 2)*m.coeffRef(3, 3)*m.coeffRef(4, 1))*detInv;

		result.coeffRef(0, 1) = (m.coeffRef(0, 2)*m.coeffRef(2, 3)*m.coeffRef(3, 1)*m.coeffRef(4, 4) +
			m.coeffRef(0, 2)*m.coeffRef(2, 4)*m.coeffRef(3, 3)*m.coeffRef(4, 1))*detInv;
		result.coeffRef(1, 1) = -(m.coeffRef(0, 0)*m.coeffRef(2, 2)*m.coeffRef(3, 3)*m.coeffRef(4, 4))*detInv;
		result.coeffRef(2, 1) = -(m.coeffRef(0, 0)*m.coeffRef(2, 3)*m.coeffRef(3, 1)*m.coeffRef(4, 4) +
			m.coeffRef(0, 0)*m.coeffRef(2, 4)*m.coeffRef(3, 3)*m.coeffRef(4, 1))*detInv;
		result.coeffRef(3, 1) = (m.coeffRef(0, 0)*m.coeffRef(2, 2)*m.coeffRef(3, 1)*m.coeffRef(4, 4))*detInv;
		result.coeffRef(4, 1) = (m.coeffRef(0, 0)*m.coeffRef(2, 2)*m.coeffRef(3, 3)*m.coeffRef(4, 1))*detInv;

		result.coeffRef(0, 2) = m.coeffRef(0, 2)*m.coeffRef(1, 1)*m.coeffRef(3, 3)*m.coeffRef(4, 4)*detInv;
		result.coeffRef(1, 2) = -m.coeffRef(0, 2)*m.coeffRef(1, 0)*m.coeffRef(3, 3)*m.coeffRef(4, 4)*detInv;
		result.coeffRef(2, 2) = -m.coeffRef(0, 0)*m.coeffRef(1, 1)*m.coeffRef(3, 3)*m.coeffRef(4, 4)*detInv;
		result.coeffRef(3, 2) = (m.coeffRef(0, 2)*m.coeffRef(1, 0)*m.coeffRef(3, 1)*m.coeffRef(4, 4))*detInv;
		result.coeffRef(4, 2) = (m.coeffRef(0, 2)*m.coeffRef(1, 0)*m.coeffRef(3, 3)*m.coeffRef(4, 1))*detInv;

		result.coeffRef(0, 3) = -m.coeffRef(0, 2)*m.coeffRef(1, 1)*m.coeffRef(2, 3)*m.coeffRef(4, 4)*detInv;
		result.coeffRef(1, 3) = m.coeffRef(0, 2)*m.coeffRef(1, 0)*m.coeffRef(2, 3)*m.coeffRef(4, 4)*detInv;
		result.coeffRef(2, 3) = m.coeffRef(0, 0)*m.coeffRef(1, 1)*m.coeffRef(2, 3)*m.coeffRef(4, 4)*detInv;
		result.coeffRef(3, 3) = -(m.coeffRef(0, 0)*m.coeffRef(1, 1)*m.coeffRef(2, 2)*m.coeffRef(4, 4) -
			m.coeffRef(0, 2)*m.coeffRef(1, 0)*m.coeffRef(2, 4)*m.coeffRef(4, 1))*detInv;
		result.coeffRef(4, 3) = -(m.coeffRef(0, 2)*m.coeffRef(1, 0)*m.coeffRef(2, 3)*m.coeffRef(4, 1))*detInv;

		result.coeffRef(0, 4) = -m.coeffRef(0, 2)*m.coeffRef(1, 1)*m.coeffRef(2, 4)*m.coeffRef(3, 3)*detInv;
		result.coeffRef(1, 4) = m.coeffRef(0, 2)*m.coeffRef(1, 0)*m.coeffRef(2, 4)*m.coeffRef(3, 3)*detInv;
		result.coeffRef(2, 4) = m.coeffRef(0, 0)*m.coeffRef(1, 1)*m.coeffRef(2, 4)*m.coeffRef(3, 3)*detInv;
		result.coeffRef(3, 4) = -(m.coeffRef(0, 2)*m.coeffRef(1, 0)*m.coeffRef(2, 4)*m.coeffRef(3, 1))*detInv;
		result.coeffRef(4, 4) = -(m.coeffRef(0, 0)*m.coeffRef(1, 1)*m.coeffRef(2, 2)*m.coeffRef(3, 3) -
			m.coeffRef(0, 2)*m.coeffRef(1, 0)*m.coeffRef(2, 3)*m.coeffRef(3, 1))*detInv;

		return result;
	}

	void WorkLaodHumanSys::SolveNonLinEq(
		Numerics::ISetEquations<Vector, Matrix>& eq,
		Vector& u_0, double epsilon)
	{
		int iteration = 0;
		Vector u_s(u_0.size());
		auto  norm0 = u_0.norm();
		do {
			if (iteration == 1000) break;
			u_s = FastInv(eq.yacobian_f(u_0)) * eq.f(u_0);
			u_0 -= u_s;
			iteration++;

		} while (u_s.norm() / norm0 > epsilon);
	}

	void WorkLaodHumanSys::LungO2Transfer(TimeSpan* timeSpan)
	{
		return;
		auto vol = respSys_->GetVol();
		auto volOld = respSys_->GetVolOld();
		auto concOld = respSys_->GetO2old();
		auto bloodO2 = bloodSys_->GetParam(Po2Art);
		if (vol > volOld)
		{
			auto conc = (concOld*volOld + (vol - volOld)*respSys_->GetAirO2(timeSpan) +
				bcm_.Do2Cf*bloodO2*timeSpan->dt) /
				(vol + bcm_.Do2Cf*bcm_.AtmPres*timeSpan->dt);
			respSys_->SetO2(conc);
		}
		else
		{
			auto conc = (concOld*volOld + bcm_.Do2Cf*bloodO2*timeSpan->dt) /
				(volOld + bcm_.Do2Cf*bcm_.AtmPres*timeSpan->dt);
			respSys_->SetO2(conc);
		}
	}

	void WorkLaodHumanSys::LungCo2Transfer(TimeSpan* timeSpan)
	{
		return;
		auto vol = respSys_->GetVol();
		auto volOld = respSys_->GetVolOld();
		auto concOld = respSys_->GetCo2old();
		auto bloodPCo2 = bloodSys_->GetParam(Pco2Art);
		if (vol > volOld)
		{
			auto conc = (concOld*volOld + (vol - volOld)*respSys_->GetAirCo2(timeSpan) + 
				bcm_.Dco2Cf*bloodPCo2*timeSpan->dt) /
				(vol + bcm_.Dco2Cf*bcm_.AtmPres*timeSpan->dt);
			respSys_->SetCo2(conc);
		}
		else
		{
			auto conc = (concOld*volOld + bcm_.Dco2Cf*bloodPCo2*timeSpan->dt) /
				(volOld + bcm_.Dco2Cf*bcm_.AtmPres*timeSpan->dt);
			respSys_->SetCo2(conc);
		}
	}
	
}