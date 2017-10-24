#pragma once

#ifndef BRONCHTREE_H_
#define BRONCHTREE_H_

#include "BaseTree.h"
#include "Alveole.h"

namespace MiptNet1DModel {

	class BronchTree : public BaseTree {
	
	public:
		BronchTree() : BaseTree(0) { }
		BronchTree(unsigned int id) : BaseTree(id) { }

		std::vector<Alveole*>* get_alveoles() { return &alveoles_; }

		Branch* get_nasopharynx() const { return nasopharynx_; }

		//Установка всех значений с предыдушего шага по времени
		void SetPrev() override {
			BaseTree::SetPrev();

			for each (auto alveole in alveoles_) {
				alveole->SubstancePrev = alveole->Substance;
				alveole->VolumePrev = alveole->Volume;
			}
		}

		void SetAlveole(Alveole* alveole) {
			for (size_t i = 0; i < alveoles_.size();i++) {
				if(alveoles_[i]->get_id() == alveole->get_id())	{
					alveoles_[i]->Volume = alveole->Volume;
					alveoles_[i]->VolumePrev = alveole->VolumePrev;
					alveoles_[i]->VolumeInit = alveole->VolumeInit;
					alveoles_[i]->Substance = alveole->Substance;
					alveoles_[i]->SubstancePrev = alveole->SubstancePrev;
					return;
				}
			}
			alveoles_.push_back(alveole);
		}

		//Настройка структуры легких
		void SetLungs(double alv_r, double alv_c) {
			for each (auto knot in knots_)
			{
				//Носоглотка
				if (knot->BranchesIn.size() == 0 &&
					knot->BranchesOut.size() == 1){
					nasopharynx_ = knot->BranchesOut.front();
				}

				//Стыковка с альвеолярным объемом
				if (knot->BranchesIn.size() == 1 &&
					knot->BranchesOut.size() == 0){
					auto branch = knot->BranchesIn.front();
					alveoles_.push_back(new Alveole(branch->get_id(), branch, alv_r, alv_c));
					alveoles_.back()->Substance = branch->get_last_point()->Substance;
					alveoles_.back()->SubstancePrev = alveoles_.back()->Substance;
				}
			}
		}

		double GetDeadSpace() const {
			auto volume = 0.;
			for(auto branch: branches_) {
				volume += branch->get_length()*branch->get_square();
			}
			return volume;
		}
		
	private:
		std::vector<Alveole*> alveoles_;
		Branch* nasopharynx_ = nullptr;
	};
}

#endif /*BRONCHTREE_H_*/