#include "ExperimentAnalysis.hpp"

int main(int argc, char** argv) {
	ControlExperimentAnalysis controller(argc, argv);
	controller.setConfig();
	controller.openInputFile();
	controller.setExpDataSet();
	controller.doBasicAnalysis();
	controller.drawHitmap();
	controller.clusterization();
	controller.drawClustermapAndClustersize();
	// controller.doDivideBySize();
	// controller.drawClusterShapeInfos();

	// clusterAnalyser.saveHitmapByClustersize(*config->getConfig("Clustermap"));

	return 0;
}