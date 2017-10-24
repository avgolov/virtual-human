#pragma once

#ifndef OXYCONSPOSTPROCESS_H_
#define OXYCONSPOSTPROCESS_H_

#include <boost/shared_ptr.hpp>
#include <string>
#include "ISaveCondition.h"
#include "IPostprocess.h"
#include "../Systems/BloodSystem/CompartmentsBloodSys.h"

namespace SystemModels {

	class OxyConsPostprocess : public IPostprocess {
	public:
		OxyConsPostprocess(ISaveCondition* condition,
			boost::shared_ptr<CompartmentsBloodSys> system,
			std::string result_path) :
			IPostprocess(condition),
			system_(system),
			result_path_(result_path){ }

	protected:
		double GetValue() override
		{
			auto value = volume_*60/n_;
			volume_ = 0;
			n_ = 0;
			return value;
		};

		void Process() override {
			volume_ += system_->O2Cons;
			n_++;
		};

		std::string GetFileName() const override
		{
			auto path = result_path_ + "/OxyCons.txt";
			return path;
		};

	private:
		boost::shared_ptr<CompartmentsBloodSys> system_;
		std::string result_path_;
		double volume_ = 0;
		int n_ = 0;
	};
}

#endif /*OXYCONSPOSTPROCESS_H_*/