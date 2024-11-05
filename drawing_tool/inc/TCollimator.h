#ifndef __TCOLLIMATOR__
#define __TCOLLIMATOR__

#include<algorithm>

#include "CppConfigFile.h"
#include "JPEGTool.h"
#include "TH2.h"
#include "TStyle.h"
#include "TExec.h"

class TCollimator : public JPEGTool {
public:
	TCollimator(const std::string file);
	~TCollimator();
private:
	bool mIsMap = false;
	TH2I* mRedMap, * mGreenMap, * mBlueMap;

	int mBoxLeft = 200, mBoxRight = 1100, mBoxBottom = 250, mBoxTop = 1600;
	double mBoxRatioLeft = .7, mBoxRatioRight = .7, mBoxRatioTop = .7, mBoxRatioBottom = .49;

	int mHoleLeft = 450, mHoleRight = 810, mHoleBottom = 450, mHoleTop = 810;
	int mHoleThreshold = 200;

	std::vector<std::array<int, 2>> mBox;
	std::vector<std::array<int, 2>> mHole;

public:
	void getComponent(); // Save RGB MAPS
	void saveComponent(const std::string savePath);

	std::array<int, 2> getBoxEndRow(const int row);
	std::array<int, 2> getBoxEndColumn(const int column);

	void getBox();
	void drawBox();

	std::array<int, 2> getHoleEndRow(const int row);
	std::array<int, 2> getHoleEndColumn(const int column);

	void getHole();
	void drawHole();

	void drawTotal();

	void sortHole();
};
#endif