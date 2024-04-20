#ifndef __PRIMARYGENERATORACTION__
#define __PRIMARYGENERATORACTION__

#ifdef __PRIMARYGENERATORACTION_HEADER__
#include "G4ios.hh"
#include "G4GeneralParticleSource.hh"
#endif

#include "G4VUserPrimaryGeneratorAction.hh"

class G4GeneralParticleSource;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
private:
	G4GeneralParticleSource* fParticleGun;
public:
	PrimaryGeneratorAction();
	virtual ~PrimaryGeneratorAction();

	void setParticleGun();

	void GeneratePrimaries(G4Event* anEvent);

	const G4GeneralParticleSource* GetParticleGun() const;
};

#endif