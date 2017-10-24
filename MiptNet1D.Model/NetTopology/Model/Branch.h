#pragma once

#ifndef BRANCH_H_
#define BRANCH_H_

#include <vector>
//#include <boost/serialization/vector.hpp>
#include "BranchPoint.h"

namespace MiptNet1DModel {

	class Branch {

	public:
		Branch() { }

		Branch(unsigned int id, 
			   double length, 
			   unsigned int knot_begin, 
			   unsigned int knot_end) :
				id_(id),
				knot_begin_(knot_begin),
				knot_end_(knot_end),
				length_ (length) { }

		unsigned int get_id() const	{ return id_; }
		unsigned int get_knot_begin() const { return knot_begin_; }
		unsigned int get_knot_end() const { return knot_end_; }

		double get_dx() const {
			return length_ / (points_.size() - 1);
		}

		BranchPoint* get_point(unsigned int index) {
			return points_[index];
		}

		BranchPoint* get_last_point() {
			return points_.back();
		}

		BranchPoint* get_first_point() {
			return points_.front();
		}

		BranchPoint* get_second_point() {
			return points_[1];
		}

		BranchPoint* get_second2last_point() {
			return points_[points_.size() - 2];
		}

		void add_point(BranchPoint* point) {
			points_.push_back(point);
		}

		size_t get_point_count() const	{
			return points_.size();
		}

		std::vector<BranchPoint*>* get_points() {
			return &points_;
		}

		double get_length() const { return length_; }
		double get_square() const { return (*points_.begin())->SquareInit; }

		//Установка значений с предыдущего шага по времени
		void setPrev() const {
			for each (auto point in points_)
			{
				point->SquarePrev = point->Square;
				point->SubstancePrev = point->Substance;
			}
		}

	private:
		unsigned int id_ = 0;
		unsigned int knot_begin_ = 0;
		unsigned int knot_end_ = 0;
		double length_ = 0.;
		std::vector<BranchPoint*> points_;

		/*friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & id_;
			ar & knot_begin_;
			ar & knot_end_;
			ar & length_;
			ar & points_;
		}*/
	};
}

#endif /*BRANCH_H_*/