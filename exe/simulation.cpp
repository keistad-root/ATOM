#include "Randomize.hh"
#include "G4RunManager.hh"
#include "QBBC.hh"

#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4SingleParticleSource.hh"
#include "G4GeneralParticleSource.hh"

#include "DetectorConstruction.h"
#include "ActionInitialization.h"
#include "AnalysisManager.h"

#include "CppConfigFile.h"
#include "cppargs.h"
#include "cppUnit.h"
#include "cppTimer.h"

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Draw plots for analysis data");
	parser.add_argument("--config").add_minor_argument("-c").help("Config file for controling simulation").set_default("").add_finish();
	parser.parse_args();
	return parser;
}

void DetectorConstruct(DetectorConstruction* detectorConstructor, const CppConfigDictionary& config) {
	double airPressure = Quantity(config.find("air_pressure")).getNum("bar");
	detectorConstructor->SetWorld(airPressure);

	double hallDiameter = Quantity(config.find("hall_diameter")).getNum("mm");
	std::string standType = config.find("stand_type");
	detectorConstructor->SetStand(standType, hallDiameter);

	double shieldWidth = Quantity(config.find("shield_width")).getNum("mm");
	if ( !(shieldWidth < 0.001) ) {
		detectorConstructor->SetShield(shieldWidth);
	}

	std::string alpideType = config.find("alpide_type");
	detectorConstructor->SetALPIDE(alpideType);

	double distance = Quantity(config.find("distance")).getNum("mm");
	detectorConstructor->Construct(standType, distance);
}

void SetParticleSource(G4SingleParticleSource* particleGun, const CppConfigDictionary& config) {
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	std::string particleType = config.find("particle_type");
	G4ParticleDefinition* particleDefinition = particleTable->FindParticle(particleType);
	particleGun->SetParticleDefinition(particleDefinition); // /gps/particle alpha
	particleGun->GetPosDist()->SetPosDisType("Plane"); // /gps/pos/type Plane
	particleGun->GetPosDist()->SetPosDisShape("Circle"); // /gps/pos/shape
	particleGun->GetPosDist()->SetRadius(2.5 * mm); // /gps/pos/radius 2.5 * mm
	particleGun->GetPosDist()->SetPosRot1(G4ThreeVector(0., 0., 1.)); // /gps/pos/rot1 0. 0. 1.
	particleGun->GetPosDist()->SetPosRot2(G4ThreeVector(1., 0., 0.)); // /gps/pos/rot2 1. 0. 0.
	particleGun->GetAngDist()->SetAngDistType("iso"); // /gps/ang/type iso
	particleGun->GetEneDist()->SetEnergyDisType("Mono");
	particleGun->GetEneDist()->ApplyEnergyWeight(false); // /gps/ene/type Mono
	double distance = Quantity(config.find("distance")).getNum("mm");
	particleGun->GetPosDist()->SetCentreCoords(G4ThreeVector(0., distance, 0.)); // /gps/pos/centre 0. 2. 0.
}

int main(int argc, char** argv) {
	TTimer timer;
	// Get config file using argument parser.
	ArgumentParser parser = set_parse(argc, argv);
	CppConfigFile config(parser.get_value<std::string>("config"));
	Quantity::setUserQuantity();

	// Set random engine
	CLHEP::RanecuEngine* RandomEngine = new CLHEP::RanecuEngine;
	G4Random::setTheEngine(RandomEngine);

	// Define Run Manager
	G4RunManager* runManager = new G4RunManager;

	// Analysis Manager
	AnalysisManager* analysisManager = new AnalysisManager(config.getConfig("Analysis").find("output_file"));

	// Geometry construction
	DetectorConstruction* detectorConstructor = new DetectorConstruction();
	DetectorConstruct(detectorConstructor, config.getConfig("Geometry"));
	runManager->SetUserInitialization(detectorConstructor);

	// Set physics
	G4VModularPhysicsList* QBBCList = new QBBC(0);
	G4VUserPhysicsList* physicsList = QBBCList;
	physicsList->SetVerboseLevel(0);
	runManager->SetUserInitialization(physicsList);

	// Set initial action 
	G4VUserActionInitialization* actionInitialator = new ActionInitialization();
	runManager->SetUserInitialization(actionInitialator);

	// Set Particle Source
	G4GeneralParticleSourceData* GPSData = G4GeneralParticleSourceData::Instance();
	G4SingleParticleSource* particleGun = GPSData->GetCurrentSource();
	SetParticleSource(particleGun, config.getConfig("Source"));

	runManager->Initialize();

	std::ifstream distFile(config.getConfig("Energy").find("alpha_energy_distribution"));
	std::string line;
	getline(distFile, line);
	while ( getline(distFile, line) ) {
		std::stringstream sstr(line, ',');
		std::string energyStr;
		std::string intensityStr;
		std::getline(sstr, energyStr, ',');
		std::getline(sstr, intensityStr, ',');
		std::getline(sstr, intensityStr, ',');
		if ( intensityStr == "" ) {
			continue;
		}
		double energy = stoi(energyStr) * 0.001;
		int intensity = static_cast<int>(round(stod(intensityStr) * 2000));
		particleGun->GetEneDist()->SetMonoEnergy(energy * MeV);
		runManager->BeamOn(intensity);
	}
	analysisManager->closeBook();

	delete analysisManager;
	delete runManager;
	delete RandomEngine;
	timer.EndProgram();
	return 0;
}