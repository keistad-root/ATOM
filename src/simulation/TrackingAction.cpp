#include "TrackingAction.h"

TrackingAction::TrackingAction() : G4UserTrackingAction() {}
TrackingAction::~TrackingAction() {}

void TrackingAction::PreUserTrackingAction(const G4Track* track) {
    AnalysisManager* analysisManager = AnalysisManager::Instance();
    // analysismanager->FillSource(track);
}

void TrackingAction::PostUserTrackingAction(const G4Track* track) {
    AnalysisManager* analysisManager = AnalysisManager::Instance();
    // analysisManager->EndOfTrack(track);
}