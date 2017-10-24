#pragma once

#ifndef ABSTRACTBLOCKEQ_H_
#define ABSTRACTBLOCKEQ_H_

#include "../NetTopology/Model/BaseTree.h"

namespace MiptNet1DModel {

	//Описание и решение системы уравнений 
	class AbstractEquation {

	public:
		AbstractEquation() { tree_ = nullptr; }
		AbstractEquation(BaseTree* tree) : tree_(tree) { }
		virtual ~AbstractEquation()	{ }
		
		AbstractEquation& AbstractEquation::operator=(const AbstractEquation& other) {
			tree_ = other.tree_;
			return *this;
		}

		virtual void Solve(double dt) = 0;
		BaseTree* GetTree() const { return tree_; }

	protected:
		BaseTree* tree_;
	};
}

#endif /*ABSTRACTBLOCKEQ_H_*/
