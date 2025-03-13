#ifndef __TANALYSISMANAGER__
#define __TANALYSISMANAGER__

#include<fstream>

#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Run.hh"
#include "G4Event.hh"

#include "TFile.h"
#include "TTree.h"
#include "cpptqdm.h"

#include "TParticleID.h"
#include "TVolumeID.h"

#include "TDetectorConstruction.h"

struct trackTuple {
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

class TAnalysisManager {
public:
	TAnalysisManager();
	~TAnalysisManager();

private:
	static TAnalysisManager* mInstance;
	std::ofstream mParticleFile;

	trackTuple mTrackTuple;

	G4String mFileName;
	TFile* mFile = nullptr;
	TTree* mTrackTree = nullptr;
	TTree* mIncidentTree = nullptr;
	ProgressBar* mProgressBar = nullptr;
	std::vector<std::string> mUnknownParticleList;

	Int_t mEventID = 0;

	G4LogicalVolume* mALPIDEMetalLogical = nullptr;
	G4LogicalVolume* mALPIDEEpitaxialLogical = nullptr;
	G4LogicalVolume* mALPIDESubstrateLogical = nullptr;
	G4LogicalVolume* mCollimatorLogical = nullptr;
	G4LogicalVolume* mScreenLogical = nullptr;
	G4LogicalVolume* mWorldLogical = nullptr;

	bool isInALPIDE;
public:
	static TAnalysisManager* Instance();

	void open(const G4String& name);
	void close();

	void doBeginOfRun(const G4Run* run);
	void doEndOfRun(const G4Run* run);
	void doBeginOfEvent(const G4Event* event);
	void doEndOfEvent(const G4Event* event);
	void doPreTracking(const G4Track* track);
	void doPostTracking(const G4Track* track);
	void doStepPhase(const G4Step* step);

	void setFileName(const G4String& name);

	Int_t getParticleID(const G4String& particleID);
	Int_t getVolumeID(const G4LogicalVolume* volumeID);
};

#endif