#include "TGeantExtract.h"

#include "cppargs.h"
#include "CppConfigFile.h"

#include<csv.h>

const std::string CONFIG_PATH = "/home/ychoi/ATOM/config/g4simulation/g4extract.conf";
const std::string FILE_LIST_CSV = "/home/ychoi/ATOM/config/g4simulation/g4extract_list.csv";

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Extract the information from Geant4 simulation");
	parser.add_argument("tag").help("Tag for the file").set_default("default").add_finish();
	parser.parse_args();
	return parser;
}

CppConfigFile setEnvironment(const ArgumentParser& parser) {
	CppConfigFile config(CONFIG_PATH);

	std::string parserTag = parser.get_value<std::string>("tag");

	io::CSVReader<4> in(FILE_LIST_CSV);

	std::string tag, inputName, primaryName, incidentName;

	while ( in.read_row(tag, inputName, primaryName, incidentName) ) {
		if ( tag == parserTag ) {
			config.modifyConfig("File").addDictionary("input_file", inputName);
			config.modifyConfig("File").addDictionary("primary_output_file", primaryName);
			config.modifyConfig("File").addDictionary("incident_output_file", incidentName);
		}
	}
	return config;
}

int main(int argc, char** argv) {
	CppConfigFile config = setEnvironment(set_parse(argc, argv));

	TGeantExtract extract(config.getConfig("File"));
	extract.initTrackTree();
	extract.initIncidentTree();
	extract.initPrimaryAnalysisTree();
	extract.initIncidentAnalysisTree();
	extract.extractTrack();


	return 0;
}