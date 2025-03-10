#ifndef __TDATAPLOTTER__
#define __TDATAPLOTTER__

#include<vector>
#include<array>
#include<filesystem>

#include "CppConfigFile.h"

class TH2D;
class TH1D;
class TH2I;
class TFile;
class TGraphErrors;
class TCanvas;

class TDataPlotter {
public:
	TDataPlotter(const CppConfigFile& config);
	~TDataPlotter();
private:
	CppConfigFile mConfig;
	TFile* mInputFile;
	std::filesystem::path mOutputPath;

	TH2D* mHitmap;
	TH1D* mHitmapProjectionX;
	TH1D* mHitmapProjectionY;

	TH2D* mClustermap;
	TH1D* mClustermapProjectionX;
	TH1D* mClustermapProjectionY;
	TH1D* mClustersize;
	std::vector<TH1D*> mClusterSizeOfRegion;
	std::vector<TH1D*> mClustermapSliceX;
	std::vector<TH1D*> mClustermapSliceY;
	TGraphErrors* mClustermapSliceXMean;
	TGraphErrors* mClustermapSliceXAmplitude;
	TGraphErrors* mClustermapSliceYMean;
	TGraphErrors* mClustermapSliceYAmplitude;

	std::vector<std::pair<TH2I*, int>> mShapeSet;
	std::vector<double> mROI = {512, 256};
	std::vector<double> mCenter = {512, 256};

	bool isHitmap = false, isClustermap = false, isClustersize = false, isClustersizeRegion = false, isHitmapProjectionX = false, isHitmapProjectionY = false, isClustermapProjectionX = false, isClustermapProjectionY = false, isClustermapSliceX = false, isClustermapSliceY = false;
public:
	void openInputFile();

	void InitPlot();
	void FillHitInfo();
	void FillClusterInfo();
	void FillShapeInfo();

	void saveTotalShape();
	void saveTop10Shape();
	void saveClusterSizeWithTime();
	void savePlots();

	std::array<int, 60> getEntry();
private:
	void drawCircle(TCanvas* canvas);
};

#endif