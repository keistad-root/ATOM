#include "SteppingAction.h"

SteppingAction::SteppingAction(EventAction* eventAction) : G4UserSteppingAction(), fEventAction(eventAction) {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) {
    AnalysisManager* analysisManager = AnalysisManager::Instance();
    if (!(step->GetPreStepPoint()->GetPhysicalVolume())) return;
    if (!(step->GetPostStepPoint()->GetPhysicalVolume())) return;

    analysisManager->FillStepper(step);
    analysisManager->FillTrack(step);
}