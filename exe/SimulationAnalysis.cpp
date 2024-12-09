#include <filesystem>

#include "TFile.h"

#include "TGeantAnalysis.h"
#include "cppargs.h"
#include "CppConfigFile.h"

ArgumentParser setParse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Geant4 Simulation");
	parser.add_argument("config").help("Config File").set_default("default").add_finish();
	parser.parse_args();

	return parser;
}

int main(int argc, char** argv) {

	ArgumentParser parser = setParse(argc, argv);
	std::string configPath = parser.get_value<std::string>("config");
	CppConfigFile config(configPath);

	TGeantAnalysis* analysis = new TGeantAnalysis(config);

	analysis->readInit();
	analysis->readALPIDE();
	analysis->readIncident();

	return 0;
}