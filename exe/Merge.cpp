#include <iostream>

#include "cppargs.h"
#include "cppconfig.h"
#include "TMerge.h"

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Draw plots for analysis data");
	parser.add_argument("config").help("Config file").set_default("default").add_finish();
	parser.parse_args();
	return parser;
}

int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);
	Configuration* config = new Configuration(parser.get_value<std::string>("config"));
	std::string outputFileName = config->getConfig("Merge")->find("output_file");
	std::vector<std::string> inputFileNames = config->getConfig("Merge")->findlist("input_files");
	TMergeExperimentROOT* merger = new TMergeExperimentROOT(outputFileName, inputFileNames);
	merger->mergeFile();
	delete merger;
}