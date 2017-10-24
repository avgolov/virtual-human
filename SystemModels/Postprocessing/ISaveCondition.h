#pragma once

#ifndef ISAVECONDITION_H_
#define ISAVECONDITION_H_

namespace SystemModels {

	//Condition for saving results
	class ISaveCondition {
	public:
		ISaveCondition() { }
		virtual ~ISaveCondition() {}
		virtual bool Check(double curr_time) = 0;

	};
}

#endif /*ISAVECONDITION_H_*/