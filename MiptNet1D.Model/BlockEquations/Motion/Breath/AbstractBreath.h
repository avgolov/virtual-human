#pragma once

#ifndef ABSTRACTBREATH_H_
#define ABSTRACTBREATH_H_

namespace MiptNet1DModel {

	//��������� ���������� ������� 
	class AbstractBreath {

	public:
		
		AbstractBreath() { }
		virtual ~AbstractBreath() {	}

		//������ ������ ���������� ������ �� ����� ���� �� �������
		virtual double GetVentVolume(double time_curr) = 0;

		//������� ������� �������
		virtual double GetRateCur() const = 0;

		//������� ����������� �����
		virtual double GetTidalVolumeCur() const = 0;

		//������� �������
		//�� ������� ���� �� �������
		virtual double GetRateOld() const = 0;

		//����������� �����
		//�� ������� ���� �� �������
		virtual double GetTidalVolumeOld() const = 0;
		virtual double GetTidalVolumeSS() const = 0;
		virtual double GetRateSS() const = 0;

		virtual void SetRate(double value) = 0;
		virtual void SetTidalVolume(double value) = 0;
		virtual void SetRateSS(double value) = 0;
		virtual void SetTidalVolumeSS(double value) = 0;

		virtual double GetPg() = 0;
		virtual void SetPg(double value) = 0;

	};
}

#endif /*ABSTRACTBREATH_H_*/