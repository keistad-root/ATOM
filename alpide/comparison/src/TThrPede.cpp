#include "TThrPede.h"

TThrPede::TThrPede(CppConfigFile* config) : mConfig(config) {
	TString thresholdFileName = mConfig->getConfig("File").find("threshold_file");
	mThresholdFile = new TFile(thresholdFileName, "READ");

	TString pedestalFileName = mConfig->getConfig("File").find("pedestal_file");
	mPedestalFile = new TFile(pedestalFileName, "READ");
}

TThrPede::~TThrPede() {
	mThresholdFile->Close();
	mPedestalFile->Close();
}

void TThrPede::getThresholdInfo() {
	TTree* tree = static_cast<TTree*>(mThresholdFile->Get("threshold"));
	Int_t x, y;
	Double_t threshold;

	tree->SetBranchAddress("x", &x);
	tree->SetBranchAddress("y", &y);
	tree->SetBranchAddress("threshold", &threshold);

	Int_t nEntry = tree->GetEntries();

	for ( Int_t iEntry = 0; iEntry < nEntry; iEntry++ ) {
		tree->GetEntry(iEntry);
		mThresholdSet[x + y * 1024] = threshold;
	}
}

void TThrPede::getPedestalInfo() {
	TTree* tree = static_cast<TTree*>(mPedestalFile->Get("hit"));
	UShort_t x, y;
	tree->SetBranchAddress("X", &x);
	tree->SetBranchAddress("Y", &y);

	Int_t nEntry = tree->GetEntries();
	for ( int i = 0; i < 1024 * 512; i++ ) {
		mPedestalSet[i] = 0;
	}

	for ( Int_t iEntry = 0; iEntry < nEntry; iEntry++ ) {
		tree->GetEntry(iEntry);
		mPedestalSet[x + y * 1024]++;
	}
}

void TThrPede::getCorrelation() {
	TCanvas* canvas = new TCanvas("canvas", "", 2000, 1000);
	TH2D* correlationMap = new TH2D("correlatoinMap", "Threshold vs Pedestal; Threshold; Pedestal Entry", 200, 0, 50, 100000, 1, 100000);
	for ( Int_t i = 0; i < 1024 * 512; i++ ) {
		correlationMap->Fill(mThresholdSet[i], mPedestalSet[i]);
	}
	canvas->SetLogz();
	canvas->SetLogy();
	correlationMap->Draw("COLZ");
	canvas->SaveAs("/mnt/homes/ychoi/PIDproject/entry_test/plots/correlation.png");
}