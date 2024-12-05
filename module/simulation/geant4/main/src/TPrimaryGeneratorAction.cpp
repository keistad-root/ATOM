#include "TPrimaryGeneratorAction.h"

TPrimaryGeneratorAction::TPrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction() {
	fParticleGun = new G4GeneralParticleSource();
}

TPrimaryGeneratorAction::~TPrimaryGeneratorAction() {
	delete fParticleGun;
}

void TPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
	fParticleGun->GeneratePrimaryVertex(anEvent);
}

const G4GeneralParticleSource* TPrimaryGeneratorAction::GetParticleGun() const {
	return fParticleGun;
}