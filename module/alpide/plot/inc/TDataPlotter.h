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
	TH1D* mTime;

	TH2D* mClustermap;
	TH1D* mClustermapProjectionX;
	TH1D* mClustermapProjectionY;
	TH1D* mClusterTime;
	TH1D* mClustersize;
	std::vector<TH1D*> mClusterSizeOfRegion;

	std::vector<std::pair<TH2I*, int>> mShapeSet;

	bool isHitmap = false, isClustermap = false, isClustersize = false, isClustersizeRegion = false, isHitmapProjectionX = false, isHitmapProjectionY = false, isClustermapProjectionX = false, isClustermapProjectionY = false;
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
	void getMeanY();

	void savePlots();

	std::array<int, 60> getEntry();
};

#endif