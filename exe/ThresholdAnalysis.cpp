#include<iostream>
#include<string>

#include "TThresholdAnalyser.h"
#include "CppConfigFile.h"
#include "cppargs.h"
#include<csv.h>

const std::string CSV_PATH = "/home/ychoi/ATOM/Data/threshold.csv";

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Draw threshold information of an ALPIDE");
	parser.add_argument("config").help("Config File").set_default("default").add_finish();
	parser.add_argument("--exp").help("Get the informations from CSV").set_default("999999").type("int").add_finish();
	parser.parse_args();
	return parser;
}

CppConfigFile setEnvironment(const ArgumentParser& parser) {
	std::string configPath = parser.get_value<std::string>("config");
	CppConfigFile config(configPath);

	io::CSVReader<6> csv(CSV_PATH);
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

	return config;
}

int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);

	CppConfigFile config = setEnvironment(parser);

	TThresholdAnalyser analyser(config);
	analyser.getThreshold();
	analyser.saveThresholdData();
	return 0;
}