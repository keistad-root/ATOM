#include <iostream>
#include <fstream>
#include <filesystem>

#include "TEvent.h"
#include "cppconfig.h"
#include "TClusterAnalyser.h"
#include "cppargs.h"

#include "TCanvas.h"
#include "TFile.h"

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Draw plots for analysis data");
	parser.add_argument("config").help("Config file").set_default("default").add_finish();
	parser.parse_args();
	return parser;
}

int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);
	Configuration* config = new Configuration(parser.get_value<std::string>("config"));
	std::filesystem::path outputPath = config->getConfig("File").value().find("output_path");
	std::filesystem::create_directory(outputPath);
	TClusterAnalyser* clusterAna = new TClusterAnalyser(config);

	clusterAna->openRootFile();
	clusterAna->storeRawEvents();
	clusterAna->refineData();
	clusterAna->savePlots();

	delete clusterAna;

	return 0;
}