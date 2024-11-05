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


int main(int argc, char** argv) {

	std::string directory = "Data/photo";

	TCollimator* collimator = new TCollimator("Data/photo/L1phi4.jpeg");

	// collimator->getComponent();
	// collimator->saveComponent("Data/");

	// collimator->getBox();
	// collimator->drawBox();
	collimator->getHole();
	collimator->drawHole();
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