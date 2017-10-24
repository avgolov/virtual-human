#pragma once

#ifndef KNOT_H_
#define KNOT_H_

#include <vector>
#include "Branch.h"

namespace MiptNet1DModel {

	class Knot {
	
	public:
		
		std::vector<Branch*> BranchesIn;
		std::vector<Branch*> BranchesOut;
		
		Knot() {}

		Knot(unsigned int id, double x, double y, double z){
			id_ = id;
			x_ = x;
			y_ = y;
			z_ = z;
		}

		unsigned int get_id() const	{ return id_; }

	private:
		unsigned int id_ = 0;
		double x_ = 0.;
		double y_ = 0.;
		double z_ = 0.;

		/*friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & id_;
			ar & x_;
			ar & y_;
			ar & z_;
		}*/
	};
}

#endif /*KNOT_H_*/