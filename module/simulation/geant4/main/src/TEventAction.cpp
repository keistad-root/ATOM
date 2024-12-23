#include "TEventAction.h"

TEventAction::TEventAction(TRunAction* runAction) : G4UserEventAction(), fRunAction(runAction) { }

void TEventAction::BeginOfEventAction(const G4Event* event) {
	TAnalysisManager* analysisManager = TAnalysisManager::Instance();
	analysisManager->doBeginOfEvent(event);
}

void TEventAction::EndOfEventAction(const G4Event* event) {
	TAnalysisManager* analysisManager = TAnalysisManager::Instance();
	analysisManager->doEndOfEvent(event);

	// analysisManager->recordALPIDE(mMetal, mEpitaxial, mSubstrate, mDepositMetal, mDepositEpitaxial, mDepositSubstrate);

	// mMetal = false;
	// mEpitaxial = false;
	// mSubstrate = false;

	// mDepositMetal = 0.;
	// mDepositEpitaxial = 0.;
	// mDepositSubstrate = 0.;

}