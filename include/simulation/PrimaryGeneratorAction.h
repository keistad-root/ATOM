#ifndef __PRIMARYGENERATORACTION__
#define __PRIMARYGENERATORACTION__

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
// #include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
// #include "Randomize.hh"
#include "G4GeneralParticleSource.hh"

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
private:
    G4GeneralParticleSource* fParticleGun;
public:
    PrimaryGeneratorAction();
    virtual ~PrimaryGeneratorAction();

    void GeneratePrimaries(G4Event* anEvent);

    const G4GeneralParticleSource* GetParticleGun() const;
};

#endif