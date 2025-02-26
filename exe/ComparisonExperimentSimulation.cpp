#include<iostream>

#include "TExperimentInfoSet.h"
#include "TGeantInfoSet.h"

#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TText.h"
#include "TLegend.h"
#include "cppargs.h"

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Compare experiment and simulation data for one collimator");
	parser.add_argument("EUT").help("Experiment tag under test").add_finish();
	parser.add_argument("REF").help("Experiment tag for reference").add_finish();
	parser.parse_args();
	return parser;
}

TGraphErrors* getExperimentGraph(const std::string& eutTag, const std::string& refTag) {
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


int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);
	std::string eutTag = parser.get_value<std::string>("EUT");
	std::string refTag = parser.get_value<std::string>("REF");

	TGraphErrors* expGraph = getExperimentGraph(eutTag, refTag);


	TGeantInfo eutAlphaGeant = geantInfo.getGeantInfo(eutTag + "AX");
	TGeantInfo refAlphaGeant = geantInfo.getGeantInfo(refTag + "AX");

	TGraphErrors* geantGraph = new TGraphErrors();

	geantGraph->SetPoint(0, 1, eutAlphaGeant.getEEM()[0] / refAlphaGeant.getEEM()[0]);
	geantGraph->SetPointError(0, 0, std::sqrt(std::pow(eutAlphaGeant.getEEM()[1] / refAlphaGeant.getEEM()[0], 2) + std::pow(eutAlphaGeant.getEEM()[0] * refAlphaGeant.getEEM()[1] / std::pow(refAlphaGeant.getEEM()[0], 2), 2)));
	geantGraph->SetPoint(1, 2, eutAlphaGeant.getEAE()[0] / refAlphaGeant.getEAE()[0]);
	geantGraph->SetPointError(1, 0, std::sqrt(std::pow(eutAlphaGeant.getEAE()[1] / refAlphaGeant.getEAE()[0], 2) + std::pow(eutAlphaGeant.getEAE()[0] * refAlphaGeant.getEAE()[1] / std::pow(refAlphaGeant.getEAE()[0], 2), 2)));
	geantGraph->SetPoint(2, 3, eutAlphaGeant.getDouble()[0] / refAlphaGeant.getDouble()[0]);
	geantGraph->SetPointError(2, 0, std::sqrt(std::pow(eutAlphaGeant.getDouble()[1] / refAlphaGeant.getDouble()[0], 2) + std::pow(eutAlphaGeant.getDouble()[0] * refAlphaGeant.getDouble()[1] / std::pow(refAlphaGeant.getDouble()[0], 2), 2)));

	expGraph->SetMarkerStyle(24);
	expGraph->SetMarkerColor(kRed);
	expGraph->SetLineColor(kRed);
	expGraph->SetLineWidth(2);
	expGraph->SetMarkerSize(1.5);
	expGraph->SetTitle("L20#phi2 per L20REF");
	expGraph->GetYaxis()->SetTitle("Ratio to Reference");

	TAxis* xAxis = expGraph->GetXaxis();
	xAxis->SetLabelOffset(999);
	xAxis->SetTickLength(0);
	std::vector<std::string> labels = {"CS1 & # Electron", "CS4 - CS32 & # Alpha", "CS33 - & # Double Cluster"};

	TText* label[6] = {new TText(.21, .07, "CS1"), new TText(.21, .04, "# Electron"), new TText(.53, .07, "CS4 To CS32"), new TText(.53, .04, "# Single Alpha"), new TText(.84, .07, "Over CS33"), new TText(.84, .04, "# Double Cluster")};
	label[0]->SetNDC();
	label[0]->SetTextAlign(22);
	label[0]->SetTextColor(kBlack);
	label[0]->SetTextFont(43);
	label[0]->SetTextSize(30);
	label[1]->SetNDC();
	label[1]->SetTextAlign(22);
	label[1]->SetTextColor(kBlack);
	label[1]->SetTextFont(43);
	label[1]->SetTextSize(30);
	label[2]->SetNDC();
	label[2]->SetTextAlign(22);
	label[2]->SetTextColor(kBlack);
	label[2]->SetTextFont(43);
	label[2]->SetTextSize(30);
	label[3]->SetNDC();
	label[3]->SetTextAlign(22);
	label[3]->SetTextColor(kBlack);
	label[3]->SetTextFont(43);
	label[3]->SetTextSize(30);
	label[4]->SetNDC();
	label[4]->SetTextAlign(22);
	label[4]->SetTextColor(kBlack);
	label[4]->SetTextFont(43);
	label[4]->SetTextSize(30);
	label[5]->SetNDC();
	label[5]->SetTextAlign(22);
	label[5]->SetTextColor(kBlack);
	label[5]->SetTextFont(43);
	label[5]->SetTextSize(30);

	geantGraph->SetMarkerStyle(20);
	geantGraph->SetMarkerColor(kRed);
	geantGraph->SetLineColor(kRed);
	geantGraph->SetLineWidth(2);
	geantGraph->SetMarkerSize(1.5);
	geantGraph->SetTitle("Geant");

	TLegend* legend = new TLegend(.7, .7, .9, .9);
	legend->AddEntry(expGraph, "Experiment", "PE");
	legend->AddEntry(geantGraph, "Simulation", "PE");

	TCanvas* canvas = new TCanvas("canvas", "canvas", 1000, 1000);
	canvas->SetLeftMargin(.15);
	canvas->SetGrid();
	expGraph->Draw("AP");
	label[0]->Draw("SAME");
	label[1]->Draw("SAME");
	label[2]->Draw("SAME");
	label[3]->Draw("SAME");
	label[4]->Draw("SAME");
	label[5]->Draw("SAME");
	legend->Draw("SAME");
	geantGraph->Draw("P SAME");
	canvas->SaveAs("Plot/Comparison.png");
	return 0;
}