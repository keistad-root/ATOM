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
		if ( length == 1 ) {
			if ( width == 11 ) {
				expRef = {regionA, regionB, regionC, regionD, regionAError, regionBError, regionCError, regionDError};
			} else {
				expData.push_back(std::make_tuple(width, area, areaError, regionA, regionB, regionC, regionD, regionAError, regionBError, regionCError, regionDError));
			}
		}
	}

	while ( simCSV.read_row(collimator, length, width, area, areaError, regionA, regionB, regionC, regionD) ) {
		if ( length == 1 ) {
			if ( width == 11 ) {
				simRef = {regionA, regionB, regionC, regionD};
			} else {
				simData.push_back(std::make_tuple(width, area, areaError, regionA, regionB, regionC - 2 * regionD, regionD));
			}
		}
	}

	TLegend* legend = new TLegend(0.1, 0.6, 0.4, 0.9);

	TGraphErrors* expGraphRegionA = new TGraphErrors();
	TGraphErrors* expGraphRegionB = new TGraphErrors();
	TGraphErrors* expGraphRegionC = new TGraphErrors();
	TGraphErrors* expGraphRegionD = new TGraphErrors();

	for ( int i = 0; i < expData.size(); i++ ) {
		expGraphRegionA->SetPoint(i, std::get<1>(expData[i]), std::get<3>(expData[i]) / expRef[0]);
		expGraphRegionA->SetPointError(i, std::get<2>(expData[i]), TMath::Sqrt(TMath::Power(std::get<7>(expData[i]) / expRef[0], 2) + TMath::Power(std::get<3>(expData[i]) * expRef[4] / TMath::Power(expRef[0], 2), 2)));
		expGraphRegionB->SetPoint(i, std::get<1>(expData[i]), std::get<4>(expData[i]) / expRef[1]);
		std::cout << TMath::Power(std::get<8>(expData[i]) / expRef[1], 2) << " " << TMath::Power(std::get<4>(expData[i]) * expRef[5] / TMath::Power(expRef[1], 2), 2) << std::endl;
		expGraphRegionB->SetPointError(i, std::get<2>(expData[i]), TMath::Sqrt(TMath::Power(std::get<8>(expData[i]) / expRef[1], 2) + TMath::Power(std::get<4>(expData[i]) * expRef[5] / TMath::Power(expRef[1], 2), 2)));
		expGraphRegionC->SetPoint(i, std::get<1>(expData[i]), std::get<5>(expData[i]) / expRef[2]);
		expGraphRegionC->SetPointError(i, std::get<2>(expData[i]), TMath::Sqrt(TMath::Power(std::get<9>(expData[i]) / expRef[2], 2) + TMath::Power(std::get<5>(expData[i]) * expRef[6] / TMath::Power(expRef[2], 2), 2)));
		expGraphRegionD->SetPoint(i, std::get<1>(expData[i]), std::get<6>(expData[i]) / expRef[3]);
		expGraphRegionD->SetPointError(i, std::get<2>(expData[i]), TMath::Sqrt(TMath::Power(std::get<10>(expData[i]) / expRef[3], 2) + TMath::Power(std::get<6>(expData[i]) * expRef[7] / TMath::Power(expRef[3], 2), 2)));
	}

	TMultiGraph* mg = new TMultiGraph();
	expGraphRegionA->SetLineColor(kRed);
	expGraphRegionA->SetLineWidth(2);
	legend->AddEntry(expGraphRegionA, "Experiment Region A", "l");
	mg->Add(expGraphRegionA);
	expGraphRegionB->SetLineColor(kBlue);
	expGraphRegionB->SetLineWidth(2);
	legend->AddEntry(expGraphRegionB, "Experiment Region B", "l");
	mg->Add(expGraphRegionB);
	expGraphRegionC->SetLineColor(kMagenta);
	expGraphRegionC->SetLineWidth(2);
	legend->AddEntry(expGraphRegionC, "Experiment Region C", "l");
	mg->Add(expGraphRegionC);
	expGraphRegionD->SetLineColor(kGreen);
	expGraphRegionD->SetLineWidth(2);
	legend->AddEntry(expGraphRegionD, "Experiment Region D", "l");
	mg->Add(expGraphRegionD);


	mg->SetMaximum(1);
	mg->SetMinimum(0);
	mg->SetTitle("Simulation VS. Experiment");
	mg->GetXaxis()->SetTitle("Width");
	mg->GetYaxis()->SetTitle("Ratio to Reference");
	TCanvas* canvas = new TCanvas("canvas", "canvas", 1000, 1000);
	mg->Draw("APL");

	TGraph* simGraphRegionA = new TGraph();
	TGraph* simGraphRegionB = new TGraph();
	TGraph* simGraphRegionC = new TGraph();
	TGraph* simGraphRegionD = new TGraph();

	for ( int i = 0; i < expData.size(); i++ ) {
		simGraphRegionA->SetPoint(i, std::get<1>(simData[i]), std::get<3>(simData[i]) / simRef[0]);
		simGraphRegionB->SetPoint(i, std::get<1>(simData[i]), std::get<4>(simData[i]) / simRef[1]);
		simGraphRegionC->SetPoint(i, std::get<1>(simData[i]), std::get<5>(simData[i]) / simRef[2]);
		simGraphRegionD->SetPoint(i, std::get<1>(simData[i]), std::get<6>(simData[i]) / simRef[3]);
	}

	TMultiGraph* mgSim = new TMultiGraph();
	simGraphRegionA->SetLineColor(kRed);
	simGraphRegionA->SetLineWidth(2);
	simGraphRegionA->SetLineStyle(9);
	legend->AddEntry(simGraphRegionA, "Simulation Region A", "l");
	mgSim->Add(simGraphRegionA);
	simGraphRegionB->SetLineColor(kBlue);
	simGraphRegionB->SetLineWidth(2);
	simGraphRegionB->SetLineStyle(9);
	legend->AddEntry(simGraphRegionB, "Simulation Region B", "l");
	mgSim->Add(simGraphRegionB);
	simGraphRegionC->SetLineColor(kMagenta);
	simGraphRegionC->SetLineWidth(2);
	simGraphRegionC->SetLineStyle(9);
	legend->AddEntry(simGraphRegionC, "Simulation Region C", "l");
	mgSim->Add(simGraphRegionC);
	simGraphRegionD->SetLineColor(kGreen);
	simGraphRegionD->SetLineWidth(2);
	simGraphRegionD->SetLineStyle(9);
	legend->AddEntry(simGraphRegionD, "Simulation Region D", "l");
	mgSim->Add(simGraphRegionD);

	mgSim->Draw("PL");
	legend->Draw();
	canvas->SaveAs("expGraph.png");
}