#include "TSteppingAction.h"

TSteppingAction::TSteppingAction(TEventAction* eventAction) : G4UserSteppingAction(), fEventAction(eventAction) { }

void TSteppingAction::UserSteppingAction(const G4Step* step) {
	TAnalysisManager* analysisManager = TAnalysisManager::Instance();
	analysisManager->doStepPhase(step);
}

G4bool TSteppingAction::isOriginOutALPIDE(const G4Track* track) {
	G4bool isOut = true;
	if ( track->GetOriginTouchableHandle()->GetVolume()->GetLogicalVolume() == mALPIDEMetalLogical ) {
		isOut = false;
	} else if ( track->GetOriginTouchableHandle()->GetVolume()->GetLogicalVolume() == mALPIDEEpitaxialLogical ) {
		isOut = false;
	} else if ( track->GetOriginTouchableHandle()->GetVolume()->GetLogicalVolume() == mALPIDESubstrateLogical ) {
		isOut = false;
	} else {
		isOut = true;
	}
	return isOut;
}