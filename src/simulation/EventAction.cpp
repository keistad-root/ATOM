#include "EventAction.h"

EventAction::EventAction(RunAction* runAction) : G4UserEventAction() {}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event* event) {}

void EventAction::EndOfEventAction(const G4Event* event) {
    AnalysisManager* analysisManager = AnalysisManager::Instance();
    // analysisManager->AutoSave();
}