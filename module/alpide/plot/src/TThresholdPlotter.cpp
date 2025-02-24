#include "TThresholdPlotter.h"

TThresholdPlotter::TThresholdPlotter(const CppConfigFile* config) : mConfig(config) {
	CppConfigDictionary fileConfig = mConfig->getConfig("File");
	std::filesystem::path dataPath = fileConfig.find("data_file");
	try {
		if ( !std::filesystem::exists(dataPath) ) throw dataPath;
		mThresholdFile = new TFile(static_cast<TString>(dataPath));
	} catch ( std::filesystem::path dataPath ) {
		std::cerr << "There isn't such file on " << dataPath << std::endl;
		exit(1);
	}

	// gStyle->SetOptStat(0);

	if ( mConfig->hasConfig("ThresholdDistribution") ) isThrDist = true;
	if ( mConfig->hasConfig("NoiseDistribution") ) isNoiseDist = true;
	if ( mConfig->hasConfig("QualityDistribution") ) isQualityDist = true;
	if ( mConfig->hasConfig("ThresholdMap") ) isThrMap = true;
	if ( mConfig->hasConfig("NoiseMap") ) isNoiseMap = true;
	if ( mConfig->hasConfig("QualityMap") ) isQualityMap = true;
	if ( mConfig->hasConfig("ThresholdNoiseCorrelation") ) isThrNoiCorr = true;
	if ( mConfig->hasConfig("ThresholdQualityCorrelation") ) isThrQuaCorr = true;
	if ( mConfig->hasConfig("NoiseQualityCorrelation") ) isNoiQuaCorr = true;

	mOutputPath = fileConfig.find("output_directory");
}

TThresholdPlotter::~TThresholdPlotter() {
	if ( isThrDist ) delete mThrDist;
	if ( isNoiseDist ) delete mNoiseDist;
	if ( isQualityDist ) delete mQualityDist;
	if ( isThrMap ) delete mThrMap;
	if ( isNoiseMap ) delete mNoiseMap;
	if ( isQualityMap ) delete mQualityMap;
	if ( isThrNoiCorr ) delete mThrNoi;
	if ( isThrQuaCorr ) delete mThrQua;
	if ( isNoiQuaCorr ) delete mNoiQua;
}

void TThresholdPlotter::InitPlot() {
	if ( isThrDist ) {
		mThrDist = TPlotter::init1DHist(mConfig->getConfig("ThresholdDistribution"));
	}
	if ( isNoiseDist ) {
		mNoiseDist = TPlotter::init1DHist(mConfig->getConfig("NoiseDistribution"));
	}
	if ( isQualityDist ) {
		mQualityDist = TPlotter::init1DHist(mConfig->getConfig("QualityDistribution"));
	}
	if ( isThrMap ) {
		mThrMap = TPlotter::init2DHist(mConfig->getConfig("ThresholdMap"));
	}
	if ( isNoiseMap ) {
		mNoiseMap = TPlotter::init2DHist(mConfig->getConfig("NoiseMap"));
	}
	if ( isQualityMap ) {
		mQualityMap = TPlotter::init2DHist(mConfig->getConfig("QualityMap"));
	}
	if ( isThrNoiCorr ) {
		mThrNoi = TPlotter::init2DHist(mConfig->getConfig("ThresholdNoiseCorrelation"));
	}
	if ( isThrQuaCorr ) {
		mThrQua = TPlotter::init2DHist(mConfig->getConfig("ThresholdQualityCorrelation"));
	}
	if ( isNoiQuaCorr ) {
		mNoiQua = TPlotter::init2DHist(mConfig->getConfig("NoiseQualityCorrelation"));
	}
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
		if ( isThrDist ) mThrDist->Fill(threshold);
		if ( isNoiseDist ) mNoiseDist->Fill(noise);
		if ( isQualityDist ) mQualityDist->Fill(quality);
		if ( isThrMap ) mThrMap->Fill(x, y, threshold);
		if ( isNoiseMap ) mNoiseMap->Fill(x, y, noise);
		if ( isQualityMap ) mQualityMap->Fill(x, y, quality);
		if ( isThrNoiCorr ) mThrNoi->Fill(threshold, noise);
		if ( isThrQuaCorr ) mThrQua->Fill(threshold, quality);
		if ( isNoiQuaCorr ) mNoiQua->Fill(noise, quality);
	}
}

void TThresholdPlotter::savePlots() {
	std::filesystem::create_directories(mOutputPath);
	if ( isThrDist ) {
		TCanvas* canvas = new TCanvas("thrDistCanvas", "");
		TPlotter::drawPlot(canvas, mThrDist, mConfig->getConfig("ThresholdDistribution"), "COLZ");
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig->getConfig("ThresholdDistribution"));
		delete canvas;
		canvas = nullptr;
	}
	if ( isNoiseDist ) {
		TCanvas* canvas = new TCanvas("noiseDistCanvas", "");
		TPlotter::drawPlot(canvas, mNoiseDist, mConfig->getConfig("NoiseDistribution"), "HIST");
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig->getConfig("NoiseDistribution"));
		delete canvas;
		canvas = nullptr;
	}
	if ( isQualityDist ) {
		TCanvas* canvas = new TCanvas("qualityDistCanvas", "");
		TPlotter::drawPlot(canvas, mQualityDist, mConfig->getConfig("QualityDistribution"), "HIST");
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig->getConfig("QualityDistribution"));
		delete canvas;
		canvas = nullptr;
	}
	if ( isThrMap ) {
		TCanvas* canvas = new TCanvas("thrMapCanvas", "");
		TPlotter::drawPlot(canvas, mThrMap, mConfig->getConfig("ThresholdMap"), "COLZ");
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig->getConfig("ThresholdMap"));
		delete canvas;
		canvas = nullptr;
	}
	if ( isNoiseMap ) {
		TCanvas* canvas = new TCanvas("noiseMapCanvas", "");
		TPlotter::drawPlot(canvas, mNoiseMap, mConfig->getConfig("NoiseMap"), "COLZ");
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig->getConfig("NoiseMap"));
		delete canvas;
		canvas = nullptr;
	}
	if ( isQualityMap ) {
		TCanvas* canvas = new TCanvas("qualityMapCanvas", "");
		TPlotter::drawPlot(canvas, mQualityMap, mConfig->getConfig("QualityMap"), "COLZ");
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig->getConfig("QualityMap"));
		delete canvas;
		canvas = nullptr;
	}
	if ( isThrNoiCorr ) {
		TCanvas* canvas = new TCanvas("thrNoiCanvas", "");
		TPlotter::drawPlot(canvas, mThrNoi, mConfig->getConfig("ThresholdNoiseCorrelation"), "COLZ");
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig->getConfig("ThresholdNoiseCorrelation"));
		delete canvas;
		canvas = nullptr;
	}
	if ( isThrQuaCorr ) {
		TCanvas* canvas = new TCanvas("thrQuaCanvas", "");
		TPlotter::drawPlot(canvas, mThrQua, mConfig->getConfig("ThresholdQualityCorrelation"), "COLZ");
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig->getConfig("ThresholdQualityCorrelation"));
		delete canvas;
		canvas = nullptr;
	}
	if ( isNoiQuaCorr ) {
		TCanvas* canvas = new TCanvas("noiQuaCanvas", "");
		TPlotter::drawPlot(canvas, mNoiQua, mConfig->getConfig("NoiseQualityCorrelation"), "COLZ");
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig->getConfig("NoiseQualityCorrelation"));
		delete canvas;
		canvas = nullptr;
	}
}


void TThresholdPlotter::saveGraphFile() {
	TString graphPath = static_cast<TString>(mConfig->getConfig("File").find("graph_file"));
	TFile* graphFile = new TFile(graphPath, "RECREATE");
	if ( isThrDist ) mThrDist->Write();
	if ( isNoiseDist ) mNoiseDist->Write();
	if ( isQualityDist ) mQualityDist->Write();
	if ( isThrMap ) mThrMap->Write();
	if ( isNoiseMap ) mNoiseMap->Write();
	if ( isQualityMap ) mQualityMap->Write();
	if ( isThrNoiCorr ) mThrNoi->Write();
	if ( isThrQuaCorr ) mThrQua->Write();
	if ( isNoiQuaCorr ) mNoiQua->Write();
	graphFile->Close();
}