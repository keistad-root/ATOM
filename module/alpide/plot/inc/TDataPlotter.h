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
	std::vector<TH1D*> mHitmapSliceX;
	std::vector<TH1D*> mClustermapSliceY;
	std::vector<TH1D*> mHitmapSliceY;
	TGraphErrors* mClustermapSliceXMean;
	TGraphErrors* mClustermapSliceXAmplitude;
	TGraphErrors* mClustermapSliceYMean;
	TGraphErrors* mClustermapSliceYAmplitude;
	TGraphErrors* mHitmapSliceXMean;
	TGraphErrors* mHitmapSliceXAmplitude;
	TGraphErrors* mHitmapSliceYMean;
	TGraphErrors* mHitmapSliceYAmplitude;

	std::vector<std::pair<TH2I*, int>> mShapeSet;
	std::vector<double> mROI = {512, 256};
	double mROITheta = 0;
	std::vector<double> mCenter = {512, 256};

	bool isHitmap = false, isClustermap = false, isClustersize = false, isClustersizeRegion = false, isHitmapProjectionX = false, isHitmapProjectionY = false, isClustermapProjectionX = false, isClustermapProjectionY = false, isHitmapSliceX = false, isHitmapSliceY = false, isClustermapSliceX = false, isClustermapSliceY = false;
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

	bool isInsideRegion(const double x, const double y);

	std::array<int, 60> getEntry();
private:
	void drawCircle(TCanvas* canvas);
};

#endif