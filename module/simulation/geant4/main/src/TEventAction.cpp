#include "TEventAction.h"

TEventAction::TEventAction(TRunAction* runAction) : G4UserEventAction(), fRunAction(runAction) { }

void TEventAction::BeginOfEventAction(const G4Event*) { }

void TEventAction::EndOfEventAction(const G4Event*) { }