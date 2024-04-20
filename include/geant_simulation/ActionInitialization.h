#ifndef __ACTIONINITIALIZATION__
#define __ACTIONINITIALIZATION__

#ifdef __ACTIONINITIALIZATION_HEADER__
#include "G4ios.hh"
#include "RunAction.h"
#include "EventAction.h"
#include "SteppingAction.h"
#include "TrackingAction.h"
#include "PrimaryGeneratorAction.h"
#endif

#include "G4VUserActionInitialization.hh"


class ActionInitialization : public G4VUserActionInitialization {
public:
	ActionInitialization();
	virtual ~ActionInitialization();

	virtual void Build() const;
	virtual void BuildForMaster() const;
};

#endif