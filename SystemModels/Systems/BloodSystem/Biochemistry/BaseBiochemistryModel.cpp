#include "BaseBiochemistryModel.h"

namespace SystemModels
{
	/*double BaseBiochemistryModel::GetPo2() const
	{
		auto coef = 1. / 22.256;
		auto hb = (*values_)[2];
		auto o2 = coef*(*values_)[0];
		double s_o2 = GetSo2();
		return (o2 -4*hb*s_o2)/a_o2_;
	}

	double BaseBiochemistryModel::GetPco2() const
	{
		auto coef = 1. / 22.256;
		auto hco3 = (*values_)[3];
		auto co2 = coef*(*values_)[1];
		return (co2- hco3)/a_co2_;
	}
	void BaseBiochemistryModel::SetPo2(double value)
	{
		auto coef = 1. / 22.256;
		auto o2 = coef*(*values_)[0];
		(*values_)[2] = std::sqrt(0.25*hb_tot_*(o2 - value*a_o2_));
		(*valuesOld_)[2] = (*values_)[2];
	}
	void BaseBiochemistryModel::SetPco2(double value)
	{
		auto coef = 1. / 22.256;
		auto co2 = coef*(*values_)[1];
		(*values_)[3] = co2 - a_co2_*value;
		(*valuesOld_)[3] = (*values_)[3];
		(*values_)[4] = k2_*co2 / (k22_*(*values_)[3]);
		(*valuesOld_)[4] = (*values_)[4];
	}*/
	double BaseBiochemistryModel::GetSo2() const
	{
		return (*values_)[2] /hb_tot_;
	}
	void BaseBiochemistryModel::Compute(TimeSpan* timeSpan)
	{
		/*auto coef = 1./22.256;
		auto hco3 = (*values_)[3];
		auto h = (*values_)[4];
		auto hb = (*values_)[2];
		auto co2_free = coef*(*values_)[1] - hco3;
		co2_free = co2_free > 0. ? co2_free : 1.e-4;
		auto o2_free = coef*(*values_)[0];
		o2_free = (o2_free - 4 * hb*hb/ hb_tot_);
		o2_free = o2_free > 0. ? o2_free : 1.e-4;

		(*values_)[2] += timeSpan->dt*(k1_*(hb_tot_- hb)*pow(o2_free,m_)-k11_*hb);
		(*values_)[3] += timeSpan->dt*(k2_*co2_free -k22_*h*hco3);
		(*values_)[4] += timeSpan->dt*(k2_*co2_free - k22_*h*hco3);*/
	}

	double BaseBiochemistryModel::GetPo2() const
	{
		/*auto o2 = (*values_)[0]/22.4;
		return (o2 - b_o2_) / a_o2_;*/
		return (*values_)[0] / b_o2_;
	}

	double BaseBiochemistryModel::GetPco2() const
	{
		/*auto co2 = (*values_)[1] / 22.4;
		return (co2 - b_co2_) / a_co2_;*/
		return (*values_)[1] / b_co2_;
	}
	void BaseBiochemistryModel::SetPo2(double value)
	{
		/*auto o2 = (*values_)[0] / 22.4;
		b_o2_ = o2 - value*a_o2_;*/
		b_o2_ = (*values_)[0] / value;
	}
	void BaseBiochemistryModel::SetPco2(double value)
	{
		/*auto co2 = (*values_)[1] / 22.4;
		b_co2_ = co2 - value*a_co2_;*/
		b_co2_ = (*values_)[1] / value;
	}
}