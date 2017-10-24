#pragma once
#ifndef INERVOUSSYSTEM_H_
#define INERVOUSSYSTEM_H_

#include <boost/smart_ptr/shared_ptr.hpp>
#include "../ISystem.h"
#include "../BloodSystem/IBloodSys.h"
#include "../RespiratorySystem/IRespiratorySys.h"
#include "../TissueSystem/ITissueSys.h"

namespace SystemModels
{
	//The model of nervous system
	class INervousSys : public ISystem
	{
	public:
		INervousSys() {}
		virtual ~INervousSys() {};

		virtual void ControlBloodSys(TimeSpan* timeSpan) = 0;
		virtual void ControlRespiratorySys(TimeSpan* timeSpan) = 0;
		virtual void ControlTissueSys(TimeSpan* timeSpan) = 0;

		void SetTissueSys(boost::shared_ptr<ITissueSys> tissue) { tissue_ = tissue; }
		void SetLungsSys(boost::shared_ptr<IRespiratorySys> lungs) { lungs_ = lungs; }
		void SetBloodSys(boost::shared_ptr<IBloodSys> blood) { blood_ = blood; }

	protected:
		boost::shared_ptr<ITissueSys> tissue_;
		boost::shared_ptr<IRespiratorySys> lungs_;
		boost::shared_ptr<IBloodSys> blood_;
	};
}

#endif /*INERVOUSSYSTEM_H_*/
