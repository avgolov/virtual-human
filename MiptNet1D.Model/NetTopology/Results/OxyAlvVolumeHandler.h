#pragma once

#ifndef OXYALVVOLUMEHANDLER_H_
#define OXYALVVOLUMEHANDLER_H_

#include "AbstractResultHandler.h"
#include "../Model/BronchTree.h"

namespace MiptNet1DModel {

	//Потребление кислорода
	class OxyAlvVolumeHandler : public AbstractResultHandler {
	public:
		OxyAlvVolumeHandler(IStoreCondition* condition,	
			                BronchTree* tree,
			                std::string path) :
			AbstractResultHandler(condition, path),
			tree_(tree) { }

	protected:
		double GetValue() override {
			auto value = 0.;
			for each (auto alveole in *tree_->get_alveoles()) {
				value += alveole->Volume*alveole->Substance.Conc[0];
			}
			return value;
		};

		std::string GetFileName() const override {
			return path_ + "oxygen_alv_volume.txt";
		};

		

	private:
		BronchTree* tree_;
	};
}

#endif /*OXYALVVOLUMEHANDLER_H_*/
