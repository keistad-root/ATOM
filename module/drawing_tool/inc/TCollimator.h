#ifndef __TCOLLIMATOR__
#define __TCOLLIMATOR__

#include<algorithm>
#include<array>

#include "CppConfigFile.h"
#include "JPEGTool.h"
#include "TH2.h"
#include "TStyle.h"
#include "TExec.h"
#include "TPlotter.h"
#include "TMathUser.h"

class TCollimatorHole {
public:
	TCollimatorHole(const std::array<int, 4>& mBoundaries, const int mThreshold);
private:
	std::array<int, 4> mBoundaries;
	int mThreshold;
	TH2I* mHole;

	int mSize = 0;
	int mError = 0;

public:
	void getHole(const cv::Mat& image);
	void drawHole(std::filesystem::path savePath);
	const std::array<int, 2> getSize() const;
};

class TCollimatorRuler {
public:
	TCollimatorRuler(const std::array<int, 2>& boundaries, const int mThreshold);
private:
	std::array<int, 2> mBoundaries;
	int mThreshold;
	std::array<int, 2> mScaleCut;
	std::array<int, 2> mDistanceCut;
	TH2I* mRuler;
public:
	void getRuler(const cv::Mat& image);
	void drawRuler(std::filesystem::path savePath);
	void setDistanceCut(std::array<int, 2> distanceCut);
	void setScaleCut(std::array<int, 2> scaleCut);
	std::array<double, 2> getInterval();
};

class TCollimator : public JPEGTool {
public:
	TCollimator(const std::string file);
	TCollimator(CppConfigFile* config);
	~TCollimator();
private:
	CppConfigFile* mConfig;
	std::filesystem::path mOutputDirectory;

	bool mIsMap = false;
	TH2I* mRedMap, * mGreenMap, * mBlueMap;

	TCollimatorHole* mHole;
	TCollimatorRuler* mRuler;
public:
	void getComponent(); // Save RGB MAPS
	void saveComponent();

	void getHole();
	void drawHole();

	void getRuler();
	void drawRuler();

	void drawRow(const int iRow);

	void getArea();
};
#endif