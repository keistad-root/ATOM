#include "TThresholdPlotter.h"

#include "TCSV.h"

#include "cppargs.h"

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Draw threshold information of an ALPIDE");
	parser.add_argument("config").help("Config File").set_default("default").add_finish();
	parser.add_argument("--csv").help("Threshold value stored in this CSV file").set_default("").add_finish();
	parser.add_argument("--exp").help("Get the informations from CSV").set_default("999999").type("int").add_finish();
	parser.parse_args();
	return parser;
}


int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);

	std::string configPath = parser.get_value<std::string>("config");
	CppConfigFile* config = new CppConfigFile(configPath);

	std::string csvPath = parser.get_value<std::string>("csv");
	TCSV csv(csvPath);

	std::vector<std::string> numSet = csv.getColumn("Number");

	std::string expStr = std::to_string(parser.get_value<int>("exp"));
	expStr.insert(0, 6 - expStr.size(), '0');

	int csvNum = find(numSet.begin(), numSet.end(), expStr) - numSet.begin();

	std::vector<std::string> line = csv.getRow(csvNum);
	config->modifyConfig("File").addDictionary("data_file", line[3]);
	config->modifyConfig("File").addDictionary("output_directory", line[4]);
	config->modifyConfig("File").addDictionary("graph_file", line[5]);

	TThresholdPlotter plotter(config);
	plotter.InitPlot();
	plotter.FillPlot();
	plotter.savePlots();

	plotter.saveGraphFile();

	return 0;
}