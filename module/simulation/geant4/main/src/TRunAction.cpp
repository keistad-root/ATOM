#include "TRunAction.h"

void TRunAction::BeginOfRunAction(const G4Run*) {
	G4cout << "RunAction::BeginOfRunAction()" << G4endl;
}

void TRunAction::EndOfRunAction(const G4Run*) {
	G4cout << "RunAction::EndOfRunAction()" << G4endl;
}

