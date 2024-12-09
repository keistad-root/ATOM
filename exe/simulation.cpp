#include "G4RunManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"

#include "QBBC.hh"
#include "Randomize.hh"

#include "TDetectorConstruction.h"
#include "TActionInitialization.h"
#include "TAnalysisManager.h"

#include "CppConfigFile.h"
#include "cppargs.h"

ArgumentParser setParse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Geant4 Simulation");
	parser.add_argument("config").help("Config File").set_default("default").add_finish();
	parser.add_argument("--mac").help("Macro File").set_default("init_vis.mac").add_finish();
	parser.parse_args();

	return parser;
}


int main(int argc, char** argv) {
	ArgumentParser parser = setParse(argc, argv);
	std::string configPath = parser.get_value<std::string>("config");
	CppConfigFile config(configPath);

	// Set UI
	G4UIExecutive* ui = nullptr;
	std::string macroFile = parser.get_value<std::string>("mac");
	if ( macroFile == "init_vis.mac" ) {
		ui = new G4UIExecutive(argc, argv);
	}

	// Set random engine
	CLHEP::RanecuEngine* RandomEngine = new CLHEP::RanecuEngine;
	G4Random::setTheEngine(RandomEngine);

	// Define Run Manager
	G4RunManager* runManager = new G4RunManager;

	// Define Analysis Manager
	std::string outputName = config.getConfig("File").find("output_file");
	TAnalysisManager* analysisManager = new TAnalysisManager();
	analysisManager->open(outputName);

	// Set geometry
	runManager->SetUserInitialization(new TDetectorConstruction(config.getConfig("Environment")));

	// Set physics
	G4VModularPhysicsList* QBBCList = new QBBC;
	G4VUserPhysicsList* physicsList = QBBCList;
	runManager->SetVerboseLevel(0);
	runManager->SetUserInitialization(physicsList);

	// Set initial action
	runManager->SetUserInitialization(new TActionInitialization());

	// Set visualization
	G4VisManager* visManager = new G4VisExecutive(argc, argv);
	visManager->Initialize();

	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	if ( !ui ) {
		// batch mode
		G4String command = "/control/execute ";
		UImanager->ApplyCommand(command + macroFile);
	} else {
		// interactive mode
		UImanager->ApplyCommand("/control/execute init_vis.mac");
		ui->SessionStart();
		delete ui;
	}

	analysisManager->close();

	delete visManager;
	delete runManager;

	return 0;

}