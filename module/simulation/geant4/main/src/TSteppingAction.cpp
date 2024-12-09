#include "TSteppingAction.h"

TSteppingAction::TSteppingAction(TEventAction* eventAction) : G4UserSteppingAction(), fEventAction(eventAction) { }

void TSteppingAction::UserSteppingAction(const G4Step* step) {
	TAnalysisManager* analysisManager = TAnalysisManager::Instance();
	if ( mALPIDEMetalLogical == nullptr ) {
		const TDetectorConstruction* detectorConstruction = static_cast<const TDetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
		mALPIDEMetalLogical = detectorConstruction->getALPIDEMetal();
		mALPIDEEpitaxialLogical = detectorConstruction->getALPIDEEpitaxial();
		mALPIDESubstrateLogical = detectorConstruction->getALPIDESubstrate();
	}
	G4Track* track = step->GetTrack();
	G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
	if ( !(track->GetOriginTouchableHandle()->GetVolume()->GetLogicalVolume() == mALPIDEMetalLogical) && !(track->GetOriginTouchableHandle()->GetVolume()->GetLogicalVolume() == mALPIDEEpitaxialLogical) && !(track->GetOriginTouchableHandle()->GetVolume()->GetLogicalVolume() == mALPIDESubstrateLogical) ) {
		if ( volume == mALPIDEMetalLogical ) {
			G4double depositEnergy = step->GetTotalEnergyDeposit();
			fEventAction->addDepositMetal(depositEnergy / MeV);
			fEventAction->passMetal();
			if ( step->IsFirstStepInVolume() ) {
				analysisManager->recordIncident(step);
			}
		} else if ( volume == mALPIDEEpitaxialLogical ) {
			G4double depositEnergy = step->GetTotalEnergyDeposit();
			fEventAction->addDepositEpitaxial(depositEnergy / MeV);
			fEventAction->passEpitaxial();
		} else if ( volume == mALPIDESubstrateLogical ) {
			G4double depositEnergy = step->GetTotalEnergyDeposit();
			fEventAction->addDepositSubstrate(depositEnergy / MeV);
			fEventAction->passSubstrate();
		} else {
			inALPIDE = false;
		}
	}
}
