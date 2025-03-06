#ifndef __TPRIMARYGENERATORACTION__
#define __TPRIMARYGENERATORACTION__

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "TAnalysisManager.h"

#include<filesystem>
#include<csv.h>

#include "CppConfigFile.h"

class G4GeneralParticleSource;

class TPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
	TPrimaryGeneratorAction();
	TPrimaryGeneratorAction(const CppConfigFile& config);
	~TPrimaryGeneratorAction() override;
private:
	G4ParticleGun* fParticleGun;

	CppConfigFile mConfig;
	// G4GeneralParticleSource* fParticleGun;

	struct EnergyEntry {
		double energy;
		double energyUncertainty;
		double intensity;
		double intensityUncertainty;
	};

	std::vector<EnergyEntry> mEnergy;
public:
	void GeneratePrimaries(G4Event*) override;

	// const G4GeneralParticleSource* GetParticleGun() const;
	const G4ParticleGun* GetParticleGun() const;

	void setEnergyDistribution(std::string_view particle);
};

#endif