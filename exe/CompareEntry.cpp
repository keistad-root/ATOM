#include<iostream>
#include<filesystem>
#include<tuple>

#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TText.h"
#include "TMath.h"
#include "cppargs.h"
#include "TLegend.h"
#include "TAxis.h"

#include<csv.h>

int main(int argc, char** argv) {
	std::filesystem::path csvPath = "../Data/entry_simulation_high_cut.csv";
	io::CSVReader<9> csv(csvPath);

	csv.read_header(io::ignore_extra_column, "LENGTH", "PHI", "AREA", "AREA_ERROR", "RATIO", "RATIO_LOW", "RATIO_HIGH", "DATA", "DATA_ERROR");

	int length = 0, phi = 0;
	double area = 0., areaError = 0.;
	double ratio = 0., ratioLow = 0., ratioHigh = 0.;
	double data = 0., dataError = 0.;

	std::vector<std::tuple<int, int, double, double, double, double, double, double, double>> valueTuple;

	std::array<std::tuple<int, int, int, double, double, double>, 4> criteriaTuple;

	int iCriteria = 0;
	while ( csv.read_row(length, phi, area, areaError, ratio, ratioLow, ratioHigh, data, dataError) ) {
		valueTuple.push_back(std::make_tuple(length, phi, area, areaError, ratio, ratioLow, ratioHigh, data, dataError));
		if ( phi == 4 ) {
			criteriaTuple[iCriteria] = std::make_tuple(length, data, dataError, ratio, ratioLow, ratioHigh);
			iCriteria++;
		}
	}

	TGraphAsymmErrors* ratioGraph = new TGraphAsymmErrors();
	TGraphErrors* dataGraph = new TGraphErrors();

	int iRow = 0;
	std::vector<TString> xTitle;
	for ( const auto& [length, phi, area, areaError, ratio, ratioLow, ratioHigh, data, dataError] : valueTuple ) {
		double expectation;
		double upperExpectation;
		double lowerExpectation;
		for ( const auto& [criteriaLength, criteriaData, criteriaDataError, criteriaRatio, criteriaRatioLow, criteriaRatioHigh] : criteriaTuple ) {
			if ( length == criteriaLength ) {
				expectation = criteriaData * ratio / criteriaRatio;
				upperExpectation = (criteriaData + criteriaDataError) * ratioHigh / criteriaRatioLow;
				lowerExpectation = (criteriaData - criteriaDataError) * ratioLow / criteriaRatioHigh;
				break;
			}
		}
		ratioGraph->SetPoint(iRow, iRow + 1, expectation);
		ratioGraph->SetPointError(iRow, 0, 0, expectation - lowerExpectation, upperExpectation);

		dataGraph->SetPoint(iRow, iRow + 1, data);
		dataGraph->SetPointError(iRow, 0, dataError);
		iRow++;
		xTitle.push_back(Form("L%d#phi%d", length, phi));
	}


	for ( int i = 0; i < ratioGraph->GetN(); i++ ) {
		ratioGraph->GetXaxis()->SetBinLabel(ratioGraph->GetXaxis()->FindBin(i + 1.), xTitle[i]);
	}

	TCanvas* ratioCanvas = new TCanvas("ratioCanvas", "", 1000, 800);
	ratioGraph->SetTitle("Data vs. Expectation; Collimator; Entry");
	ratioGraph->SetMarkerStyle(20);
	ratioGraph->SetMarkerColor(kRed);
	ratioGraph->SetLineColor(kRed);
	ratioGraph->SetMaximum(1000000);
	ratioGraph->SetMinimum(1);
	ratioGraph->GetXaxis()->SetTitleOffset(1.7);
	ratioGraph->Draw("AP");
	dataGraph->SetMarkerStyle(20);
	dataGraph->SetMarkerColor(kBlue);
	dataGraph->Draw("P");

	TLegend* legend = new TLegend(0.7, 0.2, 0.9, 0.4);
	legend->AddEntry(ratioGraph, "Expectation", "p");
	legend->AddEntry(dataGraph, "Data", "p");
	legend->Draw();

	ratioCanvas->SetGrid();
	ratioCanvas->SetLogy();
	ratioCanvas->SetBottomMargin(0.15);
	ratioCanvas->SaveAs("/mnt/homes/ychoi/entry_test/cut_251_entry_comparison_length.png");

	TGraphAsymmErrors* ratioDiffGraph = new TGraphAsymmErrors();
	for ( int i = 0; i < ratioGraph->GetN(); i++ ) {
		double ratio = (dataGraph->GetY()[i] - ratioGraph->GetY()[i]) / ratioGraph->GetY()[i];
		double upperRatio = ((dataGraph->GetY()[i] + dataGraph->GetEY()[i]) - (ratioGraph->GetY()[i] - ratioGraph->GetEYlow()[i])) / (ratioGraph->GetY()[i] - ratioGraph->GetEYlow()[i]);
		double lowerRatio = ((dataGraph->GetY()[i] - dataGraph->GetEY()[i]) - (ratioGraph->GetY()[i] + ratioGraph->GetEYhigh()[i])) / (ratioGraph->GetY()[i] + ratioGraph->GetEYhigh()[i]);

		ratioDiffGraph->SetPoint(i, i + 1, ratio);
		ratioDiffGraph->SetPointError(i, 0, 0, ratio - lowerRatio, upperRatio - ratio);
	}
	for ( int i = 0; i < ratioGraph->GetN(); i++ ) {
		ratioDiffGraph->GetXaxis()->SetBinLabel(ratioDiffGraph->GetXaxis()->FindBin(i + 1.), xTitle[i]);
	}

	TCanvas* ratioDiffCanvas = new TCanvas("ratioDiffCanvas", "", 1000, 800);
	ratioDiffGraph->SetTitle("Data vs. Expectation; Collimator; #frac{Data - Expectation}{Expectation}");
	ratioDiffGraph->SetMarkerStyle(20);
	ratioDiffGraph->SetMarkerColor(kBlack);
	ratioDiffGraph->SetMaximum(3);
	ratioDiffGraph->SetMinimum(-3);
	ratioDiffGraph->GetXaxis()->SetTitleOffset(1.7);
	ratioDiffGraph->Draw("AP");
	ratioDiffCanvas->SetGrid();
	ratioDiffCanvas->SetBottomMargin(0.15);
	ratioDiffCanvas->SaveAs("/mnt/homes/ychoi/entry_test/cut_251_entry_comparison_ratio_length.png");
}