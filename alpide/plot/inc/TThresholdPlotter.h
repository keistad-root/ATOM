#ifndef __TTHRESHOLDPLOTTER__
#define __TTHRESHOLDPLOTTER__

// Basic header
#include<filesystem>

// ROOT header
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"

// User header
#include "CppConfigFile.h"

const int ALPIDECOLUMN = 1024;
const int ALPIDEROW = 512;

const int CANVAS_WIDTH = 1500;
const int CANVAS_HEIGHT = 1000;

class TThresholdPlotter {
public:
	TThresholdPlotter(const CppConfigFile& config);
	~TThresholdPlotter();
private:
	CppConfigFile mConfig;
	std::filesystem::path mOutputPath;

	TFile* mThresholdFile;
	TH1D* mThrDist, * mNoiseDist, * mQualityDist;
	TH2D* mThrMap, * mNoiseMap, * mQualityMap;
public:
	void InitPlot();
	void FillPlot();

	void saveThresholdDistribution() const;
	void saveNoiseDistribution() const;
	void saveQualityDistribution() const;
	void saveThresholdMap() const;
	void saveNoiseMap() const;
	void saveQualityMap() const;

	void saveGraphFile() const;
};

#endif