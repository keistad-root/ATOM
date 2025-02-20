#include <filesystem>
#include <csv.h>
#include "TFile.h"

#include "TGeantAnalysis.h"
#include "cppargs.h"
#include "CppConfigFile.h"

const std::string CONFIG_PATH = "/home/ychoi/ATOM/config/g4simulation/g4analysis.conf";
const std::string INFORMATION_PATH = "/home/ychoi/ATOM/config/g4simulation/g4information.csv";
const std::string DATA_PATH = "/home/ychoi/ATOM/Data/g4data.csv";

const double EVENT_10MIN = 2580000;

CppConfigFile setEnvironment(const ArgumentParser& parser) {
	CppConfigFile config(CONFIG_PATH);
	io::CSVReader<4> infoCSV(INFORMATION_PATH);
	infoCSV.read_header(io::ignore_extra_column, "TAG", "INCIDENT_FILE", "PRIMARY_FILE", "PLOT_FILE");
	std::string tags, input_incident_file, input_primary_file, output_file;
	while ( infoCSV.read_row(tags, input_incident_file, input_primary_file, output_file) ) {
		if ( parser.get_value<std::string>("tag") == tags ) {
			config.modifyConfig("File").addDictionary("input_incident_file", input_incident_file);
			config.modifyConfig("File").addDictionary("input_primary_file", input_primary_file);
			config.modifyConfig("File").addDictionary("output_file", output_file);
		}
	}
	std::filesystem::path outputPath = config.getConfig("File").find("output_file");
	if ( !std::filesystem::exists(outputPath.parent_path()) ) {
		std::filesystem::create_directories(outputPath.parent_path());
	}

	return config;
}

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Plot the Geant4 simulation data");
	parser.add_argument("tag").help("Tag for the file").set_default("").add_finish();
	parser.parse_args();
	return parser;
}

void addEntry2CSV(const std::string tag, const std::array<int, 4> entry) {
	io::CSVReader<6> infoCsv("/home/ychoi/ATOM/config/g4simulation/g4information.csv");
	infoCsv.read_header(io::ignore_extra_column, "TAG", "LENGTH", "PHI", "COLLIMATOR_LENGTH", "COLLIMATOR_AREA", "EVENT_NUM");

	std::string infoTag;
	int infoLength, infoPhi;
	double COLLIMATOR_LENGTH, COLLIMATOR_AREA;
	int eventNum;

	while ( infoCsv.read_row(infoTag, infoLength, infoPhi, COLLIMATOR_LENGTH, COLLIMATOR_AREA, eventNum) ) {
		if ( infoTag == tag ) {
			break;
		}
	}

	io::CSVReader<1> csv(DATA_PATH);
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
		std::ofstream file(DATA_PATH, std::ios::app);
		double timeRatio = EVENT_10MIN / eventNum;
		double eem = entry[0] * timeRatio;
		double eemError = std::sqrt(eem) * timeRatio;
		double eam = (entry[1] - 2 * entry[3]) * timeRatio;
		double eamError = std::sqrt(eam) * timeRatio;
		double eae = (entry[2] - 2 * entry[3]) * timeRatio;
		double eaeError = std::sqrt(eae) * timeRatio;
		double doubleCluster = entry[3] * timeRatio;
		double doubleClusterError = std::sqrt(doubleCluster) * timeRatio;

		file << tag << ", " << infoLength << ", " << infoPhi << ", " << COLLIMATOR_LENGTH << ", " << COLLIMATOR_AREA << ", " << eventNum << ", ";
		file << eem << ", " << eemError << ", " << eam << ", " << eamError << ", " << eae << ", " << eaeError << ", " << doubleCluster << ", " << doubleClusterError << std::endl;
		file.close();
	}
}

int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);
	CppConfigFile config = setEnvironment(parser);

	TGeantAnalysis plot;
	plot.readIncidentFile(config.getConfig("File").find("input_incident_file"));
	plot.readPrimaryFile(config.getConfig("File").find("input_primary_file"));
	plot.setHistograms(config.getConfigList());
	plot.readTree();
	plot.saveFile(config.getConfig("File").find("output_file"));

	addEntry2CSV(parser.get_value<std::string>("tag"), plot.getEntry());

	return 0;
}