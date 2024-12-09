#include "TEventAction.h"

TEventAction::TEventAction(TRunAction* runAction) : G4UserEventAction(), fRunAction(runAction) { }

void TEventAction::BeginOfEventAction(const G4Event* event) {

}

void TEventAction::EndOfEventAction(const G4Event*) {
	TAnalysisManager* analysisManager = TAnalysisManager::Instance();
	analysisManager->recordALPIDE(mMetal, mEpitaxial, mSubstrate, mDepositMetal, mDepositEpitaxial, mDepositSubstrate);

	mMetal = false;
	mEpitaxial = false;
	mSubstrate = false;

	mDepositMetal = 0.;
	mDepositEpitaxial = 0.;
	mDepositSubstrate = 0.;

}