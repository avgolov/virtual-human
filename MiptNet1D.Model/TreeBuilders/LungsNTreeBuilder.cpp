#include "../NetTopology/Model/BronchTree.h"
#include "../NetTopology/Loaders/TreeDataLoader.h"
#include "LungsNTreeBuilder.h"

namespace MiptNet1DModel {

	BronchTree LungsNTreeBuilder::Build() {
		auto tree = BronchTree(1);
		TreeDataLoaderParam param;
		param.bronch_tree_path;
		param.lungs_volume = lungs_volume_;
		param.alv_c = alv_c_;
		param.alv_r = alv_r_;
		param.bronch_tree_path = "./treebronch";
		
		for(int i=1; i< pow(2,n_); i++) {
			param.knots.push_back(i);
			param.branches.push_back(i);

		}
		param.knots.push_back(pow(2, n_));
		TreeDataLoader loader;
		loader.Load(&tree, param);

		return tree;
	}
}