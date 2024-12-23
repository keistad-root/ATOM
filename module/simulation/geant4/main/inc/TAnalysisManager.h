#ifndef __TANALYSISMANAGER__
#define __TANALYSISMANAGER__

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
	Double_t initX, initY, initZ;
	Double_t initPX, initPY, initPZ;
	Double_t initKineticEnergy;
	Int_t initVolumeID;
	Double_t finalX, finalY, finalZ;
	Double_t finalPX, finalPY, finalPZ;
	Double_t finalKineticEnergy;
	Int_t finalVolumeID;
	void init() {
		eventID = 0;
		trackID = 0;
		parentID = 0;
		particleID = 0;
		initX = 0;
		initY = 0;
		initZ = 0;
		initPX = 0;
		initPY = 0;
		initPZ = 0;
		initKineticEnergy = 0;
		initVolumeID = 0;
		finalX = 0;
		finalY = 0;
		finalZ = 0;
		finalPX = 0;
		finalPY = 0;
		finalPZ = 0;
		finalKineticEnergy = 0;
		finalVolumeID = 0;
	}
};

struct incidentTuple {
	Int_t eventID = 0;
	Int_t trackID = 0;
	Double_t depositEnergy[3] = {-1, -1, -1};
	Double_t position[3] = {0, 0, 0};
	Double_t momentum[3] = {0, 0, 0};
	Double_t globalTime = 0;
	Double_t localTime = 0;
	Double_t kineticEnergy = 0;
	void init() {
		eventID = 0;
		trackID = 0;
		depositEnergy[0] = -1;
		depositEnergy[1] = -1;
		depositEnergy[2] = -1;
		position[0] = 0;
		position[1] = 0;
		position[2] = 0;
		momentum[0] = 0;
		momentum[1] = 0;
		momentum[2] = 0;
		globalTime = 0;
		localTime = 0;
		kineticEnergy = 0;
	}
};

class TAnalysisManager {
public:
	TAnalysisManager();
	~TAnalysisManager();

private:
	static TAnalysisManager* mInstance;

	trackTuple mTrackTuple;
	incidentTuple mIncidentTuple;

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
	bool isInEpitaxial;
	bool isInSubstrate;
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

	Int_t getParticleID(const G4String& particleID);
	Int_t getVolumeID(const G4LogicalVolume* volumeID);
};

#endif