#include <csv.h>
#include "CppConfigFile.h"

#include "TDataPlotter.h"


CppConfigFile setEnvironment(std::string exp) {
	CppConfigFile config("/home/ychoi/ATOM/config/experiment/analysis_plot.conf");
	io::CSVReader<3> csv("/home/ychoi/ATOM/config/experiment/analysis_plot.csv");

	csv.read_header(io::ignore_extra_column, "tag", "input_file", "output_directory");
	std::string tag;
	std::string inputFile;
	std::string outputDirectory;

	while ( csv.read_row(tag, inputFile, outputDirectory) ) {
		if ( tag == exp ) {
			config.modifyConfig("File").addDictionary("input_file", inputFile);
			config.modifyConfig("File").addDictionary("output_directory", outputDirectory);
		}
	}

	return config;
}

int main(int argc, char** argv) {
	CppConfigFile config = setEnvironment(argv[1]);

	TDataPlotter* plot = new TDataPlotter(&config);
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