#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>  // Requires C++17

#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TH2.h"
#include "TStyle.h"

#include "TCollimator.h"
#include "cppargs.h"


ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Draw plots for analysis data");
	parser.add_argument("config").help("Config file").set_default("default").add_finish();
	parser.parse_args();
	return parser;
}


int main(int argc, char** argv) {

	ArgumentParser parser = set_parse(argc, argv);

	CppConfigFile* config = new CppConfigFile(parser.get_value<std::string>("config"));

	TCollimator* collimator = new TCollimator(config);

	collimator->getComponent();
	// collimator->saveComponent();

	// collimator->getBox();
	// collimator->drawBox();
	collimator->getHole();
	collimator->drawHole();
	collimator->getRuler();
	collimator->drawRuler();
	collimator->getArea();
	collimator->drawRow(775);
	// collimator->drawTotal();

	// JPEGTool tool("Data/photo/L20phi7.jpeg");
	// tool.getComponent();
	// tool.drawRow(700);
	// // tool.drawDerRow(700);
	// tool.drawDiffRow(700);
	// tool.drawDiffColumn(700);
	// tool.drawBox();
	// tool.getBoxArea();
	// tool.drawHole();
	// tool.getHoleArea();

	return 0;
}