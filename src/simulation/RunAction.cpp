#include "RunAction.h"

RunAction::RunAction() : G4UserRunAction() {
    fAnalysisManager = AnalysisManager::Instance();
}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*) {
    // fAnalysisManager->Book();
    G4RunManager::GetRunManager()->SetRandomNumberStore(false);
}

void RunAction::EndOfRunAction(const G4Run* run) {
    G4int nofEvents = run->GetNumberOfEvent();
    if (nofEvents == 0) return;

    const PrimaryGeneratorAction* generatorAction = static_cast<const PrimaryGeneratorAction*> (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());

    G4String runCondition;
    if (generatorAction) {
        const G4GeneralParticleSource* particleGun = generatorAction->GetParticleGun();
        runCondition += particleGun->GetParticleDefinition()->GetParticleName();
        runCondition += " of ";
        G4double particleEnergy = particleGun->GetParticleEnergy();
        runCondition += G4BestUnit(particleEnergy, "Energy");
    }

    // fAnalysisManager->Write();
    if (IsMaster()) {
        G4cout << G4endl << "--------------------End of Global Run-----------------------";
    } else {
        G4cout << G4endl << "---------------------End of local Run-----------------------";
    }
    G4cout << G4endl << " the run consists of " << nofEvents << " " << runCondition << G4endl;
    G4cout << "------------------------------------------------------------" << G4endl;
}