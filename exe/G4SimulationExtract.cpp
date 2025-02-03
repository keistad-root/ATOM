#include "TGeantExtract.h"

#include "cppargs.h"
#include "CppConfigFile.h"

#include<csv.h>

const std::string CONFIG_PATH = "/home/ychoi/ATOM/config/g4simulation/g4extract.conf";
const std::string INFORMATION_PATH = "/home/ychoi/ATOM/config/g4simulation/g4information.csv";

CppConfigFile setEnvironment(const ArgumentParser& parser) {
	CppConfigFile config(CONFIG_PATH);
	io::CSVReader<6> infoCSV(INFORMATION_PATH);
	infoCSV.read_header(io::ignore_extra_column, "TAG", "SIMULATION_FILE", "PRIMARY_FILE", "INCIDENT_FILE", "SECONDARY_FILE", "DIVIDED_NUM");
	std::string tags, simulationFile, primaryFile, incidentFile, secondaryFile, dividedNum;
	while ( infoCSV.read_row(tags, simulationFile, primaryFile, incidentFile, secondaryFile, dividedNum) ) {
		if ( parser.get_value<std::string>("tag") == tags ) {
			config.modifyConfig("File").addDictionary("INPUT_FILE", simulationFile);
			config.modifyConfig("File").addDictionary("PRIMARY_OUTPUT_FILE", primaryFile);
			config.modifyConfig("File").addDictionary("INCIDENT_OUTPUT_FILE", incidentFile);
			config.modifyConfig("File").addDictionary("DIVIDED_NUM", dividedNum);
		}
	}
	std::filesystem::path primaryOutputPath = config.getConfig("File").find("PRIMARY_OUTPUT_FILE");
	if ( !std::filesystem::exists(primaryOutputPath.parent_path()) ) {
		std::filesystem::create_directories(primaryOutputPath.parent_path());
	}
	std::filesystem::path incidentOutputPath = config.getConfig("File").find("INCIDENT_OUTPUT_FILE");
	if ( !std::filesystem::exists(incidentOutputPath.parent_path()) ) {
		std::filesystem::create_directories(incidentOutputPath.parent_path());
	}

	return config;
}

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Extract the information from Geant4 simulation");
	parser.add_argument("tag").help("Tag for the file").set_default("default").add_finish();
	parser.parse_args();
	return parser;
}

int main(int argc, char** argv) {
	CppConfigFile config = setEnvironment(set_parse(argc, argv));

	TGeantExtract extract(config);
	extract.openOutputFile();
	extract.extractTrack();


	return 0;
}