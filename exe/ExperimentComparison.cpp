#include <csv.h>
#include "TCompareClustersize.h"

#include "cppargs.h"

CppConfigFile setEnvironment(const std::string& tagging) {
	CppConfigFile config("/home/ychoi/ATOM/build/config/comparison.conf");
	// io::CSVReader<5> comparisonCsv("/home/ychoi/ATOM/config/comparison/comparison_list.csv");

	// std::string comparisonListTag;
	// std::string outputDirectory;
	// std::string name;
	// std::string yRange;
	// std::string plotList;

	// while ( comparisonCsv.read_row(comparisonListTag, outputDirectory, name, yRange, plotList) ) {
	// 	if ( comparisonListTag == tagging ) {
	// 		config.modifyConfig("File").addDictionary("output_directory", outputDirectory);
	// 		config.modifyConfig("Clustersize").addDictionary("name", name);
	// 		config.modifyConfig("Clustersize").addDictionary("y_range", yRange);
	// 		break;
	// 	}
	// }
	// std::stringstream plotListSS(plotList);
	// std::vector<std::string> plotSet;
	// std::string plot;
	// while ( std::getline(plotListSS, plot, ' ') ) {
	// 	plotSet.push_back(plot);
	// }

	// io::CSVReader<2> fileListCSV("/home/ychoi/ATOM/config/comparison/file_list.csv");
	// fileListCSV.read_header(io::ignore_extra_column, "tag", "input_file");

	// std::string fileListTag;
	// std::string inputFile;

	// while ( fileListCSV.read_row(fileListTag, inputFile) ) {
	// 	config.modifyConfig("FileList").addDictionary(fileListTag, inputFile);
	// }

	// io::CSVReader<4> plotListCSV("/home/ychoi/ATOM/config/comparison/plot_list.csv");

	// std::string plotListTag;
	// std::string hist;
	// std::string legend;
	// std::string ratio;

	// std::array<std::string, 7> colour = {"red", "blue", "violet", "indigo", "green", "orange", "yellow"};
	// int iColour = 0;
	// CppConfigDictionary plotDictionary("PLOT_SET");
	// while ( plotListCSV.read_row(plotListTag, hist, legend, ratio) ) {
	// 	if ( std::find(plotSet.begin(), plotSet.end(), plotListTag) != plotSet.end() ) {
	// 		CppConfigDictionary temp(plotListTag);
	// 		temp.addDictionary("hist", hist);
	// 		temp.addDictionary("legend", legend);
	// 		temp.addDictionary("ratio", ratio);
	// 		temp.addDictionary("line_width", "2");
	// 		temp.addDictionary("line_colour", colour[iColour]);
	// 		iColour++;
	// 		plotDictionary.addSubConfigDictionary(temp);
	// 	}
	// }
	// config.modifyConfig("Clustersize").addSubConfigDictionary(plotDictionary);
	return config;
}


int main(int argc, char** argv) {
	std::string tagging = argv[1];
	CppConfigFile config = setEnvironment(tagging);

	TCompareClustersize* comparison = new TCompareClustersize(config);
	comparison->drawClustersize();
	// comparison->drawClustersize();
	// comparison->drawRegion();
	// comparison->getClusterStr();
}