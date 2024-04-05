#ifndef __ANALYSISMANAGER__
#define __ANALYSISMANAGER__

#include "G4PhysicalConstants.hh"
#include "G4Run.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"

#include "DetectorConstruction.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

struct RunTuple {
    Int_t runID;
    Int_t nEvents;
};

struct EventTuple {
    Int_t runID;
    Int_t eventGlobalID;
    Int_t eventLocalID;
    Int_t nTracks;
};

struct TrackTuple {
    Int_t eventGlobalID;
    Int_t trackGlobalID;
    Int_t trackLocalID;
    Int_t trackParentLocalID;
    TString particleName;
    TString processName;
    TString volumeName;
    Double_t genTime;
    Double_t genPosition[3];
    Double_t genKineteicEnergy;
    Double_t genMomentum[3];
    Int_t nSteps;
};

struct StepTuple {
    Int_t trackGlobalID;
    Int_t stepGlobalID;
    TString volumeName;
    Double_t time;
    Double_t position[3];
    Double_t kineticEnergy;
    Double_t momentum[3];
    Double_t deltaEnergy;
    Double_t totalDepositEnergy;
    Double_t nonIonizingEnergyLoss;
};

class AnalysisManager {
private:
    Int_t runID = 0;
    Int_t eventID = 0;
    Int_t trackID = 0;
    Int_t stepID = 0;

    static AnalysisManager* fInstance;

    TFile* mOutputFile;
    TTree* mRunTree;
    TTree* mEventTree;
    TTree* mTrackTree;
    TTree* mStepTree;

    RunTuple runTuple;
    EventTuple eventTuple;
    TrackTuple trackTuple;
    StepTuple stepTuple;
public:
    AnalysisManager();
    AnalysisManager(std::string name);
    ~AnalysisManager();

    static AnalysisManager* Instance();

    void setEventID(const Int_t id) { eventID = id; }
    void setTrackID(const Int_t id) { trackID = id; }
    Int_t getEventID() const { return eventID; }
    Int_t getTrackID() const { return trackID; }

    void openBook(std::string name);

    void setRunTree();
    void setEventTree();
    void setTrackTree();
    void setStepTree();

    void RecordingRun(const G4Run* run);
    void RecordingEvent(const G4Event* event);
    void RecordingTrackStart(const G4Track* track);
    void RecordingTrackEnd(const G4Track* track);
    void RecordingStep(const G4Step* step);

    void closeBook();

    RunTuple& getRunTuple() {return runTuple;}
    EventTuple& getEventTuple() {return eventTuple;}
    TrackTuple& getTrackTuple() {return trackTuple;}
    StepTuple& getStepTuple() {return stepTuple;}
};

#endif