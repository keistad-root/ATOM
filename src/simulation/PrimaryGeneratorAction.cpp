#include "PrimaryGeneratorAction.h"

PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(), fParticleGun(0) {
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;

    fParticleGun = new G4GeneralParticleSource();
    fParticleGun->SetParticlePosition(G4ThreeVector(0., +14.0 * mm, 0.));
    fParticleGun->SetParticleDefinition(particleTable->FindParticle(particleName = "alpha")); 
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    fParticleGun->GeneratePrimaryVertex(anEvent);
}

const G4GeneralParticleSource* PrimaryGeneratorAction::GetParticleGun() const {
    return fParticleGun;
}