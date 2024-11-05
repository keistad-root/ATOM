
#include "cppargs.h"
#include "CppConfigFile.h"

#include "TDataAnalyser.h"

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Draw threshold information of an ALPIDE");
	parser.add_argument("config").help("Config File").set_default("default").add_finish();
	parser.parse_args();
	return parser;
}

int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);

	std::string configPath = parser.get_value<std::string>("config");
	CppConfigFile config(configPath);

	TDataAnalyser* analyse = new TDataAnalyser(&config);
	analyse->extractEvent();
	analyse->excludeHotPixel();
	analyse->extractCluster();
	analyse->extractHotPixel();
	analyse->saveEvent();
	analyse->saveCluster();

	delete analyse;
}