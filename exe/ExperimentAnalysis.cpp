#include <iostream>
#include <fstream>
#include <filesystem>

#include "CppConfigFile.h"
#include "TAnalyser.h"
#include "TExperimentData.h"
#include "cppargs.h"
#include "TClusterDivideData.h"
#include "TClusterization.h"

#include "TCanvas.h"
#include "TFile.h"

#include "TDataAnalyser.h"

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Draw experiment information of an ALPIDE");
	parser.add_argument("config").help("Config File").set_default("default").add_finish();
	parser.parse_args();
	return parser;
}

int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);
	std::string configPath = parser.get_value<std::string>("config");
	CppConfigFile* config = new CppConfigFile(configPath);

	TDataAnalyser* analyser = new TDataAnalyser(config);
	analyser->extractEvent();
	analyser->excludeHotPixel();
	analyser->extractCluster();
	analyser->saveEvent();
	analyser->saveCluster();
	analyser->extractHotPixel();

	delete analyser;




	// TAnalyser* mAnalyser = new TAnalyser(controller.getConfig());
	// mAnalyser->originMasking();
	// mAnalyser->storeEvents();
	// mAnalyser->setExpSettingLegend();
	// mAnalyser->getHitmaps();
	// mAnalyser->clusterize();
	// mAnalyser->getClusterinfos();


	// controller.openInputFile();
	// controller.setExpDataSet();
	// controller.doBasicAnalysis();
	// controller.drawHitmap();
	// controller.clusterization();
	// controller.drawClustermapAndClustersize();
	// controller.doDivideBySize();
	// controller.drawClusterShapeInfos();

	// clusterAnalyser.saveHitmapByClustersize(*config->getConfig("Clustermap"));


// 	delete mAnalyser;
// mAnalyser = nullptr;

	return 0;
}