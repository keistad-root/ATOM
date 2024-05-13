#include <iostream>

#include "cppargs.h"
#include "CppConfigFile.h"

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Draw plots for analysis data");
	parser.add_argument("config").help("Config file").set_default("default").add_finish();
	parser.parse_args();
	return parser;
}

int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);
	CppConfigFile* config = new CppConfigFile(parser.get_value<std::string>("config"));
	for ( std::string_view str : config->getConfig("Basic").getKeyList() ) {
		std::cout << str << std::endl;
	}
	std::cout << config->getConfig("Basic").find("shape_info/infos/info2") << std::endl;
	return 0;
}