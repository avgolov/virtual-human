#include "Asthma/AsthmaExperiment.h"
#include "Emphysema/EmphysemaExperiment.h"
#include "Hipoxia/HipoxiaExperiment.h"
#include "../Numerics/NewtonMethod.h"
#include "../SystemModels/Systems/BloodSystem/Biochemistry/O2HbEquations.h"
#include "Hipoxia/Lump/LumpHipoxiaExperiment.h"
#include "Sport/IncremetalLoadTest.h"
#include "Hipoxia/LumpLungFullBlood/LlFbHipoxiaExperiment.h"
#include "Sport/IntervalTraining/IntervalLoad.h"

int main()
{
 	/*Experiments::AsthmaExperiment asthma;
	asthma.Run();*/
	/*Experiments::EmphysemaExperiment emphysema;
	emphysema.Run();*/
	/*Experiments::HipoxiaExperiment hipoxia;
	hipoxia.Run();

	Experiments::LumpHipoxiaExperiment hipoxia;
	hipoxia.Run();*/
	Experiments::IncremetalLoadTest sport;
	sport.Run();
	/*Experiments::LlFbHipoxiaExperiment hipoxia;
	hipoxia.Run();*/
	/*Experiments::IntervalLoad interval;
	interval.Run();
	*/
	return 0;
}

