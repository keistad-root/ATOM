#include<iostream>

#include "TExperimentInfoSet.h"
#include "TGeantInfoSet.h"

#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TText.h"
#include "TLegend.h"

int main(int argc, char** argv) {
	TExperimentInfoSet expInfo;
	TGeantInfoSet geantInfo;

	std::string eutTag = "L20F2";
	std::string refTag = "L20REF";
	TExperimentInfo eutAlphaExp = expInfo.getExperimentInfo(eutTag + "X");
	TExperimentInfo eutGammaExp = expInfo.getExperimentInfo(eutTag + "O");
	TExperimentInfo refAlphaExp = expInfo.getExperimentInfo(refTag + "X");
	TExperimentInfo refGammaExp = expInfo.getExperimentInfo(refTag + "O");

	TGeantInfo eutAlphaGeant = geantInfo.getGeantInfo(eutTag + "AX");
	TGeantInfo refAlphaGeant = geantInfo.getGeantInfo(refTag + "AX");

	TGraphErrors* expGraph = new TGraphErrors();
	TGraphErrors* geantGraph = new TGraphErrors();

	std::array<double, 2> expCS1Entry = eutAlphaExp.getCS1();
	expCS1Entry[0] -= eutGammaExp.getCS1()[0];
	expCS1Entry[1] = std::sqrt(std::pow(eutAlphaExp.getCS1()[1], 2) + std::pow(eutGammaExp.getCS1()[1], 2));

	std::array<double, 2> refCS1Entry = refAlphaExp.getCS1();
	refCS1Entry[0] -= refGammaExp.getCS1()[0];
	refCS1Entry[1] = std::sqrt(std::pow(refAlphaExp.getCS1()[1], 2) + std::pow(refGammaExp.getCS1()[1], 2));

	std::array<double, 2> expCS4TO32Entry = eutAlphaExp.getCS4TO32();
	expCS4TO32Entry[0] -= eutGammaExp.getCS4TO32()[0];
	expCS4TO32Entry[1] = std::sqrt(std::pow(eutAlphaExp.getCS4TO32()[1], 2) + std::pow(eutGammaExp.getCS4TO32()[1], 2));
	std::array<double, 2> refCS4TO32Entry = refAlphaExp.getCS4TO32();
	refCS4TO32Entry[0] -= refGammaExp.getCS4TO32()[0];
	refCS4TO32Entry[1] = std::sqrt(std::pow(refAlphaExp.getCS4TO32()[1], 2) + std::pow(refGammaExp.getCS4TO32()[1], 2));

	std::array<double, 2> expCS33OverEntry = eutAlphaExp.getCS33Over();
	expCS33OverEntry[0] -= eutGammaExp.getCS33Over()[0];
	expCS33OverEntry[1] = std::sqrt(std::pow(eutAlphaExp.getCS33Over()[1], 2) + std::pow(eutGammaExp.getCS33Over()[1], 2));
	std::array<double, 2> refCS33OverEntry = refAlphaExp.getCS33Over();
	refCS33OverEntry[0] -= refGammaExp.getCS33Over()[0];
	refCS33OverEntry[1] = std::sqrt(std::pow(refAlphaExp.getCS33Over()[1], 2) + std::pow(refGammaExp.getCS33Over()[1], 2));


	expGraph->SetPoint(0, 1, expCS1Entry[0] / refCS1Entry[0]);
	expGraph->SetPointError(0, 0, std::sqrt(std::pow(expCS1Entry[1] / refCS1Entry[0], 2) + std::pow(expCS1Entry[0] * refCS1Entry[1] / std::pow(refCS1Entry[0], 2), 2)));
	expGraph->SetPoint(1, 2, expCS4TO32Entry[0] / refCS4TO32Entry[0]);
	expGraph->SetPointError(1, 0, std::sqrt(std::pow(expCS4TO32Entry[1] / refCS4TO32Entry[0], 2) + std::pow(expCS4TO32Entry[0] * refCS4TO32Entry[1] / std::pow(refCS4TO32Entry[0], 2), 2)));
	expGraph->SetPoint(2, 3, expCS33OverEntry[0] / refCS33OverEntry[0]);
	expGraph->SetPointError(2, 0, std::sqrt(std::pow(expCS33OverEntry[1] / refCS33OverEntry[0], 2) + std::pow(expCS33OverEntry[0] * refCS33OverEntry[1] / std::pow(refCS33OverEntry[0], 2), 2)));

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