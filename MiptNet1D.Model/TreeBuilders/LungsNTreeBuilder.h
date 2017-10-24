#pragma once

#ifndef LUNGSNTREEBUILDER_H_
#define LUNGSNTREEBUILDER_H_

#include "IBronchTreeBuilder.h"

namespace MiptNet1DModel {

	class LungsNTreeBuilder : IBronchTreeBuilder {
	public:
		LungsNTreeBuilder(double alv_c,
			double alv_r,
			double lungs_volume,
			int n) :
			alv_c_(alv_c),
			alv_r_(alv_r),
			lungs_volume_(lungs_volume),
			n_(n) { }
		BronchTree Build() override;

	private:
		double alv_c_ = 0;
		double alv_r_ = 0;
		double lungs_volume_ = 0.;
		int n_ = 0;
	};
}

#endif /*LUNGSNTREEBUILDER_H_*/

