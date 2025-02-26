#include <filesystem>
#include <csv.h>
#include "TFile.h"

#include "TGeantPlot.h"
#include "cppargs.h"
#include "CppConfigFile.h"

CppConfigFile setEnvironment(const ArgumentParser& parser) {
	CppConfigFile config("/home/ychoi/ATOM/build/config/GEANT4_PLOT.conf");
	io::CSVReader<3> csv("/home/ychoi/ATOM/build/config/GEANT4_INFORMATION.csv");
	csv.read_header(io::ignore_extra_column, "TAG", "PLOT_FILE", "PLOT_DICTIONARY");

	std::string tags, input_file, output_directory;

	std::string tag = parser.get_value<std::string>("tag");
	while ( csv.read_row(tags, input_file, output_directory) ) {
		if ( tag == tags ) {
			config.modifyConfig("File").addDictionary("input_file", input_file);
			config.modifyConfig("File").addDictionary("output_directory", output_directory);
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

	TGeantPlot plot;
	std::filesystem::create_directories(config.getConfig("File").find("output_directory"));
	plot.readInputFile(config.getConfig("File").find("input_file"));
	plot.getHistorams();
	plot.setOutputDirectory(config.getConfig("File").find("output_directory"));
	plot.saveHistorams(config.getConfigList());
	plot.getEntries();

	return 0;
}