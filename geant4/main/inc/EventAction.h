#ifndef __EVENTACTION__
#define __EVENTACTION__

#ifdef __EVENTACTION_HEADER__
#include "G4Event.hh"
#include "AnalysisManager.h"
#include "RunAction.h"
#include "G4GeneralParticleSourceData.hh"
#include "G4SingleParticleSource.hh"
#endif

#include "G4UserEventAction.hh"

class RunAction;
class G4Event;
typedef int G4int;

class EventAction : public G4UserEventAction {
private:
	G4int iStartTrack;

public:
	EventAction(RunAction* runAction);
	virtual ~EventAction();

	virtual void BeginOfEventAction(const G4Event* event);
	virtual void EndOfEventAction(const G4Event* event);
};

#endif