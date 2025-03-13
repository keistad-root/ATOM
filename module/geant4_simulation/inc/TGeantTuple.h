#ifndef __TGEANTTUPLE__
#define __TGEANTTUPLE__

#include "TROOT.h"

struct TTrackTuple {
	Int_t eventID;
	Int_t trackID;
	Int_t parentID;
	Int_t particleID;
	Double_t initPosition[3];
	Double_t initMomentum[3];
	Double_t initKineticEnergy;
	Int_t initVolumeID;
	Double_t incidentPosition[3];
	Double_t incidentMomentum[3];
	Double_t incidentKineticEnergy;
	Double_t depositEnergy[3];
	Double_t globalTime;
	Double_t localTime;
	Double_t finalPosition[3];
	Double_t finalMomentum[3];
	Double_t finalKineticEnergy;
	Int_t finalVolumeID;
	Bool_t isInALPIDE;
	void init() {
		eventID = 0;
		trackID = 0;
		parentID = 0;
		particleID = 0;
		initPosition[0] = 0.;
		initPosition[1] = 0.;
		initPosition[2] = 0.;
		initMomentum[0] = 0.;
		initMomentum[1] = 0.;
		initMomentum[2] = 0.;
		initKineticEnergy = 0.;
		initVolumeID = 0;
		incidentPosition[0] = 0.;
		incidentPosition[1] = 0.;
		incidentPosition[2] = 0.;
		incidentMomentum[0] = 0.;
		incidentMomentum[1] = 0.;
		incidentMomentum[2] = 0.;
		incidentKineticEnergy = 0.;
		depositEnergy[0] = 0.;
		depositEnergy[1] = 0.;
		depositEnergy[2] = 0.;
		globalTime = 0.;
		localTime = 0.;
		finalPosition[0] = 0.;
		finalPosition[1] = 0.;
		finalPosition[2] = 0.;
		finalMomentum[0] = 0.;
		finalMomentum[1] = 0.;
		finalMomentum[2] = 0.;
		finalKineticEnergy = 0.;
		finalVolumeID = 0;
		isInALPIDE = false;
	}
};
struct TPrimaryAnalysisTuple {
	Int_t eventID;
	Double_t position[3];
	Double_t momentum[3];
	Double_t kineticEnergy;
};

struct TIncidentAnalysisTuple {
	Int_t eventID, trackID;
	Int_t particleID;
	Int_t initialVolumeID;
	Double_t initialPosition[3];
	Double_t initialMomentum[3];
	Double_t initialKineticEnergy;
	Double_t depositEnergy[3];
	Double_t position[3];
	Double_t momentum[3];
	Double_t kineticEnergy;
	Double_t globalTime;
	Double_t localTime;
	Int_t finalVolumeID;
	Double_t finalPosition[3];
	Double_t finalMomentum[3];
	Double_t finalKineticEnergy;
};

struct TSecondaryAnalysisTuple {
	Int_t eventID, trackID;
	Int_t parentID;
	Int_t particleID;
	Int_t initialVolumeID;
	Double_t initialPosition[3];
	Double_t initialMomentum[3];
	Double_t initialKineticEnergy;
	Double_t depositEnergy[3];
	Int_t finalVolumeID;
	Double_t finalPosition[3];
	Double_t finalMomentum[3];
	Double_t finalKineticEnergy;
};

#endif