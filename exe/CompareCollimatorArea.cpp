#include<iostream>
#include<filesystem>
#include<csv.h>

#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TText.h"
#include "TMath.h"

// #include "TCSV.h"

int main(int argc, char** argv) {
	std::filesystem::path csvPath = "../Data/collimator_area_low_cut.csv";
	io::CSVReader<4> csv(csvPath);

	csv.read_header(io::ignore_extra_column, "LENGTH", "PHI", "AREA", "AREA_ERROR");

	int length = 0;
	int phi = 0;
	double area = 0.;
	double error = 0.;

	TGraphErrors* areaGraph = new TGraphErrors();
	TGraphErrors* radiusGraph = new TGraphErrors();

	int iRow = 0;
	while ( csv.read_row(length, phi, area, error) ) {
		areaGraph->SetPoint(iRow, length, area);
		radiusGraph->SetPoint(iRow, length, std::sqrt(area / TMath::Pi()));
		areaGraph->SetPointError(iRow, 0, error);
		radiusGraph->SetPointError(iRow, 0, (1. / 2) * error / std::sqrt(area / TMath::Pi()));
		iRow++;
	}

	TCanvas* areaCanvas = new TCanvas("areaCanvas", "", 1000, 800);
	areaGraph->SetTitle("Collimator Area; Collimator Length [mm]; Collimator Area [mm^{2}]");
	areaGraph->SetMarkerStyle(20);
	areaGraph->SetLineWidth(3);
	areaGraph->Draw("AP");

	areaCanvas->SetGrid();
	areaCanvas->SetLogy();
	areaCanvas->SaveAs("/mnt/homes/ychoi/entry_test/cut_200_collimator_area.png");

	TCanvas* radiusCanvas = new TCanvas("radiusCanvas", "", 1000, 800);
	radiusGraph->SetTitle("Collimator Radius; Collimator Length [mm]; Collimator Radius [mm]");
	radiusGraph->SetMarkerStyle(20);
	radiusGraph->SetLineWidth(3);
	radiusGraph->Draw("AP");

	radiusCanvas->SetGrid();
	// radiusCanvas->SetLogy();
	radiusCanvas->SaveAs("/mnt/homes/ychoi/entry_test/cut_200_collimator_radius.png");
}