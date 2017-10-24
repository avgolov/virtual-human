#pragma once

#ifndef HIPOXIAPOSTPROCESS_H_
#define HIPOXIAPOSTPROCESS_H_

#include <boost/shared_ptr.hpp>
#include <string>
#include "ISaveCondition.h"
#include "IPostprocess.h"
#include "../Systems/BloodSystem/CompartmentsBloodSys.h"

namespace SystemModels {

	class HipoxiaPostprocess : public IPostprocess {
	public:
		HipoxiaPostprocess(ISaveCondition* condition,
			boost::shared_ptr<CompartmentsBloodSys> system,
			std::string result_path) :
			IPostprocess(condition),
			system_(system),
			result_path_(result_path) { }

	protected:
		double GetValue() override
		{
			return system_->Hipoxia;
		};

		std::string GetFileName() const override
		{
			auto path = result_path_ + "/Hipoxia.txt";
			return path;
		};

	private:
		boost::shared_ptr<CompartmentsBloodSys> system_;
		std::string result_path_;
	};
}

#endif /*HIPOXIAPOSTPROCESS_H_*/