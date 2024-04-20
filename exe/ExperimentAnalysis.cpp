#include <iostream>
#include <fstream>
#include <filesystem>

#include "TEvent.h"
#include "cppconfig.h"
#include "TClusterAnalyser.h"
#include "TClusterShapeAnalyser.h"
#include "TExperimentData.h"
#include "cppargs.h"
#include "TClusterDivideData.h"

#include "TCanvas.h"
#include "TFile.h"

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Draw plots for analysis data");
	parser.add_argument("config").help("Config file").set_default("default").add_finish();
	parser.add_argument("--mV").set_default("false").help("Set the dac values in real scale. (1V_ADC = 7.03mV, 1I_ADC = 40nA)").add_finish();
	parser.parse_args();
	return parser;
}

std::filesystem::path getOutputPath(const Configurable& sharedConfig, const Configurable& privateConfig) {
	std::filesystem::path outputPath = std::filesystem::absolute(sharedConfig.find("output_path"));
	if ( privateConfig.hasKey("subdirectory") ) {
		outputPath /= privateConfig.find("subdirectory");
	}
	outputPath /= privateConfig.find("filename");
	if ( sharedConfig.hasKey("extension") || privateConfig.hasKey("extension") ) {
		std::string extension = privateConfig.hasKey("extension") ? privateConfig.find("extension") : sharedConfig.find("extension");
		outputPath.replace_extension(extension);
	} else {
		outputPath.replace_extension("root");
	}
	return outputPath;
}

std::string getPlotTitles(const Configurable& privateConfig) {
	std::string plotTitles;
	if ( privateConfig.hasKey("title") ) {
		plotTitles = privateConfig.find("title");
	} else {
		plotTitles = "";
	}
	if ( privateConfig.hasKey("x_title") ) {
		plotTitles += ";" + privateConfig.find("x_title");
	}
	if ( privateConfig.hasKey("y_title") ) {
		plotTitles += ";" + privateConfig.find("y_title");
	}
	if ( privateConfig.hasKey("z_title") ) {
		plotTitles += ";" + privateConfig.find("z_title");
	}
	return plotTitles;
}

Configurable mapConfig(const Configuration* inputConfig, std::string_view key) {
	Configurable sharedConfig = inputConfig->getConfig("SharedProperty").value();
	Configurable privateConfig = inputConfig->getConfig(key).value();

	Configurable config("Map");
	// Set path for hitmap plot
	std::filesystem::path outputPath = getOutputPath(sharedConfig, privateConfig);
	config.addDictionary("file_path", std::string(outputPath));

	std::string plotTitles = getPlotTitles(privateConfig);
	config.addDictionary("plot_titles", plotTitles);

	if ( privateConfig.hasKey("options") ) {
		config.addDictionary("options", privateConfig.find("options"));
	}

	return config;
}

struct ControlBool {
	bool masking = false;
	bool hitmap = false;
	bool maskedHitmap = false;
	bool noisePixelHitmap = false;
	bool clusterization = false;
	bool maskedClusterization = false;
	bool noisePixelClusterization = false;
	bool clustermap = false;
	bool maskedClustermap = false;
	bool noisePixelClustermap = false;
	bool clustersize = false;
	bool maskedClustersize = false;
	bool noisePixelClustersize = false;
	ControlBool(const Configuration* config) {
		hitmap = config->getConfig("Hitmap").has_value();
		maskedHitmap = config->getConfig("MaskedHitmap").has_value();
		noisePixelHitmap = config->getConfig("NoisePixelHitmap").has_value();
		masking = maskedHitmap || noisePixelHitmap || config->getConfig("masking").has_value() ? true : false;
		clustermap = config->getConfig("Clustermap").has_value();
		maskedClustermap = config->getConfig("MaskedClustermap").has_value();
		noisePixelClustermap = config->getConfig("NoisePixelClustermap").has_value();
		clustersize = config->getConfig("Clustersize").has_value();
		maskedClustersize = config->getConfig("MaskedClustersize").has_value();
		noisePixelClustersize = config->getConfig("NoisePixelClustersize").has_value();
		clusterization = clustermap || clustersize ? true : false;
		maskedClusterization = maskedClustermap || maskedClustersize ? true : false;
		noisePixelClusterization = noisePixelClustermap || noisePixelClustersize ? true : false;
	}
};

int main(int argc, char** argv) {
	// Get parser
	ArgumentParser parser = set_parse(argc, argv);
	// Get Analysis config file
	Configuration* config = new Configuration(parser.get_value<std::string>("config"));
	// Open input ROOT file
	Configurable fileConfig = config->getConfig("File").has_value() ? config->getConfig("File").value() : Configurable("");
	ControlBool controlBool(config);
	std::string inputFileTitle = fileConfig.hasKey("input_file") ? fileConfig.find("input_file") : "";
	try {
		if ( inputFileTitle == "" ) throw inputFileTitle;
	} catch ( std::string title ) {
		std::cerr << "\033[1;31mFETAL\033[1;0m: No input File" << std::endl;
		exit(1);
	}
	TFile* inputFile = new TFile(static_cast<TString>(inputFileTitle), "READ");

	// Allocate experiment data class
	TExperimentData* expData = new TExperimentData();

	// make output directory
	// std::filesystem::create_directory(config->getConfig("File").value().find("output_path"));

	// Basic analyser for experiment data with ROOT
	TAnalyser analyser(inputFile, expData);
	// Getting events from ROOT and store it
	analyser.storeEvents();
	// Appoint the output path
	// analyser.setSavePath(config->getConfig("File").value().find("output_path"));
	// Making experiment config legend
	analyser.setExpSettingLegend(config->getConfig("ExperimentSetting").value());

	// Hitmap for raw data)
	if ( controlBool.hitmap ) {
		analyser.saveHitmap(mapConfig(config, "Hitmap"));
	}
	// Masking with cut. The cut is get from analysis config file.
	if ( controlBool.masking ) {
		if ( config->getConfig("Masking").has_value() ) {
			analyser.doMasking(stoi(config->getConfig("Masking").value().find("cut")));
		} else {
			analyser.doMasking(100);
		}
	}
	// Hitmap for masked data
	if ( controlBool.maskedHitmap ) {
		analyser.saveMaskedHitmap(mapConfig(config, "MaskedHitmap"));
	}
	// Pixel hitmap for masking pixels
	if ( controlBool.noisePixelHitmap ) {
		analyser.saveNoisePixelmap(mapConfig(config, "NoisePixelHitmap"));
	}

	if ( controlBool.clusterization ) {
		TClusterization clusterization(expData->getEvents());
		clusterization.clusterize();
		expData->setClusters(std::move(clusterization.getClusters()));
	}
	if ( controlBool.maskedClusterization ) {
		TClusterization maskedClusterization(expData->getMaskedEvents());
		maskedClusterization.clusterize();
		expData->setMaskedClusters(std::move(maskedClusterization.getClusters()));
	}
	if ( controlBool.noisePixelClusterization ) {
		TClusterization noisePixelClusterization(expData->getNoiseEvents());
		noisePixelClusterization.clusterize();
		expData->setNoiseClusters(std::move(noisePixelClusterization.getClusters()));
	}

	TClusterAnalyser clusterAnalyser(analyser);
	if ( controlBool.clustermap ) {
		clusterAnalyser.saveClustermap(mapConfig(config, "Clustermap"));
	}
	if ( controlBool.maskedClustermap ) {
		clusterAnalyser.saveMaskedClustermap(mapConfig(config, "MaskedClustermap"));
	}
	if ( controlBool.noisePixelClustermap ) {
		clusterAnalyser.saveNoisePixelClustermap(mapConfig(config, "NoisePixelClustermap"));
	}
	if ( controlBool.clustersize ) {
		clusterAnalyser.saveClustersize(mapConfig(config, "Clustersize"));
	}
	if ( controlBool.maskedClustersize ) {
		clusterAnalyser.saveMaskedClustersize(mapConfig(config, "MaskedClustersize"));
	}
	if ( controlBool.noisePixelClustersize ) {
		clusterAnalyser.saveNoisePixelClustersize(mapConfig(config, "NoisePixelClustersize"));
	}

	// clusterAnalyser.saveHitmapByClustersize(config->getConfig("Clustermap").value());

	// TClusterShapeAnalyser clusterShapeAnalyser(clusterAnalyser);
	// // clusterShapeAnalyser.doShaping(expData->getNoiseClusters(), stoi(config->getConfig("Shape").value().find("min_cut")), stoi(config->getConfig("Shape").value().find("max_cut")), "masked");
	// clusterShapeAnalyser.doShaping(stoi(config->getConfig("Shape").value().find("min_cut")), stoi(config->getConfig("Shape").value().find("max_cut")));
	// clusterShapeAnalyser.saveIndividualShapes();
	// clusterShapeAnalyser.saveSameSizeShapes();
	// clusterShapeAnalyser.saveTotalShapes();
	// clusterShapeAnalyser.saveSameSizeShapeEntry();
	// clusterShapeAnalyser.saveTotalShapeEntry();

	// if ( inputFile != nullptr && inputFile->IsDestructed() ) {
	// 	inputFile->Close();
	// 	delete inputFile;
	// 	inputFile = nullptr;
	// }
	// if ( expData != nullptr && expData->IsDestructed() ) {
	// 	delete expData;
	// 	expData = nullptr;
	// }

	// delete config;

	return 0;
}