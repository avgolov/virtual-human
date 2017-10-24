#pragma once

#ifndef ALVEOLE_H_
#define ALVEOLE_H_

#include "Branch.h"

namespace MiptNet1DModel {

	class Alveole {

	public:
		Alveole() {}
		Alveole(unsigned int id, 
			    Branch* branch,
			    double r,
			    double c) :
			R(r), C(c), id_(id), branch_(branch) { }

		double Volume = 0.;
		double VolumePrev = 0.;
		double VolumeInit = 0.;
		double R = 0.;
		double C = 0.;

		//Параметры вещества
		BaseSubstance Substance;

		//параметры вещества на предыдущем шаге
		BaseSubstance SubstancePrev;

		Branch* get_branch() const { return branch_;  }
		void set_branch(Branch* branch) { branch_ = branch; }
		unsigned int get_id() const	{ return id_; }

	private:
		unsigned int id_ = 0;
		Branch* branch_ = nullptr;

		/*friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & Volume;
			ar & VolumePrev;
			ar & VolumeInit;
			ar & Substance;
			ar & SubstancePrev;
			ar & id_;
		}*/
	};
}

#endif /*ALVEOLE_H_*/