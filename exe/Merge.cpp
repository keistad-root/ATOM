#include <iostream>
#include <vector>
#include <string>

#include "cppargs.h"
#include "CppConfigFile.h"
#include "TMerge.h"

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Draw plots for analysis data");
	parser.add_argument("config").help("Config file").set_default("default").add_finish();
	parser.parse_args();
	return parser;
}

int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);
	CppConfigFile* config = new CppConfigFile(parser.get_value<std::string>("config"));

	TMergeExperimentROOT* merge = new TMergeExperimentROOT(config->getConfig("Merge").find("output_file"), config->getConfig("Merge").getSubConfig("input_files").getValueList());

	merge->mergeFile();

	delete config;
	delete merge;
	return 0;
}