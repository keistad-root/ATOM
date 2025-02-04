#include <csv.h>
#include <string>

#include "cppargs.h"
#include "CppConfigFile.h"

#include "TDataAnalyser.h"

const std::string configPath = "/home/ychoi/ATOM/config/experiment/mask.conf";
const std::string csvPath = "/home/ychoi/ATOM/config/experiment/experiment_information.csv";

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Get pixels to be masked in experiment");
	parser.add_argument("TAG").help("Tag for config").add_finish();
	parser.parse_args();
	return parser;
}

CppConfigFile setEnvironment(const ArgumentParser& parser) {
	const std::string tag = parser.get_value<std::string>("TAG");

	CppConfigFile config(configPath);

	io::CSVReader<4> csv(csvPath);
	csv.read_header(io::ignore_extra_column, "TAG", "RAW_FILE", "MASK_PIXEL_FILE", "HIT_CUT");
	std::string csvTag, rawFile, maskPixelFile, hitCut;
	while ( csv.read_row(csvTag, rawFile, maskPixelFile, hitCut) ) {
		if ( csvTag == tag ) {
			config.modifyConfig("CONFIG").addDictionary("RAW_FILE", rawFile);
			config.modifyConfig("CONFIG").addDictionary("MASK_PIXEL_FILE", maskPixelFile);
			config.modifyConfig("CONFIG").addDictionary("HIT_CUT", hitCut);
			break;
		}
	}

	return config;
}

int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);

	CppConfigFile config = setEnvironment(parser);

	TDataAnalyser analyse(config);
	analyse.openInputFile();
	analyse.extractEvent();
	analyse.extractHotPixel();
}

