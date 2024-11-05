#include "TCSV.h"

#include "cppargs.h"

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Draw threshold information of an ALPIDE");
	parser.add_argument("config").help("Config File").set_default("default").add_finish();
	parser.parse_args();
	return parser;
}

int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);

	std::filesystem::path csvPath = parser.get_value<std::string>("config");
	TCSV csv(csvPath);

	for ( const std::string& value : csv.getColumn("Number") ) {
		std::cout << value << std::endl;
	}
}