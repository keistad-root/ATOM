#include<csv.h>
#include<array>
#include<vector>
#include<iostream>

#include "TGraph.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TMath.h"
#include "TF1.h"

#include "TExperimentInfo.h"
#include "TExperimentInfoSet.h"

#include "CppConfigFile.h"
#include "TPlotter.h"

void addExperimentData(const TExperimentInfoSet& expSet, TGraphErrors** graph, const int min, const int max) {
	std::vector<TExperimentInfo> refExpData;
	std::vector<TExperimentInfo> testExpData;

	for ( auto& exp : expSet.getExperimentSet() ) {
		std::string tag = exp.getTag();
		if ( tag.find("REF") != std::string::npos ) {
			refExpData.push_back(exp);
		} else {
			testExpData.push_back(exp);
		}
	}
	for ( auto& testExp : testExpData ) {
		if ( testExp.getPhi() == 2 ) {
			TExperimentInfo refL1;
			for ( auto& refExp : refExpData ) {
				if ( refExp.getLength() == testExp.getLength() ) {
					refL1 = refExp;
					break;
				}
			}
			std::array<double, 2> refL1Entry = refL1.getSubEntry(min, max);
			std::array<double, 2> testL1Entry = testExp.getSubEntry(min, max);
			double ratio = testL1Entry[0] / refL1Entry[0];
			double ratioError = sqrt(pow(testL1Entry[1] / refL1Entry[0], 2) + pow(testL1Entry[1] * refL1Entry[0] / pow(refL1Entry[0], 2), 2));
			graph[0]->SetPoint(graph[0]->GetN(), testExp.getCollimatorLength(), ratio);
			graph[0]->SetPointError(graph[0]->GetN() - 1, 0.05, ratioError);
		}
		if ( testExp.getPhi() == 3 ) {
			TExperimentInfo refL1;
			for ( auto& refExp : refExpData ) {
				if ( refExp.getLength() == testExp.getLength() ) {
					refL1 = refExp;
					break;
				}
			}
			std::array<double, 2> refL1Entry = refL1.getSubEntry(min, max);
			std::array<double, 2> testL1Entry = testExp.getSubEntry(min, max);
			double ratio = testL1Entry[0] / refL1Entry[0];
			double ratioError = sqrt(pow(testL1Entry[1] / refL1Entry[0], 2) + pow(testL1Entry[1] * refL1Entry[0] / pow(refL1Entry[0], 2), 2));
			graph[1]->SetPoint(graph[1]->GetN(), testExp.getCollimatorLength(), ratio);
			graph[1]->SetPointError(graph[1]->GetN() - 1, 0.05, ratioError);
		}
		if ( testExp.getPhi() == 4 ) {
			TExperimentInfo refL1;
			for ( auto& refExp : refExpData ) {
				if ( refExp.getLength() == testExp.getLength() ) {
					refL1 = refExp;
					break;
				}
			}
			std::array<double, 2> refL1Entry = refL1.getSubEntry(min, max);
			std::array<double, 2> testL1Entry = testExp.getSubEntry(min, max);
			double ratio = testL1Entry[0] / refL1Entry[0];
			double ratioError = sqrt(pow(testL1Entry[1] / refL1Entry[0], 2) + pow(testL1Entry[1] * refL1Entry[0] / pow(refL1Entry[0], 2), 2));
			graph[2]->SetPoint(graph[2]->GetN(), testExp.getCollimatorLength(), ratio);
			graph[2]->SetPointError(graph[2]->GetN() - 1, 0.05, ratioError);
		}
	}
}


int main() {
	CppConfigFile configFile("/home/ychoi/ATOM/config/comparison/Final_plot.conf");

	TExperimentInfoSet expSet;

	TGraphErrors* L1Graph[3] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};

	std::vector<TExperimentInfo> refExpData;
	std::vector<TExperimentInfo> testExpData;

	for ( auto& exp : expSet.getExperimentSet() ) {
		std::string tag = exp.getTag();
		if ( tag.find("REF") != std::string::npos ) {
			refExpData.push_back(exp);
		} else {
			testExpData.push_back(exp);
		}
	}

	for ( auto& testExp : testExpData ) {
		if ( testExp.getPhi() == 2 ) {
			TExperimentInfo refL1;
			for ( auto& refExp : refExpData ) {
				if ( refExp.getLength() == testExp.getLength() ) {
					refL1 = refExp;
					break;
				}
			}
			std::array<double, 2> refL1Entry = refL1.getSubEntry(1, 1);
			std::array<double, 2> testL1Entry = testExp.getSubEntry(1, 1);
			double ratio = testL1Entry[0] / refL1Entry[0];
			double ratioError = sqrt(pow(testL1Entry[1] / refL1Entry[0], 2) + pow(testL1Entry[1] * refL1Entry[0] / pow(refL1Entry[0], 2), 2));
			L1Graph[0]->SetPoint(L1Graph[0]->GetN(), testExp.getCollimatorLength(), ratio);
			L1Graph[0]->SetPointError(L1Graph[0]->GetN() - 1, 0.05, ratioError);
		}
		if ( testExp.getPhi() == 3 ) {
			TExperimentInfo refL1;
			for ( auto& refExp : refExpData ) {
				if ( refExp.getLength() == testExp.getLength() ) {
					refL1 = refExp;
					break;
				}
			}
			std::array<double, 2> refL1Entry = refL1.getSubEntry(1, 1);
			std::array<double, 2> testL1Entry = testExp.getSubEntry(1, 1);
			double ratio = testL1Entry[0] / refL1Entry[0];
			double ratioError = sqrt(pow(testL1Entry[1] / refL1Entry[0], 2) + pow(testL1Entry[1] * refL1Entry[0] / pow(refL1Entry[0], 2), 2));
			L1Graph[1]->SetPoint(L1Graph[1]->GetN(), testExp.getCollimatorLength(), ratio);
			L1Graph[1]->SetPointError(L1Graph[1]->GetN() - 1, 0.05, ratioError);
		}
		if ( testExp.getPhi() == 4 ) {
			TExperimentInfo refL1;
			for ( auto& refExp : refExpData ) {
				if ( refExp.getLength() == testExp.getLength() ) {
					refL1 = refExp;
					break;
				}
			}
			std::array<double, 2> refL1Entry = refL1.getSubEntry(1, 1);
			std::array<double, 2> testL1Entry = testExp.getSubEntry(1, 1);
			double ratio = testL1Entry[0] / refL1Entry[0];
			double ratioError = sqrt(pow(testL1Entry[1] / refL1Entry[0], 2) + pow(testL1Entry[1] * refL1Entry[0] / pow(refL1Entry[0], 2), 2));
			L1Graph[2]->SetPoint(L1Graph[2]->GetN(), testExp.getCollimatorLength(), ratio);
			L1Graph[2]->SetPointError(L1Graph[2]->GetN() - 1, 0.05, ratioError);
		}
	}

	TMultiGraph* mg = new TMultiGraph();
	L1Graph[0]->SetMaximum(0.5);
	L1Graph[0]->SetMarkerStyle(24);
	L1Graph[0]->SetMarkerSize(2);
	L1Graph[0]->SetMarkerColor(kRed);
	L1Graph[0]->SetLineColor(kRed);
	mg->Add(L1Graph[0]);
	L1Graph[1]->SetMarkerStyle(24);
	L1Graph[1]->SetMarkerSize(2);
	L1Graph[1]->SetMarkerColor(kBlue);
	L1Graph[1]->SetLineColor(kBlue);
	L1Graph[1]->Draw("P");
	mg->Add(L1Graph[1]);
	L1Graph[2]->SetMarkerStyle(24);
	L1Graph[2]->SetMarkerSize(2);
	L1Graph[2]->SetMarkerColor(kMagenta);
	L1Graph[2]->SetLineColor(kMagenta);
	L1Graph[2]->Draw("P");
	mg->Add(L1Graph[2]);

	TCanvas* canvas = new TCanvas("L1", "L1", 1000, 1000);
	TPlotter plotter;
	plotter.savePlot(canvas, mg, configFile.getConfig("CS1_VS_ELECTRON"));
	std::filesystem::path path = "/home/ychoi/ATOM/build/Data/";
	plotter.saveCanvas(canvas, path, configFile.getConfig("CS1_VS_ELECTRON"));

	return 0;
}

// void drawForLength(int drawWidth, std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>>& expData, std::vector<std::tuple<int, int, std::array<double, 4>>>& simData) {
// 	TGraphErrors* expGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
// 	TGraph* simGraphPhi[4] = {new TGraph(), new TGraph(), new TGraph(), new TGraph()};
// 	for ( auto& expDataEntry : expData ) {
// 		if ( std::get<1>(expDataEntry) == drawWidth ) {
// 			expGraphPhi2[0]->SetPoint(expGraphPhi2[0]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[0]->SetPointError(expGraphPhi2[0]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 			expGraphPhi2[1]->SetPoint(expGraphPhi2[1]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[1]);
// 			expGraphPhi2[1]->SetPointError(expGraphPhi2[1]->GetN() - 1, 0, std::get<3>(expDataEntry)[1]);
// 			expGraphPhi2[2]->SetPoint(expGraphPhi2[2]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[2]);
// 			expGraphPhi2[2]->SetPointError(expGraphPhi2[2]->GetN() - 1, 0, std::get<3>(expDataEntry)[2]);
// 			expGraphPhi2[3]->SetPoint(expGraphPhi2[3]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
// 			expGraphPhi2[3]->SetPointError(expGraphPhi2[3]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
// 		}
// 	}
// 	for ( auto& simDataEntry : simData ) {
// 		if ( std::get<1>(simDataEntry) == drawWidth ) {
// 			simGraphPhi[0]->SetPoint(simGraphPhi[0]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[0]);
// 			simGraphPhi[1]->SetPoint(simGraphPhi[1]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[1]);
// 			simGraphPhi[2]->SetPoint(simGraphPhi[2]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[2]);
// 			simGraphPhi[3]->SetPoint(simGraphPhi[3]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[3]);
// 		}
// 	}

// 	TCanvas* canvasPhi = new TCanvas("canvasphi", "", 1000, 1000);
// 	TMultiGraph* mgPhi = new TMultiGraph();
// 	expGraphPhi2[0]->SetLineColor(kRed);
// 	expGraphPhi2[0]->SetLineWidth(2);
// 	expGraphPhi2[0]->SetMarkerColor(kRed);
// 	expGraphPhi2[0]->SetMarkerStyle(24);
// 	expGraphPhi2[0]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[0]);
// 	expGraphPhi2[1]->SetLineColor(kBlue);
// 	expGraphPhi2[1]->SetLineWidth(2);
// 	expGraphPhi2[1]->SetMarkerColor(kBlue);
// 	expGraphPhi2[1]->SetMarkerStyle(24);
// 	expGraphPhi2[1]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[1]);
// 	expGraphPhi2[2]->SetLineColor(kMagenta);
// 	expGraphPhi2[2]->SetLineWidth(2);
// 	expGraphPhi2[2]->SetMarkerColor(kMagenta);
// 	expGraphPhi2[2]->SetMarkerStyle(24);
// 	expGraphPhi2[2]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[2]);
// 	expGraphPhi2[3]->SetLineColor(kGreen + 3);
// 	expGraphPhi2[3]->SetLineWidth(2);
// 	expGraphPhi2[3]->SetMarkerColor(kGreen + 3);
// 	expGraphPhi2[3]->SetMarkerStyle(24);
// 	expGraphPhi2[3]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[3]);
// 	mgPhi->SetTitle(static_cast<TString>("Comparison for " + std::to_string(drawWidth) + "#phi collimators; Length[mm]; Ratio to Reference"));
// 	mgPhi->Draw("AP");

// 	TMultiGraph* mgSimPhi = new TMultiGraph();
// 	simGraphPhi[0]->SetLineColor(kBlue);
// 	simGraphPhi[0]->SetLineWidth(2);
// 	simGraphPhi[0]->SetLineStyle(9);
// 	simGraphPhi[0]->SetMarkerColor(kBlue);
// 	simGraphPhi[0]->SetMarkerSize(2);
// 	simGraphPhi[0]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[0]);
// 	simGraphPhi[1]->SetLineColor(kRed);
// 	simGraphPhi[1]->SetLineWidth(2);
// 	simGraphPhi[1]->SetLineStyle(9);
// 	simGraphPhi[1]->SetMarkerColor(kRed);
// 	simGraphPhi[1]->SetMarkerSize(2);
// 	simGraphPhi[1]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[1]);
// 	simGraphPhi[2]->SetLineColor(kMagenta);
// 	simGraphPhi[2]->SetLineWidth(2);
// 	simGraphPhi[2]->SetLineStyle(9);
// 	simGraphPhi[2]->SetMarkerColor(kMagenta);
// 	simGraphPhi[2]->SetMarkerSize(2);
// 	simGraphPhi[2]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[2]);
// 	simGraphPhi[3]->SetLineColor(kGreen + 3);
// 	simGraphPhi[3]->SetLineWidth(2);
// 	simGraphPhi[3]->SetLineStyle(9);
// 	simGraphPhi[3]->SetMarkerColor(kGreen + 3);
// 	simGraphPhi[3]->SetMarkerSize(2);
// 	simGraphPhi[3]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[3]);
// 	mgSimPhi->Draw("P");

// 	TLegend* legendPhi = new TLegend(0.3, 0.6, 0.8, 0.9);
// 	legendPhi->AddEntry(expGraphPhi2[0], "Region A", "p");
// 	legendPhi->AddEntry(expGraphPhi2[1], "Region B", "p");
// 	legendPhi->AddEntry(expGraphPhi2[2], "Region C", "p");
// 	legendPhi->AddEntry(expGraphPhi2[3], "Region D", "p");
// 	legendPhi->AddEntry(simGraphPhi[1], "The # of electron in metal (Simulation)", "p");
// 	legendPhi->AddEntry(simGraphPhi[0], "The # of single alpha in metal (Simulation)", "p");
// 	legendPhi->AddEntry(simGraphPhi[2], "The # of single alpha in epitaxial (Simulation)", "p");
// 	legendPhi->AddEntry(simGraphPhi[3], "The # of double cluster (Simulation)", "p");
// 	legendPhi->Draw("SAME");

// 	canvasPhi->SetLeftMargin(0.12);
// 	canvasPhi->SetGrid();
// 	canvasPhi->SaveAs(static_cast<TString>("phi" + std::to_string(drawWidth) + "_ratio_to_reference.png"));
// 	delete canvasPhi;
// }

// void drawOnlyBC(int drawWidth, std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>>& expData, std::vector<std::tuple<int, int, std::array<double, 4>>>& simData) {
// 	TGraphErrors* expGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
// 	TGraph* simGraphPhi[4] = {new TGraph(), new TGraph(), new TGraph(), new TGraph()};
// 	for ( auto& expDataEntry : expData ) {
// 		if ( std::get<1>(expDataEntry) == drawWidth ) {
// 			expGraphPhi2[0]->SetPoint(expGraphPhi2[0]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[0]->SetPointError(expGraphPhi2[0]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 			expGraphPhi2[1]->SetPoint(expGraphPhi2[1]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[1]);
// 			expGraphPhi2[1]->SetPointError(expGraphPhi2[1]->GetN() - 1, 0, std::get<3>(expDataEntry)[1]);
// 			expGraphPhi2[2]->SetPoint(expGraphPhi2[2]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[2]);
// 			expGraphPhi2[2]->SetPointError(expGraphPhi2[2]->GetN() - 1, 0, std::get<3>(expDataEntry)[2]);
// 			expGraphPhi2[3]->SetPoint(expGraphPhi2[3]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
// 			expGraphPhi2[3]->SetPointError(expGraphPhi2[3]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
// 		}
// 	}
// 	for ( auto& simDataEntry : simData ) {
// 		if ( std::get<1>(simDataEntry) == drawWidth ) {
// 			// simGraphPhi[0]->SetPoint(simGraphPhi[0]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[0]);
// 			// simGraphPhi[1]->SetPoint(simGraphPhi[1]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[1]);
// 			simGraphPhi[2]->SetPoint(simGraphPhi[2]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[2]);
// 			// simGraphPhi[3]->SetPoint(simGraphPhi[3]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[3]);
// 		}
// 	}

// 	TCanvas* canvasPhi = new TCanvas("canvasphi", "", 1000, 1000);
// 	TMultiGraph* mgPhi = new TMultiGraph();
// 	// expGraphPhi2[0]->SetLineColor(kRed);
// 	// expGraphPhi2[0]->SetLineWidth(2);
// 	// expGraphPhi2[0]->SetMarkerColor(kRed);
// 	// expGraphPhi2[0]->SetMarkerStyle(24);
// 	// expGraphPhi2[0]->SetMarkerSize(2);
// 	// mgPhi->Add(expGraphPhi2[0]);
// 	expGraphPhi2[1]->SetLineColor(kBlue);
// 	expGraphPhi2[1]->SetLineWidth(2);
// 	expGraphPhi2[1]->SetMarkerColor(kBlue);
// 	expGraphPhi2[1]->SetMarkerStyle(24);
// 	expGraphPhi2[1]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[1]);
// 	expGraphPhi2[2]->SetLineColor(kMagenta);
// 	expGraphPhi2[2]->SetLineWidth(2);
// 	expGraphPhi2[2]->SetMarkerColor(kMagenta);
// 	expGraphPhi2[2]->SetMarkerStyle(24);
// 	expGraphPhi2[2]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[2]);
// 	expGraphPhi2[3]->SetLineColor(kGreen + 3);
// 	expGraphPhi2[3]->SetLineWidth(2);
// 	expGraphPhi2[3]->SetMarkerColor(kGreen + 3);
// 	expGraphPhi2[3]->SetMarkerStyle(24);
// 	expGraphPhi2[3]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[3]);
// 	mgPhi->SetTitle(static_cast<TString>("Comparison for " + std::to_string(drawWidth) + "#phi collimators; Length[mm]; Ratio to Reference"));
// 	// mgPhi->SetMinimum(0);
// 	mgPhi->Draw("AP");

// 	TMultiGraph* mgSimPhi = new TMultiGraph();
// 	// simGraphPhi[0]->SetLineColor(kBlue);
// 	// simGraphPhi[0]->SetLineWidth(2);
// 	// simGraphPhi[0]->SetLineStyle(9);
// 	// simGraphPhi[0]->SetMarkerColor(kBlue);
// 	// simGraphPhi[0]->SetMarkerSize(2);
// 	// simGraphPhi[0]->SetMarkerStyle(21);
// 	// mgSimPhi->Add(simGraphPhi[0]);
// 	// simGraphPhi[1]->SetLineColor(kRed);
// 	// simGraphPhi[1]->SetLineWidth(2);
// 	// simGraphPhi[1]->SetLineStyle(9);
// 	// simGraphPhi[1]->SetMarkerColor(kRed);
// 	// simGraphPhi[1]->SetMarkerSize(2);
// 	// simGraphPhi[1]->SetMarkerStyle(21);
// 	// mgSimPhi->Add(simGraphPhi[1]);
// 	simGraphPhi[2]->SetLineColor(kMagenta);
// 	simGraphPhi[2]->SetLineWidth(2);
// 	simGraphPhi[2]->SetLineStyle(9);
// 	simGraphPhi[2]->SetMarkerColor(kMagenta);
// 	simGraphPhi[2]->SetMarkerSize(2);
// 	simGraphPhi[2]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[2]);
// 	// simGraphPhi[3]->SetLineColor(kGreen + 3);
// 	// simGraphPhi[3]->SetLineWidth(2);
// 	// simGraphPhi[3]->SetLineStyle(9);
// 	// simGraphPhi[3]->SetMarkerColor(kGreen + 3);
// 	// simGraphPhi[3]->SetMarkerSize(2);
// 	// simGraphPhi[3]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[3]);
// 	mgSimPhi->Draw("P");

// 	TLegend* legendPhi = new TLegend(0.3, 0.7, 0.9, 0.9);
// 	// legendPhi->AddEntry(expGraphPhi2[0], "Region B + C + cluster size 4", "p");
// 	legendPhi->AddEntry(expGraphPhi2[1], "Region B", "p");
// 	legendPhi->AddEntry(expGraphPhi2[2], "Region C", "p");
// 	legendPhi->AddEntry(expGraphPhi2[3], "Region B + C", "p");
// 	// legendPhi->AddEntry(simGraphPhi[1], "The # of electron in metal (Simulation)", "p");
// 	// legendPhi->AddEntry(simGraphPhi[0], "The # of single alpha in metal (Simulation)", "p");
// 	legendPhi->AddEntry(simGraphPhi[2], "The # of single alpha in epitaxial (Simulation)", "p");
// 	// legendPhi->AddEntry(simGraphPhi[3], "The # of double cluster (Simulation)", "p");
// 	legendPhi->Draw("SAME");

// 	canvasPhi->SetLeftMargin(0.13);
// 	canvasPhi->SetGrid();
// 	canvasPhi->SaveAs(static_cast<TString>("phi" + std::to_string(drawWidth) + "_ratio_to_reference_BC.png"));
// 	delete canvasPhi;
// }


// void drawOnlyA(int drawWidth, std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>>& expData, std::vector<std::tuple<int, int, std::array<double, 4>>>& simData) {
// 	TGraphErrors* expGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
// 	TGraph* simGraphPhi[4] = {new TGraph(), new TGraph(), new TGraph(), new TGraph()};
// 	for ( auto& expDataEntry : expData ) {
// 		if ( std::get<1>(expDataEntry) == drawWidth ) {
// 			expGraphPhi2[0]->SetPoint(expGraphPhi2[0]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[0]->SetPointError(expGraphPhi2[0]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 			expGraphPhi2[1]->SetPoint(expGraphPhi2[1]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[1]);
// 			expGraphPhi2[1]->SetPointError(expGraphPhi2[1]->GetN() - 1, 0, std::get<3>(expDataEntry)[1]);
// 			expGraphPhi2[2]->SetPoint(expGraphPhi2[2]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[2]);
// 			expGraphPhi2[2]->SetPointError(expGraphPhi2[2]->GetN() - 1, 0, std::get<3>(expDataEntry)[2]);
// 			expGraphPhi2[3]->SetPoint(expGraphPhi2[3]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
// 			expGraphPhi2[3]->SetPointError(expGraphPhi2[3]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
// 		}
// 	}
// 	for ( auto& simDataEntry : simData ) {
// 		if ( std::get<1>(simDataEntry) == drawWidth ) {
// 			// simGraphPhi[0]->SetPoint(simGraphPhi[0]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[0]);
// 			simGraphPhi[1]->SetPoint(simGraphPhi[1]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[1]);
// 			// simGraphPhi[2]->SetPoint(simGraphPhi[2]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[2]);
// 			// simGraphPhi[3]->SetPoint(simGraphPhi[3]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[3]);
// 		}
// 	}

// 	TCanvas* canvasPhi = new TCanvas("canvasphi", "", 1000, 1000);
// 	TMultiGraph* mgPhi = new TMultiGraph();
// 	expGraphPhi2[0]->SetLineColor(kRed);
// 	expGraphPhi2[0]->SetLineWidth(2);
// 	expGraphPhi2[0]->SetMarkerColor(kRed);
// 	expGraphPhi2[0]->SetMarkerStyle(24);
// 	expGraphPhi2[0]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[0]);
// 	// expGraphPhi2[1]->SetLineColor(kBlue);
// 	// expGraphPhi2[1]->SetLineWidth(2);
// 	// expGraphPhi2[1]->SetMarkerColor(kBlue);
// 	// expGraphPhi2[1]->SetMarkerStyle(24);
// 	// expGraphPhi2[1]->SetMarkerSize(2);
// 	// mgPhi->Add(expGraphPhi2[1]);
// 	// expGraphPhi2[2]->SetLineColor(kMagenta);
// 	// expGraphPhi2[2]->SetLineWidth(2);
// 	// expGraphPhi2[2]->SetMarkerColor(kMagenta);
// 	// expGraphPhi2[2]->SetMarkerStyle(24);
// 	// expGraphPhi2[2]->SetMarkerSize(2);
// 	// mgPhi->Add(expGraphPhi2[2]);
// 	// expGraphPhi2[3]->SetLineColor(kGreen + 3);
// 	// expGraphPhi2[3]->SetLineWidth(2);
// 	// expGraphPhi2[3]->SetMarkerColor(kGreen + 3);
// 	// expGraphPhi2[3]->SetMarkerStyle(24);
// 	// expGraphPhi2[3]->SetMarkerSize(2);
// 	// mgPhi->Add(expGraphPhi2[3]);
// 	mgPhi->SetTitle(static_cast<TString>("Comparison for " + std::to_string(drawWidth) + "#phi collimators; Length[mm]; Ratio to Reference"));
// 	mgPhi->SetMinimum(0);
// 	mgPhi->SetMaximum(.8);
// 	mgPhi->Draw("AP");

// 	TMultiGraph* mgSimPhi = new TMultiGraph();
// 	// simGraphPhi[0]->SetLineColor(kBlue);
// 	// simGraphPhi[0]->SetLineWidth(2);
// 	// simGraphPhi[0]->SetLineStyle(9);
// 	// simGraphPhi[0]->SetMarkerColor(kBlue);
// 	// simGraphPhi[0]->SetMarkerSize(2);
// 	// simGraphPhi[0]->SetMarkerStyle(21);
// 	// mgSimPhi->Add(simGraphPhi[0]);
// 	simGraphPhi[1]->SetLineColor(kRed);
// 	simGraphPhi[1]->SetLineWidth(2);
// 	simGraphPhi[1]->SetLineStyle(9);
// 	simGraphPhi[1]->SetMarkerColor(kRed);
// 	simGraphPhi[1]->SetMarkerSize(2);
// 	simGraphPhi[1]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[1]);
// 	// simGraphPhi[2]->SetLineColor(kMagenta);
// 	// simGraphPhi[2]->SetLineWidth(2);
// 	// simGraphPhi[2]->SetLineStyle(9);
// 	// simGraphPhi[2]->SetMarkerColor(kMagenta);
// 	// simGraphPhi[2]->SetMarkerSize(2);
// 	// simGraphPhi[2]->SetMarkerStyle(21);
// 	// mgSimPhi->Add(simGraphPhi[2]);
// 	// simGraphPhi[3]->SetLineColor(kGreen + 3);
// 	// simGraphPhi[3]->SetLineWidth(2);
// 	// simGraphPhi[3]->SetLineStyle(9);
// 	// simGraphPhi[3]->SetMarkerColor(kGreen + 3);
// 	// simGraphPhi[3]->SetMarkerSize(2);
// 	// simGraphPhi[3]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[3]);
// 	mgSimPhi->Draw("P");

// 	TLegend* legendPhi = new TLegend(0.3, 0.6, 0.8, 0.9);
// 	legendPhi->AddEntry(expGraphPhi2[0], "Cluster size 1", "p");
// 	// legendPhi->AddEntry(expGraphPhi2[1], "Region B", "p");
// 	// legendPhi->AddEntry(expGraphPhi2[2], "Region C", "p");
// 	// legendPhi->AddEntry(expGraphPhi2[3], "Region B + C", "p");
// 	legendPhi->AddEntry(simGraphPhi[1], "The # of electron in metal (Simulation)", "p");
// 	// legendPhi->AddEntry(simGraphPhi[0], "The # of single alpha in metal (Simulation)", "p");
// 	// legendPhi->AddEntry(simGraphPhi[2], "The # of single alpha in epitaxial (Simulation)", "p");
// 	// legendPhi->AddEntry(simGraphPhi[3], "The # of double cluster (Simulation)", "p");
// 	legendPhi->Draw("SAME");

// 	canvasPhi->SetLeftMargin(0.12);
// 	canvasPhi->SetGrid();
// 	canvasPhi->SaveAs(static_cast<TString>("phi" + std::to_string(drawWidth) + "_ratio_to_reference_1.png"));
// 	delete canvasPhi;
// }


// void drawOnly4(int drawWidth, std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>>& expData, std::vector<std::tuple<int, int, std::array<double, 4>>>& simData) {
// 	TGraphErrors* expGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
// 	TGraph* simGraphPhi[4] = {new TGraph(), new TGraph(), new TGraph(), new TGraph()};
// 	for ( auto& expDataEntry : expData ) {
// 		if ( std::get<1>(expDataEntry) == drawWidth ) {
// 			expGraphPhi2[0]->SetPoint(expGraphPhi2[0]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[0]->SetPointError(expGraphPhi2[0]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 			expGraphPhi2[1]->SetPoint(expGraphPhi2[1]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[1]);
// 			expGraphPhi2[1]->SetPointError(expGraphPhi2[1]->GetN() - 1, 0, std::get<3>(expDataEntry)[1]);
// 			expGraphPhi2[2]->SetPoint(expGraphPhi2[2]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[2]);
// 			expGraphPhi2[2]->SetPointError(expGraphPhi2[2]->GetN() - 1, 0, std::get<3>(expDataEntry)[2]);
// 			expGraphPhi2[3]->SetPoint(expGraphPhi2[3]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
// 			expGraphPhi2[3]->SetPointError(expGraphPhi2[3]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
// 		}
// 	}

// 	TCanvas* canvasPhi = new TCanvas("canvasphi", "", 1000, 1000);
// 	TMultiGraph* mgPhi = new TMultiGraph();
// 	expGraphPhi2[0]->SetLineColor(kRed);
// 	expGraphPhi2[0]->SetLineWidth(2);
// 	expGraphPhi2[0]->SetMarkerColor(kRed);
// 	expGraphPhi2[0]->SetMarkerStyle(24);
// 	expGraphPhi2[0]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[0]);
// 	// expGraphPhi2[1]->SetLineColor(kBlue);
// 	// expGraphPhi2[1]->SetLineWidth(2);
// 	// expGraphPhi2[1]->SetMarkerColor(kBlue);
// 	// expGraphPhi2[1]->SetMarkerStyle(24);
// 	// expGraphPhi2[1]->SetMarkerSize(2);
// 	// mgPhi->Add(expGraphPhi2[1]);
// 	// expGraphPhi2[2]->SetLineColor(kMagenta);
// 	// expGraphPhi2[2]->SetLineWidth(2);
// 	// expGraphPhi2[2]->SetMarkerColor(kMagenta);
// 	// expGraphPhi2[2]->SetMarkerStyle(24);
// 	// expGraphPhi2[2]->SetMarkerSize(2);
// 	// mgPhi->Add(expGraphPhi2[2]);
// 	// expGraphPhi2[3]->SetLineColor(kGreen + 3);
// 	// expGraphPhi2[3]->SetLineWidth(2);
// 	// expGraphPhi2[3]->SetMarkerColor(kGreen + 3);
// 	// expGraphPhi2[3]->SetMarkerStyle(24);
// 	// expGraphPhi2[3]->SetMarkerSize(2);
// 	// mgPhi->Add(expGraphPhi2[3]);
// 	mgPhi->SetTitle(static_cast<TString>("Comparison for " + std::to_string(drawWidth) + "#phi collimators; Length[mm]; Ratio to Reference"));
// 	mgPhi->Draw("AP");

// 	TLegend* legendPhi = new TLegend(0.3, 0.6, 0.8, 0.9);
// 	legendPhi->AddEntry(expGraphPhi2[0], "Cluster size 4", "p");
// 	// legendPhi->AddEntry(expGraphPhi2[1], "Region B", "p");
// 	// legendPhi->AddEntry(expGraphPhi2[2], "Region C", "p");
// 	// legendPhi->AddEntry(expGraphPhi2[3], "Region B + C", "p");
// 	// legendPhi->AddEntry(simGraphPhi[1], "The # of electron in metal (Simulation)", "p");
// 	// legendPhi->AddEntry(simGraphPhi[0], "The # of single alpha in metal (Simulation)", "p");
// 	// legendPhi->AddEntry(simGraphPhi[2], "The # of single alpha in epitaxial (Simulation)", "p");
// 	// legendPhi->AddEntry(simGraphPhi[3], "The # of double cluster (Simulation)", "p");
// 	legendPhi->Draw("SAME");

// 	canvasPhi->SetLeftMargin(0.12);
// 	canvasPhi->SetGrid();
// 	canvasPhi->SaveAs(static_cast<TString>("phi" + std::to_string(drawWidth) + "_ratio_to_reference_4.png"));
// 	delete canvasPhi;
// }

// void drawOnly1All(std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>>& expData, std::vector<std::tuple<int, int, std::array<double, 4>>> simData) {
// 	TGraphErrors* expGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
// 	TGraph* simGraphPhi[4] = {new TGraph(), new TGraph(), new TGraph(), new TGraph()};
// 	for ( auto& expDataEntry : expData ) {
// 		if ( std::get<1>(expDataEntry) == 2 ) {
// 			expGraphPhi2[0]->SetPoint(expGraphPhi2[0]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[0]->SetPointError(expGraphPhi2[0]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 		} else if ( std::get<1>(expDataEntry) == 3 ) {
// 			expGraphPhi2[1]->SetPoint(expGraphPhi2[1]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[1]->SetPointError(expGraphPhi2[1]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 		} else if ( std::get<1>(expDataEntry) == 4 ) {
// 			expGraphPhi2[2]->SetPoint(expGraphPhi2[2]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[2]->SetPointError(expGraphPhi2[2]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 		} else if ( std::get<1>(expDataEntry) == 7 ) {
// 			expGraphPhi2[3]->SetPoint(expGraphPhi2[3]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[3]->SetPointError(expGraphPhi2[3]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 		}
// 	}
// 	for ( auto& simDataEntry : simData ) {
// 		if ( std::get<1>(simDataEntry) == 2 ) {
// 			simGraphPhi[0]->SetPoint(simGraphPhi[0]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[1]);
// 		} else if ( std::get<1>(simDataEntry) == 3 ) {
// 			simGraphPhi[1]->SetPoint(simGraphPhi[1]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[1]);
// 		} else if ( std::get<1>(simDataEntry) == 4 ) {
// 			simGraphPhi[2]->SetPoint(simGraphPhi[2]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[1]);
// 		} else if ( std::get<1>(simDataEntry) == 7 ) {
// 			simGraphPhi[3]->SetPoint(simGraphPhi[3]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[1]);
// 		}
// 	}
// 	TCanvas* canvasPhi = new TCanvas("canvasphi", "", 1000, 1000);
// 	TMultiGraph* mgPhi = new TMultiGraph();
// 	expGraphPhi2[0]->SetLineColor(kRed);
// 	expGraphPhi2[0]->SetLineWidth(2);
// 	expGraphPhi2[0]->SetMarkerColor(kRed);
// 	expGraphPhi2[0]->SetMarkerStyle(24);
// 	expGraphPhi2[0]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[0]);
// 	expGraphPhi2[1]->SetLineColor(kBlue);
// 	expGraphPhi2[1]->SetLineWidth(2);
// 	expGraphPhi2[1]->SetMarkerColor(kBlue);
// 	expGraphPhi2[1]->SetMarkerStyle(24);
// 	expGraphPhi2[1]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[1]);
// 	expGraphPhi2[2]->SetLineColor(kMagenta);
// 	expGraphPhi2[2]->SetLineWidth(2);
// 	expGraphPhi2[2]->SetMarkerColor(kMagenta);
// 	expGraphPhi2[2]->SetMarkerStyle(24);
// 	expGraphPhi2[2]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[2]);
// 	expGraphPhi2[3]->SetLineColor(kGreen + 3);
// 	expGraphPhi2[3]->SetLineWidth(2);
// 	expGraphPhi2[3]->SetMarkerColor(kGreen + 3);
// 	expGraphPhi2[3]->SetMarkerStyle(24);
// 	expGraphPhi2[3]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[3]);
// 	mgPhi->SetTitle(static_cast<TString>("Cluster Size 1 vs. Electrons with full energy deposit in metal layer; Length [mm]; Ratio to Reference"));
// 	mgPhi->SetMaximum(0.8);
// 	mgPhi->SetMinimum(0);
// 	mgPhi->Draw("AP");

// 	TMultiGraph* mgSimPhi = new TMultiGraph();
// 	simGraphPhi[0]->SetLineColor(kRed);
// 	simGraphPhi[0]->SetLineWidth(2);
// 	simGraphPhi[0]->SetLineStyle(9);
// 	simGraphPhi[0]->SetMarkerColor(kRed);
// 	simGraphPhi[0]->SetMarkerSize(2);
// 	simGraphPhi[0]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[0]);
// 	simGraphPhi[1]->SetLineColor(kBlue);
// 	simGraphPhi[1]->SetLineWidth(2);
// 	simGraphPhi[1]->SetLineStyle(9);
// 	simGraphPhi[1]->SetMarkerColor(kBlue);
// 	simGraphPhi[1]->SetMarkerSize(2);
// 	simGraphPhi[1]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[1]);
// 	simGraphPhi[2]->SetLineColor(kMagenta);
// 	simGraphPhi[2]->SetLineWidth(2);
// 	simGraphPhi[2]->SetLineStyle(9);
// 	simGraphPhi[2]->SetMarkerColor(kMagenta);
// 	simGraphPhi[2]->SetMarkerSize(2);
// 	simGraphPhi[2]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[2]);
// 	simGraphPhi[3]->SetLineColor(kGreen + 3);
// 	simGraphPhi[3]->SetLineWidth(2);
// 	simGraphPhi[3]->SetLineStyle(9);
// 	simGraphPhi[3]->SetMarkerColor(kGreen + 3);
// 	simGraphPhi[3]->SetMarkerSize(2);
// 	simGraphPhi[3]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[3]);
// 	mgSimPhi->Draw("P");

// 	TF1* fitFunc[4] = {new TF1("fitFunc1", "[0] / (x - [1]) + [2]", 0, 20), new TF1("fitFunc2", "[0] / (x - [1]) + [2]", 0, 20), new TF1("fitFunc3", "[0] / (x - [1]) + [2]", 0, 20), new TF1("fitFunc4", "[0] / (x - [1]) + [2]", 0, 20)};
// 	simGraphPhi[0]->Fit(fitFunc[0], "R");
// 	fitFunc[0]->SetLineColor(kRed);
// 	simGraphPhi[1]->Fit(fitFunc[1], "R");
// 	fitFunc[1]->SetLineColor(kBlue);
// 	simGraphPhi[2]->Fit(fitFunc[2], "R");
// 	fitFunc[2]->SetLineColor(kMagenta);
// 	simGraphPhi[3]->Fit(fitFunc[3], "R");
// 	fitFunc[3]->SetLineColor(kGreen + 3);
// 	fitFunc[0]->Draw("SAME");
// 	fitFunc[1]->Draw("SAME");
// 	fitFunc[2]->Draw("SAME");
// 	fitFunc[3]->Draw("SAME");

// 	TLegend* legendPhi = new TLegend(0.5, 0.7, 0.9, 0.9);
// 	legendPhi->SetNColumns(2);
// 	legendPhi->AddEntry(expGraphPhi2[0], "Data (#phi2)", "p");
// 	legendPhi->AddEntry(simGraphPhi[0], "GEANT4 (#phi2)", "p");
// 	legendPhi->AddEntry(expGraphPhi2[1], "Data (#phi3)", "p");
// 	legendPhi->AddEntry(simGraphPhi[1], "GEANT4 (#phi3)", "p");
// 	legendPhi->AddEntry(expGraphPhi2[2], "Data (#phi4)", "p");
// 	legendPhi->AddEntry(simGraphPhi[2], "GEANT4 (#phi4)", "p");
// 	legendPhi->AddEntry(expGraphPhi2[3], "Data (#phi7)", "p");
// 	legendPhi->AddEntry(simGraphPhi[3], "GEANT4 (#phi7)", "p");
// 	legendPhi->Draw("SAME");

// 	canvasPhi->SetLeftMargin(0.12);
// 	canvasPhi->SetGrid();
// 	canvasPhi->SaveAs(static_cast<TString>("ratio_to_reference_cluster_size_1.png"));
// }

// void drawOnly4All(std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>>& expData) {
// 	TGraphErrors* expGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
// 	for ( auto& expDataEntry : expData ) {
// 		if ( std::get<1>(expDataEntry) == 2 ) {
// 			expGraphPhi2[0]->SetPoint(expGraphPhi2[0]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[0]->SetPointError(expGraphPhi2[0]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 		} else if ( std::get<1>(expDataEntry) == 3 ) {
// 			expGraphPhi2[1]->SetPoint(expGraphPhi2[1]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[1]->SetPointError(expGraphPhi2[1]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 		} else if ( std::get<1>(expDataEntry) == 4 ) {
// 			expGraphPhi2[2]->SetPoint(expGraphPhi2[2]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[2]->SetPointError(expGraphPhi2[2]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 		} else if ( std::get<1>(expDataEntry) == 7 ) {
// 			expGraphPhi2[3]->SetPoint(expGraphPhi2[3]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[3]->SetPointError(expGraphPhi2[3]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 		}

// 	}

// 	TCanvas* canvasPhi = new TCanvas("canvasphi", "", 1000, 1000);
// 	TMultiGraph* mgPhi = new TMultiGraph();
// 	expGraphPhi2[0]->SetLineColor(kRed);
// 	expGraphPhi2[0]->SetLineWidth(2);
// 	expGraphPhi2[0]->SetMarkerColor(kRed);
// 	expGraphPhi2[0]->SetMarkerStyle(24);
// 	expGraphPhi2[0]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[0]);
// 	expGraphPhi2[1]->SetLineColor(kBlue);
// 	expGraphPhi2[1]->SetLineWidth(2);
// 	expGraphPhi2[1]->SetMarkerColor(kBlue);
// 	expGraphPhi2[1]->SetMarkerStyle(24);
// 	expGraphPhi2[1]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[1]);
// 	expGraphPhi2[2]->SetLineColor(kMagenta);
// 	expGraphPhi2[2]->SetLineWidth(2);
// 	expGraphPhi2[2]->SetMarkerColor(kMagenta);
// 	expGraphPhi2[2]->SetMarkerStyle(24);
// 	expGraphPhi2[2]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[2]);
// 	expGraphPhi2[3]->SetLineColor(kGreen + 3);
// 	expGraphPhi2[3]->SetLineWidth(2);
// 	expGraphPhi2[3]->SetMarkerColor(kGreen + 3);
// 	expGraphPhi2[3]->SetMarkerStyle(24);
// 	expGraphPhi2[3]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[3]);
// 	mgPhi->SetTitle(static_cast<TString>("Cluster Size 4; Length [mm]; Ratio to Reference"));
// 	mgPhi->Draw("AP");

// 	TLegend* legendPhi = new TLegend(0.5, 0.7, 0.9, 0.9);
// 	legendPhi->AddEntry(expGraphPhi2[0], "Data (#phi2)", "p");
// 	legendPhi->AddEntry(expGraphPhi2[1], "Data (#phi3)", "p");
// 	legendPhi->AddEntry(expGraphPhi2[2], "Data (#phi4)", "p");
// 	legendPhi->AddEntry(expGraphPhi2[3], "Data (#phi7)", "p");
// 	legendPhi->Draw("SAME");

// 	canvasPhi->SetLeftMargin(0.12);
// 	canvasPhi->SetGrid();
// 	canvasPhi->SaveAs(static_cast<TString>("ratio_to_reference_cluster_size_4.png"));
// 	delete canvasPhi;
// }

// void drawDoubleCluster(std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>>& expData, std::vector<std::tuple<int, int, std::array<double, 4>>>& simData) {
// 	TGraphErrors* expGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
// 	TGraphErrors* simGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
// 	for ( auto& expDataEntry : expData ) {
// 		if ( std::get<1>(expDataEntry) == 2 ) {
// 			expGraphPhi2[0]->SetPoint(expGraphPhi2[0]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
// 			expGraphPhi2[0]->SetPointError(expGraphPhi2[0]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
// 		} else if ( std::get<1>(expDataEntry) == 3 ) {
// 			expGraphPhi2[1]->SetPoint(expGraphPhi2[1]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
// 			expGraphPhi2[1]->SetPointError(expGraphPhi2[1]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
// 		} else if ( std::get<1>(expDataEntry) == 4 ) {
// 			expGraphPhi2[2]->SetPoint(expGraphPhi2[2]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
// 			expGraphPhi2[2]->SetPointError(expGraphPhi2[2]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
// 		} else if ( std::get<1>(expDataEntry) == 7 ) {
// 			expGraphPhi2[3]->SetPoint(expGraphPhi2[3]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
// 			expGraphPhi2[3]->SetPointError(expGraphPhi2[3]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
// 		}

// 	}
// 	for ( auto& simDataEntry : simData ) {
// 		if ( std::get<1>(simDataEntry) == 2 ) {
// 			simGraphPhi2[0]->SetPoint(simGraphPhi2[0]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[3]);
// 		} else if ( std::get<1>(simDataEntry) == 3 ) {
// 			simGraphPhi2[1]->SetPoint(simGraphPhi2[1]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[3]);
// 		} else if ( std::get<1>(simDataEntry) == 4 ) {
// 			simGraphPhi2[2]->SetPoint(simGraphPhi2[2]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[3]);
// 		} else if ( std::get<1>(simDataEntry) == 7 ) {
// 			simGraphPhi2[3]->SetPoint(simGraphPhi2[3]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[3]);
// 		}
// 	}

// 	TCanvas* canvasPhi = new TCanvas("canvasphi", "", 1000, 1000);
// 	TMultiGraph* mgPhi = new TMultiGraph();
// 	expGraphPhi2[0]->SetLineColor(kRed);
// 	expGraphPhi2[0]->SetLineWidth(2);
// 	expGraphPhi2[0]->SetMarkerColor(kRed);
// 	expGraphPhi2[0]->SetMarkerStyle(24);
// 	expGraphPhi2[0]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[0]);
// 	expGraphPhi2[1]->SetLineColor(kBlue);
// 	expGraphPhi2[1]->SetLineWidth(2);
// 	expGraphPhi2[1]->SetMarkerColor(kBlue);
// 	expGraphPhi2[1]->SetMarkerStyle(24);
// 	expGraphPhi2[1]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[1]);
// 	expGraphPhi2[2]->SetLineColor(kMagenta);
// 	expGraphPhi2[2]->SetLineWidth(2);
// 	expGraphPhi2[2]->SetMarkerColor(kMagenta);
// 	expGraphPhi2[2]->SetMarkerStyle(24);
// 	expGraphPhi2[2]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[2]);
// 	// expGraphPhi2[3]->SetLineColor(kGreen + 3);
// 	// expGraphPhi2[3]->SetLineWidth(2);
// 	// expGraphPhi2[3]->SetMarkerColor(kGreen + 3);
// 	// expGraphPhi2[3]->SetMarkerStyle(24);
// 	// expGraphPhi2[3]->SetMarkerSize(2);
// 	// mgPhi->Add(expGraphPhi2[3]);
// 	mgPhi->SetTitle(static_cast<TString>("Region D (data) vs. Double Cluster; Length [mm]; Ratio to Reference"));
// 	mgPhi->SetMaximum(0.4);
// 	mgPhi->Draw("AP");

// 	TMultiGraph* mgSimPhi = new TMultiGraph();
// 	simGraphPhi2[0]->SetLineColor(kRed);
// 	simGraphPhi2[0]->SetLineWidth(2);
// 	simGraphPhi2[0]->SetLineStyle(9);
// 	simGraphPhi2[0]->SetMarkerColor(kRed);
// 	simGraphPhi2[0]->SetMarkerSize(2);
// 	simGraphPhi2[0]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi2[0]);
// 	simGraphPhi2[1]->SetLineColor(kBlue);
// 	simGraphPhi2[1]->SetLineWidth(2);
// 	simGraphPhi2[1]->SetLineStyle(9);
// 	simGraphPhi2[1]->SetMarkerColor(kBlue);
// 	simGraphPhi2[1]->SetMarkerSize(2);
// 	simGraphPhi2[1]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi2[1]);
// 	simGraphPhi2[2]->SetLineColor(kMagenta);
// 	simGraphPhi2[2]->SetLineWidth(2);
// 	simGraphPhi2[2]->SetLineStyle(9);
// 	simGraphPhi2[2]->SetMarkerColor(kMagenta);
// 	simGraphPhi2[2]->SetMarkerSize(2);
// 	simGraphPhi2[2]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi2[2]);
// 	// simGraphPhi2[3]->SetLineColor(kGreen + 3);
// 	// simGraphPhi2[3]->SetLineWidth(2);
// 	// simGraphPhi2[3]->SetLineStyle(9);
// 	// simGraphPhi2[3]->SetMarkerColor(kGreen + 3);
// 	// simGraphPhi2[3]->SetMarkerSize(2);
// 	// simGraphPhi2[3]->SetMarkerStyle(21);
// 	// mgSimPhi->Add(simGraphPhi2[3]);
// 	mgSimPhi->Draw("P");

// 	TLegend* legendPhi = new TLegend(0.5, 0.7, 0.9, 0.9);
// 	legendPhi->SetNColumns(2);
// 	legendPhi->AddEntry(expGraphPhi2[0], "Data (#phi2)", "p");
// 	legendPhi->AddEntry(simGraphPhi2[0], "GEANT4 (#phi2)", "p");
// 	legendPhi->AddEntry(expGraphPhi2[1], "Data (#phi3)", "p");
// 	legendPhi->AddEntry(simGraphPhi2[1], "GEANT4 (#phi3)", "p");
// 	legendPhi->AddEntry(expGraphPhi2[2], "Data (#phi4)", "p");
// 	// legendPhi->AddEntry(expGraphPhi2[3], "Data (#phi7)", "p");
// 	legendPhi->AddEntry(simGraphPhi2[2], "GEANT4 (#phi4)", "p");
// 	// legendPhi->AddEntry(simGraphPhi2[3], "Geant4 (#phi7)", "p");
// 	legendPhi->Draw("SAME");

// 	canvasPhi->SetLeftMargin(0.12);
// 	canvasPhi->SetGrid();
// 	// canvasPhi->SetLogy();
// 	canvasPhi->SaveAs(static_cast<TString>("double_cluster_ratio_to_reference.png"));
// 	delete canvasPhi;
// }

// int main() {
// 	std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>> expData = getExperimentData();
// 	std::vector<std::tuple<int, int, std::array<double, 4>>> simData = getSimulationData();

// 	// drawForLength(2, expData, simData);
// 	// drawForLength(3, expData, simData);
// 	// drawForLength(4, expData, simData);
// 	// drawForLength(7, expData, simData);

// 	drawOnlyBC(2, expData, simData);
// 	drawOnlyBC(3, expData, simData);
// 	drawOnlyBC(4, expData, simData);
// 	drawOnlyBC(7, expData, simData);

// 	// drawOnlyA(2, expData, simData);
// 	// drawOnlyA(3, expData, simData);
// 	// drawOnlyA(4, expData, simData);
// 	// drawOnlyA(7, expData, simData);

// 	// drawOnly4(2, expData, simData);
// 	// drawOnly4(3, expData, simData);
// 	// drawOnly4(4, expData, simData);
// 	// drawOnly4(7, expData, simData);
// 	// drawOnly4All(expData);
// 	// drawDoubleCluster(expData, simData);
// 	// drawOnly1All(expData, simData);


// 	return 0;
// }