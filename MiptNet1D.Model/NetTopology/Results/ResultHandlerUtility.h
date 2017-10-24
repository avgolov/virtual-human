#pragma once

#ifndef RESULTHANDLERUTILITY_H_
#define RESULTHANDLERUTILITY_H_

#include "AbstractResultHandler.h"
#include <vector>

namespace MiptNet1DModel {

	//Обработка списка логгеров результатов
	class ResultHandlerUtility {
	public:
		ResultHandlerUtility() { }

		void add_handler(AbstractResultHandler* handler) {
			handlers_.push_back(handler);
		}

		void WriteResult(double curr_time) const {
			for(auto handler : handlers_) {
				handler->WriteResult(curr_time);
			}
		}

	private:
		std::vector<AbstractResultHandler*> handlers_;
	};
}

#endif /*RESULTHANDLERUTILITY_H_*/
