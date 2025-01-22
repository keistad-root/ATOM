#include "cppargs.h"
#include "CppConfigFile.h"

#include "TGeantComparison.h"

const std::string CONFIG_FILE = "/home/ychoi/ATOM/config/g4simulation/g4comparison.conf";
const std::string LIST_FILE = "/home/ychoi/ATOM/config/g4simulation/g4comparison_list.csv";
const std::string DATA_FILE = "/home/ychoi/ATOM/Data/g4data.csv";

ArgumentParser set_parser(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Plot the Geant4 simulation data");
	parser.add_argument("tag").help("Tag for the file").set_default("").add_finish();
	parser.parse_args();
	return parser;
}

CppConfigFile setEnvironment(const ArgumentParser& parser) {
	CppConfigFile config = CppConfigFile(CONFIG_FILE);
	io::CSVReader<5> fileListCSV(LIST_FILE);
	fileListCSV.read_header(io::ignore_extra_column, "tag", "length", "width", "area", "file");
	std::string tag, length, width, area, file;
	while ( fileListCSV.read_row(tag, length, width, area, file) ) {
		config.modifyConfig("FileList").addDictionary(tag, file);
	}
	return config;
}

int main(int argc, char** argv) {
	ArgumentParser parser = set_parser(argc, argv);
	CppConfigFile config = setEnvironment(parser);

	TGeantComparison comparison = TGeantComparison(config);
	comparison.getComparedPlot();
	comparison.getPlotNormalized();
	// comparison.getDividePlot();
}