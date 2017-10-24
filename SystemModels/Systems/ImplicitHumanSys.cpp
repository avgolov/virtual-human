#include <vector>
#include <array>

#include "ImplicitHumanSys.h"

namespace SystemModels
{
	void ImplicitHumanSys::SetMethod()
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
	}
	
	void ImplicitHumanSys::SetTissueSys(boost::shared_ptr<ITissueSys> tissueSys)
	{
		tissueSys_ = tissueSys;
	}

	void ImplicitHumanSys::SetRespiratorySys(boost::shared_ptr<IRespiratorySys> RespSys)
	{
		RespSys_ = boost::dynamic_pointer_cast<MiptNet1DRespSys>(RespSys);
	}

	void ImplicitHumanSys::SetNervousSys(boost::shared_ptr<INervousSys> nervousSys)
	{
		nervousSys_ = nervousSys;
	}

	void ImplicitHumanSys::SetBloodSys(boost::shared_ptr<IBloodSys> bloodSys)
	{
		bloodSys_ = boost::dynamic_pointer_cast<CompartmentsBloodSys>(bloodSys);
	}

	void ImplicitHumanSys::Compute(TimeSpan* timeSpan)	{
		
		RespSys_->WriteResult(timeSpan);
		//if(timeSpan->timeCurr > 100)
			nervousSys_->Compute(timeSpan);
		
		auto motionEq = RespSys_->GetMotionEquation();
		auto tree = motionEq->GetTree();
		tree->SetPrev();
		motionEq->Solve(timeSpan->dt); 
		
		auto massTransfEq = RespSys_->GetMassTransfEquation();
		massTransfEq->ModifyConcentration();
		massTransfEq->ComputeMultiKnots(timeSpan->dt);
		massTransfEq->ComputeInternalPoints(timeSpan->dt);
		massTransfEq->ComputeNasopharynx(timeSpan->dt);
		
		if (bloodSys_) bloodSys_->Compute(timeSpan);
		eqO2Hb_.OldValues = bloodSys_->GetO2FreeOldValues();
		eqO2Hb_.Dt = timeSpan->dt;

		eqCo2Bb_.OldValues = bloodSys_->GetHco3OldValues();
		eqCo2Bb_.Dt = timeSpan->dt;
		
		eqBb_.OldValues = eqCo2Bb_.OldValues - bloodSys_->GetHOldValues();
		eqBb_.Dt = timeSpan->dt;

		eqO2Hb_.TissueUptake = tissueSys_->GetParam(OtherO2Uptake);
		eqO2Hb_.BrainUptake = tissueSys_->GetParam(BrainO2Uptake);
		eqO2Hb_.CardiacOutput = bloodSys_->GetParam(CardicOutput);
		eqO2Hb_.BrainFlow= bloodSys_->GetParam(BrainFlow);

		eqCo2Bb_.TissueProd = tissueSys_->GetParam(OtherCO2Dep);
		eqCo2Bb_.BrainProd = tissueSys_->GetParam(BrainCO2Dep);
		eqCo2Bb_.CardiacOutput = bloodSys_->GetParam(CardicOutput);
		eqCo2Bb_.BrainFlow = bloodSys_->GetParam(BrainFlow);

		eqBb_.CardiacOutput = bloodSys_->GetParam(CardicOutput);
		eqBb_.BrainFlow = bloodSys_->GetParam(BrainFlow);
	
		//Solve O2 equations
		auto slnO2Old = eqO2Hb_.OldValues;
		auto alvBloodO2 = AlvBloodTransferO2(timeSpan->dt);
		eqO2Hb_.O2AlvLung = alvBloodO2[0]/bcm_.LitMolCoef;
		eqO2Hb_.O2AlvBlood = alvBloodO2[1] / bcm_.LitMolCoef;

		auto  slnO2 = slnO2Old;
		SolveNonLinEq(eqO2Hb_, slnO2, 1e-6);
		
		while((slnO2- slnO2Old).norm() > slnO2Old.norm()*1e-5) {
			slnO2Old = slnO2;
			SolveNonLinEq(eqO2Hb_, slnO2, 1e-6);
			bloodSys_->SetO2Free(slnO2);
			alvBloodO2 = AlvBloodTransferO2(timeSpan->dt);
			eqO2Hb_.O2AlvLung = alvBloodO2[0] / bcm_.LitMolCoef;
			eqO2Hb_.O2AlvBlood = alvBloodO2[1] / bcm_.LitMolCoef;
		}
		bloodSys_->SetO2Free(slnO2);
		AlvBloodTransferO2(timeSpan->dt);

		//Solve Co2 equations
		auto  slnBb = eqBb_.OldValues;
//		SolveNonLinEq(eqBb_, slnBb, 1e-6);
		eqCo2Bb_.Hhco3 = std::vector<double>(slnBb.data(), slnBb.data() + slnBb.rows() * slnBb.cols());

		auto slnCo2Old = eqCo2Bb_.OldValues;
		auto alvBloodCo2 = AlvBloodTransferCo2(timeSpan->dt);
		eqCo2Bb_.Co2AlvLung = alvBloodCo2[0] / bcm_.LitMolCoef;
		eqCo2Bb_.Co2AlvBlood = alvBloodCo2[1] / bcm_.LitMolCoef;

		auto  slnCo2 = slnCo2Old;
		SolveNonLinEq(eqCo2Bb_, slnCo2, 1e-6);

		while ((slnCo2 - slnCo2Old).norm() > slnCo2Old.norm()*1e-5) {
			slnCo2Old = slnCo2;
			SolveNonLinEq(eqCo2Bb_, slnCo2, 1e-6);
			bloodSys_->SetO2Free(slnCo2);
			alvBloodCo2 = AlvBloodTransferCo2(timeSpan->dt);
			eqCo2Bb_.Co2AlvLung = alvBloodCo2[0] / bcm_.LitMolCoef;
			eqCo2Bb_.Co2AlvBlood = alvBloodCo2[1] / bcm_.LitMolCoef;
		}
		bloodSys_->SetHco3(slnCo2, slnBb);
		AlvBloodTransferCo2(timeSpan->dt);

		Alv1dNetTransfer(timeSpan->dt);
	}

	double ImplicitHumanSys::GetTimeStep()	{
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

	Matrix ImplicitHumanSys::FastInv(const Matrix& m) const
	{
		auto detInv = 1. / 
			(m.coeffRef(0, 2)*m.coeffRef(1, 0)*m.coeffRef(2, 3)*m.coeffRef(3, 1)*m.coeffRef(4, 4)-
			 m.coeffRef(0, 0)*m.coeffRef(1, 1)*m.coeffRef(2, 2)*m.coeffRef(3, 3)*m.coeffRef(4, 4)+
			 m.coeffRef(0, 2)*m.coeffRef(1, 0)*m.coeffRef(2, 4)*m.coeffRef(3, 3)*m.coeffRef(4, 1));

		Matrix result = Matrix::Zero(5, 5);
		result.coeffRef(0, 0) = -(m.coeffRef(1, 1)*m.coeffRef(2, 2)*m.coeffRef(3, 3)*m.coeffRef(4, 4))*detInv;
		result.coeffRef(1, 0) = (m.coeffRef(1, 0)*m.coeffRef(2, 2)*m.coeffRef(3, 3)*m.coeffRef(4, 4))*detInv;
		result.coeffRef(2, 0) = (m.coeffRef(1, 0)*m.coeffRef(2, 3)*m.coeffRef(3, 1)*m.coeffRef(4, 4)+
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
		result.coeffRef(3, 4) =  -(m.coeffRef(0, 2)*m.coeffRef(1, 0)*m.coeffRef(2, 4)*m.coeffRef(3, 1))*detInv;
		result.coeffRef(4, 4) = -(m.coeffRef(0, 0)*m.coeffRef(1, 1)*m.coeffRef(2, 2)*m.coeffRef(3, 3) -
			                      m.coeffRef(0, 2)*m.coeffRef(1, 0)*m.coeffRef(2, 3)*m.coeffRef(3, 1))*detInv;

		return result;
	}

	void ImplicitHumanSys::SolveNonLinEq(
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
	
	std::array<double, 2> ImplicitHumanSys::AlvBloodTransferO2(double dt)
	{
		std::array<double, 2> result = { 0., 0. };

		auto alveoles = RespSys_->GetAlveoles();
		auto po2 = bloodSys_->GetParam(Po2AlvArt);
		auto squareSum = 0.;
		for (size_t i = 0; i < alveoles->size(); i++)
		{
			auto alveole = (*alveoles)[i];
			double square = pow(36*M_PI*alveole->Volume*alveole->Volume, 1./3.);
			squareSum += square;

			Branch* branch = alveole->get_branch();
			BranchPoint* point = branch->get_last_point();
			double vol = point->Substance.Velocity * point->Square * dt;
			if (vol > 0.) {

				alveole->Substance.Conc[0] = 
				   (alveole->VolumePrev * alveole->SubstancePrev.Conc[0] +
					vol*point->Substance.Conc[0] +
					bcm_.Do2Cf*square*po2*dt) /
					(alveole->Volume + bcm_.Do2Cf*bcm_.AtmPres*square*dt);
			}
			else {
				alveole->Substance.Conc[0] =
					  (alveole->VolumePrev * alveole->SubstancePrev.Conc[0] +
					   bcm_.Do2Cf*square*po2*dt) /
					  (alveole->Volume + bcm_.Do2Cf*bcm_.AtmPres*square*dt - vol);
			}
			result[0] += bcm_.Do2Cf*bcm_.AtmPres*square*alveole->Substance.Conc[0];
		}
		result[1] = -bcm_.Do2Cf*squareSum;
		
		return result;
	}
	
	std::array<double, 2> ImplicitHumanSys::AlvBloodTransferCo2(double dt)
	{
		std::array<double, 2> result = { 0., 0. };

		auto alveoles = RespSys_->GetAlveoles();
		auto pco2 = bloodSys_->GetParam(Pco2AlvArt);
		auto squareSum = 0.;
		for (size_t i = 0; i < alveoles->size(); i++)
		{
			auto alveole = (*alveoles)[i];
			double square = pow(36 * M_PI*alveole->Volume*alveole->Volume, 1. / 3.);
			squareSum += square;

			Branch* branch = alveole->get_branch();
			BranchPoint* point = branch->get_last_point();
			double vol = point->Substance.Velocity * point->Square * dt;
			if (vol > 0.) {

				alveole->Substance.Conc[1] =
					(alveole->VolumePrev * alveole->SubstancePrev.Conc[1] +
						vol*point->Substance.Conc[1] +
						bcm_.Do2Cf*square*pco2*dt) /
						(alveole->Volume + bcm_.Do2Cf*bcm_.AtmPres*square*dt);
			}
			else {
				alveole->Substance.Conc[1] =
					(alveole->VolumePrev * alveole->SubstancePrev.Conc[1] +
						bcm_.Do2Cf*square*pco2*dt) /
						(alveole->Volume + bcm_.Do2Cf*bcm_.AtmPres*square*dt - vol);
			}
			result[0] += bcm_.Do2Cf*bcm_.AtmPres*square*alveole->Substance.Conc[1];
		}
		result[1] = -bcm_.Do2Cf*squareSum;

		return result;
	}

	void  ImplicitHumanSys::Alv1dNetTransfer(double dt) const
	{
		auto alveoles = RespSys_->GetAlveoles();
		for (size_t i = 0; i < alveoles->size(); i++)
		{
			auto alveole = (*alveoles)[i];
			Branch* branch = alveole->get_branch();
			BranchPoint* point = branch->get_last_point();
			BranchPoint* pointPrev = branch->get_second2last_point();
			double dx = branch->get_dx();
			double vol = point->Substance.Velocity * point->Square * dt;
			if (vol > 0.) {
				for (size_t s = 0; s < 2; s++) {
					point->Substance.Conc[s] += point->Substance.Velocity * (dt / dx) *
						(pointPrev->SubstancePrev.Conc[s] - point->SubstancePrev.Conc[s]);
				}
			}
			else {
				for (size_t s = 0; s < 2; s++) {
					point->Substance.Conc[s] -= point->Substance.Velocity * (dt / dx) *
						(alveole->Substance.Conc[s] - point->SubstancePrev.Conc[s]);
				}
			}
		}
	}
}