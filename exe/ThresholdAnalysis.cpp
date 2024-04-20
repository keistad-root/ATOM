#include <iostream>
#include <string>

#include "TThresholdAnalyser.h"
#include "TString.h"
#include "cppargs.h"

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Draw threshold information of an ALPIDE");
	parser.add_argument("thrFile").metavar("FILENAME").help("threshold file to be processed").add_finish();
	parser.add_argument("--output").help("output path").set_default("default").help("output path").add_finish();
	parser.add_argument("--drawing_plots").nargs().set_default("default").add_domain({"threshold_distribution", "error_distribution", "thresholed_map", "quality_factor"}).help("Choose the kind of plots to draw").add_finish();
	parser.parse_args();
	return parser;
}


int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);

	std::filesystem::path input(parser.get_value<std::string>("thrFile"));
	std::filesystem::path path;

	if ( parser.get_value<std::string>("output") == "default" ) {
		path = std::filesystem::absolute(input.parent_path());
	} else {
		path = parser.get_value<std::string>("output");
	}

	std::filesystem::path stem = input.stem();

	std::ifstream file(input.replace_extension(".dat"));
	TThresholdAnalyser analyser(file);
	analyser.refineData();

	std::vector<std::string> plotList = parser.get_value<std::vector<std::string>>("drawing_plots");
	if ( find(plotList.begin(), plotList.end(), "threshold_distribution") != plotList.end() ) {
		std::filesystem::create_directory(path / stem);
		analyser.saveThresholdDistribution(static_cast<TString>(path / stem / "Threshold_distribution.png"));
	}
	if ( find(plotList.begin(), plotList.end(), "error_distribution") != plotList.end() ) {
		std::filesystem::create_directory(path / stem);
		analyser.saveErrorDistribution(static_cast<TString>(path / stem / "Error_distribution.png"));
	}
	if ( find(plotList.begin(), plotList.end(), "threshold_map") != plotList.end() ) {
		std::filesystem::create_directory(path / stem);
		analyser.saveThresholdmap(static_cast<TString>(path / stem / "Thresholdmap.png"));
	}
	if ( find(plotList.begin(), plotList.end(), "quality_factor") != plotList.end() ) {
		std::filesystem::create_directory(path / stem);
		analyser.saveQualityDistribution(static_cast<TString>(path / stem / "Quality_distribution.png"));
	}
	return 0;
}