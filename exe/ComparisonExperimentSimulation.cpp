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

std::vector<std::tuple<int, int, std::array<double, 4>>> getSimulationData() {
	io::CSVReader<9> simCSV("/home/ychoi/ATOM/Data/simulation_entry.csv");

	simCSV.read_header(io::ignore_extra_column, "Collimator", "Length", "Width", "Area", "Area_Error", "Region_A", "Region_B", "Region_C", "Region_D");

	std::string collimator;
	int length, width;
	double area, areaError, regionA, regionB, regionC, regionD;

	std::vector<std::tuple<int, int, std::array<double, 4>>> simRefData;
	std::vector<std::tuple<int, int, std::array<double, 4>>> simData;

	while ( simCSV.read_row(collimator, length, width, area, areaError, regionA, regionB, regionC, regionD) ) {
		if ( width == 11 ) {
			simRefData.push_back(std::make_tuple(length, width, std::array<double, 4>{regionA, regionB, regionC, regionD}));
		} else {
			simData.push_back(std::make_tuple(length, width, std::array<double, 4>{regionA, regionB, regionC, regionD}));
		}
	}
	std::vector<std::tuple<int, int, std::array<double, 4>>> simRatioData;

	for ( auto& simDataEntry : simData ) {
		for ( auto& simRefDataEntry : simRefData ) {
			if ( std::get<0>(simDataEntry) == std::get<0>(simRefDataEntry) ) {
				std::array<double, 4> ratio = {std::get<2>(simDataEntry)[0] / std::get<2>(simRefDataEntry)[0], std::get<2>(simDataEntry)[1] / std::get<2>(simRefDataEntry)[1], std::get<2>(simDataEntry)[2] / std::get<2>(simRefDataEntry)[2], std::get<2>(simDataEntry)[3] / std::get<2>(simRefDataEntry)[3]};

				simRatioData.push_back(std::make_tuple(std::get<0>(simDataEntry), std::get<1>(simDataEntry), ratio));
			}
		}
	}
	return simRatioData;
}

std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>> getExperimentData() {
	io::CSVReader<63> expCSV("/home/ychoi/ATOM/Data/clustersize_entry.csv");

	expCSV.read_header(io::ignore_extra_column, "length", "width", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "60", "61");

	std::tuple<int, int, std::array<double, 61>> expEntry;
	std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>> expData;
	std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>> expRefData;
	// std::vector<std::array<int, 2>> regionDivide = {{1, 4}, {5, 10}, {11, 32}, {40, 61}};
	std::vector<std::array<int, 2>> regionDivide = {{4, 32}, {5, 10}, {11, 32}, {5, 32}};
	// std::vector<std::array<int, 2>> regionDivide = {{1, 1}, {5, 10}, {11, 32}, {5, 32}};
	// std::vector<std::array<int, 2>> regionDivide = {{4, 4}, {5, 10}, {11, 32}, {5, 32}};
	std::array<double, 4> regionEntry = {0, 0, 0, 0};
	std::array<double, 4> regionEntryError = {0, 0, 0, 0};

	while ( expCSV.read_row(std::get<0>(expEntry), std::get<1>(expEntry), std::get<2>(expEntry)[0], std::get<2>(expEntry)[1], std::get<2>(expEntry)[2], std::get<2>(expEntry)[3], std::get<2>(expEntry)[4], std::get<2>(expEntry)[5], std::get<2>(expEntry)[6], std::get<2>(expEntry)[7], std::get<2>(expEntry)[8], std::get<2>(expEntry)[9], std::get<2>(expEntry)[10], std::get<2>(expEntry)[11], std::get<2>(expEntry)[12], std::get<2>(expEntry)[13], std::get<2>(expEntry)[14], std::get<2>(expEntry)[15], std::get<2>(expEntry)[16], std::get<2>(expEntry)[17], std::get<2>(expEntry)[18], std::get<2>(expEntry)[19], std::get<2>(expEntry)[20], std::get<2>(expEntry)[21], std::get<2>(expEntry)[22], std::get<2>(expEntry)[23], std::get<2>(expEntry)[24], std::get<2>(expEntry)[25], std::get<2>(expEntry)[26], std::get<2>(expEntry)[27], std::get<2>(expEntry)[28], std::get<2>(expEntry)[29], std::get<2>(expEntry)[30], std::get<2>(expEntry)[31], std::get<2>(expEntry)[32], std::get<2>(expEntry)[33], std::get<2>(expEntry)[34], std::get<2>(expEntry)[35], std::get<2>(expEntry)[36], std::get<2>(expEntry)[37], std::get<2>(expEntry)[38], std::get<2>(expEntry)[39], std::get<2>(expEntry)[40], std::get<2>(expEntry)[41], std::get<2>(expEntry)[42], std::get<2>(expEntry)[43], std::get<2>(expEntry)[44], std::get<2>(expEntry)[45], std::get<2>(expEntry)[46], std::get<2>(expEntry)[47], std::get<2>(expEntry)[48], std::get<2>(expEntry)[49], std::get<2>(expEntry)[50], std::get<2>(expEntry)[51], std::get<2>(expEntry)[52], std::get<2>(expEntry)[53], std::get<2>(expEntry)[54], std::get<2>(expEntry)[55], std::get<2>(expEntry)[56], std::get<2>(expEntry)[57], std::get<2>(expEntry)[58], std::get<2>(expEntry)[59], std::get<2>(expEntry)[60]) ) {
		for ( int i = 0; i < std::get<2>(expEntry).size(); i++ ) {
			if ( i >= regionDivide[0][0] - 1 && i <= regionDivide[0][1] - 1 ) {
				regionEntry[0] += std::get<2>(expEntry)[i];
			}
			if ( i >= regionDivide[1][0] - 1 && i <= regionDivide[1][1] - 1 ) {
				regionEntry[1] += std::get<2>(expEntry)[i];
			}
			if ( i >= regionDivide[2][0] - 1 && i <= regionDivide[2][1] - 1 ) {
				regionEntry[2] += std::get<2>(expEntry)[i];
			}
			if ( i >= regionDivide[3][0] - 1 && i <= regionDivide[3][1] - 1 ) {
				regionEntry[3] += std::get<2>(expEntry)[i];
			}
		}
		if ( std::get<1>(expEntry) == 11 ) {
			expRefData.push_back(std::make_tuple(std::get<0>(expEntry), std::get<1>(expEntry), regionEntry, regionEntryError));
		} else {
			expData.push_back(std::make_tuple(std::get<0>(expEntry), std::get<1>(expEntry), regionEntry, regionEntryError));
		}
		regionEntry = {0, 0, 0, 0};
	}

	io::CSVReader<4> scaleCSV("/home/ychoi/ATOM/Data/time_scale.csv");
	scaleCSV.read_header(io::ignore_extra_column, "length", "width", "time", "scale");
	int length, width, time;
	double scale;
	while ( scaleCSV.read_row(length, width, time, scale) ) {
		for ( auto& expDataEntry : expData ) {
			if ( std::get<0>(expDataEntry) == length && std::get<1>(expDataEntry) == width ) {
				regionEntryError[0] = TMath::Sqrt(TMath::Abs(std::get<2>(expDataEntry)[0] / scale)) * scale;
				regionEntryError[1] = TMath::Sqrt(TMath::Abs(std::get<2>(expDataEntry)[1] / scale)) * scale;
				regionEntryError[2] = TMath::Sqrt(TMath::Abs(std::get<2>(expDataEntry)[2] / scale)) * scale;
				regionEntryError[3] = TMath::Sqrt(TMath::Abs(std::get<2>(expDataEntry)[3] / scale)) * scale;
				std::get<3>(expDataEntry) = regionEntryError;
			}
		}
	}
	std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>> expRatioData;
	for ( auto& expDataEntry : expData ) {
		for ( auto& expRefDataEntry : expRefData ) {
			if ( std::get<0>(expDataEntry) == std::get<0>(expRefDataEntry) ) {
				std::array<double, 4> ratio = {std::get<2>(expDataEntry)[0] / std::get<2>(expRefDataEntry)[0], std::get<2>(expDataEntry)[1] / std::get<2>(expRefDataEntry)[1], std::get<2>(expDataEntry)[2] / std::get<2>(expRefDataEntry)[2], std::get<2>(expDataEntry)[3] / std::get<2>(expRefDataEntry)[3]};

				std::array<double, 4> ratioError = {TMath::Sqrt(TMath::Power(std::get<3>(expDataEntry)[0] / std::get<2>(expRefDataEntry)[0], 2) + TMath::Power(std::get<2>(expDataEntry)[0] * std::get<3>(expRefDataEntry)[0] / TMath::Power(std::get<2>(expRefDataEntry)[0], 2), 2)), TMath::Sqrt(TMath::Power(std::get<3>(expDataEntry)[1] / std::get<2>(expRefDataEntry)[1], 2) + TMath::Power(std::get<2>(expDataEntry)[1] * std::get<3>(expRefDataEntry)[1] / TMath::Power(std::get<2>(expRefDataEntry)[1], 2), 2)), TMath::Sqrt(TMath::Power(std::get<3>(expDataEntry)[2] / std::get<2>(expRefDataEntry)[2], 2) + TMath::Power(std::get<2>(expDataEntry)[2] * std::get<3>(expRefDataEntry)[2] / TMath::Power(std::get<2>(expRefDataEntry)[2], 2), 2)), TMath::Sqrt(TMath::Power(std::get<3>(expDataEntry)[3] / std::get<2>(expRefDataEntry)[3], 2) + TMath::Power(std::get<2>(expDataEntry)[3] * std::get<3>(expRefDataEntry)[3] / TMath::Power(std::get<2>(expRefDataEntry)[3], 2), 2))};

				expRatioData.push_back(std::make_tuple(std::get<0>(expDataEntry), std::get<1>(expDataEntry), ratio, ratioError));
			}
		}
	}
	return expRatioData;
}

void drawForLength(int drawWidth, std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>>& expData, std::vector<std::tuple<int, int, std::array<double, 4>>>& simData) {
	TGraphErrors* expGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
	TGraph* simGraphPhi[4] = {new TGraph(), new TGraph(), new TGraph(), new TGraph()};
	for ( auto& expDataEntry : expData ) {
		if ( std::get<1>(expDataEntry) == drawWidth ) {
			expGraphPhi2[0]->SetPoint(expGraphPhi2[0]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
			expGraphPhi2[0]->SetPointError(expGraphPhi2[0]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
			expGraphPhi2[1]->SetPoint(expGraphPhi2[1]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[1]);
			expGraphPhi2[1]->SetPointError(expGraphPhi2[1]->GetN() - 1, 0, std::get<3>(expDataEntry)[1]);
			expGraphPhi2[2]->SetPoint(expGraphPhi2[2]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[2]);
			expGraphPhi2[2]->SetPointError(expGraphPhi2[2]->GetN() - 1, 0, std::get<3>(expDataEntry)[2]);
			expGraphPhi2[3]->SetPoint(expGraphPhi2[3]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
			expGraphPhi2[3]->SetPointError(expGraphPhi2[3]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
		}
	}
	for ( auto& simDataEntry : simData ) {
		if ( std::get<1>(simDataEntry) == drawWidth ) {
			simGraphPhi[0]->SetPoint(simGraphPhi[0]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[0]);
			simGraphPhi[1]->SetPoint(simGraphPhi[1]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[1]);
			simGraphPhi[2]->SetPoint(simGraphPhi[2]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[2]);
			simGraphPhi[3]->SetPoint(simGraphPhi[3]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[3]);
		}
	}

	TCanvas* canvasPhi = new TCanvas("canvasphi", "", 1000, 1000);
	TMultiGraph* mgPhi = new TMultiGraph();
	expGraphPhi2[0]->SetLineColor(kRed);
	expGraphPhi2[0]->SetLineWidth(2);
	expGraphPhi2[0]->SetMarkerColor(kRed);
	expGraphPhi2[0]->SetMarkerStyle(24);
	expGraphPhi2[0]->SetMarkerSize(2);
	mgPhi->Add(expGraphPhi2[0]);
	expGraphPhi2[1]->SetLineColor(kBlue);
	expGraphPhi2[1]->SetLineWidth(2);
	expGraphPhi2[1]->SetMarkerColor(kBlue);
	expGraphPhi2[1]->SetMarkerStyle(24);
	expGraphPhi2[1]->SetMarkerSize(2);
	mgPhi->Add(expGraphPhi2[1]);
	expGraphPhi2[2]->SetLineColor(kMagenta);
	expGraphPhi2[2]->SetLineWidth(2);
	expGraphPhi2[2]->SetMarkerColor(kMagenta);
	expGraphPhi2[2]->SetMarkerStyle(24);
	expGraphPhi2[2]->SetMarkerSize(2);
	mgPhi->Add(expGraphPhi2[2]);
	expGraphPhi2[3]->SetLineColor(kGreen + 3);
	expGraphPhi2[3]->SetLineWidth(2);
	expGraphPhi2[3]->SetMarkerColor(kGreen + 3);
	expGraphPhi2[3]->SetMarkerStyle(24);
	expGraphPhi2[3]->SetMarkerSize(2);
	mgPhi->Add(expGraphPhi2[3]);
	mgPhi->SetTitle(static_cast<TString>("Comparison for " + std::to_string(drawWidth) + "#phi collimators; Length[mm]; Ratio to Reference"));
	mgPhi->Draw("AP");

	TMultiGraph* mgSimPhi = new TMultiGraph();
	simGraphPhi[0]->SetLineColor(kBlue);
	simGraphPhi[0]->SetLineWidth(2);
	simGraphPhi[0]->SetLineStyle(9);
	simGraphPhi[0]->SetMarkerColor(kBlue);
	simGraphPhi[0]->SetMarkerSize(2);
	simGraphPhi[0]->SetMarkerStyle(21);
	mgSimPhi->Add(simGraphPhi[0]);
	simGraphPhi[1]->SetLineColor(kRed);
	simGraphPhi[1]->SetLineWidth(2);
	simGraphPhi[1]->SetLineStyle(9);
	simGraphPhi[1]->SetMarkerColor(kRed);
	simGraphPhi[1]->SetMarkerSize(2);
	simGraphPhi[1]->SetMarkerStyle(21);
	mgSimPhi->Add(simGraphPhi[1]);
	simGraphPhi[2]->SetLineColor(kMagenta);
	simGraphPhi[2]->SetLineWidth(2);
	simGraphPhi[2]->SetLineStyle(9);
	simGraphPhi[2]->SetMarkerColor(kMagenta);
	simGraphPhi[2]->SetMarkerSize(2);
	simGraphPhi[2]->SetMarkerStyle(21);
	mgSimPhi->Add(simGraphPhi[2]);
	simGraphPhi[3]->SetLineColor(kGreen + 3);
	simGraphPhi[3]->SetLineWidth(2);
	simGraphPhi[3]->SetLineStyle(9);
	simGraphPhi[3]->SetMarkerColor(kGreen + 3);
	simGraphPhi[3]->SetMarkerSize(2);
	simGraphPhi[3]->SetMarkerStyle(21);
	mgSimPhi->Add(simGraphPhi[3]);
	mgSimPhi->Draw("P");

	TLegend* legendPhi = new TLegend(0.3, 0.6, 0.8, 0.9);
	legendPhi->AddEntry(expGraphPhi2[0], "Region A", "p");
	legendPhi->AddEntry(expGraphPhi2[1], "Region B", "p");
	legendPhi->AddEntry(expGraphPhi2[2], "Region C", "p");
	legendPhi->AddEntry(expGraphPhi2[3], "Region D", "p");
	legendPhi->AddEntry(simGraphPhi[1], "The # of electron in metal (Simulation)", "p");
	legendPhi->AddEntry(simGraphPhi[0], "The # of single alpha in metal (Simulation)", "p");
	legendPhi->AddEntry(simGraphPhi[2], "The # of single alpha in epitaxial (Simulation)", "p");
	legendPhi->AddEntry(simGraphPhi[3], "The # of double cluster (Simulation)", "p");
	legendPhi->Draw("SAME");

	canvasPhi->SetLeftMargin(0.12);
	canvasPhi->SetGrid();
	canvasPhi->SaveAs(static_cast<TString>("phi" + std::to_string(drawWidth) + "_ratio_to_reference.png"));
	delete canvasPhi;
}

void drawOnlyBC(int drawWidth, std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>>& expData, std::vector<std::tuple<int, int, std::array<double, 4>>>& simData) {
	TGraphErrors* expGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
	TGraph* simGraphPhi[4] = {new TGraph(), new TGraph(), new TGraph(), new TGraph()};
	for ( auto& expDataEntry : expData ) {
		if ( std::get<1>(expDataEntry) == drawWidth ) {
			expGraphPhi2[0]->SetPoint(expGraphPhi2[0]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
			expGraphPhi2[0]->SetPointError(expGraphPhi2[0]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
			expGraphPhi2[1]->SetPoint(expGraphPhi2[1]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[1]);
			expGraphPhi2[1]->SetPointError(expGraphPhi2[1]->GetN() - 1, 0, std::get<3>(expDataEntry)[1]);
			expGraphPhi2[2]->SetPoint(expGraphPhi2[2]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[2]);
			expGraphPhi2[2]->SetPointError(expGraphPhi2[2]->GetN() - 1, 0, std::get<3>(expDataEntry)[2]);
			expGraphPhi2[3]->SetPoint(expGraphPhi2[3]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
			expGraphPhi2[3]->SetPointError(expGraphPhi2[3]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
		}
	}
	for ( auto& simDataEntry : simData ) {
		if ( std::get<1>(simDataEntry) == drawWidth ) {
			// simGraphPhi[0]->SetPoint(simGraphPhi[0]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[0]);
			// simGraphPhi[1]->SetPoint(simGraphPhi[1]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[1]);
			simGraphPhi[2]->SetPoint(simGraphPhi[2]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[2]);
			// simGraphPhi[3]->SetPoint(simGraphPhi[3]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[3]);
		}
	}

	TCanvas* canvasPhi = new TCanvas("canvasphi", "", 1000, 1000);
	TMultiGraph* mgPhi = new TMultiGraph();
	// expGraphPhi2[0]->SetLineColor(kRed);
	// expGraphPhi2[0]->SetLineWidth(2);
	// expGraphPhi2[0]->SetMarkerColor(kRed);
	// expGraphPhi2[0]->SetMarkerStyle(24);
	// expGraphPhi2[0]->SetMarkerSize(2);
	// mgPhi->Add(expGraphPhi2[0]);
	expGraphPhi2[1]->SetLineColor(kBlue);
	expGraphPhi2[1]->SetLineWidth(2);
	expGraphPhi2[1]->SetMarkerColor(kBlue);
	expGraphPhi2[1]->SetMarkerStyle(24);
	expGraphPhi2[1]->SetMarkerSize(2);
	mgPhi->Add(expGraphPhi2[1]);
	expGraphPhi2[2]->SetLineColor(kMagenta);
	expGraphPhi2[2]->SetLineWidth(2);
	expGraphPhi2[2]->SetMarkerColor(kMagenta);
	expGraphPhi2[2]->SetMarkerStyle(24);
	expGraphPhi2[2]->SetMarkerSize(2);
	mgPhi->Add(expGraphPhi2[2]);
	expGraphPhi2[3]->SetLineColor(kGreen + 3);
	expGraphPhi2[3]->SetLineWidth(2);
	expGraphPhi2[3]->SetMarkerColor(kGreen + 3);
	expGraphPhi2[3]->SetMarkerStyle(24);
	expGraphPhi2[3]->SetMarkerSize(2);
	mgPhi->Add(expGraphPhi2[3]);
	mgPhi->SetTitle(static_cast<TString>("Comparison for " + std::to_string(drawWidth) + "#phi collimators; Length[mm]; Ratio to Reference"));
	mgPhi->Draw("AP");

	TMultiGraph* mgSimPhi = new TMultiGraph();
	// simGraphPhi[0]->SetLineColor(kBlue);
	// simGraphPhi[0]->SetLineWidth(2);
	// simGraphPhi[0]->SetLineStyle(9);
	// simGraphPhi[0]->SetMarkerColor(kBlue);
	// simGraphPhi[0]->SetMarkerSize(2);
	// simGraphPhi[0]->SetMarkerStyle(21);
	// mgSimPhi->Add(simGraphPhi[0]);
	// simGraphPhi[1]->SetLineColor(kRed);
	// simGraphPhi[1]->SetLineWidth(2);
	// simGraphPhi[1]->SetLineStyle(9);
	// simGraphPhi[1]->SetMarkerColor(kRed);
	// simGraphPhi[1]->SetMarkerSize(2);
	// simGraphPhi[1]->SetMarkerStyle(21);
	// mgSimPhi->Add(simGraphPhi[1]);
	simGraphPhi[2]->SetLineColor(kMagenta);
	simGraphPhi[2]->SetLineWidth(2);
	simGraphPhi[2]->SetLineStyle(9);
	simGraphPhi[2]->SetMarkerColor(kMagenta);
	simGraphPhi[2]->SetMarkerSize(2);
	simGraphPhi[2]->SetMarkerStyle(21);
	mgSimPhi->Add(simGraphPhi[2]);
	// simGraphPhi[3]->SetLineColor(kGreen + 3);
	// simGraphPhi[3]->SetLineWidth(2);
	// simGraphPhi[3]->SetLineStyle(9);
	// simGraphPhi[3]->SetMarkerColor(kGreen + 3);
	// simGraphPhi[3]->SetMarkerSize(2);
	// simGraphPhi[3]->SetMarkerStyle(21);
	mgSimPhi->Add(simGraphPhi[3]);
	mgSimPhi->Draw("P");

	TLegend* legendPhi = new TLegend(0.3, 0.6, 0.9, 0.9);
	// legendPhi->AddEntry(expGraphPhi2[0], "Region B + C + cluster size 4", "p");
	legendPhi->AddEntry(expGraphPhi2[1], "5 #leq Cluster Size #leq 10", "p");
	legendPhi->AddEntry(expGraphPhi2[2], "11 #leq Cluster Size #leq 32", "p");
	legendPhi->AddEntry(expGraphPhi2[3], "5 #leq Cluster Size #leq 32", "p");
	// legendPhi->AddEntry(simGraphPhi[1], "The # of electron in metal (Simulation)", "p");
	// legendPhi->AddEntry(simGraphPhi[0], "The # of single alpha in metal (Simulation)", "p");
	legendPhi->AddEntry(simGraphPhi[2], "The # of single alpha in epitaxial (Simulation)", "p");
	// legendPhi->AddEntry(simGraphPhi[3], "The # of double cluster (Simulation)", "p");
	legendPhi->Draw("SAME");

	canvasPhi->SetLeftMargin(0.13);
	canvasPhi->SetGrid();
	canvasPhi->SaveAs(static_cast<TString>("phi" + std::to_string(drawWidth) + "_ratio_to_reference_BC.png"));
	delete canvasPhi;
}


void drawOnlyA(int drawWidth, std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>>& expData, std::vector<std::tuple<int, int, std::array<double, 4>>>& simData) {
	TGraphErrors* expGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
	TGraph* simGraphPhi[4] = {new TGraph(), new TGraph(), new TGraph(), new TGraph()};
	for ( auto& expDataEntry : expData ) {
		if ( std::get<1>(expDataEntry) == drawWidth ) {
			expGraphPhi2[0]->SetPoint(expGraphPhi2[0]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
			expGraphPhi2[0]->SetPointError(expGraphPhi2[0]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
			expGraphPhi2[1]->SetPoint(expGraphPhi2[1]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[1]);
			expGraphPhi2[1]->SetPointError(expGraphPhi2[1]->GetN() - 1, 0, std::get<3>(expDataEntry)[1]);
			expGraphPhi2[2]->SetPoint(expGraphPhi2[2]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[2]);
			expGraphPhi2[2]->SetPointError(expGraphPhi2[2]->GetN() - 1, 0, std::get<3>(expDataEntry)[2]);
			expGraphPhi2[3]->SetPoint(expGraphPhi2[3]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
			expGraphPhi2[3]->SetPointError(expGraphPhi2[3]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
		}
	}
	for ( auto& simDataEntry : simData ) {
		if ( std::get<1>(simDataEntry) == drawWidth ) {
			// simGraphPhi[0]->SetPoint(simGraphPhi[0]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[0]);
			simGraphPhi[1]->SetPoint(simGraphPhi[1]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[1]);
			// simGraphPhi[2]->SetPoint(simGraphPhi[2]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[2]);
			// simGraphPhi[3]->SetPoint(simGraphPhi[3]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[3]);
		}
	}

	TCanvas* canvasPhi = new TCanvas("canvasphi", "", 1000, 1000);
	TMultiGraph* mgPhi = new TMultiGraph();
	expGraphPhi2[0]->SetLineColor(kRed);
	expGraphPhi2[0]->SetLineWidth(2);
	expGraphPhi2[0]->SetMarkerColor(kRed);
	expGraphPhi2[0]->SetMarkerStyle(24);
	expGraphPhi2[0]->SetMarkerSize(2);
	mgPhi->Add(expGraphPhi2[0]);
	// expGraphPhi2[1]->SetLineColor(kBlue);
	// expGraphPhi2[1]->SetLineWidth(2);
	// expGraphPhi2[1]->SetMarkerColor(kBlue);
	// expGraphPhi2[1]->SetMarkerStyle(24);
	// expGraphPhi2[1]->SetMarkerSize(2);
	// mgPhi->Add(expGraphPhi2[1]);
	// expGraphPhi2[2]->SetLineColor(kMagenta);
	// expGraphPhi2[2]->SetLineWidth(2);
	// expGraphPhi2[2]->SetMarkerColor(kMagenta);
	// expGraphPhi2[2]->SetMarkerStyle(24);
	// expGraphPhi2[2]->SetMarkerSize(2);
	// mgPhi->Add(expGraphPhi2[2]);
	// expGraphPhi2[3]->SetLineColor(kGreen + 3);
	// expGraphPhi2[3]->SetLineWidth(2);
	// expGraphPhi2[3]->SetMarkerColor(kGreen + 3);
	// expGraphPhi2[3]->SetMarkerStyle(24);
	// expGraphPhi2[3]->SetMarkerSize(2);
	// mgPhi->Add(expGraphPhi2[3]);
	mgPhi->SetTitle(static_cast<TString>("Comparison for " + std::to_string(drawWidth) + "#phi collimators; Length[mm]; Ratio to Reference"));
	mgPhi->SetMinimum(0);
	mgPhi->SetMaximum(.8);
	mgPhi->Draw("AP");

	TMultiGraph* mgSimPhi = new TMultiGraph();
	// simGraphPhi[0]->SetLineColor(kBlue);
	// simGraphPhi[0]->SetLineWidth(2);
	// simGraphPhi[0]->SetLineStyle(9);
	// simGraphPhi[0]->SetMarkerColor(kBlue);
	// simGraphPhi[0]->SetMarkerSize(2);
	// simGraphPhi[0]->SetMarkerStyle(21);
	// mgSimPhi->Add(simGraphPhi[0]);
	simGraphPhi[1]->SetLineColor(kRed);
	simGraphPhi[1]->SetLineWidth(2);
	simGraphPhi[1]->SetLineStyle(9);
	simGraphPhi[1]->SetMarkerColor(kRed);
	simGraphPhi[1]->SetMarkerSize(2);
	simGraphPhi[1]->SetMarkerStyle(21);
	mgSimPhi->Add(simGraphPhi[1]);
	// simGraphPhi[2]->SetLineColor(kMagenta);
	// simGraphPhi[2]->SetLineWidth(2);
	// simGraphPhi[2]->SetLineStyle(9);
	// simGraphPhi[2]->SetMarkerColor(kMagenta);
	// simGraphPhi[2]->SetMarkerSize(2);
	// simGraphPhi[2]->SetMarkerStyle(21);
	// mgSimPhi->Add(simGraphPhi[2]);
	// simGraphPhi[3]->SetLineColor(kGreen + 3);
	// simGraphPhi[3]->SetLineWidth(2);
	// simGraphPhi[3]->SetLineStyle(9);
	// simGraphPhi[3]->SetMarkerColor(kGreen + 3);
	// simGraphPhi[3]->SetMarkerSize(2);
	// simGraphPhi[3]->SetMarkerStyle(21);
	mgSimPhi->Add(simGraphPhi[3]);
	mgSimPhi->Draw("P");

	TLegend* legendPhi = new TLegend(0.3, 0.6, 0.8, 0.9);
	legendPhi->AddEntry(expGraphPhi2[0], "Cluster size 1", "p");
	// legendPhi->AddEntry(expGraphPhi2[1], "Region B", "p");
	// legendPhi->AddEntry(expGraphPhi2[2], "Region C", "p");
	// legendPhi->AddEntry(expGraphPhi2[3], "Region B + C", "p");
	legendPhi->AddEntry(simGraphPhi[1], "The # of electron in metal (Simulation)", "p");
	// legendPhi->AddEntry(simGraphPhi[0], "The # of single alpha in metal (Simulation)", "p");
	// legendPhi->AddEntry(simGraphPhi[2], "The # of single alpha in epitaxial (Simulation)", "p");
	// legendPhi->AddEntry(simGraphPhi[3], "The # of double cluster (Simulation)", "p");
	legendPhi->Draw("SAME");

	canvasPhi->SetLeftMargin(0.12);
	canvasPhi->SetGrid();
	canvasPhi->SaveAs(static_cast<TString>("phi" + std::to_string(drawWidth) + "_ratio_to_reference_1.png"));
	delete canvasPhi;
}


void drawOnly4(int drawWidth, std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>>& expData, std::vector<std::tuple<int, int, std::array<double, 4>>>& simData) {
	TGraphErrors* expGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
	TGraph* simGraphPhi[4] = {new TGraph(), new TGraph(), new TGraph(), new TGraph()};
	for ( auto& expDataEntry : expData ) {
		if ( std::get<1>(expDataEntry) == drawWidth ) {
			expGraphPhi2[0]->SetPoint(expGraphPhi2[0]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
			expGraphPhi2[0]->SetPointError(expGraphPhi2[0]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
			expGraphPhi2[1]->SetPoint(expGraphPhi2[1]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[1]);
			expGraphPhi2[1]->SetPointError(expGraphPhi2[1]->GetN() - 1, 0, std::get<3>(expDataEntry)[1]);
			expGraphPhi2[2]->SetPoint(expGraphPhi2[2]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[2]);
			expGraphPhi2[2]->SetPointError(expGraphPhi2[2]->GetN() - 1, 0, std::get<3>(expDataEntry)[2]);
			expGraphPhi2[3]->SetPoint(expGraphPhi2[3]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
			expGraphPhi2[3]->SetPointError(expGraphPhi2[3]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
		}
	}

	TCanvas* canvasPhi = new TCanvas("canvasphi", "", 1000, 1000);
	TMultiGraph* mgPhi = new TMultiGraph();
	expGraphPhi2[0]->SetLineColor(kRed);
	expGraphPhi2[0]->SetLineWidth(2);
	expGraphPhi2[0]->SetMarkerColor(kRed);
	expGraphPhi2[0]->SetMarkerStyle(24);
	expGraphPhi2[0]->SetMarkerSize(2);
	mgPhi->Add(expGraphPhi2[0]);
	// expGraphPhi2[1]->SetLineColor(kBlue);
	// expGraphPhi2[1]->SetLineWidth(2);
	// expGraphPhi2[1]->SetMarkerColor(kBlue);
	// expGraphPhi2[1]->SetMarkerStyle(24);
	// expGraphPhi2[1]->SetMarkerSize(2);
	// mgPhi->Add(expGraphPhi2[1]);
	// expGraphPhi2[2]->SetLineColor(kMagenta);
	// expGraphPhi2[2]->SetLineWidth(2);
	// expGraphPhi2[2]->SetMarkerColor(kMagenta);
	// expGraphPhi2[2]->SetMarkerStyle(24);
	// expGraphPhi2[2]->SetMarkerSize(2);
	// mgPhi->Add(expGraphPhi2[2]);
	// expGraphPhi2[3]->SetLineColor(kGreen + 3);
	// expGraphPhi2[3]->SetLineWidth(2);
	// expGraphPhi2[3]->SetMarkerColor(kGreen + 3);
	// expGraphPhi2[3]->SetMarkerStyle(24);
	// expGraphPhi2[3]->SetMarkerSize(2);
	// mgPhi->Add(expGraphPhi2[3]);
	mgPhi->SetTitle(static_cast<TString>("Comparison for " + std::to_string(drawWidth) + "#phi collimators; Length[mm]; Ratio to Reference"));
	mgPhi->Draw("AP");

	TLegend* legendPhi = new TLegend(0.3, 0.6, 0.8, 0.9);
	legendPhi->AddEntry(expGraphPhi2[0], "Cluster size 4", "p");
	// legendPhi->AddEntry(expGraphPhi2[1], "Region B", "p");
	// legendPhi->AddEntry(expGraphPhi2[2], "Region C", "p");
	// legendPhi->AddEntry(expGraphPhi2[3], "Region B + C", "p");
	// legendPhi->AddEntry(simGraphPhi[1], "The # of electron in metal (Simulation)", "p");
	// legendPhi->AddEntry(simGraphPhi[0], "The # of single alpha in metal (Simulation)", "p");
	// legendPhi->AddEntry(simGraphPhi[2], "The # of single alpha in epitaxial (Simulation)", "p");
	// legendPhi->AddEntry(simGraphPhi[3], "The # of double cluster (Simulation)", "p");
	legendPhi->Draw("SAME");

	canvasPhi->SetLeftMargin(0.12);
	canvasPhi->SetGrid();
	canvasPhi->SaveAs(static_cast<TString>("phi" + std::to_string(drawWidth) + "_ratio_to_reference_4.png"));
	delete canvasPhi;
}

void drawOnly1All(std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>>& expData, std::vector<std::tuple<int, int, std::array<double, 4>>> simData) {
	TGraphErrors* expGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
	TGraph* simGraphPhi[4] = {new TGraph(), new TGraph(), new TGraph(), new TGraph()};
	for ( auto& expDataEntry : expData ) {
		if ( std::get<1>(expDataEntry) == 2 ) {
			expGraphPhi2[0]->SetPoint(expGraphPhi2[0]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
			expGraphPhi2[0]->SetPointError(expGraphPhi2[0]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
		} else if ( std::get<1>(expDataEntry) == 3 ) {
			expGraphPhi2[1]->SetPoint(expGraphPhi2[1]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
			expGraphPhi2[1]->SetPointError(expGraphPhi2[1]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
		} else if ( std::get<1>(expDataEntry) == 4 ) {
			expGraphPhi2[2]->SetPoint(expGraphPhi2[2]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
			expGraphPhi2[2]->SetPointError(expGraphPhi2[2]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
		} else if ( std::get<1>(expDataEntry) == 7 ) {
			expGraphPhi2[3]->SetPoint(expGraphPhi2[3]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
			expGraphPhi2[3]->SetPointError(expGraphPhi2[3]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
		}
	}
	for ( auto& simDataEntry : simData ) {
		if ( std::get<1>(simDataEntry) == 2 ) {
			simGraphPhi[0]->SetPoint(simGraphPhi[0]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[1]);
		} else if ( std::get<1>(simDataEntry) == 3 ) {
			simGraphPhi[1]->SetPoint(simGraphPhi[1]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[1]);
		} else if ( std::get<1>(simDataEntry) == 4 ) {
			simGraphPhi[2]->SetPoint(simGraphPhi[2]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[1]);
		} else if ( std::get<1>(simDataEntry) == 7 ) {
			simGraphPhi[3]->SetPoint(simGraphPhi[3]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[1]);
		}
	}
	TCanvas* canvasPhi = new TCanvas("canvasphi", "", 1000, 1000);
	TMultiGraph* mgPhi = new TMultiGraph();
	expGraphPhi2[0]->SetLineColor(kRed);
	expGraphPhi2[0]->SetLineWidth(2);
	expGraphPhi2[0]->SetMarkerColor(kRed);
	expGraphPhi2[0]->SetMarkerStyle(24);
	expGraphPhi2[0]->SetMarkerSize(2);
	mgPhi->Add(expGraphPhi2[0]);
	expGraphPhi2[1]->SetLineColor(kBlue);
	expGraphPhi2[1]->SetLineWidth(2);
	expGraphPhi2[1]->SetMarkerColor(kBlue);
	expGraphPhi2[1]->SetMarkerStyle(24);
	expGraphPhi2[1]->SetMarkerSize(2);
	mgPhi->Add(expGraphPhi2[1]);
	expGraphPhi2[2]->SetLineColor(kMagenta);
	expGraphPhi2[2]->SetLineWidth(2);
	expGraphPhi2[2]->SetMarkerColor(kMagenta);
	expGraphPhi2[2]->SetMarkerStyle(24);
	expGraphPhi2[2]->SetMarkerSize(2);
	mgPhi->Add(expGraphPhi2[2]);
	expGraphPhi2[3]->SetLineColor(kGreen + 3);
	expGraphPhi2[3]->SetLineWidth(2);
	expGraphPhi2[3]->SetMarkerColor(kGreen + 3);
	expGraphPhi2[3]->SetMarkerStyle(24);
	expGraphPhi2[3]->SetMarkerSize(2);
	mgPhi->Add(expGraphPhi2[3]);
	mgPhi->SetTitle(static_cast<TString>("Cluster Size 1 vs. Electrons in Metal Layer; Length [mm]; Ratio to Reference"));
	mgPhi->SetMaximum(0.8);
	mgPhi->Draw("AP");

	TMultiGraph* mgSimPhi = new TMultiGraph();
	simGraphPhi[0]->SetLineColor(kRed);
	simGraphPhi[0]->SetLineWidth(2);
	simGraphPhi[0]->SetLineStyle(9);
	simGraphPhi[0]->SetMarkerColor(kRed);
	simGraphPhi[0]->SetMarkerSize(2);
	simGraphPhi[0]->SetMarkerStyle(21);
	mgSimPhi->Add(simGraphPhi[0]);
	simGraphPhi[1]->SetLineColor(kBlue);
	simGraphPhi[1]->SetLineWidth(2);
	simGraphPhi[1]->SetLineStyle(9);
	simGraphPhi[1]->SetMarkerColor(kBlue);
	simGraphPhi[1]->SetMarkerSize(2);
	simGraphPhi[1]->SetMarkerStyle(21);
	mgSimPhi->Add(simGraphPhi[1]);
	simGraphPhi[2]->SetLineColor(kMagenta);
	simGraphPhi[2]->SetLineWidth(2);
	simGraphPhi[2]->SetLineStyle(9);
	simGraphPhi[2]->SetMarkerColor(kMagenta);
	simGraphPhi[2]->SetMarkerSize(2);
	simGraphPhi[2]->SetMarkerStyle(21);
	mgSimPhi->Add(simGraphPhi[2]);
	simGraphPhi[3]->SetLineColor(kGreen + 3);
	simGraphPhi[3]->SetLineWidth(2);
	simGraphPhi[3]->SetLineStyle(9);
	simGraphPhi[3]->SetMarkerColor(kGreen + 3);
	simGraphPhi[3]->SetMarkerSize(2);
	simGraphPhi[3]->SetMarkerStyle(21);
	mgSimPhi->Add(simGraphPhi[3]);
	mgSimPhi->Draw("P");

	TLegend* legendPhi = new TLegend(0.3, 0.6, 0.8, 0.9);
	legendPhi->AddEntry(expGraphPhi2[0], "Experiment: Width = 2 mm", "p");
	legendPhi->AddEntry(expGraphPhi2[1], "Experiment: Width = 3 mm", "p");
	legendPhi->AddEntry(expGraphPhi2[2], "Experiment: Width = 4 mm", "p");
	legendPhi->AddEntry(expGraphPhi2[3], "Experiment: Width = 7 mm", "p");
	legendPhi->AddEntry(simGraphPhi[0], "Simulation: Width = 2 mm", "p");
	legendPhi->AddEntry(simGraphPhi[1], "Simulation: Width = 3 mm", "p");
	legendPhi->AddEntry(simGraphPhi[2], "Simulation: Width = 4 mm", "p");
	legendPhi->AddEntry(simGraphPhi[3], "Simulation: Width = 7 mm", "p");
	legendPhi->Draw("SAME");

	canvasPhi->SetLeftMargin(0.12);
	canvasPhi->SetGrid();
	canvasPhi->SaveAs(static_cast<TString>("ratio_to_reference_cluster_size_1.png"));
}

void drawOnly4All(std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>>& expData) {
	TGraphErrors* expGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
	for ( auto& expDataEntry : expData ) {
		if ( std::get<1>(expDataEntry) == 2 ) {
			expGraphPhi2[0]->SetPoint(expGraphPhi2[0]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
			expGraphPhi2[0]->SetPointError(expGraphPhi2[0]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
		} else if ( std::get<1>(expDataEntry) == 3 ) {
			expGraphPhi2[1]->SetPoint(expGraphPhi2[1]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
			expGraphPhi2[1]->SetPointError(expGraphPhi2[1]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
		} else if ( std::get<1>(expDataEntry) == 4 ) {
			expGraphPhi2[2]->SetPoint(expGraphPhi2[2]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
			expGraphPhi2[2]->SetPointError(expGraphPhi2[2]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
		} else if ( std::get<1>(expDataEntry) == 7 ) {
			expGraphPhi2[3]->SetPoint(expGraphPhi2[3]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
			expGraphPhi2[3]->SetPointError(expGraphPhi2[3]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
		}

	}

	TCanvas* canvasPhi = new TCanvas("canvasphi", "", 1000, 1000);
	TMultiGraph* mgPhi = new TMultiGraph();
	expGraphPhi2[0]->SetLineColor(kRed);
	expGraphPhi2[0]->SetLineWidth(2);
	expGraphPhi2[0]->SetMarkerColor(kRed);
	expGraphPhi2[0]->SetMarkerStyle(24);
	expGraphPhi2[0]->SetMarkerSize(2);
	mgPhi->Add(expGraphPhi2[0]);
	expGraphPhi2[1]->SetLineColor(kBlue);
	expGraphPhi2[1]->SetLineWidth(2);
	expGraphPhi2[1]->SetMarkerColor(kBlue);
	expGraphPhi2[1]->SetMarkerStyle(24);
	expGraphPhi2[1]->SetMarkerSize(2);
	mgPhi->Add(expGraphPhi2[1]);
	expGraphPhi2[2]->SetLineColor(kMagenta);
	expGraphPhi2[2]->SetLineWidth(2);
	expGraphPhi2[2]->SetMarkerColor(kMagenta);
	expGraphPhi2[2]->SetMarkerStyle(24);
	expGraphPhi2[2]->SetMarkerSize(2);
	mgPhi->Add(expGraphPhi2[2]);
	expGraphPhi2[3]->SetLineColor(kGreen + 3);
	expGraphPhi2[3]->SetLineWidth(2);
	expGraphPhi2[3]->SetMarkerColor(kGreen + 3);
	expGraphPhi2[3]->SetMarkerStyle(24);
	expGraphPhi2[3]->SetMarkerSize(2);
	mgPhi->Add(expGraphPhi2[3]);
	mgPhi->SetTitle(static_cast<TString>("Cluster Size 4 vs. Electrons in Metal Layer; Length [mm]; Ratio to Reference"));
	mgPhi->Draw("AP");

	TLegend* legendPhi = new TLegend(0.4, 0.6, 0.9, 0.9);
	legendPhi->AddEntry(expGraphPhi2[0], "Experiment: Width = 2 mm", "p");
	legendPhi->AddEntry(expGraphPhi2[1], "Experiment: Width = 3 mm", "p");
	legendPhi->AddEntry(expGraphPhi2[2], "Experiment: Width = 4 mm", "p");
	legendPhi->AddEntry(expGraphPhi2[3], "Experiment: Width = 7 mm", "p");
	legendPhi->Draw("SAME");

	canvasPhi->SetLeftMargin(0.12);
	canvasPhi->SetGrid();
	canvasPhi->SaveAs(static_cast<TString>("ratio_to_reference_cluster_size_4.png"));
	delete canvasPhi;
}

void drawDoubleCluster(std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>>& expData, std::vector<std::tuple<int, int, std::array<double, 4>>>& simData) {
	TGraphErrors* expGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
	TGraphErrors* simGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
	for ( auto& expDataEntry : expData ) {
		if ( std::get<1>(expDataEntry) == 2 ) {
			expGraphPhi2[0]->SetPoint(expGraphPhi2[0]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
			expGraphPhi2[0]->SetPointError(expGraphPhi2[0]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
		} else if ( std::get<1>(expDataEntry) == 3 ) {
			expGraphPhi2[1]->SetPoint(expGraphPhi2[1]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
			expGraphPhi2[1]->SetPointError(expGraphPhi2[1]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
		} else if ( std::get<1>(expDataEntry) == 4 ) {
			expGraphPhi2[2]->SetPoint(expGraphPhi2[2]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
			expGraphPhi2[2]->SetPointError(expGraphPhi2[2]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
		} else if ( std::get<1>(expDataEntry) == 7 ) {
			expGraphPhi2[3]->SetPoint(expGraphPhi2[3]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
			expGraphPhi2[3]->SetPointError(expGraphPhi2[3]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
		}

	}
	for ( auto& simDataEntry : simData ) {
		if ( std::get<1>(simDataEntry) == 2 ) {
			simGraphPhi2[0]->SetPoint(simGraphPhi2[0]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[3]);
		} else if ( std::get<1>(simDataEntry) == 3 ) {
			simGraphPhi2[1]->SetPoint(simGraphPhi2[1]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[3]);
		} else if ( std::get<1>(simDataEntry) == 4 ) {
			simGraphPhi2[2]->SetPoint(simGraphPhi2[2]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[3]);
		} else if ( std::get<1>(simDataEntry) == 7 ) {
			simGraphPhi2[3]->SetPoint(simGraphPhi2[3]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[3]);
		}
	}

	TCanvas* canvasPhi = new TCanvas("canvasphi", "", 1000, 1000);
	TMultiGraph* mgPhi = new TMultiGraph();
	expGraphPhi2[0]->SetLineColor(kRed);
	expGraphPhi2[0]->SetLineWidth(2);
	expGraphPhi2[0]->SetMarkerColor(kRed);
	expGraphPhi2[0]->SetMarkerStyle(24);
	expGraphPhi2[0]->SetMarkerSize(2);
	mgPhi->Add(expGraphPhi2[0]);
	expGraphPhi2[1]->SetLineColor(kBlue);
	expGraphPhi2[1]->SetLineWidth(2);
	expGraphPhi2[1]->SetMarkerColor(kBlue);
	expGraphPhi2[1]->SetMarkerStyle(24);
	expGraphPhi2[1]->SetMarkerSize(2);
	mgPhi->Add(expGraphPhi2[1]);
	expGraphPhi2[2]->SetLineColor(kMagenta);
	expGraphPhi2[2]->SetLineWidth(2);
	expGraphPhi2[2]->SetMarkerColor(kMagenta);
	expGraphPhi2[2]->SetMarkerStyle(24);
	expGraphPhi2[2]->SetMarkerSize(2);
	mgPhi->Add(expGraphPhi2[2]);
	expGraphPhi2[3]->SetLineColor(kGreen + 3);
	expGraphPhi2[3]->SetLineWidth(2);
	expGraphPhi2[3]->SetMarkerColor(kGreen + 3);
	expGraphPhi2[3]->SetMarkerStyle(24);
	expGraphPhi2[3]->SetMarkerSize(2);
	mgPhi->Add(expGraphPhi2[3]);
	mgPhi->SetTitle(static_cast<TString>("Cluster size (40 - 61) vs. Double Cluster in Simulation; Length [mm]; Ratio to Reference"));
	mgPhi->Draw("AP");

	TMultiGraph* mgSimPhi = new TMultiGraph();
	simGraphPhi2[0]->SetLineColor(kRed);
	simGraphPhi2[0]->SetLineWidth(2);
	simGraphPhi2[0]->SetLineStyle(9);
	simGraphPhi2[0]->SetMarkerColor(kRed);
	simGraphPhi2[0]->SetMarkerSize(2);
	simGraphPhi2[0]->SetMarkerStyle(21);
	mgSimPhi->Add(simGraphPhi2[0]);
	simGraphPhi2[1]->SetLineColor(kBlue);
	simGraphPhi2[1]->SetLineWidth(2);
	simGraphPhi2[1]->SetLineStyle(9);
	simGraphPhi2[1]->SetMarkerColor(kBlue);
	simGraphPhi2[1]->SetMarkerSize(2);
	simGraphPhi2[1]->SetMarkerStyle(21);
	mgSimPhi->Add(simGraphPhi2[1]);
	simGraphPhi2[2]->SetLineColor(kMagenta);
	simGraphPhi2[2]->SetLineWidth(2);
	simGraphPhi2[2]->SetLineStyle(9);
	simGraphPhi2[2]->SetMarkerColor(kMagenta);
	simGraphPhi2[2]->SetMarkerSize(2);
	simGraphPhi2[2]->SetMarkerStyle(21);
	mgSimPhi->Add(simGraphPhi2[2]);
	simGraphPhi2[3]->SetLineColor(kGreen + 3);
	simGraphPhi2[3]->SetLineWidth(2);
	simGraphPhi2[3]->SetLineStyle(9);
	simGraphPhi2[3]->SetMarkerColor(kGreen + 3);
	simGraphPhi2[3]->SetMarkerSize(2);
	simGraphPhi2[3]->SetMarkerStyle(21);
	mgSimPhi->Add(simGraphPhi2[3]);
	mgSimPhi->Draw("P");

	TLegend* legendPhi = new TLegend(0.4, 0.6, 0.9, 0.9);
	legendPhi->AddEntry(expGraphPhi2[0], "Experiment: Width = 2 mm", "p");
	legendPhi->AddEntry(expGraphPhi2[1], "Experiment: Width = 3 mm", "p");
	legendPhi->AddEntry(expGraphPhi2[2], "Experiment: Width = 4 mm", "p");
	legendPhi->AddEntry(expGraphPhi2[3], "Experiment: Width = 7 mm", "p");
	legendPhi->AddEntry(simGraphPhi2[0], "Simulation: Width = 2 mm", "p");
	legendPhi->AddEntry(simGraphPhi2[1], "Simulation: Width = 3 mm", "p");
	legendPhi->AddEntry(simGraphPhi2[2], "Simulation: Width = 4 mm", "p");
	legendPhi->AddEntry(simGraphPhi2[3], "Simulation: Width = 7 mm", "p");
	legendPhi->Draw("SAME");

	canvasPhi->SetLeftMargin(0.12);
	canvasPhi->SetGrid();
	// canvasPhi->SetLogy();
	canvasPhi->SaveAs(static_cast<TString>("double_cluster_ratio_to_reference.png"));
	delete canvasPhi;
}

int main() {
	std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>> expData = getExperimentData();
	std::vector<std::tuple<int, int, std::array<double, 4>>> simData = getSimulationData();

	// drawForLength(2, expData, simData);
	// drawForLength(3, expData, simData);
	// drawForLength(4, expData, simData);
	// drawForLength(7, expData, simData);

	drawOnlyBC(2, expData, simData);
	drawOnlyBC(3, expData, simData);
	drawOnlyBC(4, expData, simData);
	drawOnlyBC(7, expData, simData);

	// drawOnlyA(2, expData, simData);
	// drawOnlyA(3, expData, simData);
	// drawOnlyA(4, expData, simData);
	// drawOnlyA(7, expData, simData);

	// drawOnly4(2, expData, simData);
	// drawOnly4(3, expData, simData);
	// drawOnly4(4, expData, simData);
	// drawOnly4(7, expData, simData);
	// drawOnly4All(expData);
	// drawDoubleCluster(expData, simData);
	// drawOnly1All(expData, simData);


	return 0;
}