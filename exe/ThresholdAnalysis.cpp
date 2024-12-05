#include<iostream>
#include<string>

#include "TThresholdAnalyser.h"
#include "CppConfigFile.h"
#include "cppargs.h"
#include<csv.h>
// #include "TCSV.h"

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Draw threshold information of an ALPIDE");
	parser.add_argument("config").help("Config File").set_default("default").add_finish();
	parser.add_argument("--csv").help("Threshold value stored in this CSV file").set_default("").add_finish();
	parser.add_argument("--exp").help("Get the informations from CSV").set_default("999999").type("int").add_finish();
	parser.parse_args();
	return parser;
}

int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);

	std::string configPath = parser.get_value<std::string>("config");
	CppConfigFile config(configPath);

	std::string csvPath = parser.get_value<std::string>("csv");
	io::CSVReader<6> csv(csvPath);
	csv.read_header(io::ignore_extra_column, "Number", "RawPath", "ConfigPath", "DataPath", "OutputPath", "GraphPath");
	int expNum;
	std::string rawPath, cPath, dataPath, outputPath, graphPath;

	int eNum = parser.get_value<int>("exp");
	while ( csv.read_row(expNum, rawPath, cPath, dataPath, outputPath, graphPath) ) {
		if ( expNum == eNum ) {
			config.modifyConfig("File").addDictionary("dat_file", rawPath);
			config.modifyConfig("File").addDictionary("cfg_file", cPath);
			config.modifyConfig("File").addDictionary("output_file", dataPath);
			break;
		}
	}

	TThresholdAnalyser analyser(config);
	analyser.getThreshold();
	analyser.saveThresholdData();
	return 0;
}