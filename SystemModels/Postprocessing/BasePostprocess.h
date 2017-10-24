#pragma once

#ifndef BASEPOSTPROCESS_H_
#define BASEPOSTPROCESS_H_

#include <memory>
#include <boost/shared_ptr.hpp>
#include <string>
#include <boost/algorithm/string/replace.hpp>
#include "ISaveCondition.h"
#include "IPostprocess.h"

namespace SystemModels {

	template <typename Sys, typename SysParam>
	class BasePostprocess: public IPostprocess {
	public:
		BasePostprocess(ISaveCondition* condition, 
			            boost::shared_ptr<Sys> system,
		                SysParam param,
						std::string result_path) :
			IPostprocess(condition),
			system_(system),
			param_(param),
			result_path_(result_path) { }

	protected:
		double GetValue() override
		{
			return system_->GetParam(param_);
		};

		std::string GetFileName() const override
		{
			auto paramName = "";
			switch (param_)
			{
			case O2Art: {
				paramName = "O2Art";
				break;
			}
			case O2Ven: {
				paramName = "O2Ven";
				break;
			}
			case Co2Art: {
				paramName = "Co2Art";
				break;
			}
			case Co2Ven: {
				paramName = "Co2Ven";
				break;
			}
			case O2Brain: {
				paramName = "O2Brain";
				break;
			}
			case Co2Brain: {
				paramName = "Co2Brain";
				break;
			}
			case O2AlvArt: {
				paramName = "O2AlvArt";
				break;
			}
			case Co2AlvArt: {
				paramName = "Co2AlvArt";
				break;
			}
			case O2AlvVen: {
				paramName = "O2AlvVen";
				break;
			}
			case Co2AlvVen: {
				paramName = "Co2AlvVen";
				break;
			}
			case Po2Art: {
				paramName = "Po2Art";
				break;
			}
			case Po2Ven: {
				paramName = "Po2Ven";
				break;
			}
			case Pco2Art: {
				paramName = "Pco2Art";
				break;
			}
			case Pco2Ven: {
				paramName = "Pco2Ven";
				break;
			}
			case Po2Brain: {
				paramName = "Po2Brain";
				break;
			}
			case Pco2Brain: {
				paramName = "Pco2Brain";
				break;
			}
			case Po2AlvArt: {
				paramName = "Po2AlvArt";
				break;
			}
			case Pco2AlvArt: {
				paramName = "Pco2AlvArt";
				break;
			}
			case Po2AlvVen: {
				paramName = "Po2AlvVen";
				break;
			}
			case Pco2AlvVen: {
				paramName = "Pco2AlvVen";
				break;
			}
			case CardicOutput: {
				paramName = "CardicOutput";
				break;
			}
			case BrainFlow: {
				paramName = "BrainFlow";
				break;
			}
			case Rate: {
				paramName = "Rate";
				break;
			}
			case TidalVolume: {
				paramName = "TidalVolume";
				break;
			}
			case Hco3Art: {
				paramName = "Hco3Art";
				break;
			}
			case HbArt: {
				paramName = "HbArt";
				break;
			}
			case HArt: {
				paramName = "HArt";
				break;
			}
			case PhArt: {
				paramName = "PhArt";
				break;
			}
			case SysArtO2Sat: {
				paramName = "SysArtO2Sat";
				break;
			}
			case O2: {
				paramName = "O2";
				break;
			}
			case CO2: {
				paramName = "CO2";
				break;
			}
			case Ve: {
				paramName = "VE";
				break;
			}
			case LaArt: {
				paramName = "LaArt";
				break;
			}
			case LaAlvArt: {
				paramName = "LaAlvArt";
				break;
			}
			case LaAlvVen: {
				paramName = "LaAlvVen";
				break;
			}
			case LaVen: {
				paramName = "LaVen";
				break;
			}
			case LaBrain: {
				paramName = "LaBrain";
				break;
			}
			case Vol: {
				paramName = "Vol";
				break;
			}
			}
			auto path = result_path_+ "/"+
				   std::string(typeid(Sys).name()) + "_"+ 
				   paramName +".txt";
			boost::replace_all(path, "class ", "");
			boost::replace_all(path, "SystemModels::", "");
			return path;
		};

	private:
		boost::shared_ptr<Sys> system_;
		SysParam param_;
		std::string result_path_;
	};
}

#endif /*BASEPOSTPROCESS_H_*/