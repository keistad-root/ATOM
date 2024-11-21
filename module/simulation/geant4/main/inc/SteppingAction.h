#ifndef __STEPPINGACTION__
#define __STEPPINGACTION__

#ifdef __STEPPINGACTION_HEADER__
#include "G4ios.hh"
#include "EventAction.h"
#include "AnalysisManager.h"
#endif

#include "G4UserSteppingAction.hh"

class EventAction;
class G4Step;

class SteppingAction : public G4UserSteppingAction {
public:
	SteppingAction(EventAction* eventAction);
	virtual ~SteppingAction();

	virtual void UserSteppingAction(const G4Step*);
};

#endif