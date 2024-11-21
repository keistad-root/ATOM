#include "TCompareClustersize.h"

#include "cppargs.h"

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

	TCompareClustersize* comparison = new TCompareClustersize(&config);
	comparison->drawClustersize();
}