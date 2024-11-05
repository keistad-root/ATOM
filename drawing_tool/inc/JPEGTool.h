#ifndef __JPEGTOOL__
#define __JPEGTOOL__

#include<string>
#include <opencv2/opencv.hpp>

#include "TH2I.h"
#include "TStyle.h"
#include "TF1.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TPad.h"

#include "TMathUser.h"

const int N_COLOUR_STEP = 255;

class JPEGTool {
public:
	JPEGTool();
	JPEGTool(const std::string& filePath);
private:
	cv::Mat mImage;
	int mNRow, mNColumn;
	std::array<std::array<int, N_COLOUR_STEP>, 3> mRGBSet;

public:
	void loadJPEGToRGB(const std::string& filePath);

	const cv::Mat getImage() const;
	const int getNRow() const;
	const int getNColumn() const;

	std::array<std::array<int, N_COLOUR_STEP>, 3>& getRGBSet();

	void setRGBSet();

	const std::array<std::vector<int>, 3> getRow(const int row);
	const std::array<std::vector<int>, 3> getColumn(const int column);
};

#endif