
#include "cppargs.h"
#include "CppConfigFile.h"

#include "TDataPlotter.h"

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Draw experiment information of an ALPIDE");
	parser.add_argument("config").help("Config File").set_default("default").add_finish();
	parser.parse_args();
	return parser;
}

int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);

	std::string configPath = parser.get_value<std::string>("config");
	CppConfigFile config(configPath);

	TDataPlotter* plot = new TDataPlotter(config);
	plot->InitPlot();
	plot->FillHitInfo();
	plot->FillClusterInfo();
	plot->FillShapeInfo();
	plot->savePlots();
	if ( config.hasConfig("TotalShape") ) plot->saveTotalShape();
	if ( config.hasConfig("Top10Shape") ) plot->saveTop10Shape();
	if ( config.hasConfig("ClusterSizeWithTime") ) plot->saveClusterSizeWithTime();
	delete plot;
}