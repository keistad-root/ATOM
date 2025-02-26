#ifndef __TGEANTTUPLE__
#define __TGEANTTUPLE__

#include "TROOT.h"

struct TTrackTuple {
	Int_t eventID, trackID, parentID;
	Int_t particleID;
	Int_t initialVolumeID;
	Double_t initialPosition[3];
	Double_t initialMomentum[3];
	Double_t initialKineticEnergy;
	Int_t finalVolumeID;
	Double_t finalPosition[3];
	Double_t finalMomentum[3];
	Double_t finalKineticEnergy;
};

struct TIncidentTuple {
	Int_t eventID, trackID;
	Double_t depositEnergy[3];
	Double_t position[3];
	Double_t momentum[3];
	Double_t kineticEnergy;
	Double_t globalTime;
	Double_t localTime;
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
	Int_t particleID;
	Int_t initialVolumeID;
	Double_t initialPosition[3];
	Double_t initialMomentum[3];
	Double_t initialKineticEnergy;
	Int_t finalVolumeID;
	Double_t finalPosition[3];
	Double_t finalMomentum[3];
	Double_t finalKineticEnergy;
};

#endif