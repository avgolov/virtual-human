#include <algorithm>

#include "../Connections/NervousTissue/NervousTissueCnFactory.h"
#include "../Connections/BloodTissue/BloodTissueCnFactory.h"
#include "../Connections/NervousRespiratory/NervousRespiratoryCnFactory.h"
#include "../Connections/BloodRespiratory/BloodRespiratoryCnFactory.h"
#include "../Connections/NervousBlood/NervousBloodCnFactory.h"
#include "HumanSys.h"


namespace SystemModels
{
	void HumanSys::SetTissueSys(boost::shared_ptr<ITissueSys> tissueSys)
	{
		tissueSys_ = tissueSys;
		if (nervousSys_)
		{
			nervousSys_->SetTissueSys(tissueSys_);
			NervousTissueCnFactory ntFactory;
			nervousTissueCn_ = ntFactory.Create(tissueSys_, nervousSys_);
		}

		if (bloodSys_)
		{
			BloodTissueCnFactory btFactory;
			bloodTissueCn_ = btFactory.Create(bloodSys_, tissueSys_);
		}
	}

	void HumanSys::SetRespiratorySys(boost::shared_ptr<IRespiratorySys> RespSys)
	{
		RespSys_ = RespSys;
		if (nervousSys_)
		{
			nervousSys_->SetLungsSys(RespSys_);
			NervousRespiratoryCnFactory nrFactory;
			nervousRespCn_ = nrFactory.Create(RespSys_, nervousSys_);
		}

		if (bloodSys_)
		{
			BloodRespiratoryCnFactory brFactory;
			bloodRespCn_ = brFactory.Create(bloodSys_, RespSys);
		}
	}

	void HumanSys::SetNervousSys(boost::shared_ptr<INervousSys> nervousSys)
	{
		nervousSys_ = nervousSys;
		if (bloodSys_)
		{
			nervousSys_->SetBloodSys(bloodSys_);
			NervousBloodCnFactory nbFactory;
			nervousBloodCn_ = nbFactory.Create(bloodSys_, nervousSys_);
		}

		if (RespSys_)
		{
			nervousSys_->SetLungsSys(RespSys_);
			NervousRespiratoryCnFactory rnFactory;
			nervousRespCn_ = rnFactory.Create(RespSys_, nervousSys_);
		}

		if (tissueSys_)
		{
			nervousSys_->SetTissueSys(tissueSys_);
			NervousTissueCnFactory tnFactory;
			nervousTissueCn_ = tnFactory.Create(tissueSys_, nervousSys);
		}
	}

	void HumanSys::SetBloodSys(boost::shared_ptr<IBloodSys> bloodSys)
	{
		bloodSys_ = bloodSys;
		if (nervousSys_)
		{
			nervousSys_->SetBloodSys(bloodSys_);
			NervousBloodCnFactory nbFactory;
			nervousBloodCn_ = nbFactory.Create(bloodSys_, nervousSys_);
		}

		if (RespSys_)
		{
			BloodRespiratoryCnFactory rnFactory;
			bloodRespCn_ = rnFactory.Create(bloodSys_, RespSys_);
		}

		if (tissueSys_)
		{
			BloodTissueCnFactory tnFactory;
			bloodTissueCn_ = tnFactory.Create(bloodSys_, tissueSys_);
		}
	}

	void HumanSys::Compute(TimeSpan* timeSpan)
	{
		if (RespSys_) RespSys_->Compute(timeSpan);
		if (bloodSys_) bloodSys_->Compute(timeSpan);
		if (tissueSys_) tissueSys_->Compute(timeSpan);
		if (nervousSys_) nervousSys_->Compute(timeSpan);

		if (bloodSys_ && RespSys_) bloodRespCn_->Interact(timeSpan);
		if (bloodSys_ && tissueSys_) bloodTissueCn_->Interact(timeSpan);
		if (bloodSys_ && nervousSys_) nervousBloodCn_->Interact(timeSpan);
		if (nervousSys_ && RespSys_) nervousRespCn_->Interact(timeSpan);
		if (nervousSys_ && tissueSys_) nervousTissueCn_->Interact(timeSpan);
	}

	double HumanSys::GetTimeStep()
	{
		auto dt = 1.e10;
		if (bloodSys_) {
			auto bloodDt = bloodSys_->GetTimeStep();
			dt = std::min(dt, bloodDt);
		}
		if (RespSys_) {
			auto lungsDt = RespSys_->GetTimeStep();
			dt = std::min(dt, lungsDt);
		}
		if (tissueSys_) {
			auto tissueDt = tissueSys_->GetTimeStep();
			dt = std::min(dt, tissueDt);
		}
		return (dt == 1.e10) ? 1. : dt;
			
	}
}