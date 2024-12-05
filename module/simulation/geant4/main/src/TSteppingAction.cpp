#include "TSteppingAction.h"

TSteppingAction::TSteppingAction(TEventAction* eventAction) : G4UserSteppingAction(), fEventAction(eventAction) { }

void TSteppingAction::UserSteppingAction(const G4Step* step) { }
