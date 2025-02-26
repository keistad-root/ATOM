#include<memory>
#include<filesystem>
#include<iostream>

#include "TFile.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TLegend.h"

void compareGeant4(int leftNum, int rightNum) {
	std::filesystem::path plotPath = "/mnt/homes/ychoi/entry_test/geant4/plot/";

	// Open the first file and get the first histogram
	TString file1Path = Form("%sincident_plot_%dmm_nocollimator_alpha_noscreen.root", plotPath.c_str(), leftNum);
	TFile* file1 = new TFile(file1Path, "READ");
	TH2D* hist1 = static_cast<TH2D*>(file1->Get("AlphaCorrelationDepositEnergyEpitaxialAndIncidentAngle"));
	if ( leftNum == 20 ) {
		hist1->Scale(1. / 6);
		for ( int i = 1; i <= hist1->GetNbinsX(); ++i ) {
			for ( int j = 1; j <= hist1->GetNbinsY(); ++j ) {
				hist1->SetBinContent(i, j, (int) hist1->GetBinContent(i, j));
			}
		}
	}
	// Open the second file and get the second histogram
	TFile* file2 = new TFile(static_cast<TString>(plotPath / ("incident_plot_" + std::to_string(rightNum) + "mm_nocollimator_alpha_noscreen.root")), "READ");

	TH2D* hist2 = static_cast<TH2D*>(file2->Get("AlphaCorrelationDepositEnergyEpitaxialAndIncidentAngle"));
	// Create a new histogram to store the result
	TH2D* resultHist = (static_cast<TH2D*>(hist1->Clone("resultHist")));
	if ( rightNum == 20 ) {
		hist2->Scale(1. / 6);
		for ( int i = 1; i <= hist2->GetNbinsX(); ++i ) {
			for ( int j = 1; j <= hist2->GetNbinsY(); ++j ) {
				hist2->SetBinContent(i, j, (int) hist2->GetBinContent(i, j));
			}
		}
	}

	// Subtract the second histogram from the first
	resultHist->Add(hist2, -1);

	// Set negative values to zero
	for ( int i = 1; i <= resultHist->GetNbinsX(); ++i ) {
		for ( int j = 1; j <= resultHist->GetNbinsY(); ++j ) {
			if ( resultHist->GetBinContent(i, j) < 0 ) {
				resultHist->SetBinContent(i, j, 0);
			}
		}
	}
	resultHist->SetEntries(resultHist->GetEffectiveEntries());

	// Create a canvas to draw the result
	TCanvas* canvas = new TCanvas("canvas", "", 1000, 1000);
	TString title = Form("L%dREF - L%dREF (Positive Only); Incident Angle [Deg]; Energy [MeV]", leftNum, rightNum);
	resultHist->SetTitle(title);
	resultHist->Draw("COLZ0");
	resultHist->SetMinimum(0);
	// Save the result to a file
	TString savePath = Form("/mnt/homes/ychoi/entry_test/geant4/result/alpha_correlation_%dmm_%dmm.pdf", leftNum, rightNum);
	canvas->SetRightMargin(0.12);
	canvas->SaveAs(savePath);
	delete canvas;
}

void getClusterSizeHist(TH1D*& hist, int leftNum, int rightNum) {
	std::filesystem::path plotPath = "/mnt/homes/ychoi/entry_test/analysis/";

	TFile* LeftAlphaGammaFile = new TFile((plotPath / (std::to_string(leftNum) + "mm_nocollimator_alphagamma.root")).c_str(), "READ");
	TTree* LeftAlphaGammaTree = (TTree*) LeftAlphaGammaFile->Get("cluster");

	TH1D* LeftAlphaGammaHist = new TH1D("LeftAlphaGammaHist", "Cluster Size; Cluster Size; Counts", 100, 0, 100);

	UInt_t clusterSize;
	LeftAlphaGammaTree->SetBranchAddress("Size", &clusterSize);

	for ( int i = 0; i < LeftAlphaGammaTree->GetEntries(); ++i ) {
		LeftAlphaGammaTree->GetEntry(i);
		LeftAlphaGammaHist->Fill(clusterSize);
	}

	TFile* LeftGammaFile = new TFile((plotPath / (std::to_string(leftNum) + "mm_nocollimator_gamma.root")).c_str(), "READ");

	TTree* LeftGammaTree = (TTree*) LeftGammaFile->Get("cluster");

	TH1D* LeftGammaHist = new TH1D("LeftGammaHist", "Cluster Size; Cluster Size; Counts", 100, 0, 100);

	LeftGammaTree->SetBranchAddress("Size", &clusterSize);
	for ( int i = 0; i < LeftGammaTree->GetEntries(); ++i ) {
		LeftGammaTree->GetEntry(i);
		LeftGammaHist->Fill(clusterSize);
	}


	TFile* RightAlphaGammaFile = new TFile((plotPath / (std::to_string(rightNum) + "mm_nocollimator_alphagamma.root")).c_str(), "READ");

	TTree* RightAlphaGammaTree = (TTree*) RightAlphaGammaFile->Get("cluster");

	TH1D* RightAlphaGammaHist = new TH1D("RightAlphaGammaHist", "Cluster Size; Cluster Size; Counts", 100, 0, 100);

	RightAlphaGammaTree->SetBranchAddress("Size", &clusterSize);

	for ( int i = 0; i < RightAlphaGammaTree->GetEntries(); ++i ) {
		RightAlphaGammaTree->GetEntry(i);
		RightAlphaGammaHist->Fill(clusterSize);
	}

	TFile* RightGammaFile = new TFile((plotPath / (std::to_string(rightNum) + "mm_nocollimator_gamma.root")).c_str(), "READ");

	TTree* RightGammaTree = (TTree*) RightGammaFile->Get("cluster");

	TH1D* RightGammaHist = new TH1D("RightGammaHist", "Cluster Size; Cluster Size; Counts", 100, 0, 100);

	RightGammaTree->SetBranchAddress("Size", &clusterSize);
	for ( int i = 0; i < RightGammaTree->GetEntries(); ++i ) {
		RightGammaTree->GetEntry(i);
		RightGammaHist->Fill(clusterSize);
	}

	hist = static_cast<TH1D*>(LeftAlphaGammaHist->Clone("hist"));
	hist->Add(LeftGammaHist, -1);
	hist->Add(RightAlphaGammaHist, -1);
	hist->Add(RightGammaHist, 1);
}



int main(int argc, char** argv) {
	compareGeant4(3, 1);
	compareGeant4(7, 1);
	compareGeant4(20, 1);
	compareGeant4(1, 3);
	compareGeant4(1, 7);
	compareGeant4(1, 20);
	return 0;
}