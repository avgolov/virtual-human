#pragma once

#ifndef ABSTRACTSTORECONDITION_H_
#define ABSTRACTSTORECONDITION_H_

namespace MiptNet1DModel {

	//Условие записи результата результатов
	class IStoreCondition {
	public:
		IStoreCondition() { }
		virtual ~IStoreCondition() {}
		virtual bool Check(double curr_time) = 0;
	
	};
}

#endif /*ABSTRACTSTORECONDITION_H_*/