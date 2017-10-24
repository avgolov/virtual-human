#pragma once

#ifndef LUNGS8TREEBUILDER_H_
#define LUNGS8TREEBUILDER_H_

#include "IBronchTreeBuilder.h"

namespace MiptNet1DModel {

	class Lungs8TreeBuilder : IBronchTreeBuilder {
	public:
		Lungs8TreeBuilder(double alv_c,
			double alv_r,
			double lungs_volume) :
			alv_c_(alv_c),
			alv_r_(alv_r),
			lungs_volume_(lungs_volume) { }
		BronchTree Build() override;

	private:
		double alv_c_ = 0;
		double alv_r_ = 0;
		double lungs_volume_ = 0.;
	};
}

#endif /*LUNGS8TREEBUILDER_H_*/

