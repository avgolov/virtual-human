#pragma once

#ifndef TIDALVOLUMEHANDLER_H_
#define TIDALVOLUMEHANDLER_H_

#include "AbstractResultHandler.h"
#include "../Model/BronchTree.h"

namespace MiptNet1DModel {

	class TidalVolumeHandler : public AbstractResultHandler {
	public:
		TidalVolumeHandler(IStoreCondition* condition,
						   BronchTree* tree, 
						   std::string path) :
			AbstractResultHandler(condition, path),
			tree_(tree) { }

	protected:
		double GetValue() override {
			auto value = tidal_vol_;
			tidal_vol_ = 0.;
			return value;
		};

		void Process() override {
			auto point = tree_->get_nasopharynx()->get_first_point();
			auto vel = point->Substance.Velocity;
			auto square = point->Square;
			if (vel > 0.0) {
				tidal_vol_ += square * vel*(curr_time_ - last_time_);
			}
		};

		std::string GetFileName() const override {
			return path_ + "/tidal_volume.txt";
		};

	private:
		double tidal_vol_ = 0.;
		BronchTree* tree_;
	};
}

#endif /*TIDALVOLUMEHANDLER_H_*/