#pragma once

#ifndef POSTPROCESSUTILITY_H_
#define POSTPROCESSUTILITY_H_

#include <vector>
#include "IPostprocess.h"

namespace SystemModels {

	class PostprocessUtility {
	public:
		PostprocessUtility() { }

		void AddHandler(IPostprocess* handler) {
			handlers_.push_back(handler);
		}

		void Handle(double curr_time) const {
			for (auto handler : handlers_) {
				handler->Handle(curr_time);
			}
		}

	private:
		std::vector<IPostprocess*> handlers_;
	};
}

#endif /*POSTPROCESSUTILITY_H_*/