#include "SteppingAction.h"

SteppingAction::SteppingAction(EventAction* eventAction) : G4UserSteppingAction() {
    G4cout  << "\033[1;36m" << "Stepping Action" << "\033[0m" << " is armed" << G4endl;
}

SteppingAction::~SteppingAction() {
    G4cout  << "\033[1;36m" << "Stepping Action"  << "\033[0m" << " is terminated" << G4endl;
}

void SteppingAction::UserSteppingAction(const G4Step* step) {
    AnalysisManager* analysisManager = AnalysisManager::Instance();
    analysisManager->RecordingStep(step);
}