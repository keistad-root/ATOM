#ifndef __STEPPINGACTION__
#define __STEPPINGACTION__

#include "G4UserSteppingAction.hh"

#include "EventAction.h"
#include "AnalysisManager.h"

class SteppingAction : public G4UserSteppingAction {
private:
    EventAction* fEventAction;
public:
    SteppingAction(EventAction* eventAction);
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step*);
};

#endif