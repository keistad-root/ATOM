#include "G4RunManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"

#include "Randomize.hh"

#include "QBBC.hh"

#include "TPhysicsList.h"
#include "TDetectorConstruction.h"
#include "TActionInitialization.h"
#include "TAnalysisManager.h"

#include "CppConfigFile.h"
#include "cppargs.h"
#include <csv.h>

const std::string CONFIG_PATH = "/home/ychoi/ATOM/config/g4simulation/g4simulation.conf";
const std::string INFORMATION_PATH = "/home/ychoi/ATOM/config/g4simulation/g4information.csv";

CppConfigFile setEnvironment(const ArgumentParser& parser) {
	CppConfigFile config(CONFIG_PATH);
	io::CSVReader<7> infoCSV(INFORMATION_PATH);

	infoCSV.read_header(io::ignore_extra_column, "TAG", "SIMULATION_FILE", "COLLIMATOR_LENGTH", "COLLIMATOR_AREA", "AL_SHIELD", "DISTANCE_SOURCE_ALPIDE", "EVENT_NUM");

	std::string tags, simulationFile, collimatorLength, collimatorArea, alShield, distanceSourceALPIDE, nEvent;

	while ( infoCSV.read_row(tags, simulationFile, collimatorLength, collimatorArea, alShield, distanceSourceALPIDE, nEvent) ) {
		if ( tags == parser.get_value<std::string>("tag") ) {
			config.modifyConfig("File").addDictionary("output_file", simulationFile);
			config.modifyConfig("Environment").addDictionary("collimator_length", collimatorLength);
			config.modifyConfig("Environment").addDictionary("collimator_area", collimatorArea);
			config.modifyConfig("Environment").addDictionary("screen", alShield);
			double distanceALPIDECollimator = stod(distanceSourceALPIDE) - .5 * mm - stod(collimatorLength);
			config.modifyConfig("Environment").addDictionary("distance_alpide_and_collimator", std::to_string(distanceALPIDECollimator));
			config.modifyConfig("Environment").addDictionary("activity", nEvent);
		}
	}

	// io::CSVReader<7> csv("/home/ychoi/ATOM/config/g4simulation/g4simulation.csv");

	// csv.read_header(io::ignore_extra_column, "num", "output_file", "collimator_length", "collimator_area", "screen", "distance_alpide_and_collimator", "n_event");
	// int num;
	// std::string output_file;
	// std::string collimator_length;
	// std::string collimator_area;
	// std::string screen;
	// std::string distance_alpide_and_collimator;
	// std::string n_event;

	// while ( csv.read_row(num, output_file, collimator_length, collimator_area, screen, distance_alpide_and_collimator, n_event) ) {
	// 	if ( num == exp ) {
	// 		config.modifyConfig("File").addDictionary("output_file", output_file);
	// 		config.modifyConfig("Environment").addDictionary("collimator_length", collimator_length);
	// 		config.modifyConfig("Environment").addDictionary("collimator_area", collimator_area);
	// 		config.modifyConfig("Environment").addDictionary("screen", screen);
	// 		config.modifyConfig("Environment").addDictionary("distance_alpide_and_collimator", distance_alpide_and_collimator);
	// 		config.modifyConfig("Environment").addDictionary("activity", n_event);
	// 	}
	// }

	return config;
}

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Plot the Geant4 simulation data");
	parser.add_argument("tag").help("Tag for the file").set_default("").add_finish();
	parser.parse_args();
	return parser;
}

int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);
	CppConfigFile config = setEnvironment(parser);

	// Set random engine
	CLHEP::RanecuEngine* RandomEngine = new CLHEP::RanecuEngine;
	G4Random::setTheEngine(RandomEngine);

	// Define Run Manager
	G4RunManager* runManager = new G4RunManager;

	// Set geometry
	runManager->SetUserInitialization(new TDetectorConstruction(config.getConfig("Environment")));

	// Define Analysis Manager
	std::string outputName = config.getConfig("File").find("output_file");
	TAnalysisManager* analysisManager = new TAnalysisManager();
	analysisManager->open(outputName);

	// Set physics
	TPhysicsList* physicsList = new TPhysicsList;
	// G4VModularPhysicsList* physicsList = new QBBC;
	runManager->SetVerboseLevel(0);
	runManager->SetUserInitialization(physicsList);

	// Set initial action
	runManager->SetUserInitialization(new TActionInitialization(config));

	// Set visualization
	G4VisManager* visManager = new G4VisExecutive(argc, argv);
	visManager->Initialize();

	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	// G4UIExecutive* ui = new G4UIExecutive(argc, argv);
	// UImanager->ApplyCommand("/control/execute init_vis.mac");
	// ui->SessionStart();
	// delete ui;

	UImanager->ApplyCommand("/run/initialize");

	UImanager->ApplyCommand("/control/verbose 1");
	UImanager->ApplyCommand("/run/verbose 1");
	UImanager->ApplyCommand("/event/verbose 0");
	UImanager->ApplyCommand("/tracking/verbose 0");

	G4String activity = config.getConfig("Environment").find("activity");
	UImanager->ApplyCommand("/run/beamOn " + activity);
	analysisManager->close();

	delete visManager;
	delete runManager;

	return 0;

}