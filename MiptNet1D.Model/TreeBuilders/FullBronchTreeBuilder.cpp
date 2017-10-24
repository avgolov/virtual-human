#include "FullBronchTreeBuilder.h"
#include "../NetTopology/Model/BronchTree.h"
#include "../NetTopology/Loaders/TreeDataLoader.h"

namespace MiptNet1DModel {

	BronchTree FullBronchTreeBuilder::Build()	{
		auto tree = BronchTree(1);
		TreeDataLoaderParam param;
		param.bronch_tree_path;
		param.lungs_volume = 5000;
		param.alv_r = 0.2;
		param.alv_c = 1.;
		param.bronch_tree_path = "./treebronch";
		param.knots = { };
		param.branches = { };

		TreeDataLoader loader;
		loader.Load(&tree, param);

		return tree;
	}
}