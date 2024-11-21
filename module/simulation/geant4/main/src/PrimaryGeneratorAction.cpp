#define __PRIMARYGENERATORACTION_HEADER__
#include "PrimaryGeneratorAction.h"

PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(), fParticleGun(0) {
	G4cout << "\033[1;36m" << "Primary Generator Action" << "\033[0m" << " is armed" << G4endl;
	// G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	// G4String particleName;

	fParticleGun = new G4GeneralParticleSource();
	// fParticleGun->SetParticlePosition(G4ThreeVector(0., +14.0 * mm, 0.));
	// fParticleGun->SetParticleDefinition(particleTable->FindParticle(particleName = "alpha")); 
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
	G4cout << "\033[1;36m" << "Primary Generator Action" << "\033[0m" << " is terminated" << G4endl;
	delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
	fParticleGun->GeneratePrimaryVertex(anEvent);
}

const G4GeneralParticleSource* PrimaryGeneratorAction::GetParticleGun() const {
	return fParticleGun;
}