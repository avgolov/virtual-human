#pragma once

#ifndef LUNGSFLUIDEQUATION_H_
#define LUNGSFLUIDEQUATION_H_

#include <exception>
#include "tbb/tbb.h"
#include <Eigen/Dense>
#include "IncompFluidEquation.h"
#include "../../NetTopology/Model/BronchTree.h"
#include "Breath/AbstractBreath.h"

namespace MiptNet1DModel {

	class LungsFluidEquation : public IncompFluidEquation {

	public:
		LungsFluidEquation(BaseTree* tree) :
			IncompFluidEquation(tree) {

			if (!dynamic_cast<BronchTree*>(tree)) {
				throw std::exception("Wrong tree type!");
			}

			//yacobian_ = Eigen::MatrixXd(2, 2);
			//yacobian_ << 0., 1., -alv_c_ / alv_i_, -alv_r_ / alv_i_;
			//yacobian2_ = yacobian_ * yacobian_;
		}
		LungsFluidEquation() {}
		LungsFluidEquation& LungsFluidEquation::operator=(const LungsFluidEquation& other) {
			tree_ = other.tree_;
			state_ = other.state_;
			breath_param_ = other.breath_param_;
			//yacobian_ = other.yacobian_;
			//yacobian2_= other.yacobian2_;
			time_ = other.time_;
			return *this;
		}

		void set_breath_param(AbstractBreath* breath_param) {
			breath_param_ = breath_param;
		}
	    void SetRate(double value);
		void SetTidalVolume(double value);
		void SetRateSS(double value);
		void SetTidalVolumeSS(double value);
		double GetRate() const;
		double GetTidalVolume() const;
		double GetRateOld() const;
		double GetTidalVolumeOld() const;
		double GetRateSS() const;
		double GetTidalVolumeSS() const;
		std::vector<Alveole*>* GetAlveoles() const;

	protected:

		//Расчет внешних узлов бронхиального дерева
		void ComputeBoundaryKnots(double dt) override {
			auto tree = dynamic_cast<BronchTree*>(tree_);
			time_ += dt;

			//Носоглотка
			//double flow = breath_param_->GetVentVolume(time_);
			auto nasopharynx = tree->get_nasopharynx();
			auto point_second = nasopharynx->get_second_point();
			auto w = M_PI * breath_param_->GetRateCur() / 30.;
			double flow = point_second->Square*point_second->Substance.Velocity*dt;// 500 * sin(w*time_);//;
			/*double alpha, beta;
			OutCompatibilityCoef(nasopharynx, alpha,  beta, dt);
			auto point = nasopharynx->get_first_point();
			double det = sqrt(beta * beta + 4. * alpha * flow);
			point->Square = 0.5 * (-beta + det) / alpha;
			point->Substance.Velocity = 0.5 * (beta + det);*/
			auto point = nasopharynx->get_first_point();
			point->Substance.Velocity = point_second->Substance.Velocity;//flow / point->Square;
			//auto point = nasopharynx->get_first_point();
			point->Square = point_second->Square;
			//double alpha, beta;
			//OutCompatibilityCoef(nasopharynx, alpha, beta, dt);
			//point->Substance.Velocity = alpha*point->Square + beta;
			//point->Substance.Velocity = point_second->Substance.Velocity*point_second->Square / point->Square;
			auto alveoles = tree->get_alveoles();
			tbb::parallel_for(size_t(0), alveoles->size(), [&](size_t i)
			{
				ComputeAlveole((*alveoles)[i], dt);
			});
		}

	private:
		AbstractBreath* breath_param_ = nullptr;

		//const double alv_i_ = 0.0021;
		//const double alv_r_ = 0.2;//2.0;//0.2e-5;//0.11;
		//const double alv_c_ = 1.;//10.0;//1.e-3;//0.208;
		//const double alv_v0_ = 78.125;

		//const double alv_a0_ = -1.;
		//const double alv_a1_ = 1.;
		//const double alv_c1_ = -1. / 6.;
		//const double alv_b0_ = 0.5 - 1. / 6;
		//const double alv_b1_ = 0.5 + 1. / 6;

		//Eigen::MatrixXd yacobian_;
		//Eigen::MatrixXd yacobian2_;

		double time_ = 0;

		//double GetPleuralPressure(double& dt) const;

		//double GetDiffPleuralPressure(double& dt) const;

		//void GetAlvRightPart(Eigen::VectorXd& y, Eigen::VectorXd& rp, double press ,double dt);

		//void GetAlvDiffRightPart(Eigen::VectorXd& y, Eigen::VectorXd& rp, double dt);

		void ComputeAlveole(Alveole* alveole, double dt);

	};
}

#endif /*LUNGSFLUIDEQUATION_H_*/