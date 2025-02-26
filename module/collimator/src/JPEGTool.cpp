#include "JPEGTool.h"

JPEGTool::JPEGTool() {
	setRGBSet();
}

JPEGTool::JPEGTool(const std::string& filePath) {
	setRGBSet();
	loadJPEGToRGB(filePath);
}

const cv::Mat JPEGTool::getImage() const {
	return mImage;
}

const int JPEGTool::getNRow() const {
	return mNRow;
}

const int JPEGTool::getNColumn() const {
	return mNColumn;
}

// Function to load a JPEG image and convert to RGB
void JPEGTool::loadJPEGToRGB(const std::string& filePath) {
	// Read the image
	cv::Mat image = cv::imread(filePath, cv::IMREAD_COLOR); // Load in BGR format
	// Convert BGR to RGB
	cv::cvtColor(image, mImage, cv::COLOR_BGR2RGB);

	mNRow = mImage.rows;
	mNColumn = mImage.cols;
}

void JPEGTool::setRGBSet() {
	double alphaStart = 0.;
	double alphaEnd = 1.;

	for ( int i = 0; i < N_COLOUR_STEP; i++ ) {
		double alpha = alphaStart + i * (alphaEnd - alphaStart) / (N_COLOUR_STEP - 1);
		mRGBSet[0][i] = TColor::GetColorTransparent(kRed, alpha);
		mRGBSet[1][i] = TColor::GetColorTransparent(kGreen, alpha);
		mRGBSet[2][i] = TColor::GetColorTransparent(kBlue, alpha);
	}
}

std::array<std::array<int, N_COLOUR_STEP>, 3>& JPEGTool::getRGBSet() {
	return mRGBSet;
}

const std::array<std::vector<int>, 3> JPEGTool::getRow(const int row) {
	std::vector<int> redArray;
	std::vector<int> greenArray;
	std::vector<int> blueArray;

	for ( int col = 0; col < mImage.cols; col++ ) {
		cv::Vec3b pixel = mImage.at<cv::Vec3b>(row, col);

		redArray.push_back(pixel[0]);
		greenArray.push_back(pixel[1]);
		blueArray.push_back(pixel[2]);
	}

	return {redArray, greenArray, blueArray};
}

const std::array<std::vector<int>, 3> JPEGTool::getColumn(const int column) {
	std::vector<int> redArray;
	std::vector<int> greenArray;
	std::vector<int> blueArray;

	for ( int row = 0; row < mImage.rows; row++ ) {
		cv::Vec3b pixel = mImage.at<cv::Vec3b>(row, column);

		redArray.push_back(pixel[0]);
		greenArray.push_back(pixel[1]);
		blueArray.push_back(pixel[2]);
	}

	return {redArray, greenArray, blueArray};
}
