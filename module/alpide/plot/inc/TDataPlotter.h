#ifndef __TDATAPLOTTER__
#define __TDATAPLOTTER__

// ROOT header
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TStyle.h"
#include "TLine.h"
#include "TPad.h"
#include "TText.h"
#include "TEllipse.h"
#include "TF1.h"

// User header
#include "TALPIDEEvent.h"
#include "TPlotter.h"
#include "CppConfigFile.h"
#include "TClusterShape.h"

const int ALPIDECOLUMN = 1024;
const int ALPIDEROW = 512;

// const int CANVAS_WIDTH = 2000; 
// const int CANVAS_HEIGHT = 1000;

// const std::string EXTENSION = "png";

class TDataPlotter : public TPlotter {
public:
	TDataPlotter(const CppConfigFile& config);
	~TDataPlotter();
private:
	CppConfigFile mConfig;
	std::unique_ptr<TFile> mInputFile;
	std::filesystem::path mOutputPath;

	TH2D* mHitmap;
	TH1D* mHitmapProjectionX;
	TH1D* mHitmapProjectionY;
	TH1D* mTime;

	TH2D* mClustermap;
	TH1D* mClusterTime;
	TH1D* mClustersize;
	std::vector<TH1D*> mClusterSizeOfRegion;

	std::vector<std::pair<TH2I*, int>> mShapeSet;

	bool isHitmap = false, isClustermap = false, isClustersize = false, isClustersizeRegion = false, isHitmapProjectionX = false, isHitmapProjectionY = false;
public:
	void openInputFile();

	void InitPlot();
	void FillHitInfo();
	void FillClusterInfo();
	void FillShapeInfo();

	void saveTotalShape();
	void saveTop10Shape();
	void saveClusterSizeWithTime();

	void getMeanX();

	void savePlots();

	std::array<int, 60> getEntry();
};

#endif