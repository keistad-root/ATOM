#include "G4UIExecutive.hh"
#include "G4RunManager.hh"
#include "G4VisExecutive.hh"
#include "G4UImanager.hh"

#include "QBBC.hh"

#include "TDetectorConstruction.h"
#include "TActionInitialization.h"

int main(int argc, char** argv) {
	// Set UI
	G4UIExecutive* ui = nullptr;
	if ( argc == 1 ) {
		ui = new G4UIExecutive(argc, argv);
	}

	// Set random engine
	CLHEP::RanecuEngine* RandomEngine = new CLHEP::RanecuEngine;
	G4Random::setTheEngine(RandomEngine);

	// Define Run Manager
	G4RunManager* runManager = new G4RunManager;

	// Set geometry
	runManager->SetUserInitialization(new TDetectorConstruction());

	// Set physics
	G4VModularPhysicsList* physicsList = new QBBC(0);
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
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command + fileName);
	} else {
		// interactive mode
		UImanager->ApplyCommand("/control/execute init_vis.mac");
		ui->SessionStart();
		delete ui;
	}

	delete visManager;
	delete runManager;

	return 0;

}