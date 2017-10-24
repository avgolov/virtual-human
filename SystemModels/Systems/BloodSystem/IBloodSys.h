#pragma once
#ifndef IBLOODSYSTEM_H_
#define IBLOODSYSTEM_H_

#include "../ISystem.h"

namespace SystemModels
{
	enum BloodSysParam
	{
		//Flows
		CardicOutput,
		BrainFlow,
		TissueFlow,
		CardicOutputOld,
		BrainFlowOld,
		TissueFlowOld,
		CardicOutputSS,
		BrainFlowSS,
		TissueFlowSS,

		//Partitial pressures
		Po2Art,
		Pco2Art,
		Po2Ven,
		Pco2Ven,
		Po2Brain,
		Pco2Brain,
		Po2AlvArt,
		Pco2AlvArt,
		Po2AlvVen,
		Pco2AlvVen,
		Po2ArtOld,
		Pco2ArtOld,
		Po2VenOld,
		Pco2VenOld,
		Po2BrainOld,
		Pco2BrainOld,
		Po2AlvArtOld,
		Pco2AlvArtOld,
		Po2AlvVenOld,
		Pco2AlvVenOld,

		//Fractions
		O2Art,
		Co2Art,
		O2Ven,
		Co2Ven,
		O2Brain,
		Co2Brain,
		O2AlvArt,
		Co2AlvArt,
		O2AlvVen,
		Co2AlvVen,
		Hco3Art,
		HArt,
		HbArt,
		O2ArtOld,
		Co2ArtOld,
		O2VenOld,
		Co2VenOld,
		O2BrainOld,
		Co2BrainOld,
		O2AlvArtOld,
		Co2AlvArtOld,
		O2AlvVenOld,
		Co2AlvVenOld,
		LaArt,
		LaVen,
		LaBrain,
		LaAlvArt,
		LaAlvVen,
		LaArtOld,
		LaVenOld,
		LaBrainOld,
		LaAlvArtOld,
		LaAlvVenOld,

		//Volums
		PulmArtVol,
		PulmVenVol,
		BrainArtVol,
		SysArtVol,
		SysVenVol,
		AllVol,

		//Saturation
		PulmArtO2Sat,
		PulmVenO2Sat,
		BrainArtO2Sat,
		SysArtO2Sat,
		SysVenO2Sat,

		//Other
		PhArt
	};

	//The model of blood system
	class IBloodSys : public ISystem
	{
	public:
		IBloodSys() {}
		virtual ~IBloodSys() {};

		virtual void SetParam(BloodSysParam param, double value) = 0;
		virtual double GetParam(BloodSysParam param) const = 0;
	};
}

#endif /*IBLOODSYSTEM_H_*/