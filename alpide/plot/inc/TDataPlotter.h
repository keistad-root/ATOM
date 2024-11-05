#ifndef __TDATAPLOTTER__
#define __TDATAPLOTTER__

// ROOT header
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TStyle.h"

// User header
#include "TALPIDEEvent.h"
#include "TPlotter.h"
#include "CppConfigFile.h"

const int ALPIDECOLUMN = 1024;
const int ALPIDEROW = 512;

// const int CANVAS_WIDTH = 2000; 
// const int CANVAS_HEIGHT = 1000;

// const std::string EXTENSION = "png";

class TDataPlotter : public TPlotter {
public:
	TDataPlotter(const CppConfigFile* config);
	~TDataPlotter();
private:
	const CppConfigFile* mConfig;
	TFile* mInputFile;
	std::filesystem::path mOutputPath;

	TH2D* mHitmap;
	TH1D* mTime;

	TH2D* mClustermap;
	TH1D* mClusterTime;
	TH1D* mClustersize;

	bool isHitmap = false, isClustermap = false, isClustersize = false;
public:
	void InitPlot();
	void FillHitInfo();
	void FillClusterInfo();

	void savePlots();
};

#endif