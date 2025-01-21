#include <filesystem>
#include <csv.h>
#include "TFile.h"

#include "TGeantAnalysis.h"
#include "cppargs.h"
#include "CppConfigFile.h"

CppConfigFile setEnvironment(const ArgumentParser& parser) {
	CppConfigFile config("/home/ychoi/ATOM/config/g4simulation/g4analysis.conf");
	io::CSVReader<4> infoCsv("/home/ychoi/ATOM/config/g4simulation/g4information.csv");
	infoCsv.read_header(io::ignore_extra_column, "TAG", "INCIDENT_FILE", "PRIMARY_FILE", "PLOT_FILE");
	std::string tags, input_incident_file, input_primary_file, output_file;
	while ( infoCsv.read_row(tags, input_incident_file, input_primary_file, output_file) ) {
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
	io::CSVReader<4> infoCsv("/home/ychoi/ATOM/config/g4simulation/g4information.csv");
	infoCsv.read_header(io::ignore_extra_column, "TAG", "LENGTH", "PHI", "WIDTH");

	std::string infoTag;
	int infoLength, infoPhi;
	double infoWidth;

	while ( infoCsv.read_row(infoTag, infoLength, infoPhi, infoWidth) ) {
		if ( infoTag == tag ) {
			break;
		}
	}

	io::CSVReader<8> csv("/home/ychoi/ATOM/Data/SL1F2AX.csv");
	csv.read_header(io::ignore_extra_column, "Tag", "Length", "Phi", "Area", "EEM", "EAM", "EAE", "Double");

	std::string csvTag;
	int csvLength, csvPhi;
	double csvArea;
	int csvEEM, csvEAM, csvEAE, csvDouble;

	bool isExist = false;
	while ( csv.read_row(csvTag, csvLength, csvPhi, csvArea, csvEEM, csvEAM, csvEAE, csvDouble) ) {
		if ( csvTag == tag ) {
			isExist = true;
			break;
		}
	}

	if ( !isExist ) {
		std::ofstream file("/home/ychoi/ATOM/Data/SL1F2AX.csv", std::ios::app);
		file << tag << ", " << infoLength << ", " << infoPhi << ", " << infoWidth << ", " << entry[0] << ", " << entry[1] - 2 * entry[3] << ", " << entry[2] - 2 * entry[3] << ", " << entry[3] << std::endl;
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