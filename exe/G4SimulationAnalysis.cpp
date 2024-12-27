#include <filesystem>
#include <csv.h>
#include "TFile.h"

#include "TGeantAnalysis.h"
#include "cppargs.h"
#include "CppConfigFile.h"

CppConfigFile setEnvironment(const ArgumentParser& parser) {
	CppConfigFile config("/home/ychoi/ATOM/config/g4simulation/g4analysis.conf");
	io::CSVReader<3> csv("/home/ychoi/ATOM/config/g4simulation/g4analysis.csv");
	csv.read_header(io::ignore_extra_column, "num", "input_file", "output_file");

	std::string tags, input_file, output_file;
	double collimatorLength;

	std::string tag = parser.get_value<std::string>("tag");
	while ( csv.read_row(tags, input_file, output_file) ) {
		if ( tag == tags ) {
			config.modifyConfig("File").addDictionary("input_file", input_file);
			config.modifyConfig("File").addDictionary("output_file", output_file);
		}
	}

	return config;
}

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Plot the Geant4 simulation data");
	parser.add_argument("tag").help("Tag for the file").set_default("").add_finish();
	parser.parse_args();
	return parser;
}

int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);
	CppConfigFile config = setEnvironment(parser);

	TGeantAnalysis plot;
	plot.readInputFile(config.getConfig("File").find("input_file"));
	plot.setHistograms(config.getConfigList());
	plot.readTree();
	plot.fillHistograms();
	plot.saveFile(config.getConfig("File").find("output_file"));

	return 0;
}