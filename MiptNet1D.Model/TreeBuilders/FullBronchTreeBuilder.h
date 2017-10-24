#pragma once

#ifndef FULLBRONCHTREEBUILDER_H_
#define FULLBRONCHTREEBUILDER_H_

#include "IBronchTreeBuilder.h"

namespace MiptNet1DModel {

	class FullBronchTreeBuilder: IBronchTreeBuilder {
	public:
		FullBronchTreeBuilder() { }
		BronchTree Build() override;
	};
}

#endif /*FULLBRONCHTREEBUILDER_H_*/
