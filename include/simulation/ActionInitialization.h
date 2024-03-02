#ifndef __ACTIONINITIALIZATION__
#define __ACTIONINITIALIZATION__

#include "G4VUserActionInitialization.hh"
#include "RunAction.h"
#include "EventAction.h"
#include "SteppingAction.h"
#include "TrackingAction.h"

class ActionInitialization : public G4VUserActionInitialization {
public:
    ActionInitialization();
    virtual ~ActionInitialization();

    virtual void Build() const;
    virtual void BuildForMaster() const;
};

#endif