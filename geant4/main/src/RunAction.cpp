#define __RUNACTION_HEADER__
#include "RunAction.h"

RunAction::RunAction() : G4UserRunAction() {
	G4cout << "\033[1;36m" << "Run Action" << "\033[0m" << " is armed" << G4endl;
	fAnalysisManager = AnalysisManager::Instance();
}

RunAction::~RunAction() {
	G4cout << "\033[1;36m" << "Run Action" << "\033[0m" << " is terminated" << G4endl;
}

void RunAction::BeginOfRunAction(const G4Run* run) {
	mRunStart = clock();
	G4int nRun = run->GetNumberOfEventToBeProcessed();
	G4cout << "\033[1;32m" << "Run #" << run->GetRunID() << "\033[0m" << ">> " << "\033[1;32m" << nRun << "\033[0m";
}

void RunAction::EndOfRunAction(const G4Run* run) {
	AnalysisManager* analysisManager = AnalysisManager::Instance();
	analysisManager->RecordingRun(run);
	mRunFinish = clock();
	std::cout << "Time taken: " << static_cast<double>(mRunFinish - mRunStart) / CLOCKS_PER_SEC << "s" << std::endl;
}