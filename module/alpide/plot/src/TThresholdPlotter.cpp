#include "TThresholdPlotter.h"

TThresholdPlotter::TThresholdPlotter(const CppConfigFile* config) : TPlotter(config), mConfig(config) {
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
		mThrDist = new TH1D("thresholdDistribution", "", 50, 0, 50);
		setBins(mThrDist, mConfig->getConfig("ThresholdDistribution"));
	}
	if ( isNoiseDist ) {
		mNoiseDist = new TH1D("noiseDistribution", "", 30, 0, 30);
		setBins(mNoiseDist, mConfig->getConfig("NoiseDistribution"));
	}
	if ( isQualityDist ) {
		mQualityDist = new TH1D("qualityDistribution", "", 30, 0, 30);
		setBins(mQualityDist, mConfig->getConfig("QualityDistribution"));
	}
	if ( isThrMap ) {
		mThrMap = new TH2D("thresholdMap", "", ALPIDECOLUMN, 0, ALPIDECOLUMN, ALPIDEROW, 0, ALPIDEROW);
		setBins(mThrMap, mConfig->getConfig("ThresholdMap"));
	}
	if ( isNoiseMap ) {
		mNoiseMap = new TH2D("noiseMap", "", ALPIDECOLUMN, 0, ALPIDECOLUMN, ALPIDEROW, 0, ALPIDEROW);
		setBins(mNoiseMap, mConfig->getConfig("NoiseMap"));
	}
	if ( isQualityMap ) {
		mQualityMap = new TH2D("qualityMap", "", ALPIDECOLUMN, 0, ALPIDECOLUMN, ALPIDEROW, 0, ALPIDEROW);
		setBins(mThrMap, mConfig->getConfig("QualityMap"));
	}
	if ( isThrNoiCorr ) {
		mThrNoi = new TH2D("thrNoiCorrelation", "", 50, 0, 50, 30, 0, 30);
		setBins(mThrNoi, mConfig->getConfig("ThresholdNoiseCorrelation"));
	}
	if ( isThrQuaCorr ) {
		mThrQua = new TH2D("thrQuaCorrelation", "", 50, 0, 50, 10, 0, 10);
		setBins(mThrQua, mConfig->getConfig("ThresholdQualityCorrelation"));
	}
	if ( isNoiQuaCorr ) {
		mNoiQua = new TH2D("NoiseQualityCorrelation", "", 30, 0, 30, 10, 0, 10);
		setBins(mNoiQua, mConfig->getConfig("NoiseQualityCorrelation"));
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
	if ( isThrDist ) savePlot(mThrDist, "ThresholdDistribution");
	if ( isNoiseDist ) savePlot(mNoiseDist, "NoiseDistribution");
	if ( isQualityDist ) savePlot(mQualityDist, "QualityDistribution");
	if ( isThrMap ) savePlot(mThrMap, "ThresholdMap");
	if ( isNoiseMap ) savePlot(mNoiseMap, "NoiseMap");
	if ( isQualityMap ) savePlot(mQualityMap, "QualityMap");
	if ( isThrNoiCorr ) savePlot(mThrNoi, "ThresholdNoiseCorrelation");
	if ( isThrQuaCorr ) savePlot(mThrQua, "ThresholdQualityCorrelation");
	if ( isNoiQuaCorr ) savePlot(mNoiQua, "NoiseQualityCorrelation");
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