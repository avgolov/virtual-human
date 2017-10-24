#pragma once
#ifndef BODYBIOCHEMTISSUECN_H_
#define BODYBIOCHEMTISSUECN_H_

#include <memory>
#include "IBloodTissueCn.h"
#include "../../Systems/BloodSystem/BodyBioChemBloodSys.h"

namespace SystemModels
{
	class BodyBioChemTissueCn : public IBloodTissueCn
	{
	public:
		BodyBioChemTissueCn(
			boost::shared_ptr<IBloodSys> bloodSys,
			boost::shared_ptr<ITissueSys> tissueSys) :
			IBloodTissueCn(bloodSys, tissueSys)	{

			bodyBioChemBloodSys_ = boost::dynamic_pointer_cast<BodyBioChemBloodSys>(bloodSys);
		}

		void Interact(TimeSpan* timeSpan) override;

	private:
		boost::shared_ptr<BodyBioChemBloodSys> bodyBioChemBloodSys_;
	};
}

#endif /*BODYBIOCHEMTISSUECN_H_*/

