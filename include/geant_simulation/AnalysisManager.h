#ifndef __ANALYSISMANAGER__
#define __ANALYSISMANAGER__

#ifdef __ANALYSISMANAGER_HEADER__
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include"TFile.h"
#include"TTree.h"
#endif

#include <string>

class TFile;
class TTree;

class G4Run;
class G4Event;
class G4Track;
class G4Step;

struct RunTuple {
	int runID;
	int nEvents;
};

struct EventTuple {
	int runID;
	int eventGlobalID;
	int eventLocalID;
	int nTracks;
};

struct TrackTuple {
	int eventGlobalID;
	int trackGlobalID;
	int trackLocalID;
	int trackParentLocalID;
	std::string particleName;
	std::string processName;
	std::string volumeName;
	double genTime;
	double genPosition[3];
	double genKineteicEnergy;
	double genMomentum[3];
	int nSteps;
};

struct StepTuple {
	int trackGlobalID;
	int stepGlobalID;
	std::string volumeName;
	double time;
	double position[3];
	double kineticEnergy;
	double momentum[3];
	double deltaEnergy;
	double totalDepositEnergy;
	double nonIonizingEnergyLoss;
};

class AnalysisManager {
private:
	int runID = 0;
	int eventID = 0;
	int trackID = 0;
	int stepID = 0;

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

	void setEventID(const int id) { eventID = id; }
	void setTrackID(const int id) { trackID = id; }
	int getEventID() const { return eventID; }
	int getTrackID() const { return trackID; }

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

	RunTuple& getRunTuple() { return runTuple; }
	EventTuple& getEventTuple() { return eventTuple; }
	TrackTuple& getTrackTuple() { return trackTuple; }
	StepTuple& getStepTuple() { return stepTuple; }
};

#endif