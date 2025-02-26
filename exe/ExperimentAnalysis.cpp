#include<csv.h>
#include<string>

#include "cppargs.h"
#include "CppConfigFile.h"
#include "config.h"

#include "TDataAnalyser.h"

const std::string configPath = std::filesystem::path(SOURCE_DIR) / "build/config/EXPERIMENT_ANALYSIS.conf";
const std::string csvPath = std::filesystem::path(SOURCE_DIR) / "build/config/EXPERIMENT_INFORMATION.csv";

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Get masked file from experiment");
	parser.add_argument("TAG").help("Tag for config").add_finish();
	parser.parse_args();
	return parser;
}

CppConfigFile setEnvironment(const ArgumentParser& parser) {
	const std::string tag = parser.get_value<std::string>("TAG");

	CppConfigFile config(configPath);

	io::CSVReader<4> csv(csvPath);
	csv.read_header(io::ignore_extra_column, "TAG", "RAW_FILE", "MASKED_FILE", "MASK_PIXEL_FILE");
	std::string csvTag, rawFile, maskedFile, maskPixelFile;
	for ( int i = 0; csv.read_row(csvTag, rawFile, maskedFile, maskPixelFile); i++ ) {
		if ( csvTag == tag ) {
			config.modifyConfig("CONFIG").addDictionary("RAW_FILE", rawFile);
			config.modifyConfig("CONFIG").addDictionary("MASKED_FILE", maskedFile);
			config.modifyConfig("CONFIG").addDictionary("MASK_PIXEL_FILE", maskPixelFile);
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
	analyse.openOutputFile();
	analyse.extractEvent();
	analyse.excludeHotPixel();
	analyse.extractCluster();
	analyse.extractShape();
	analyse.saveEvent();
	analyse.saveCluster();
	analyse.saveShape();
}