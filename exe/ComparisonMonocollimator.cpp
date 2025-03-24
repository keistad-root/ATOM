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

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Compare experiment and simulation data for one collimator");
	parser.add_argument("EUT").help("Experiment tag under test").add_finish();
	parser.add_argument("REF").help("Experiment tag for reference").add_finish();
	parser.add_argument("--sim").help("Use simulation data").set_default("true").add_finish();
	parser.parse_args();
	return parser;
}

TGraphErrors* getExperimentOriginGraph(const std::string& eutTag) {
	TGraphErrors* graph = new TGraphErrors();

	TExperimentInfoSet expInfo;
	TExperimentInfo alphagamma = expInfo.getExperimentInfo(eutTag + "X");
	TExperimentInfo gamma = expInfo.getExperimentInfo(eutTag + "O");

	Double_t cs1 = alphagamma.getCS1()[0] - gamma.getCS1()[0];
	Double_t cs1Error = std::sqrt(std::pow(alphagamma.getCS1()[1], 2) + std::pow(gamma.getCS1()[1], 2));
	Double_t cs4to32 = alphagamma.getCS4TO32()[0] - gamma.getCS4TO32()[0];
	Double_t cs4to32Error = std::sqrt(std::pow(alphagamma.getCS4TO32()[1], 2) + std::pow(gamma.getCS4TO32()[1], 2));
	Double_t cs33over = alphagamma.getCS33Over()[0] - gamma.getCS33Over()[0];
	Double_t cs33overError = std::sqrt(std::pow(alphagamma.getCS33Over()[1], 2) + std::pow(gamma.getCS33Over()[1], 2));

	graph->SetPoint(0, 1, cs1);
	graph->SetPointError(0, 0, cs1Error);
	graph->SetPoint(1, 2, cs4to32);
	graph->SetPointError(1, 0, cs4to32Error);
	graph->SetPoint(2, 3, cs33over);
	graph->SetPointError(2, 0, cs33overError);

	return graph;
}

TGraphErrors* getExperimentRatioGraph(const std::string& eutTag, const std::string& refTag) {
	TGraphErrors* graph = new TGraphErrors();

	TExperimentInfoSet expInfo;
	TExperimentInfo alphagamma = expInfo.getExperimentInfo(eutTag + "X");
	TExperimentInfo gamma = expInfo.getExperimentInfo(eutTag + "O");
	TExperimentInfo refAlphagamma = expInfo.getExperimentInfo(refTag + "X");
	TExperimentInfo refGamma = expInfo.getExperimentInfo(refTag + "O");

	Double_t cs1Ratio = (alphagamma.getCS1()[0] - gamma.getCS1()[0]) / (refAlphagamma.getCS1()[0] - refGamma.getCS1()[0]);
	Double_t cs1RatioError = std::pow(1. / (refAlphagamma.getCS1()[0] - refGamma.getCS1()[0]), 2) * (std::pow(alphagamma.getCS1()[1], 2) + std::pow(gamma.getCS1()[1], 2)) + std::pow((alphagamma.getCS1()[0] - gamma.getCS1()[0]) / (std::pow(refAlphagamma.getCS1()[0] - refGamma.getCS1()[0], 2)), 2) * (std::pow(refAlphagamma.getCS1()[1], 2) + std::pow(refGamma.getCS1()[1], 2));

	Double_t cs4to32Ratio = (alphagamma.getCS4TO32()[0] - gamma.getCS4TO32()[0]) / (refAlphagamma.getCS4TO32()[0] - refGamma.getCS4TO32()[0]);
	Double_t cs4to32RatioError = std::pow(1. / (refAlphagamma.getCS4TO32()[0] - refGamma.getCS4TO32()[0]), 2) * (std::pow(alphagamma.getCS4TO32()[1], 2) + std::pow(gamma.getCS4TO32()[1], 2)) + std::pow((alphagamma.getCS4TO32()[0] - gamma.getCS4TO32()[0]) / (std::pow(refAlphagamma.getCS4TO32()[0] - refGamma.getCS4TO32()[0], 2)), 2) * (std::pow(refAlphagamma.getCS4TO32()[1], 2) + std::pow(refGamma.getCS4TO32()[1], 2));

	Double_t cs33overRatio = (alphagamma.getCS33Over()[0] - gamma.getCS33Over()[0]) / (refAlphagamma.getCS33Over()[0] - refGamma.getCS33Over()[0]);
	Double_t cs33overRatioError = std::pow(1. / (refAlphagamma.getCS33Over()[0] - refGamma.getCS33Over()[0]), 2) * (std::pow(alphagamma.getCS33Over()[1], 2) + std::pow(gamma.getCS33Over()[1], 2)) + std::pow((alphagamma.getCS33Over()[0] - gamma.getCS33Over()[0]) / (std::pow(refAlphagamma.getCS33Over()[0] - refGamma.getCS33Over()[0], 2)), 2) * (std::pow(refAlphagamma.getCS33Over()[1], 2) + std::pow(refGamma.getCS33Over()[1], 2));

	graph->SetPoint(0, 1, cs1Ratio);
	graph->SetPointError(0, 0, cs1RatioError);
	graph->SetPoint(1, 2, cs4to32Ratio);
	graph->SetPointError(1, 0, cs4to32RatioError);
	graph->SetPoint(2, 3, cs33overRatio);
	graph->SetPointError(2, 0, cs33overRatioError);

	return graph;
}

TGraphErrors* getSimulationOriginGraph(const std::string& eutTag) {
	TGraphErrors* graph = new TGraphErrors();

	TGeantInfoSet geantInfo;
	TGeantInfo alphagamma = geantInfo.getGeantInfo(eutTag + "AX");
	TGeantInfo gamma = geantInfo.getGeantInfo(eutTag + "AO");

	Double_t eem = alphagamma.getEEM()[0] - gamma.getEEM()[0];
	Double_t eemError = std::sqrt(std::pow(alphagamma.getEEM()[1], 2) + std::pow(gamma.getEEM()[1], 2));
	Double_t eae = alphagamma.getEAE()[0] - gamma.getEAE()[0];
	Double_t eaeError = std::sqrt(std::pow(alphagamma.getEAE()[1], 2) + std::pow(gamma.getEAE()[1], 2));
	Double_t doubleCluster = alphagamma.getDouble()[0] - gamma.getDouble()[0];
	Double_t doubleClusterError = std::sqrt(std::pow(alphagamma.getDouble()[1], 2) + std::pow(gamma.getDouble()[1], 2));

	graph->SetPoint(0, 1, eem);
	graph->SetPointError(0, 0, eemError);
	graph->SetPoint(1, 2, eae);
	graph->SetPointError(1, 0, eaeError);
	graph->SetPoint(2, 3, doubleCluster);
	graph->SetPointError(2, 0, doubleClusterError);

	return graph;
}

TGraphErrors* getSimulationRatioGraph(const std::string& eutTag, const std::string& refTag) {
	TGraphErrors* graph = new TGraphErrors();

	TGeantInfoSet geantInfo;
	TGeantInfo alphagamma = geantInfo.getGeantInfo(eutTag + "AX");
	TGeantInfo gamma = geantInfo.getGeantInfo(eutTag + "AO");
	TGeantInfo refAlphagamma = geantInfo.getGeantInfo(refTag + "AX");
	TGeantInfo refGamma = geantInfo.getGeantInfo(refTag + "AO");

	Double_t eemRatio = (alphagamma.getEEM()[0] - gamma.getEEM()[0]) / (refAlphagamma.getEEM()[0] - refGamma.getEEM()[0]);
	Double_t eemRatioError = std::pow(1. / (refAlphagamma.getEEM()[0] - refGamma.getEEM()[0]), 2) * (std::pow(alphagamma.getEEM()[1], 2) + std::pow(gamma.getEEM()[1], 2)) + std::pow((alphagamma.getEEM()[0] - gamma.getEEM()[0]) / (std::pow(refAlphagamma.getEEM()[0] - refGamma.getEEM()[0], 2)), 2) * (std::pow(refAlphagamma.getEEM()[1], 2) + std::pow(refGamma.getEEM()[1], 2));

	Double_t eaeRatio = (alphagamma.getEAE()[0] - gamma.getEAE()[0]) / (refAlphagamma.getEAE()[0] - refGamma.getEAE()[0]);
	Double_t eaeRatioError = std::pow(1. / (refAlphagamma.getEAE()[0] - refGamma.getEAE()[0]), 2) * (std::pow(alphagamma.getEAE()[1], 2) + std::pow(gamma.getEAE()[1], 2)) + std::pow((alphagamma.getEAE()[0] - gamma.getEAE()[0]) / (std::pow(refAlphagamma.getEAE()[0] - refGamma.getEAE()[0], 2)), 2) * (std::pow(refAlphagamma.getEAE()[1], 2) + std::pow(refGamma.getEAE()[1], 2));

	Double_t doubleRatio = (alphagamma.getDouble()[0] - gamma.getDouble()[0]) / (refAlphagamma.getDouble()[0] - refGamma.getDouble()[0]);
	Double_t doubleRatioError = std::pow(1. / (refAlphagamma.getDouble()[0] - refGamma.getDouble()[0]), 2) * (std::pow(alphagamma.getDouble()[1], 2) + std::pow(gamma.getDouble()[1], 2)) + std::pow((alphagamma.getDouble()[0] - gamma.getDouble()[0]) / (std::pow(refAlphagamma.getDouble()[0] - refGamma.getDouble()[0], 2)), 2) * (std::pow(refAlphagamma.getDouble()[1], 2) + std::pow(refGamma.getDouble()[1], 2));

	graph->SetPoint(0, 1, eemRatio);
	graph->SetPointError(0, 0, eemRatioError);
	graph->SetPoint(1, 2, eaeRatio);
	graph->SetPointError(1, 0, eaeRatioError);
	graph->SetPoint(2, 3, doubleRatio);
	graph->SetPointError(2, 0, doubleRatioError);

	return graph;
}

std::array<TText*, 6> getAxis() {
	std::array<TText*, 6> label = {new TText(.21, .07, "CS1"), new TText(.21, .04, "# Electron"), new TText(.53, .07, "CS4 To CS32"), new TText(.53, .04, "# Single Alpha"), new TText(.84, .07, "Over CS33"), new TText(.84, .04, "# Double Cluster")};
	for ( int i = 0; i < 6; i++ ) {
		label[i]->SetNDC();
		label[i]->SetTextAlign(22);
		label[i]->SetTextColor(kBlack);
		label[i]->SetTextFont(43);
		label[i]->SetTextSize(30);
	}

	return label;
}

TCanvas* getRatioGraph(const std::string& eutTag, const std::string& refTag, const CppConfigDictionary& config, bool sim = false) {
	TGraphErrors* expGraph = getExperimentRatioGraph(eutTag, refTag);
	TGraphErrors* simGraph;
	if ( sim ) {
		simGraph = getSimulationRatioGraph(eutTag, refTag);
	}

	TCanvas* canvas = TPlotter::initCanvas(config);
	TLegend* legend = TPlotter::initLegend(config);
	legend->AddEntry(expGraph, "Experiment", "PE");
	TPlotter::drawPlot(canvas, expGraph, config.getSubConfig("EXPERIMENT"));
	if ( sim ) {
		legend->AddEntry(simGraph, "Simulation", "PE");
		TPlotter::drawPlot(canvas, simGraph, config.getSubConfig("SIMULATION"));
	}
	expGraph->GetXaxis()->SetLabelOffset(999);
	expGraph->GetXaxis()->SetTickLength(0);
	std::array<TText*, 6> axisLabel = getAxis();
	for ( int i = 0; i < 6; i++ ) {
		axisLabel[i]->Draw("SAME");
	}

	TPlotter::saveLegend(canvas, legend);

	return canvas;
}

TCanvas* getOriginGraph(const std::string& eutTag, const CppConfigDictionary& config, bool sim = false) {
	TGraphErrors* expGraph = getExperimentOriginGraph(eutTag);
	TGraphErrors* simGraph;
	if ( sim ) {
		simGraph = getSimulationOriginGraph(eutTag);
	}

	TCanvas* canvas = TPlotter::initCanvas(config);
	TLegend* legend = TPlotter::initLegend(config);
	legend->AddEntry(expGraph, "Experiment", "PE");
	TPlotter::drawPlot(canvas, expGraph, config.getSubConfig("EXPERIMENT"));
	if ( sim ) {
		legend->AddEntry(simGraph, "Simulation", "PE");
		TPlotter::drawPlot(canvas, simGraph, config.getSubConfig("SIMULATION"));
	}

	expGraph->GetXaxis()->SetLabelOffset(999);
	expGraph->GetXaxis()->SetTickLength(0);
	std::array<TText*, 6> axisLabel = getAxis();
	for ( int i = 0; i < 6; i++ ) {
		axisLabel[i]->Draw("SAME");
	}

	TPlotter::saveLegend(canvas, legend);

	return canvas;
}

const std::string configPath = std::filesystem::path(SOURCE_DIR) / "build/config/COMPARISON_MONOCOLLIMATOR.conf";

CppConfigFile setEnvironment(const ArgumentParser& parser) {
	CppConfigFile config = CppConfigFile(configPath);

	std::string eutTag = parser.get_value<std::string>("EUT");
	std::string collimatorName = eutTag.find("REF") != std::string::npos ? eutTag : eutTag.substr(0, eutTag.find("F")) + "\#phi" + eutTag.substr(eutTag.find("F") + 1, eutTag.size() - eutTag.find("F") - 1);
	config.modifyConfig("RATIO").addDictionary("NAME", config.modifyConfig("RATIO").find("NAME") + "_" + eutTag);
	config.modifyConfig("RATIO").addDictionary("TITLE", "\"" + collimatorName + " Collimator\" \"\" \"Ratio to Reference\"");
	config.modifyConfig("ORIGIN").addDictionary("NAME", config.modifyConfig("ORIGIN").find("NAME") + "_" + eutTag);
	config.modifyConfig("ORIGIN").addDictionary("TITLE", "\"" + collimatorName + " Collimator\" \"\" \"Entry\"");

	return config;
}

int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);
	CppConfigFile config = setEnvironment(parser);
	std::string eutTag = parser.get_value<std::string>("EUT");
	std::string refTag = parser.get_value<std::string>("REF");
	bool sim = parser.get_value<bool>("sim");

	TCanvas* ratioCanvas = getRatioGraph(eutTag, refTag, config.getConfig("RATIO"), sim);
	std::filesystem::path path = config.getConfig("FILE").find("OUTPUT_DIRECTORY");
	TPlotter::saveCanvas(ratioCanvas, path, config.getConfig("RATIO"));

	TCanvas* originCanvas = getOriginGraph(eutTag, config.getConfig("ORIGIN"), sim);
	TPlotter::saveCanvas(originCanvas, path, config.getConfig("ORIGIN"));
	return 0;
}