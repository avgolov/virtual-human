#pragma once

#ifndef SIMPLEBRONCHTREEBUILDER_H_
#define SIMPLEBRONCHTREEBUILDER_H_

#include "IBronchTreeBuilder.h"

namespace MiptNet1DModel {

	class SimpleBronchTreeBuilder : IBronchTreeBuilder {
	public:
		SimpleBronchTreeBuilder(double alv_c,
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

#endif /*SIMPLEBRONCHTREEBUILDER_H_*/
