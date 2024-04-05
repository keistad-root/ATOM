#ifndef __EVENTACTION__
#define __EVENTACTION__

#include "G4UserEventAction.hh"
#include "G4Event.hh"

#include "RunAction.h"

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