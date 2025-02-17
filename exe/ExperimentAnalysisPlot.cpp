#include <csv.h>
#include <string>

#include "cppargs.h"
#include "CppConfigFile.h"

#include "TDataPlotter.h"

const std::string configPath = "/home/ychoi/ATOM/config/experiment/analysis_plot.conf";
const std::string csvPath = "/home/ychoi/ATOM/config/experiment/experiment_information.csv";
const std::string dataPath = "/home/ychoi/ATOM/Data/experiment.csv";

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Drawing experiment plots");
	parser.add_argument("TAG").help("Tag for config").add_finish();
	parser.parse_args();
	return parser;
}

CppConfigFile setEnvironment(const ArgumentParser& parser) {
	const std::string tag = parser.get_value<std::string>("TAG");

	CppConfigFile config(configPath);

	io::CSVReader<4> csv(csvPath);
	csv.read_header(io::ignore_extra_column, "TAG", "MASKED_FILE", "OUTPUT_DIRECTORY", "CENTER");
	std::string csvTag, maskedFile, outputDirectory, center;
	while ( csv.read_row(csvTag, maskedFile, outputDirectory, center) ) {
		if ( csvTag == tag ) {
			config.modifyConfig("CONFIG").addDictionary("MASKED_FILE", maskedFile);
			config.modifyConfig("CONFIG").addDictionary("OUTPUT_DIRECTORY", outputDirectory);
			config.modifyConfig("CLUSTERSIZE_REGION").addDictionary("center", center);
			break;
		}
	}

	return config;
}

void addEntry2CSV(const std::string tag, const std::array<int, 60> entry) {
	io::CSVReader<6> infoCsv(csvPath);
	infoCsv.read_header(io::ignore_extra_column, "TAG", "LENGTH", "PHI", "COLLIMATOR_LENGTH", "COLLIMATOR_AREA", "MINUTE");
	std::string infoTag;
	int infoLength, infoPhi;
	double COLLIMATOR_LENGTH, COLLIMATOR_AREA;
	int minute;

	while ( infoCsv.read_row(infoTag, infoLength, infoPhi, COLLIMATOR_LENGTH, COLLIMATOR_AREA, minute) ) {
		if ( infoTag == tag ) {
			break;
		}
	}

	io::CSVReader<1> csv(dataPath);
	csv.read_header(io::ignore_extra_column, "TAG");
	std::string csvTag;
	bool isExist = false;
	while ( csv.read_row(csvTag) ) {
		if ( csvTag == tag ) {
			isExist = true;
			break;
		}
	}

	if ( !isExist ) {
		std::ofstream file(dataPath, std::ios::app);
		file << tag << ", " << infoLength << ", " << infoPhi << ", " << COLLIMATOR_LENGTH << ", " << COLLIMATOR_AREA << ", " << minute;
		for ( int i = 0; i < 60; i++ ) {
			file << ", " << (entry[i] * 10. / minute);
		}
		file << std::endl;
		file.close();
	}
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
	addEntry2CSV(parser.get_value<std::string>("TAG"), plot.getEntry());
	plot.savePlots();
	// if ( config.hasConfig("TotalShape") ) plot->saveTotalShape();
	// if ( config.hasConfig("Top10Shape") ) plot->saveTop10Shape();
	// if ( config.hasConfig("ClusterSizeWithTime") ) plot->saveClusterSizeWithTime();
}