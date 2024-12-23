#include <csv.h>
#include "TCompareClustersize.h"

#include "cppargs.h"

CppConfigFile setEnvironment(const std::vector<std::string>& plotSet) {
	CppConfigFile config("/home/ychoi/ATOM/config/comparison/comparison.conf");
	io::CSVReader<2> fileListCSV("/home/ychoi/ATOM/config/comparison/file_list.csv");
	io::CSVReader<5> comparisonCsv("/home/ychoi/ATOM/config/comparison/comparison_list.csv");
	io::CSVReader<4> plotListCSV("/home/ychoi/ATOM/config/comparison/plot_list.csv");

	fileListCSV.read_header(io::ignore_extra_column, "tag", "input_file");

	std::string tag;
	std::string inputFile;

	while ( fileListCSV.read_row(tag, inputFile) ) {
		config.modifyConfig("FileList").addDictionary(tag, inputFile);
	}

	comparisonCsv.read_header(io::ignore_extra_column, "tag", "output_directory", "name", "y_range", "plot_list");
	std::string outputDirectory;
	std::string name;
	std::string yRange;
	std::string plotList;



	std::string hist;
	std::string legend;
	std::string ratio;

	plotListCSV.read_header(io::ignore_extra_column, "tag", "hist", "legend", "ratio");
	std::array<std::string, 7> colour = {"red", "blue", "violet", "indigo", "green", "orange", "yellow"};
	int iColour = 0;
	CppConfigDictionary plotDictionary("plots");
	while ( plotListCSV.read_row(tag, hist, legend, ratio) ) {
		if ( std::find(plotSet.begin(), plotSet.end(), tag) != plotSet.end() ) {
			CppConfigDictionary temp(tag);
			temp.addDictionary("hist", hist);
			temp.addDictionary("legend", legend);
			temp.addDictionary("ratio", ratio);
			temp.addDictionary("line_width", "2");
			temp.addDictionary("line_colour", colour[iColour]);
			iColour++;
			plotDictionary.addSubConfigDictionary(temp);
		}
	}
	config.modifyConfig("Clustersize").addSubConfigDictionary(plotDictionary);
	return config;
}


int main(int argc, char** argv) {
	std::vector<std::string> plotSet;
	for ( int i = 1; i < argc; i++ ) {
		plotSet.push_back(argv[i]);
	}
	CppConfigFile config = setEnvironment(plotSet);

	TCompareClustersize* comparison = new TCompareClustersize(&config);
	comparison->drawClustersize();
	comparison->drawRegion();
}