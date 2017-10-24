#pragma once

#ifndef TREEDATALOADER_H_
#define TREEDATALOADER_H_

#include <string>
#include <fstream>
#include "../Model/BronchTree.h"
#include "../Model/BaseSubstance.h"
#include "../Model/Branch.h"


namespace MiptNet1DModel {

	struct TreeDataLoaderParam
	{
		std::string bronch_tree_path;
		std::vector<int> knots;
		std::vector<int> branches;
		double lungs_volume;
		double alv_c;
		double alv_r;
	};

	class TreeDataLoader {

	public:

		void Load(BronchTree* tree, TreeDataLoaderParam info) {
			info_ = info;
			LoadBronchTree(tree);
		}

	private:
		TreeDataLoaderParam info_;

		std::vector<std::string>& split(std::string& s, char delim, std::vector<std::string>& elems) const;
		std::vector<std::string> split(std::string& s, char delim);

		void LoadBronchTree(BronchTree* tree){
			std::string   line;
			std::ifstream stream(info_.bronch_tree_path + "\\tree.tre");
			getline(stream, line);
			int knots_count = stoi(line);
			getline(stream, line);
			int branch_count = stoi(line);

			//read knotes
			stream = std::ifstream(info_.bronch_tree_path + "\\knot.tre");
			for (int i = 0; i < knots_count; i++) {
				getline(stream, line);
				unsigned int id = stoi(line);
				getline(stream, line);
				auto items = split(line, ' ');
				double x = stod(items[0]);
				double y = stod(items[1]);
				if (info_.knots.size() ==0 || 
					find(info_.knots.begin(), info_.knots.end(), id) != info_.knots.end()){
					tree->add_knot(new Knot(id, x, y, 0.0));
				}
				getline(stream, line);
			}

			//read branches
			stream = std::ifstream(info_.bronch_tree_path + "\\branch.tre");
			std::ifstream stream2(info_.bronch_tree_path + "\\tdbranch.tre");
			for (int i = 0; i < branch_count; i++) {
				getline(stream, line);
				unsigned int id = stoi(line);

				getline(stream, line);
				auto items = split(line, ' ');
				unsigned knot1 = stoi(items[0]);
				unsigned knot2 = stoi(items[1]);

				getline(stream, line);
				double length = stod(line);
				getline(stream, line);
				double square = 0.25 * M_PI * std::pow(stod(line), 2);// *2.5;//!!!!

				getline(stream, line);
				int point_count = stoi(line);
				
				getline(stream2, line);
				getline(stream2, line);
				double c = stod(line);
				getline(stream2, line);
				getline(stream2, line);
				getline(stream2, line);
				getline(stream2, line);
				getline(stream2, line);

				point_count = 5;

				BaseSubstance substance;
				substance.Density = 1.23e-6;
				substance.Viscosity = 1.78e-1;
				substance.Velocity = 0.0;
				substance.Pressure = 0.0;
				substance.Conc = {0.,0.};
				substance.D_conc = { 0.,0.};
				//O2
				substance.Conc[0] = 0.209;
				substance.D_conc[0] = 0.000000000018;
				//CO2
				substance.Conc[1] = 2.8e-4;
				substance.D_conc[1] = 0.00000000000001;//0.000014;
				
				if (info_.branches.size() == 0 || 
					find(info_.branches.begin(), info_.branches.end(), id) != info_.branches.end()){
					tree->add_branch(new Branch(id, length, knot1, knot2));
					Branch* branch = tree->get_branch(id);
					for (int pts = 0; pts < point_count; pts++){
						branch->add_point(new BranchPoint(id, pts, square, c, substance));
					}

					tree->get_knot(knot1)->BranchesOut.push_back(tree->get_branch(id));
					tree->get_knot(knot2)->BranchesIn.push_back(tree->get_branch(id));
				}
			}

			//—оздание альвеол€рных объемов и определение носоглотки 
			tree->SetLungs(info_.alv_r, info_.alv_c);

			//”становка альвеол€рных объемов
			info_.lungs_volume -= tree->GetDeadSpace();
			auto alveoles = tree->get_alveoles();
			auto alv_count = alveoles->size();
			for each (auto alveole in *alveoles)
			{
				alveole->Volume = info_.lungs_volume / alv_count;
				alveole->VolumePrev = info_.lungs_volume / alv_count;
				alveole->VolumeInit = info_.lungs_volume / alv_count;
				
			}

		}
	};
}

#endif /*TREEDATALOADER_H_*/