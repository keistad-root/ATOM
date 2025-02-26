#include<iostream>

#include "TExperimentInfoSet.h"
#include "TGeantInfoSet.h"

#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TText.h"
#include "TLegend.h"
#include "cppargs.h"
#include "CppConfigFile.h"
#include "TPlotter.h"

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Compare experiment and simulation data for one collimator");
	parser.add_argument("EUT").nargs().help("Experiment tag under test").add_finish();
	parser.add_argument("--REF").help("Experiment tag for reference").add_finish();
	parser.parse_args();
	return parser;
}

int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);

	std::vector<std::string> tagSet = parser.get_value<std::vector<std::string>>("EUT");
	std::string refTag = parser.get_value<std::string>("REF");

	std::cout << refTag << std::endl;

	return 0;
}