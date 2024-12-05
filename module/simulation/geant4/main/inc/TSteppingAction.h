#ifndef __TSTEPPINGACTION__
#define __TSTEPPINGACTION__

#include "G4UserSteppingAction.hh"

#include "TEventAction.h"

class TSteppingAction : public G4UserSteppingAction {
public:
	TSteppingAction(TEventAction* eventAction);
	~TSteppingAction() override = default;
private:
	TEventAction* fEventAction = nullptr;

public:
	void UserSteppingAction(const G4Step*) override;

};

#endif