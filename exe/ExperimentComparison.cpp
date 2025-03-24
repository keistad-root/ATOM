#include <csv.h>
#include "TCompareClustersize.h"

#include "cppargs.h"
#include "config.h"

const std::string cfgPath = CONFIG_DIR"/EXPERIMENT_COMPARISON.conf";
const std::string csvPath = CONFIG_DIR"/EXPERIMENT_COMPARISON.csv";
const std::string listCsvPath = CONFIG_DIR"/PLOT_LIST.csv";

CppConfigFile setEnvironment(const ArgumentParser& parser) {
	const std::string tag = parser.get_value<std::string>("TAG");
	CppConfigFile config(cfgPath);
	io::CSVReader<5> csv(csvPath);
	csv.set_header("TAG", "OUTPUT_DIRECTORY", "NAME", "Y_RANGE", "PLOT_LIST");

	std::string csvTag, csvOutputDir, csvName, csvYRange, csvPlotList;

	while ( csv.read_row(csvTag, csvOutputDir, csvName, csvYRange, csvPlotList) ) {
		if ( csvTag == tag ) {
			config.modifyConfig("FILE").addDictionary("OUTPUT_DIRECTORY", csvOutputDir);
			config.modifyConfig("CLUSTERSIZE").addDictionary("NAME", csvName);
			config.modifyConfig("CLUSTERSIZE").addDictionary("Y_RANGE", csvYRange);
			break;
		}
	}

	std::stringstream ss(csvPlotList);
	std::vector<std::string> plotSet;
	std::string plot;
	while ( std::getline(ss, plot, ' ') ) {
		plotSet.push_back(plot);
	}

	io::CSVReader<4> listCsv(listCsvPath);

	listCsv.read_header(io::ignore_extra_column, "TAG", "HIST", "LEGEND", "RATIO");

	std::string listTag, listHist, listLegend, listRatio;

	std::array<std::string, 7> colour = {"red", "blue", "violet", "indigo", "green", "orange", "yellow"};
	int iColour = 0;

	while ( listCsv.read_row(listTag, listHist, listLegend, listRatio) ) {
		if ( std::find(plotSet.begin(), plotSet.end(), listTag) != plotSet.end() ) {
			CppConfigDictionary dictionary("TAG");
			dictionary.addDictionary("HIST", listHist);
			dictionary.addDictionary("LEGEND", listLegend);
			dictionary.addDictionary("RATIO", listRatio);
			dictionary.addDictionary("LINE_WIDTH", "2");
			dictionary.addDictionary("LINE_COLOUR", colour[iColour]);
			iColour++;
			dictionary.addDictionary("LINE_STYLE", "1");
			config.modifyConfig("PLOT_SET").addSubConfigDictionary(dictionary);
		}
	}

	return config;
}

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Compare experimental data");
	parser.add_argument("TAG").help("Tagging for the experiment").add_finish();
	parser.parse_args();
	return parser;
}

int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);
	CppConfigFile config = setEnvironment(parser);
	TCompareClustersize comparison(config);
	comparison.drawClustersize();

	return 0;
}