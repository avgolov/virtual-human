#pragma once

#ifndef SWEEPMETHOD_H_
#define SWEEPMETHOD_H_

#include <vector>

namespace MiptNet1DModel {

	//Решение системы A*Un-1 - B*Un + C*Un+1 = F
	//методом прогонки
	class SweepMethod {

	public:
		SweepMethod(std::vector<double>* a,
			std::vector<double>* b,
			std::vector<double>* c,
			std::vector<double>* f) :
			a_(a),
			b_(b),
			c_(c),
			f_(f) { }

		void Execute(std::vector<double>* solution) {
			//прямой ход
			std::vector<double> p(solution->size());
			std::vector<double> q(solution->size());
			/*p[0] = c_->at(0) / b_->at(0);
			q[0] = -f_->at(0) / b_->at(0);*/
			p[0] = 0.0;
			q[0] = 0.0;
			for (size_t i = 1; i < solution->size(); i++) {
				double a = (*a_)[i];
				double b = (*b_)[i];
				p[i] = -(*c_)[i] / (b + a * p[i - 1]);
				q[i] = ((*f_)[i] - a * q[i - 1]) /
					(b + a * p[i - 1]);
			}

			//обратный ход
			for (size_t i = solution->size() - 2; i > 1; i--) {
				(*solution)[i] = p[i] * (*solution)[i + 1] + q[i];
			}
		}

	private:
		//Коэф. A
		std::vector<double>* a_;

		//Коэф. B
		std::vector<double>* b_;

		//Коэф. C
		std::vector<double>* c_;

		//F - Правая часть
		std::vector<double>* f_;
	};
}

#endif /*SWEEPMETHOD_H_*/
