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

void modeLength(int drawLength) {

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
	expGraphRegionA->SetMarkerSize(2);
	legend->AddEntry(expGraphRegionA, "Experiment Region A", "p");
	mg->Add(expGraphRegionA);
	expGraphRegionB->SetLineColor(kBlue);
	expGraphRegionB->SetLineWidth(2);
	expGraphRegionB->SetMarkerColor(kBlue);
	expGraphRegionB->SetMarkerStyle(24);
	expGraphRegionB->SetMarkerSize(2);
	// legend->AddEntry(expGraphRegionB, "Experiment Region B", "p");
	// mg->Add(expGraphRegionB);
	expGraphRegionC->SetLineColor(kMagenta);
	expGraphRegionC->SetLineWidth(2);
	expGraphRegionC->SetMarkerColor(kMagenta);
	expGraphRegionC->SetMarkerStyle(24);
	expGraphRegionC->SetMarkerSize(2);
	legend->AddEntry(expGraphRegionC, "Experiment Region B + C", "p");
	mg->Add(expGraphRegionC);
	expGraphRegionD->SetLineColor(kGreen + 3);
	expGraphRegionD->SetLineWidth(2);
	expGraphRegionD->SetMarkerColor(kGreen + 3);
	expGraphRegionD->SetMarkerStyle(24);
	expGraphRegionD->SetMarkerSize(2);
	legend->AddEntry(expGraphRegionD, "Experiment Region D", "p");
	mg->Add(expGraphRegionD);

	TCanvas* canvas = new TCanvas("canvas", "canvas", 2000, 1000);

	mg->SetTitle(static_cast<TString>("Simulation VS. Experiment (L=" + std::to_string(drawLength) + "mm); Area [mm^{2}]; Ratio to Refernce"));
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
	simGraphRegionA->SetLineColor(kBlue);
	simGraphRegionA->SetLineWidth(2);
	simGraphRegionA->SetLineStyle(9);
	simGraphRegionA->SetMarkerColor(kBlue);
	simGraphRegionA->SetMarkerSize(2);
	simGraphRegionA->SetMarkerStyle(21);
	mgSim->Add(simGraphRegionA);
	simGraphRegionB->SetLineColor(kRed);
	simGraphRegionB->SetLineWidth(2);
	simGraphRegionB->SetLineStyle(9);
	simGraphRegionB->SetMarkerColor(kRed);
	simGraphRegionB->SetMarkerSize(2);
	simGraphRegionB->SetMarkerStyle(21);
	legend->AddEntry(simGraphRegionB, "The # of electron in metal (Simulation)", "p");
	legend->AddEntry(simGraphRegionA, "The # of single alpha in metal (Simulation)", "p");
	mgSim->Add(simGraphRegionB);
	simGraphRegionC->SetLineColor(kMagenta);
	simGraphRegionC->SetLineWidth(2);
	simGraphRegionC->SetLineStyle(9);
	simGraphRegionC->SetMarkerColor(kMagenta);
	simGraphRegionC->SetMarkerSize(2);
	simGraphRegionC->SetMarkerStyle(21);
	legend->AddEntry(simGraphRegionC, "The # of single alpha in epitaxial (Simulation)", "p");
	mgSim->Add(simGraphRegionC);
	simGraphRegionD->SetLineColor(kGreen + 3);
	simGraphRegionD->SetLineWidth(2);
	simGraphRegionD->SetLineStyle(9);
	simGraphRegionD->SetMarkerColor(kGreen + 3);
	simGraphRegionD->SetMarkerSize(2);
	simGraphRegionD->SetMarkerStyle(21);
	legend->AddEntry(simGraphRegionD, "The # of double cluster (Simulation)", "p");
	mgSim->Add(simGraphRegionD);

	mgSim->Draw("P");
	legend->Draw("SAME");
	canvas->SetLeftMargin(1.1);
	canvas->SaveAs(static_cast<TString>(std::to_string(drawLength) + "mm_ratio_to_refernce.png"));
}

void modeWidth(int drawWidth) {
	io::CSVReader<63> expCSV("/home/ychoi/ATOM/Data/clustersize_entry.csv");

	expCSV.read_header(io::ignore_extra_column, "length", "width", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "60", "61");

	std::tuple<int, int, std::array<double, 61>> expEntry;
	std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>> expData;
	std::vector<std::array<int, 2>> regionDivide = {{1, 4}, {5, 10}, {11, 32}, {40, 61}};
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
		expData.push_back(std::make_tuple(std::get<0>(expEntry), std::get<1>(expEntry), regionEntry, regionEntryError));
		regionEntry = {0, 0, 0, 0};
	}

	io::CSVReader<4> scaleCSV("/home/ychoi/ATOM/Data/time_scale.csv");
	scaleCSV.read_header(io::ignore_extra_column, "length", "width", "time", "scale");
	int length, width, time;
	double scale;
	while ( scaleCSV.read_row(length, width, time, scale) ) {
		for ( auto& expDataEntry : expData ) {
			if ( std::get<0>(expDataEntry) == length && std::get<1>(expDataEntry) == width ) {
				regionEntryError[0] = TMath::Sqrt(std::get<2>(expDataEntry)[0] / scale) * scale;
				regionEntryError[1] = TMath::Sqrt(std::get<2>(expDataEntry)[1] / scale) * scale;
				regionEntryError[2] = TMath::Sqrt(std::get<2>(expDataEntry)[2] / scale) * scale;
				regionEntryError[3] = TMath::Sqrt(std::get<2>(expDataEntry)[3] / scale) * scale;
				std::get<3>(expDataEntry) = regionEntryError;
			}
		}
	}
	for ( auto& expDataEntry : expData ) {
		std::cout << std::get<0>(expDataEntry) << " " << std::get<1>(expDataEntry) << " " << std::get<2>(expDataEntry)[0] << " " << std::get<3>(expDataEntry)[0] << " " << std::get<2>(expDataEntry)[1] << " " << std::get<3>(expDataEntry)[1] << " " << std::get<2>(expDataEntry)[2] << " " << std::get<3>(expDataEntry)[2] << " " << std::get<2>(expDataEntry)[3] << std::get<3>(expDataEntry)[3] << " " << std::endl;
	}
}

int main(int argc, char** argv) {
	std::string mode = argv[1];
	int drawLength = atoi(argv[2]);
	if ( mode == "length" ) {
		modeLength(drawLength);
	}
	if ( mode == "width" ) {
		modeWidth(drawLength);
	}



}