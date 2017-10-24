#include "../NetTopology/Model/BronchTree.h"
#include "../NetTopology/Loaders/TreeDataLoader.h"
#include "Lungs8TreeBuilder.h"

namespace MiptNet1DModel {

	BronchTree Lungs8TreeBuilder::Build() {
		auto tree = BronchTree(1);
		TreeDataLoaderParam param;
		param.bronch_tree_path;
		param.lungs_volume = lungs_volume_;
		param.alv_c = alv_c_;
		param.alv_r = alv_r_;
		param.bronch_tree_path = "./treebronch";
		/*param.knots = { 1, 2};
		param.branches = { 1 };*/
		param.knots = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
		param.branches = { 1,2,3,4,5,6,7,8,9,10,11,12,13, 14, 15 };
		TreeDataLoader loader;
		loader.Load(&tree, param);

		return tree;
	}
}