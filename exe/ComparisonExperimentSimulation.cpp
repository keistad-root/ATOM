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

int main(int argc, char** argv) {
	int drawLength = atoi(argv[1]);

	io::CSVReader<13> expCSV("/home/ychoi/ATOM/Data/experiment_entry.csv");
	io::CSVReader<9> simCSV("/home/ychoi/ATOM/Data/simulation_entry.csv");

	expCSV.read_header(io::ignore_extra_column, "Collimator", "Length", "Width", "Area", "Area_Error", "Region_A", "Region_A_Error", "Region_B", "Region_B_Error", "Region_C", "Region_C_Error", "Region_D", "Region_D_Error");
	simCSV.read_header(io::ignore_extra_column, "Collimator", "Length", "Width", "Area", "Area_Error", "Region_A", "Region_B", "Region_C", "Region_D");

	std::string collimator;
	int length, width;
	double area, areaError, regionA, regionB, regionC, regionD, regionAError, regionBError, regionCError, regionDError;

	std::array<double, 8> expRef;
	std::vector<std::tuple<int, double, double, double, double, double, double, double, double, double, double>> expData;

	std::array<double, 4> simRef;
	std::vector<std::tuple<int, double, double, double, double, double, double>> simData;

	while ( expCSV.read_row(collimator, length, width, area, areaError, regionA, regionAError, regionB, regionBError, regionC, regionCError, regionD, regionDError) ) {
		if ( length == drawLength ) {
			if ( width == 11 ) {
				expRef = {regionA, regionB, regionC, regionD, regionAError, regionBError, regionCError, regionDError};
			} else {
				expData.push_back(std::make_tuple(width, area, areaError, regionA, regionB, regionC, regionD, regionAError, regionBError, regionCError, regionDError));
			}
		}
	}

	while ( simCSV.read_row(collimator, length, width, area, areaError, regionA, regionB, regionC, regionD) ) {
		if ( length == drawLength ) {
			if ( width == 11 ) {
				simRef = {regionA, regionB, regionC, regionD};
			} else {
				simData.push_back(std::make_tuple(width, area, areaError, regionA, regionB, regionC, regionD));
			}
		}
	}

	TLegend* legend = new TLegend(0.1, 0.6, 0.6, 0.9);

	TGraphErrors* expGraphRegionA = new TGraphErrors();
	TGraphErrors* expGraphRegionB = new TGraphErrors();
	TGraphErrors* expGraphRegionC = new TGraphErrors();
	TGraphErrors* expGraphRegionD = new TGraphErrors();

	for ( int i = 0; i < expData.size(); i++ ) {
		expGraphRegionA->SetPoint(i, std::get<1>(expData[i]), std::get<3>(expData[i]) / expRef[0]);
		expGraphRegionA->SetPointError(i, std::get<2>(expData[i]), TMath::Sqrt(TMath::Power(std::get<7>(expData[i]) / expRef[0], 2) + TMath::Power(std::get<3>(expData[i]) * expRef[4] / TMath::Power(expRef[0], 2), 2)));
		expGraphRegionB->SetPoint(i, std::get<1>(expData[i]), std::get<4>(expData[i]) / expRef[1]);
		expGraphRegionB->SetPointError(i, std::get<2>(expData[i]), TMath::Sqrt(TMath::Power(std::get<8>(expData[i]) / expRef[1], 2) + TMath::Power(std::get<4>(expData[i]) * expRef[5] / TMath::Power(expRef[1], 2), 2)));
		expGraphRegionC->SetPoint(i, std::get<1>(expData[i]), std::get<5>(expData[i]) / expRef[2]);
		expGraphRegionC->SetPointError(i, std::get<2>(expData[i]), TMath::Sqrt(TMath::Power(std::get<9>(expData[i]) / expRef[2], 2) + TMath::Power(std::get<5>(expData[i]) * expRef[6] / TMath::Power(expRef[2], 2), 2)));
		expGraphRegionD->SetPoint(i, std::get<1>(expData[i]), std::get<6>(expData[i]) / expRef[3]);
		expGraphRegionD->SetPointError(i, std::get<2>(expData[i]), TMath::Sqrt(TMath::Power(std::get<10>(expData[i]) / expRef[3], 2) + TMath::Power(std::get<6>(expData[i]) * expRef[7] / TMath::Power(expRef[3], 2), 2)));
	}

	TMultiGraph* mg = new TMultiGraph();
	expGraphRegionA->SetLineColor(kRed);
	expGraphRegionA->SetLineWidth(2);
	expGraphRegionA->SetMarkerColor(kRed);
	expGraphRegionA->SetMarkerStyle(24);
	expGraphRegionA->SetMarkerSize(1.5);
	legend->AddEntry(expGraphRegionA, "Experiment Region A", "pl");
	mg->Add(expGraphRegionA);
	expGraphRegionB->SetLineColor(kBlue);
	expGraphRegionB->SetLineWidth(2);
	expGraphRegionB->SetMarkerColor(kBlue);
	expGraphRegionB->SetMarkerStyle(24);
	expGraphRegionB->SetMarkerSize(1.5);
	legend->AddEntry(expGraphRegionB, "Experiment Region B", "pl");
	mg->Add(expGraphRegionB);
	expGraphRegionC->SetLineColor(kMagenta);
	expGraphRegionC->SetLineWidth(2);
	expGraphRegionC->SetMarkerColor(kMagenta);
	expGraphRegionC->SetMarkerStyle(24);
	expGraphRegionC->SetMarkerSize(1.5);
	legend->AddEntry(expGraphRegionC, "Experiment Region C", "pl");
	mg->Add(expGraphRegionC);
	expGraphRegionD->SetLineColor(kOrange);
	expGraphRegionD->SetLineWidth(2);
	expGraphRegionD->SetMarkerColor(kOrange);
	expGraphRegionD->SetMarkerStyle(24);
	expGraphRegionD->SetMarkerSize(1.5);
	legend->AddEntry(expGraphRegionD, "Experiment Region D", "pl");
	mg->Add(expGraphRegionD);

	TCanvas* canvas = new TCanvas("canvas", "canvas", 1000, 1000);

	mg->SetTitle(static_cast<TString>("Simulation VS. Experiment (L=" + std::to_string(drawLength) + "mm); Width [mm^{2}]; Ratio to Refernce"));
	mg->GetYaxis()->SetRangeUser(0, 1);
	mg->Draw("AP");

	TGraph* simGraphRegionA = new TGraph();
	TGraph* simGraphRegionB = new TGraph();
	TGraph* simGraphRegionC = new TGraph();
	TGraph* simGraphRegionD = new TGraph();

	for ( int i = 0; i < simData.size(); i++ ) {
		simGraphRegionA->SetPoint(i, std::get<1>(simData[i]), std::get<3>(simData[i]) / simRef[0]);
		simGraphRegionB->SetPoint(i, std::get<1>(simData[i]), std::get<4>(simData[i]) / simRef[1]);
		simGraphRegionC->SetPoint(i, std::get<1>(simData[i]), std::get<5>(simData[i]) / simRef[2]);
		simGraphRegionD->SetPoint(i, std::get<1>(simData[i]), std::get<6>(simData[i]) / simRef[3]);
	}

	TMultiGraph* mgSim = new TMultiGraph();
	simGraphRegionA->SetLineColor(kRed);
	simGraphRegionA->SetLineWidth(2);
	simGraphRegionA->SetLineStyle(9);
	simGraphRegionA->SetMarkerColor(kRed);
	simGraphRegionA->SetMarkerSize(1.5);
	simGraphRegionA->SetMarkerStyle(25);
	legend->AddEntry(simGraphRegionA, "The # of single alpha in metal (Simulation)", "pl");
	mgSim->Add(simGraphRegionA);
	simGraphRegionB->SetLineColor(kBlue);
	simGraphRegionB->SetLineWidth(2);
	simGraphRegionB->SetLineStyle(9);
	simGraphRegionB->SetMarkerColor(kBlue);
	simGraphRegionB->SetMarkerSize(1.5);
	simGraphRegionB->SetMarkerStyle(25);
	legend->AddEntry(simGraphRegionB, "The # of electron in metal (Simulation)", "pl");
	mgSim->Add(simGraphRegionB);
	simGraphRegionC->SetLineColor(kMagenta);
	simGraphRegionC->SetLineWidth(2);
	simGraphRegionC->SetLineStyle(9);
	simGraphRegionC->SetMarkerColor(kMagenta);
	simGraphRegionC->SetMarkerSize(1.5);
	simGraphRegionC->SetMarkerStyle(25);
	legend->AddEntry(simGraphRegionC, "The # of single alpha in epitaxial (Simulation)", "pl");
	mgSim->Add(simGraphRegionC);
	simGraphRegionD->SetLineColor(kOrange);
	simGraphRegionD->SetLineWidth(2);
	simGraphRegionD->SetLineStyle(9);
	simGraphRegionD->SetMarkerColor(kOrange);
	simGraphRegionD->SetMarkerSize(1.5);
	simGraphRegionD->SetMarkerStyle(25);
	legend->AddEntry(simGraphRegionD, "The # of double cluster (Simulation)", "pl");
	mgSim->Add(simGraphRegionD);

	mgSim->Draw("P");
	legend->Draw("SAME");
	canvas->SetLeftMargin(1.1);
	canvas->SaveAs(static_cast<TString>(std::to_string(drawLength) + "mm_ratio_to_refernce.png"));
}