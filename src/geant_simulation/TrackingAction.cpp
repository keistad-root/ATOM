#include "TrackingAction.h"

TrackingAction::TrackingAction() : G4UserTrackingAction() {
    G4cout << "\033[1;36m" << "Tracking Action" << "\033[0m" << " is armed" << G4endl;
}
TrackingAction::~TrackingAction() {
    G4cout << "\033[1;36m" << "Tracking Action" << "\033[0m" << " is terminated" << G4endl;
}

void TrackingAction::PreUserTrackingAction(const G4Track* track) {
    AnalysisManager* analysisManager = AnalysisManager::Instance();
    analysisManager->RecordingTrackStart(track);
}

void TrackingAction::PostUserTrackingAction(const G4Track* track) {
    AnalysisManager* analysisManager = AnalysisManager::Instance();
    analysisManager->RecordingTrackEnd(track);
}