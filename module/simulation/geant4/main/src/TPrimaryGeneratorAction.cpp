#include "TPrimaryGeneratorAction.h"

TPrimaryGeneratorAction::TPrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction() {
	fParticleGun = new G4ParticleGun(1);

	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* particle = particleTable->FindParticle("alpha");
	fParticleGun->SetParticleDefinition(particle);
}

TPrimaryGeneratorAction::TPrimaryGeneratorAction(const CppConfigFile& config) : G4VUserPrimaryGeneratorAction(), mConfig(config) {
	fParticleGun = new G4ParticleGun(1);

	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* particle = particleTable->FindParticle("alpha");
	fParticleGun->SetParticleDefinition(particle);
}

TPrimaryGeneratorAction::~TPrimaryGeneratorAction() {
	delete fParticleGun;
}

void TPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
	// Select a random energy based on intensity
	if ( !mAlphaEnergy.empty() ) {
		// Create cumulative distribution
		std::vector<double> cumulativeDistribution;
		double sum = 0.0;
		for ( const auto& entry : mAlphaEnergy ) {
			sum += entry.intensity;
			cumulativeDistribution.push_back(sum);
		}

		// Normalize cumulative distribution
		for ( auto& value : cumulativeDistribution ) {
			value /= sum;
		}

		// Generate a random number and find corresponding energy
		double randomValue = G4UniformRand();
		auto it = std::lower_bound(cumulativeDistribution.begin(), cumulativeDistribution.end(), randomValue);
		size_t index = std::distance(cumulativeDistribution.begin(), it);
		double selectedEnergy = G4RandGauss::shoot(mAlphaEnergy[index].energy, mAlphaEnergy[index].energyUncertainty);

		fParticleGun->SetParticleEnergy(selectedEnergy * keV);
	} else {
		// Default energy if mAlphaEnergy is empty
		fParticleGun->SetParticleEnergy(5.4 * MeV);
	}

	G4double collimatorLength = stod(mConfig.getConfig("Environment").find("collimator_length")) * mm;
	G4double centerPositionZ = collimatorLength + 2.5 * mm;
	G4double centerPosition[3] = {0, 0, centerPositionZ};
	G4double radius = stod(mConfig.getConfig("Environment").find("source_radius")) * mm;

	G4double r = radius * std::sqrt(G4UniformRand());
	G4double theta = 2 * M_PI * G4UniformRand();
	G4double randomX = centerPosition[0] + r * std::cos(theta);
	G4double randomY = centerPosition[1] + r * std::sin(theta);
	G4double randomZ = centerPosition[2];

	fParticleGun->SetParticlePosition(G4ThreeVector(randomX, randomY, randomZ));

	G4double cosTheta = 2 * G4UniformRand() - 1.0;
	G4double sinTheta = std::sqrt(1 - cosTheta * cosTheta);
	G4double phi = 2 * M_PI * G4UniformRand();
	G4double pX = sinTheta * std::cos(phi);
	G4double pY = sinTheta * std::sin(phi);
	G4double pZ = cosTheta;

	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(pX, pY, pZ));

	fParticleGun->GeneratePrimaryVertex(anEvent);
}

const G4ParticleGun* TPrimaryGeneratorAction::GetParticleGun() const {
	return fParticleGun;
}

void TPrimaryGeneratorAction::setAlphaEnergy() {
	std::string csvPath = mConfig.getConfig("File").find("alpha_energy");
	io::CSVReader<4> in(csvPath);
	in.read_header(io::ignore_extra_column, "energy", "energy_uncertainty", "intensity", "intensity_uncertainty");
	double energy, energyUncertainty, intensity, intensityUncertainty;
	while ( in.read_row(energy, energyUncertainty, intensity, intensityUncertainty) ) {
		mAlphaEnergy.push_back({energy, energyUncertainty, intensity, intensityUncertainty});
	}
}