#pragma once

#ifndef OXYTREEVOLUMEHANDLER_H_
#define OXYTREEVOLUMEHANDLER_H_

#include "AbstractResultHandler.h"
#include "../Model/BronchTree.h"

namespace MiptNet1DModel {

	class OxyTreeVolumeHandler : public AbstractResultHandler {
	public:
		OxyTreeVolumeHandler(IStoreCondition* condition,
							 BronchTree* tree,
							 std::string path) :
			AbstractResultHandler(condition, path),
			tree_(tree) { }

	protected:
		double GetValue() override {
			auto value = 0.;
			auto branches = tree_->get_branches();
			for(auto branch : *branches) {
				auto points = branch->get_points();
				for(auto point : *points) {
					value += point->Square*branch->get_dx()*point->Substance.Conc[0];
				}
			}
			return value;
		};

		std::string GetFileName() const override {
			return path_ + "/oxygen_tree_volume.txt";
		};



	private:
		BronchTree* tree_;
	};
}

#endif /*OXYTREEVOLUMEHANDLER_H_*/

