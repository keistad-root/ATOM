#include <filesystem>
#include <csv.h>
#include "TFile.h"

#include "TGeantAnalysis.h"
#include "cppargs.h"
#include "CppConfigFile.h"

CppConfigFile setEnvironment(const int tag) {
	CppConfigFile config("/home/ychoi/ATOM/config/g4simulation/plot/g4plot.conf");
	io::CSVReader<5> csv("/home/ychoi/ATOM/config/g4simulation/plot/g4plot.csv");
	csv.read_header(io::ignore_extra_column, "num", "input_file", "output_directory", "incident_file", "collimator_length");

	int num;
	std::string input_file, output_directory, incident_file;
	double collimatorLength;

	while ( csv.read_row(num, input_file, output_directory, incident_file, collimatorLength) ) {
		if ( tag == num ) {
			config.modifyConfig("File").addDictionary("input_file", input_file);
			config.modifyConfig("File").addDictionary("output_directory", output_directory);
			config.modifyConfig("File").addDictionary("incident_file", incident_file);

			if ( config.hasConfig("SourceZ") ) {
				std::string collimatorLengthStr = "10 " + std::to_string(collimatorLength + 2) + " " + std::to_string(collimatorLength + 3);
				config.modifyConfig("SourceZ").addDictionary("collimator_length", collimatorLengthStr);
			}
		}
	}

	return config;
}

int main(int argc, char** argv) {
	CppConfigFile config = setEnvironment(atoi(argv[1]));

	TGeantAnalysis* analysis = new TGeantAnalysis(config);
	analysis->setHistograms();
	analysis->readTrack();
	// analysis->getTrackInformations();
	// analysis->getIncidentInformations();
	analysis->saveHistograms();


	return 0;
}