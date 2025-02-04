#include <csv.h>
#include <string>

#include "cppargs.h"
#include "CppConfigFile.h"

#include "TDataPlotter.h"

const std::string configPath = "/home/ychoi/ATOM/config/experiment/analysis_plot.conf";
const std::string csvPath = "/home/ychoi/ATOM/config/experiment/experiment_information.csv";
const std::string dataPath = "/mnt/homes/ychoi/entry_test/DATA/experiment.csv";

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Drawing experiment plots");
	parser.add_argument("TAG").help("Tag for config").add_finish();
	parser.parse_args();
	return parser;
}

CppConfigFile setEnvironment(const ArgumentParser& parser) {
	const std::string tag = parser.get_value<std::string>("TAG");

	CppConfigFile config(configPath);

	io::CSVReader<3> csv(csvPath);
	csv.read_header(io::ignore_extra_column, "TAG", "MASKED_FILE", "OUTPUT_DIRECTORY");
	std::string csvTag, maskedFile, outputDirectory;
	while ( csv.read_row(csvTag, maskedFile, outputDirectory) ) {
		if ( csvTag == tag ) {
			config.modifyConfig("CONFIG").addDictionary("MASKED_FILE", maskedFile);
			config.modifyConfig("CONFIG").addDictionary("OUTPUT_DIRECTORY", outputDirectory);
			break;
		}
	}

	return config;
}

void addEntry2CSV(const std::string tag, const std::array<int, 60> entry) {
	io::CSVReader<6> infoCsv(csvPath);
	infoCsv.read_header(io::ignore_extra_column, "TAG", "LENGTH", "PHI", "COLLIMATOR_LENGTH", "COLLIMATOR_AREA", "EVENT_NUM");
}

int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);

	CppConfigFile config = setEnvironment(parser);

	TDataPlotter plot(config);
	plot.openInputFile();
	plot.InitPlot();
	plot.FillHitInfo();
	plot.FillClusterInfo();
	plot.FillShapeInfo();
	plot.savePlots();
	// if ( config.hasConfig("TotalShape") ) plot->saveTotalShape();
	// if ( config.hasConfig("Top10Shape") ) plot->saveTop10Shape();
	// if ( config.hasConfig("ClusterSizeWithTime") ) plot->saveClusterSizeWithTime();
}