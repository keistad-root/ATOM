#include <csv.h>
#include <string>

#include "cppargs.h"
#include "CppConfigFile.h"

#include "TDataAnalyser.h"

const std::string configPath = "/home/ychoi/ATOM/config/experiment/analysis.conf";
const std::string csvPath = "/home/ychoi/ATOM/config/experiment/experiment_information.csv";

CppConfigFile setEnvironment(std::string configTag) {
	CppConfigFile config("/home/ychoi/ATOM/config/experiment/analysis.conf");
	io::CSVReader<6> csv("/home/ychoi/ATOM/config/experiment/analysis.csv");
	csv.read_header(io::ignore_extra_column, "tag", "input_file", "output_file", "mask_file", "hot_pixel_file", "hit_cut");

	std::string tag, inputFile, outputFile, maskFile, hotPixelFile, hitCut;
	for ( int i = 0; csv.read_row(tag, inputFile, outputFile, maskFile, hotPixelFile, hitCut); i++ ) {
		if ( tag == configTag ) {
			config.modifyConfig("File").addDictionary("input_file", inputFile);
			config.modifyConfig("File").addDictionary("output_file", outputFile);
			config.modifyConfig("File").addDictionary("mask_file", maskFile);
			config.modifyConfig("File").addDictionary("hot_pixel_file", hotPixelFile);
			config.modifyConfig("Masking").addDictionary("hit_cut", hitCut);
		}
	}
	return config;
}



int main(int argc, char** argv) {
	CppConfigFile config = setEnvironment(argv[1]);

	TDataAnalyser* analyse = new TDataAnalyser(config);
	analyse->extractEvent();
	// analyse->excludeHotPixel();
	analyse->extractCluster();
	analyse->extractHotPixel();
	analyse->extractShape();
	analyse->saveEvent();
	analyse->saveCluster();
	analyse->saveShape();


	delete analyse;
}