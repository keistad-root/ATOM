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
#include "config.h"

const std::string configPath = std::filesystem::path(SOURCE_DIR) / "build/config/COMPARISON_LENGTH.conf";

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Compare experiment and simulation data for one collimator");
	parser.add_argument("EUT").nargs().help("Experiment tag under test").add_finish();
	parser.add_argument("--REF").help("Experiment tag for reference").add_finish();
	parser.parse_args();
	return parser;
}

std::array<TGraphErrors*, 3> getExperimentGraphSet(const std::vector<std::string> tagSet, const std::string& refTag) {
	std::array<TGraphErrors*, 3> graphSet = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
	TExperimentInfoSet expSet;

	TExperimentInfo refAlphagammaExp = expSet.getExperimentInfo(refTag + "X");
	TExperimentInfo refGammaExp = expSet.getExperimentInfo(refTag + "O");
	std::array<double, 2> refCS1 = {refAlphagammaExp.getCS1()[0] - refGammaExp.getCS1()[0], std::sqrt(std::pow(refAlphagammaExp.getCS1()[1], 2) + std::pow(refGammaExp.getCS1()[1], 2))};
	std::array<double, 2> refCS4TO32 = {refAlphagammaExp.getCS4TO32()[0] - refGammaExp.getCS4TO32()[0], std::sqrt(std::pow(refAlphagammaExp.getCS4TO32()[1], 2) + std::pow(refGammaExp.getCS4TO32()[1], 2))};
	std::array<double, 2> refCS33Over = {refAlphagammaExp.getCS33Over()[0] - refGammaExp.getCS33Over()[0], std::sqrt(std::pow(refAlphagammaExp.getCS33Over()[1], 2) + std::pow(refGammaExp.getCS33Over()[1], 2))};

	for ( auto& expTag : tagSet ) {
		TExperimentInfo alphagamma = expSet.getExperimentInfo(expTag + "X");
		TExperimentInfo gamma = expSet.getExperimentInfo(expTag + "O");

		std::array<double, 2> cs1 = {alphagamma.getCS1()[0] - gamma.getCS1()[0], std::sqrt(std::pow(alphagamma.getCS1()[1], 2) + std::pow(gamma.getCS1()[1], 2))};
		std::array<double, 2> cs4to32 = {alphagamma.getCS4TO32()[0] - gamma.getCS4TO32()[0], std::sqrt(std::pow(alphagamma.getCS4TO32()[1], 2) + std::pow(gamma.getCS4TO32()[1], 2))};
		std::array<double, 2> cs33over = {alphagamma.getCS33Over()[0] - gamma.getCS33Over()[0], std::sqrt(std::pow(alphagamma.getCS33Over()[1], 2) + std::pow(gamma.getCS33Over()[1], 2))};

		graphSet[0]->SetPoint(graphSet[0]->GetN(), alphagamma.getCollimatorArea(), cs1[0] / refCS1[0]);
		graphSet[0]->SetPointError(graphSet[0]->GetN() - 1, 0.05, std::sqrt(std::pow(cs1[1] / refCS1[0], 2) + std::pow(cs1[0] * refCS1[1] / std::pow(refCS1[0], 2), 2)));
		graphSet[1]->SetPoint(graphSet[1]->GetN(), alphagamma.getCollimatorArea(), cs4to32[0] / refCS4TO32[0]);
		graphSet[1]->SetPointError(graphSet[1]->GetN() - 1, 0.05, std::sqrt(std::pow(cs4to32[1] / refCS4TO32[0], 2) + std::pow(cs4to32[0] * refCS4TO32[1] / std::pow(refCS4TO32[0], 2), 2)));
		graphSet[2]->SetPoint(graphSet[2]->GetN(), alphagamma.getCollimatorArea(), cs33over[0] / refCS33Over[0]);
		graphSet[2]->SetPointError(graphSet[2]->GetN() - 1, 0.05, std::sqrt(std::pow(cs33over[1] / refCS33Over[0], 2) + std::pow(cs33over[0] * refCS33Over[1] / std::pow(refCS33Over[0], 2), 2)));
	}

	return graphSet;
}

int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);
	CppConfigFile config(configPath);
	std::vector<std::string> tagSet = parser.get_value<std::vector<std::string>>("EUT");
	std::string refTag = parser.get_value<std::string>("REF");

	std::array<TGraphErrors*, 3> expGraphSet = getExperimentGraphSet(tagSet, refTag);

	std::filesystem::path savePath = std::filesystem::path(config.getConfig("FILE").find("OUTPUT_DIRECTORY"));
	TCanvas* canvas = TPlotter::initCanvas(config.getConfig("SINGLE_CLUSTER"));
	TLegend* legend = TPlotter::initLegend(config.getConfig("SINGLE_CLUSTER"));
	TPlotter::drawPlot(canvas, expGraphSet[1], config.getConfig("SINGLE_CLUSTER").getSubConfig("EXPERIMENT"), "AP");
	legend->AddEntry(expGraphSet[1], "Experiment", "PLE");
	TPlotter::saveLegend(canvas, legend);
	TPlotter::saveCanvas(canvas, savePath, config.getConfig("SINGLE_CLUSTER"));


	return 0;
}