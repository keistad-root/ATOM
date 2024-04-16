#include <iostream>
#include <fstream>
#include <filesystem>

#include "TEvent.h"
#include "cppconfig.h"
#include "TClusterAnalyser.h"
#include "TClusterShapeAnalyser.h"
#include "TExperimentData.h"
#include "cppargs.h"

#include "TCanvas.h"
#include "TFile.h"

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Draw plots for analysis data");
	parser.add_argument("config").help("Config file").set_default("default").add_finish();
	parser.add_argument("--mV").set_default("false").help("Set the dac values in real scale. (1V_ADC = 7.03mV, 1I_ADC = 40nA)").add_finish();
	parser.parse_args();
	return parser;
}

int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);
	Configuration* config = new Configuration(parser.get_value<std::string>("config"));

	TString inputFileTitle = config->getConfig("File").value().find("input_file");
	TFile* inputFile = new TFile(inputFileTitle, "READ");

	TExperimentData* expData = new TExperimentData();

	std::filesystem::create_directory(config->getConfig("File").value().find("output_path"));

	TAnalyser analyser(inputFile, expData);
	analyser.storeEvents();
	analyser.setSavePath(config->getConfig("File").value().find("output_path"));
	analyser.setExpSettingLegend(config->getConfig("ExperimentSetting").value());
	analyser.saveHitmap();
	analyser.doMasking(stoi(config->getConfig("Masking").value().find("cut")));
	analyser.saveMaskedHitmap();
	TClusterization clusterization(expData->getEvents());
	clusterization.clusterize();
	expData->setClusters(std::move(clusterization.getClusters()));
	TClusterization maskedClusterization(expData->getMaskedEvents());
	maskedClusterization.clusterize();
	expData->setMaskedClusters(std::move(maskedClusterization.getClusters()));

	TClusterAnalyser clusterAnalyser(analyser);
	clusterAnalyser.saveClustermap();
	clusterAnalyser.saveMaskedClustermap();
	clusterAnalyser.saveClustersize();
	clusterAnalyser.saveMaskedClustersize();

	TClusterShapeAnalyser clusterShapeAnalyser(clusterAnalyser);
	clusterShapeAnalyser.doShaping(stoi(config->getConfig("Shape").value().find("min_cut")), stoi(config->getConfig("Shape").value().find("max_cut")));
	// clusterShapeAnalyser.saveIndividualShapes();
	// clusterShapeAnalyser.saveSameSizeShapes();
	// clusterShapeAnalyser.saveTotalShapes();
	clusterShapeAnalyser.saveSameSizeShapeEntry();

	return 0;
}