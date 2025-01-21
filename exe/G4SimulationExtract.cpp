#include "TGeantExtract.h"

#include "cppargs.h"
#include "CppConfigFile.h"

#include<csv.h>

const std::string CONFIG_PATH = "/home/ychoi/ATOM/config/g4simulation/g4extract.conf";
const std::string INFORMATION_PATH = "/home/ychoi/ATOM/config/g4simulation/g4information.csv";

CppConfigFile setEnvironment(const ArgumentParser& parser) {
	CppConfigFile config(CONFIG_PATH);
	io::CSVReader<5> infoCSV(INFORMATION_PATH);
	infoCSV.read_header(io::ignore_extra_column, "TAG", "SIMULATION_FILE", "PRIMARY_FILE", "INCIDENT_FILE", "SECONDARY_FILE");
	std::string tags, simulationFile, primaryFile, incidentFile, secondaryFile;
	while ( infoCSV.read_row(tags, simulationFile, primaryFile, incidentFile, secondaryFile) ) {
		if ( parser.get_value<std::string>("tag") == tags ) {
			config.modifyConfig("File").addDictionary("input_file", simulationFile);
			config.modifyConfig("File").addDictionary("primary_output_file", primaryFile);
			config.modifyConfig("File").addDictionary("incident_output_file", incidentFile);
			config.modifyConfig("File").addDictionary("secondary_output_file", secondaryFile);
		}
	}
	std::filesystem::path primaryOutputPath = config.getConfig("File").find("primary_output_file");
	if ( !std::filesystem::exists(primaryOutputPath.parent_path()) ) {
		std::filesystem::create_directories(primaryOutputPath.parent_path());
	}
	std::filesystem::path incidentOutputPath = config.getConfig("File").find("incident_output_file");
	if ( !std::filesystem::exists(incidentOutputPath.parent_path()) ) {
		std::filesystem::create_directories(incidentOutputPath.parent_path());
	}
	std::filesystem::path secondaryOutputPath = config.getConfig("File").find("secondary_output_file");
	if ( !std::filesystem::exists(secondaryOutputPath.parent_path()) ) {
		std::filesystem::create_directories(secondaryOutputPath.parent_path());
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

	TGeantExtract extract(config.getConfig("File"));
	extract.initTrackTree();
	extract.initIncidentTree();
	extract.initPrimaryAnalysisTree();
	extract.initIncidentAnalysisTree();
	extract.initSecondaryAnalysisTree();
	extract.extractTrack();


	return 0;
}