#include "G4UIExecutive.hh"
#include "Randomize.hh"
#include "G4RunManager.hh"
#include "QBBC.hh"
#include "G4VisExecutive.hh"

#include "DetectorConstruction.h"
#include "ActionInitialization.h"
#include "AnalysisManager.h"

int main(int argc, char** argv) {
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    // Set random engine
    CLHEP::RanecuEngine* RandomEngine = new CLHEP::RanecuEngine;
    G4Random::setTheEngine(RandomEngine);

    G4RunManager* runManager = new G4RunManager;
    // Geometry construction
    runManager->SetUserInitialization(new DetectorConstruction());

    // Set physics
    G4VModularPhysicsList* physicsList = new QBBC;
    physicsList->SetVerboseLevel(1);
    runManager->SetUserInitialization(physicsList);

    // Set initial action 
    runManager->SetUserInitialization(new ActionInitialization());

    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    if (!ui) { 
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
        delete ui;
    } else { 
        // interactive mode
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }
    
    return 0;
}