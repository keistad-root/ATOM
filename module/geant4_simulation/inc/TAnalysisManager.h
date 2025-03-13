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
#include "TGeantTuple.h"

#include "TDetectorConstruction.h"

class TAnalysisManager {
public:
	TAnalysisManager();
	~TAnalysisManager();

private:
	static TAnalysisManager* mInstance;
	std::ofstream mParticleFile;

	TTrackTuple mTrackTuple;

	G4String mFileName;
	TFile* mFile = nullptr;
	TTree* mTrackTree = nullptr;
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