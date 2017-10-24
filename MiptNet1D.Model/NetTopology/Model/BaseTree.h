#pragma once

#ifndef BASETREE_H_
#define BASETREE_H_

#include "tbb/tbb.h"
#include "Knot.h"

namespace MiptNet1DModel {

	class BaseTree {

	public:
		BaseTree(size_t id) : id_(id) { }
		virtual ~BaseTree()	{ }

		virtual void add_knot(Knot* knot) {
			knots_.push_back(knot);
		}

		virtual void add_branch(Branch* branch) {
			branches_.push_back(branch);
		}

		std::vector<Branch*>* get_branches() { return &branches_; }
		std::vector<Knot*>* get_knots() { return &knots_; }

		Branch* get_branch(size_t id) const	{
			for (auto branch : branches_) {
				if (branch->get_id() == id)
					return branch;
			}
			return nullptr;
		}

		Knot* get_knot(size_t id) const	{
			for (auto knot : knots_) {
				if (knot->get_id() == id)
					return knot;
			}
			return nullptr;
		}

		std::vector<Knot*>* GetMultiKnots() { 
			if (multi_knots_.size() == 0) {
				for (auto knot : knots_)
				{
					if (knot->BranchesIn.size() > 0 &&
						knot->BranchesOut.size() > 0) {
						multi_knots_.push_back(knot);
					}
				}
			}
			return &multi_knots_;
		}

		//Установка всех значений с предыдушего шага по времени
		virtual void SetPrev() {
			tbb::parallel_for(size_t(0), branches_.size(), [&](size_t i) {
				branches_[i]->setPrev();
			});
		}

	protected:
		size_t id_;
		std::vector<Knot*> knots_;
		std::vector<Knot*> multi_knots_;
		std::vector<Branch*> branches_;
	};
}

#endif /*BASETREE_H_*/