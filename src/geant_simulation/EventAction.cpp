#include "EventAction.h"

EventAction::EventAction(RunAction* runAction) : G4UserEventAction() {
    G4cout << "\033[1;36m" << "Event Action" << "\033[0m" << " is armed" << G4endl;
}

EventAction::~EventAction() {
    G4cout << "\033[1;36m" << "Event Action" << "\033[0m" << " is terminated" << G4endl;
}

void EventAction::BeginOfEventAction(const G4Event* event) {
    if (event->GetEventID() == 0) {
        G4GeneralParticleSourceData* GPSData = G4GeneralParticleSourceData::Instance();
        G4SingleParticleSource* particleGun = GPSData->GetCurrentSource();
        G4float energy = particleGun->GetParticleEnergy();
        G4cout << " of Events with " << "\033[1;32m" << energy << "\033[0m" << " MeV Energy." << G4endl;
    }
    AnalysisManager* analysisManager = AnalysisManager::Instance();
    iStartTrack = analysisManager->getTrackID();
    G4RunManager * runManager;
}

void EventAction::EndOfEventAction(const G4Event* event) {
    AnalysisManager* analysisManager = AnalysisManager::Instance();
    analysisManager->getEventTuple().nTracks = analysisManager->getTrackID() - iStartTrack;
    analysisManager->RecordingEvent(event);
}