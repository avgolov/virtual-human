#pragma once

#ifndef IBRONCHTREEBUILDER_H_
#define IBRONCHTREEBUILDER_H_

#include "../NetTopology/Model/BronchTree.h"

namespace MiptNet1DModel {

	class IBronchTreeBuilder {
	public:
		IBronchTreeBuilder() { }
		virtual ~IBronchTreeBuilder() {}
		virtual BronchTree Build() = 0;

	};
}

#endif /*IBRONCHTREEBUILDER_H_*/