#ifndef __TPRIMARYGENERATORACTION__
#define __TPRIMARYGENERATORACTION__

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "TAnalysisManager.h"

class G4GeneralParticleSource;

class TPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
	TPrimaryGeneratorAction();
	~TPrimaryGeneratorAction() override;
private:
	G4GeneralParticleSource* fParticleGun;
public:
	void GeneratePrimaries(G4Event*) override;

	const G4GeneralParticleSource* GetParticleGun() const;
};

#endif