#include "TThresholdPlotter.h"

TThresholdPlotter::TThresholdPlotter(const CppConfigFile& config) : mConfig(config) {
	CppConfigDictionary fileConfig = mConfig.getConfig("File");
	std::filesystem::path dataPath = fileConfig.find("data_file");
	try {
		if ( !std::filesystem::exists(dataPath) ) throw dataPath;
		mThresholdFile = new TFile(static_cast<TString>(dataPath));
	} catch ( std::filesystem::path dataPath ) {
		std::cerr << "There isn't such file on " << dataPath << std::endl;
		exit(1);
	}
	mOutputPath = fileConfig.find("output_directory");
	std::filesystem::create_directories(mOutputPath);

	gStyle->SetOptStat(0);
}


TThresholdPlotter::~TThresholdPlotter() {
	delete mThrDist;
	delete mNoiseDist;
	delete mQualityDist;
	delete mThrMap;
	delete mNoiseMap;
	delete mQualityMap;
}

void TThresholdPlotter::InitPlot() {
	mThrDist = new TH1D("thresholdDistribution", "", 500, 0, 50);
	mNoiseDist = new TH1D("noiseDistribution", "", 300, 0, 30);
	mQualityDist = new TH1D("qualityDistribution", "", 30, 0, 30);

	mThrMap = new TH2D("thresholdMap", "", ALPIDECOLUMN, 0, ALPIDECOLUMN, ALPIDEROW, 0, ALPIDEROW);
	mNoiseMap = new TH2D("noiseMap", "", ALPIDECOLUMN, 0, ALPIDECOLUMN, ALPIDEROW, 0, ALPIDEROW);
	mQualityMap = new TH2D("qualityMap", "", ALPIDECOLUMN, 0, ALPIDECOLUMN, ALPIDEROW, 0, ALPIDEROW);
}

void TThresholdPlotter::FillPlot() {
	TTree* thresholdTree = static_cast<TTree*>(mThresholdFile->Get("threshold"));
	Int_t x, y;
	Double_t threshold, noise, quality;

	thresholdTree->SetBranchAddress("x", &x);
	thresholdTree->SetBranchAddress("y", &y);
	thresholdTree->SetBranchAddress("threshold", &threshold);
	thresholdTree->SetBranchAddress("noise", &noise);
	thresholdTree->SetBranchAddress("q_factor", &quality);

	Int_t nEntry = thresholdTree->GetEntries();
	for ( Int_t iEntry = 0; iEntry < nEntry; iEntry++ ) {
		thresholdTree->GetEntry(iEntry);
		mThrDist->Fill(threshold);
		mNoiseDist->Fill(noise);
		mQualityDist->Fill(quality);
		mThrMap->Fill(x, y, threshold);
		mNoiseMap->Fill(x, y, noise);
		mQualityMap->Fill(x, y, quality);
	}
}

void TThresholdPlotter::saveThresholdDistribution() const {
	TCanvas* thrDistCanvas = new TCanvas("thrDistCanvas", "", CANVAS_WIDTH, CANVAS_HEIGHT);
	mThrDist->SetTitle("Threshold Distribution; Threshold [DAC]; Entry");
	mThrDist->Draw();
	TString thrDistPath = static_cast<TString>(mOutputPath / "threshold_distribution.png");
	thrDistCanvas->SaveAs(thrDistPath);

	delete thrDistCanvas;
	thrDistCanvas = nullptr;
}

void TThresholdPlotter::saveNoiseDistribution() const {
	TCanvas* noiseDistCanvas = new TCanvas("noiseDistCanvas", "", CANVAS_WIDTH, CANVAS_HEIGHT);
	mNoiseDist->SetTitle("Noise Distribution; Noise [DAC]; Entry");
	mNoiseDist->Draw();

	TString noiseDistPath = static_cast<TString>(mOutputPath / "noise_distribution.png");
	noiseDistCanvas->SaveAs(noiseDistPath);

	delete noiseDistCanvas;
	noiseDistCanvas = nullptr;
}

void TThresholdPlotter::saveQualityDistribution() const {
	TCanvas* qualityDistCanvas = new TCanvas("qualityDistCanvas", "", CANVAS_WIDTH, CANVAS_HEIGHT);
	mQualityDist->SetTitle("Qualtiy Distribution; #chi^{2}/NDoF; Entry");
	mQualityDist->Draw();

	TString qualityDistPath = static_cast<TString>(mOutputPath / "quality_distribution.png");
	qualityDistCanvas->SaveAs(qualityDistPath);

	delete qualityDistCanvas;
	qualityDistCanvas = nullptr;
}

void TThresholdPlotter::saveThresholdMap() const {
	TCanvas* thrMapCanvas = new TCanvas("thrMapCanvas", "", CANVAS_WIDTH, CANVAS_HEIGHT);
	mThrMap->SetTitle("Threshold Map; Column; Row");
	mThrMap->GetZaxis()->SetRangeUser(0, 50);
	mThrMap->Draw("COLZ");

	TString thrMapPath = static_cast<TString>(mOutputPath / "thresholdmap.png");
	thrMapCanvas->SaveAs(thrMapPath);

	delete thrMapCanvas;
	thrMapCanvas = nullptr;
}

void TThresholdPlotter::saveNoiseMap() const {
	TCanvas* noiseMapCanvas = new TCanvas("noiseMapCanvas", "", CANVAS_WIDTH, CANVAS_HEIGHT);
	mNoiseMap->SetTitle("Noise Map; Column; Row");
	mNoiseMap->GetZaxis()->SetRangeUser(0, 20);
	mNoiseMap->Draw("COLZ");

	TString noiseMapPath = static_cast<TString>(mOutputPath / "noisemap.png");
	noiseMapCanvas->SaveAs(noiseMapPath);

	delete noiseMapCanvas;
	noiseMapCanvas = nullptr;
}

void TThresholdPlotter::saveQualityMap() const {
	TCanvas* qualityMapCanvas = new TCanvas("qualityMapCanvas", "", CANVAS_WIDTH, CANVAS_HEIGHT);
	mQualityMap->SetTitle("Quality Map; Column; Row");
	mQualityMap->GetZaxis()->SetRangeUser(0, 20);
	mQualityMap->Draw("COLZ");

	TString qualityMapPath = static_cast<TString>(mOutputPath / "qualtiymap.png");
	qualityMapCanvas->SaveAs(qualityMapPath);

	delete qualityMapCanvas;
	qualityMapCanvas = nullptr;
}

void TThresholdPlotter::saveGraphFile() const {
	TString graphPath = static_cast<TString>(mConfig.getConfig("File").find("graph_file"));
	TFile* graphFile = new TFile(graphPath, "RECREATE");
	mThrDist->Write();
	mNoiseDist->Write();
	mQualityDist->Write();
	mThrMap->Write();
	mNoiseMap->Write();
	mQualityMap->Write();
	graphFile->Close();
}