#include "CompartmentsBloodSys.h"
#include <vector>

namespace SystemModels
{
	void CompartmentsBloodSys::Compute(TimeSpan* timeSpan)
	{
		//Set old values
		cardiacOutputOld_ = cardiacOutput_;
		brainFlowOld_ = brainFlowOld_;
		tissueFlowOld_ = tissueFlow_;
		pulmArtTank_.SetOldValues();
		pulmVenTank_.SetOldValues();
		brainArtTank_.SetOldValues();
		tissueArtTank_.SetOldValues();
		sysVenTank_.SetOldValues();

		pulmArtTank_.Compute(timeSpan);
		pulmVenTank_.Compute(timeSpan);
		brainArtTank_.Compute(timeSpan);
		tissueArtTank_.Compute(timeSpan);
		sysVenTank_.Compute(timeSpan);
		/*Mass = pulmArtTank_.GetVolume()*pulmArtTank_.GetCo2() +
			pulmVenTank_.GetVolume()*pulmVenTank_.GetCo2() +
			brainArtTank_.GetVolume()*brainArtTank_.GetCo2() +
			tissueArtTank_.GetVolume()*tissueArtTank_.GetCo2() +
			sysVenTank_.GetVolume()*sysVenTank_.GetCo2();*/
	}

	void CompartmentsBloodSys::SetParam(BloodSysParam param, double value)
	{
		switch(param)
		{
			case CardicOutput: {
				cardiacOutputOld_ = cardiacOutput_;
				cardiacOutput_ = value;
				break;
			}
			case BrainFlow: {
				brainFlowOld_ = brainFlow_;
				brainFlow_ = value;
				break;
			}
			case TissueFlow: {
				tissueFlowOld_ = tissueFlow_;
				tissueFlow_ = value;
				break;
			}
			case SysArtVol: {
				tissueArtTank_.SetVolume(value);
				break;
			}
			case SysVenVol: {
				sysVenTank_.SetVolume(value);
				break;
			}
			case PulmArtVol: {
				pulmArtTank_.SetVolume(value);
				break;
			}
			case PulmVenVol: {
				pulmVenTank_.SetVolume(value);
				break;
			}
			case BrainArtVol: {
				brainArtTank_.SetVolume(value);
				break;
			}
			//partitial pressures
			case Po2Art: {
				tissueArtTank_.SetPo2(value);
				break;
			}
			case Pco2Art: {
				tissueArtTank_.SetPco2(value);
				break;
			}
			case Po2Ven: {
				sysVenTank_.SetPo2(value);
				break;
			}
			case Pco2Ven: {
				sysVenTank_.SetPco2(value);
				break;
			}
			case Po2Brain: {
				brainArtTank_.SetPo2(value);
				break;
			}
			case Pco2Brain: {
				brainArtTank_.SetPco2(value);
				break;
			}
			case Po2AlvArt: {
				pulmArtTank_.SetPo2(value);
				break;
			}
			case Pco2AlvArt: {
				pulmArtTank_.SetPco2(value);
				break;
			}
			case Po2AlvVen: {
				pulmVenTank_.SetPo2(value);
				break;
			}
			case Pco2AlvVen: {
				pulmVenTank_.SetPco2(value);
				break;
			}

			//fractions
			case O2Art: {
				tissueArtTank_.SetO2(value);
				break;
			}
			case Co2Art: {
				tissueArtTank_.SetCo2(value);
				break;
			}
			case O2Ven: {
				sysVenTank_.SetO2(value);
				break;
			}
			case Co2Ven: {
				sysVenTank_.SetCo2(value);
				break;
			}
			case O2Brain: {
				brainArtTank_.SetO2(value);
				break;
			}
			case Co2Brain: {
				brainArtTank_.SetCo2(value);
				break;
			}
			case O2AlvArt: {
				pulmArtTank_.SetO2(value);
				break;
			}
			case Co2AlvArt: {
				pulmArtTank_.SetCo2(value);
				break;
			}
			case O2AlvVen: {
				pulmVenTank_.SetO2(value);
				break;
			}
			case Co2AlvVen: {
				pulmVenTank_.SetCo2(value);
				break;
			}
			case CardicOutputSS: {
				cardiacOutputSS_ = value;
				break;
			}
			case BrainFlowSS: {
				brainFlowSS_ = value;
				break;
			}
			case TissueFlowSS: {
				tissueFlowSS_ = value;
				break;
			}
			default:
				throw std::exception("Wrong for setting blood system's parameter");
		}
	}

	double CompartmentsBloodSys::GetParam(BloodSysParam param) const
	{
		switch (param)
		{
		case CardicOutput: {
			return cardiacOutput_;
		}
		case BrainFlow: {
			return brainFlow_;
		}
		case TissueFlow: {
			return tissueFlow_;
		}
		case CardicOutputOld: {
			return cardiacOutputOld_;
		}
		case BrainFlowOld: {
			return brainFlowOld_;
		}
		case TissueFlowOld: {
			return tissueFlowOld_;
		}
		case CardicOutputSS: {
			return cardiacOutputSS_;
		}
		case BrainFlowSS: {
			return brainFlowSS_;
		}
		case TissueFlowSS: {
			return tissueFlowSS_;
		}

		//Partitial pressures
		case Po2Art: {
			return tissueArtTank_.GetPo2();
		}
		case Pco2Art: {
			return tissueArtTank_.GetPco2();
		}
		case Po2Ven: {
			return sysVenTank_.GetPo2();
		}
		case Pco2Ven: {
			return sysVenTank_.GetPco2();
		}
		case Po2Brain: {
			return brainArtTank_.GetPo2();
		}
		case Pco2Brain: {
			return brainArtTank_.GetPco2();
		}
		case Po2AlvArt: {
			return pulmArtTank_.GetPo2();
		}
		case Pco2AlvArt: {
			return pulmArtTank_.GetPco2();
		}
		case Po2AlvVen: {
			return pulmVenTank_.GetPo2();
		}
		case Pco2AlvVen: {
			return pulmVenTank_.GetPco2();
		}
		
		//fractions
		case O2Art: {
			return tissueArtTank_.GetO2();
		}
		case Co2Art: {
			return tissueArtTank_.GetCo2();
		}
		case O2Ven: {
			return sysVenTank_.GetO2();
		}
		case Co2Ven: {
			return sysVenTank_.GetCo2();
		}
		case O2Brain: {
			return brainArtTank_.GetO2();
		}
		case Co2Brain: {
			return brainArtTank_.GetCo2();
		}
		case O2AlvArt: {
			return pulmArtTank_.GetO2();
		}
		case Co2AlvArt: {
			return pulmArtTank_.GetCo2();
		}
		case O2AlvVen: {
			return pulmVenTank_.GetO2();
		}
		case Co2AlvVen: {
			return pulmVenTank_.GetCo2();
		}
		case O2ArtOld: {
			return tissueArtTank_.GetO2Old();
		}
		case Co2ArtOld: {
			return tissueArtTank_.GetCo2Old();
		}
		case O2VenOld: {
			return sysVenTank_.GetO2Old();
		}
		case Co2VenOld: {
			return sysVenTank_.GetCo2Old();
		}
		case O2BrainOld: {
			return brainArtTank_.GetO2Old();
		}
		case Co2BrainOld: {
			return brainArtTank_.GetCo2Old();
		}
		case O2AlvArtOld: {
			return pulmArtTank_.GetO2Old();
		}
		case Co2AlvArtOld: {
			return pulmArtTank_.GetCo2Old();
		}
		case O2AlvVenOld: {
			return pulmVenTank_.GetO2Old();
		}
		case Co2AlvVenOld: {
			return pulmVenTank_.GetCo2Old();
		}
		case HArt: {
			return tissueArtTank_.GetH();
		}
		case Hco3Art: {
			return tissueArtTank_.GetHCo3();
		}
		case HbArt: {
			return tissueArtTank_.GetHb();
		}
		case PhArt: {
			return -log10(tissueArtTank_.GetH());
		}

		case LaArt: {
			return tissueArtTank_.GetLa();
		}
		case LaBrain: {
			return brainArtTank_.GetLa();
		}
		case LaVen: {
			return sysVenTank_.GetLa();
		}
		case LaAlvVen: {
			return pulmVenTank_.GetLa();
		}
		case LaAlvArt: {
			return pulmArtTank_.GetLa();
		}
		case LaArtOld: {
			return tissueArtTank_.GetLaOld();
		}
		case LaBrainOld: {
			return brainArtTank_.GetLaOld();
		}
		case LaVenOld: {
			return sysVenTank_.GetLaOld();
		}
		case LaAlvVenOld: {
			return pulmVenTank_.GetLaOld();
		}
		case LaAlvArtOld: {
			return pulmArtTank_.GetLaOld();
		}

		case SysArtVol: {
			return tissueArtTank_.GetVolume();
		}
		case SysVenVol: {
			return sysVenTank_.GetVolume();
		}
		case PulmArtVol: {
			return pulmArtTank_.GetVolume();
		}
		case PulmVenVol: {
			return pulmVenTank_.GetVolume();
		}
		case BrainArtVol: {
			return brainArtTank_.GetVolume();
		}
		

		//Saturation
		case SysArtO2Sat: {
			return 100.*tissueArtTank_.GetHb() / tissueArtTank_.GetHbTot();
		}
		case SysVenO2Sat: {
			return sysVenTank_.GetVolume();
		}
		case PulmArtO2Sat: {
			return pulmArtTank_.GetVolume();
		}
		case PulmVenO2Sat: {
			return pulmVenTank_.GetVolume();
		}
		case BrainArtO2Sat: {
			return brainArtTank_.GetVolume();
		}
						 
		default:
			throw std::exception("Bad blood system's parameter!");
		}
	}

	void CompartmentsBloodSys::SetSubstances(std::valarray<double> substances)
	{
		pulmArtTank_.SetSubstances(substances);
		pulmVenTank_.SetSubstances(substances);
		brainArtTank_.SetSubstances(substances);
		tissueArtTank_.SetSubstances(substances);
		sysVenTank_.SetSubstances(substances);

		/*auto o2 = pulmArtTank_.GetO2Free();
		tissueArtTank_.SetO2Free(o2 * 0.79);
		sysVenTank_.SetO2Free(o2 * 0.8);
		pulmVenTank_.SetO2Free(o2 * 0.8);
		brainArtTank_.SetO2Free(o2 * 0.7);
		*/
	}

	Eigen::VectorXd CompartmentsBloodSys::GetO2FreeOldValues() {
		Eigen::VectorXd result(5);
		result.coeffRef(0) = pulmArtTank_.GetO2Free();
		result.coeffRef(1) = pulmVenTank_.GetO2Free();
		result.coeffRef(2) = sysVenTank_.GetO2Free();
		result.coeffRef(3) = brainArtTank_.GetO2Free();
		result.coeffRef(4) = tissueArtTank_.GetO2Free();
		return result;
	}

	Eigen::VectorXd CompartmentsBloodSys::GetHco3OldValues() {
		Eigen::VectorXd result(5);
		result.coeffRef(0) = pulmArtTank_.GetHCo3();
		result.coeffRef(1) = pulmVenTank_.GetHCo3();
		result.coeffRef(2) = sysVenTank_.GetHCo3();
		result.coeffRef(3) = brainArtTank_.GetHCo3();
		result.coeffRef(4) = tissueArtTank_.GetHCo3();
		return result;
	}

	Eigen::VectorXd CompartmentsBloodSys::GetHOldValues() {
		Eigen::VectorXd result(5);
		result.coeffRef(0) = pulmArtTank_.GetH();
		result.coeffRef(1) = pulmVenTank_.GetH();
		result.coeffRef(2) = sysVenTank_.GetH();
		result.coeffRef(3) = brainArtTank_.GetH();
		result.coeffRef(4) = tissueArtTank_.GetH();
		return result;
	}

	Eigen::VectorXd CompartmentsBloodSys::GetLaOldValues() {
		Eigen::VectorXd result(5);
		result.coeffRef(0) = pulmArtTank_.GetLa();
		result.coeffRef(1) = pulmVenTank_.GetLa();
		result.coeffRef(2) = sysVenTank_.GetLa();
		result.coeffRef(3) = brainArtTank_.GetLa();
		result.coeffRef(4) = tissueArtTank_.GetLa();
		return result;
	}

	void CompartmentsBloodSys::SetO2Free(const Eigen::VectorXd& u)	{
		//{PA, PV, SV, BA, TA}
		pulmArtTank_.SetO2Free(u.coeffRef(0));
		pulmVenTank_.SetO2Free(u.coeffRef(1));
		sysVenTank_.SetO2Free(u.coeffRef(2));
		brainArtTank_.SetO2Free(u.coeffRef(3));
		tissueArtTank_.SetO2Free(u.coeffRef(4));
	}

	void CompartmentsBloodSys::SetHco3(
		const Eigen::VectorXd& u, 
		const Eigen::VectorXd& m) {
		//{PA, PV, SV, BA, TA}
		pulmArtTank_.SetHco3(u.coeffRef(0), m.coeffRef(0));
		pulmVenTank_.SetHco3(u.coeffRef(1), m.coeffRef(1));
		sysVenTank_.SetHco3(u.coeffRef(2), m.coeffRef(2));
		brainArtTank_.SetHco3(u.coeffRef(3), m.coeffRef(3));
		tissueArtTank_.SetHco3(u.coeffRef(4), m.coeffRef(4));
	}

	void CompartmentsBloodSys::SetLa(const Eigen::VectorXd& u) {
		pulmArtTank_.SetLa(u.coeffRef(0));
		pulmVenTank_.SetLa(u.coeffRef(1));
		sysVenTank_.SetLa(u.coeffRef(2));
		brainArtTank_.SetLa(u.coeffRef(3));
		tissueArtTank_.SetLa(u.coeffRef(4));
	}
}