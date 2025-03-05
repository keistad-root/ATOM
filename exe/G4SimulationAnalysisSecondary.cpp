#include <filesystem>
#include <csv.h>
#include "TFile.h"

#include "TGeantAnalysisSecondary.h"
#include "cppargs.h"
#include "CppConfigFile.h"

const std::string CONFIG_PATH = "/home/ychoi/ATOM/build/config/GEANT4_ANALYSIS_SECONDARY.conf";
const std::string INFORMATION_PATH = "/home/ychoi/ATOM/build/config/GEANT4_INFORMATION.csv";
const std::string DATA_PATH = "/home/ychoi/ATOM/build/Data/g4data.csv";

const double EVENT_10MIN = 2580000;

CppConfigFile setEnvironment(const ArgumentParser& parser) {
	CppConfigFile config(CONFIG_PATH);
	io::CSVReader<4> infoCSV(INFORMATION_PATH);
	infoCSV.read_header(io::ignore_extra_column, "TAG", "SIMULATION_FILE", "PLOT_FILE", "DIVIDED_NUM");
	std::string tags, inputFile, outputFile;
	std::string dividedNum;
	while ( infoCSV.read_row(tags, inputFile, outputFile, dividedNum) ) {
		if ( parser.get_value<std::string>("tag") == tags ) {
			config.modifyConfig("FILE").addDictionary("SIMULATION_FILE", inputFile);
			std::filesystem::path outputPath = outputFile;
			std::string outputPathStr = std::string(outputPath.stem()) + "_Secondary" + std::string(outputPath.extension());
			config.modifyConfig("FILE").addDictionary("PLOT_FILE", outputPathStr);
			config.modifyConfig("FILE").addDictionary("DIVIDED_NUM", dividedNum);
		}
	}
	std::filesystem::path outputPath = config.getConfig("FILE").find("PLOT_FILE");
	if ( !std::filesystem::exists(outputPath.parent_path()) ) {
		std::filesystem::create_directories(outputPath.parent_path());
	}

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

	TGeantAnalysisSecondary plot(config);
	int nFile = stoi(config.getConfig("FILE").find("DIVIDED_NUM"));
	plot.setHistograms();
	for ( int i = 0; i < nFile; i++ ) {
		plot.openFile();
		plot.readTree();
	}
	plot.saveFile();

	return 0;
}