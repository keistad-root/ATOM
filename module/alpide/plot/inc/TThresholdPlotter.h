#ifndef __TTHRESHOLDPLOTTER__
#define __TTHRESHOLDPLOTTER__

// Basic header
#include<string>
#include<filesystem>
#include<array>
#include<sstream>

// ROOT header
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"

// User header
#include "CppConfigFile.h"
#include "TPlotter.h"

const int ALPIDECOLUMN = 1024;
const int ALPIDEROW = 512;

class TThresholdPlotter : public TPlotter {
public:
	TThresholdPlotter(const CppConfigFile* config);
	~TThresholdPlotter();
private:
	const CppConfigFile* mConfig;
	std::filesystem::path mOutputPath;

	TFile* mThresholdFile;
	TH1D* mThrDist, * mNoiseDist, * mQualityDist;
	TH2D* mThrMap, * mNoiseMap, * mQualityMap;
	TH2D* mThrNoi, * mThrQua, * mNoiQua;

	bool isThrDist = false, isNoiseDist = false, isQualityDist = false, isThrMap = false, isNoiseMap = false, isQualityMap = false, isThrNoiCorr = false, isThrQuaCorr = false, isNoiQuaCorr = false;
public:
	void InitPlot();
	void FillPlot();
	void savePlots();

	void saveGraphFile();
};

#endif