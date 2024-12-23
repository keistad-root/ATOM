#include "TTrackingAction.h"

void TTrackingAction::PreUserTrackingAction(const G4Track* track) {
	TAnalysisManager* analysisManager = TAnalysisManager::Instance();
	analysisManager->doPreTracking(track);
}

void TTrackingAction::PostUserTrackingAction(const G4Track* track) {
	TAnalysisManager* analysisManager = TAnalysisManager::Instance();
	analysisManager->doPostTracking(track);


	// if ( track->GetParentID() > 0 ) {
	// 	const G4LogicalVolume* originVolume = track->GetLogicalVolumeAtVertex();
	// 	if ( originVolume != mALPIDEMetalLogical && originVolume != mALPIDEEpitaxialLogical && originVolume != mALPIDESubstrateLogical ) {
	// 		analysisManager->recordSecondary(track);
	// 	}
	// }
}

void TTrackingAction::getLogicalVolumes() {
	const TDetectorConstruction* detectorConstruction = static_cast<const TDetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
	mALPIDEMetalLogical = detectorConstruction->getALPIDEMetalLogical();
	mALPIDEEpitaxialLogical = detectorConstruction->getALPIDEEpitaxialLogical();
	mALPIDESubstrateLogical = detectorConstruction->getALPIDESubstrateLogical();
	mCollimatorLogical = detectorConstruction->getCollimatorLogical();
	mScreenLogical = detectorConstruction->getScreenLogical();
	mWorldLogical = detectorConstruction->getWorldLogical();
}