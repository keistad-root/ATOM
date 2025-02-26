#include "TCollimator.h"

TCollimatorHole::TCollimatorHole(const std::array<int, 4>& boundaries, const int threshold) {
	mBoundaries = boundaries;
	mThreshold = threshold;

	mHole = new TH2I("hist", "Hole; Column; Row", mBoundaries[1] - mBoundaries[0], mBoundaries[0], mBoundaries[1], mBoundaries[3] - mBoundaries[2], mBoundaries[2], mBoundaries[3]);
}

void TCollimatorHole::getHole(const cv::Mat& image) {
	int maxSize = 0;
	int minSize = 0;
	for ( int y = mBoundaries[2]; y < mBoundaries[3]; y++ ) {
		for ( int x = mBoundaries[0]; x < mBoundaries[1]; x++ ) {
			cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);
			int dRed = pixel[0] > mThreshold ? 1 : 0;
			int dGreen = pixel[1] > mThreshold ? 1 : 0;
			int dBlue = pixel[2] > mThreshold ? 1 : 0;

			if ( dRed + dGreen + dBlue > 0 ) {
				mHole->SetBinContent(x - mBoundaries[0], y - mBoundaries[2], 1);
				maxSize++;
			}
			if ( dRed + dGreen + dBlue > 2 ) {
				mHole->SetBinContent(x - mBoundaries[0], y - mBoundaries[2], 2);
				minSize++;
			}
		}
	}
	mSize = (maxSize + minSize) / 2;
	mError = (maxSize - minSize) / 2;
}

const std::array<int, 2> TCollimatorHole::getSize() const {
	return {mSize, mError};
}

void TCollimatorHole::drawHole(std::filesystem::path savePath) {
	int canvasWidth = static_cast<int>((mBoundaries[1] - mBoundaries[0]) * 12.5);
	int canvasHeight = static_cast<int>((mBoundaries[3] - mBoundaries[2]) * 12.5);

	gStyle->SetOptStat(0);
	TCanvas* canvas = new TCanvas("holeCanvas", "", canvasWidth, canvasHeight);
	mHole->SetTitle("Hole; Column; Row");
	mHole->Draw("COLZ");
	canvas->SaveAs(static_cast<TString>(std::string(savePath)));
}





TCollimatorRuler::TCollimatorRuler(const std::array<int, 2>& boundaries, const int threshold) {
	mBoundaries = boundaries;
	mThreshold = threshold;
}

void TCollimatorRuler::getRuler(const cv::Mat& image) {
	bool inLine = false;
	bool outLine = true;
	mRuler = new TH2I("ruler", "Ruler ; Column; Row", image.cols, 0, image.cols, mBoundaries[1] - mBoundaries[0], mBoundaries[0], mBoundaries[1]);

	for ( int y = mBoundaries[0]; y < mBoundaries[1]; y++ ) {
		for ( int x = 0; x < image.cols; x++ ) {
			cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);
			int dRed = pixel[0] < mThreshold ? 1 : 0;
			int dGreen = pixel[1] < mThreshold ? 1 : 0;
			int dBlue = pixel[2] < mThreshold ? 1 : 0;

			if ( dRed + dGreen + dBlue > 2 ) {
				mRuler->Fill(x, y);
			}
		}
	}
}

void TCollimatorRuler::setDistanceCut(std::array<int, 2> distanceCut) {
	mDistanceCut = distanceCut;
}

void TCollimatorRuler::setScaleCut(std::array<int, 2> scaleCut) {
	mScaleCut = scaleCut;
}

void TCollimatorRuler::drawRuler(std::filesystem::path savePath) {
	gStyle->SetOptStat(0);
	TCanvas* canvas = new TCanvas("rulerCanvas", "", 1000, 800);
	mRuler->SetTitle("Hole; Column; Row");
	mRuler->Draw("COLZ");
	canvas->SetGrid();
	canvas->SaveAs(static_cast<TString>(std::string(savePath)));
}

std::array<double, 2> TCollimatorRuler::getInterval() {
	double interval;
	double stdev;

	bool onScale = false;

	std::vector<int> leftInterval;
	std::vector<int> rightInterval;
	std::vector<int> centerInterval;

	TH1I* scale = new TH1I("scale", "", 30, 0, 30);

	for ( int y = mBoundaries[0]; y < mBoundaries[1]; y++ ) {
		int xInit = 0;
		int xEnd = 0;

		int preLeftX = 0;
		int preRightX = 0;
		double preCenterX = 0;

		for ( int x = 0; x < mRuler->GetNbinsX(); x++ ) {
			if ( mRuler->GetBinContent(x, y - mBoundaries[0]) == 1 && onScale == false ) {
				onScale = true;
				xInit = x;
			}
			if ( mRuler->GetBinContent(x, y - mBoundaries[0]) == 0 && onScale == true ) {
				onScale = false;
				xEnd = x;
				scale->Fill(xEnd - xInit);
				if ( xEnd - xInit >= mScaleCut[0] && xEnd - xInit <= mScaleCut[1] ) {
					if ( preLeftX != 0 ) {
						leftInterval.push_back(xInit - preLeftX);
					}
					if ( preRightX != 0 ) {
						rightInterval.push_back(xEnd - preRightX);
					}
					if ( preCenterX != 0 ) {
						centerInterval.push_back(.5 * (xEnd + xInit) - preCenterX);
					}

					preLeftX = xInit;
					preRightX = xEnd;
					preCenterX = .5 * (xEnd + xInit);
				}
			}
		}
	}

	TCanvas* scaleCanvas = new TCanvas("scaleCanvas", "", 1000, 800);
	scale->Draw();
	scaleCanvas->SaveAs("Data/scale.png");

	TH1I* left = new TH1I("left", "", 100, 0, 100);
	TH1I* right = new TH1I("right", "", 100, 0, 100);
	TH1I* center = new TH1I("center", "", 100, 0, 100);

	double leftIntervalMean = 0;
	double rightIntervalMean = 0;
	double centerIntervalMean = 0;

	double leftIntervalStdev = 0;
	double rightIntervalStdev = 0;
	double centerIntervalStdev = 0;

	int nLeftInterval = 0;
	int nRightInterval = 0;
	int nCenterInterval = 0;

	for ( int i = 0; i < leftInterval.size(); i++ ) {
		if ( leftInterval[i] > mDistanceCut[0] && leftInterval[i] < mDistanceCut[1] ) {
			leftIntervalMean += leftInterval[i];
			leftIntervalStdev += std::pow(leftInterval[i], 2);
			nLeftInterval++;
		}
		left->Fill(leftInterval[i]);
	}
	for ( int i = 0; i < rightInterval.size(); i++ ) {
		if ( rightInterval[i] > mDistanceCut[0] && rightInterval[i] < mDistanceCut[1] ) {
			rightIntervalMean += rightInterval[i];
			rightIntervalStdev += std::pow(rightInterval[i], 2);
			nRightInterval++;
		}
		right->Fill(rightInterval[i]);
	}
	for ( int i = 0; i < centerInterval.size(); i++ ) {
		if ( centerInterval[i] > mDistanceCut[0] && centerInterval[i] < mDistanceCut[1] ) {
			centerIntervalMean += centerInterval[i];
			centerIntervalStdev += std::pow(centerInterval[i], 2);
			nCenterInterval++;
		}
		center->Fill(centerInterval[i]);
	}

	leftIntervalMean /= nLeftInterval;
	rightIntervalMean /= nRightInterval;
	centerIntervalMean /= nCenterInterval;

	leftIntervalStdev /= nLeftInterval;
	rightIntervalStdev /= nRightInterval;
	centerIntervalStdev /= nCenterInterval;

	leftIntervalStdev -= std::pow(leftIntervalMean, 2);
	rightIntervalStdev -= std::pow(rightIntervalMean, 2);
	centerIntervalStdev -= std::pow(centerIntervalMean, 2);

	interval = (leftIntervalMean + rightIntervalMean + centerIntervalMean) / 3;
	stdev = std::sqrt(leftIntervalStdev + rightIntervalStdev + centerIntervalStdev) / 3;

	TCanvas* canvas = new TCanvas("intervalCanvas", "", 1000, 800);
	left->Draw();
	right->Draw("SAME");
	center->Draw("SAME");
	canvas->SaveAs("Data/interval.png");

	return {interval, std::sqrt(stdev)};
}


TCollimator::TCollimator(const std::string file) : JPEGTool(file) { }

TCollimator::TCollimator(CppConfigFile* config) : JPEGTool(config->getConfig("File").find("input")), mConfig(config) {
	mOutputDirectory = mConfig->getConfig("File").find("output_directory");
	std::filesystem::create_directories(mOutputDirectory);

	CppConfigDictionary holeConfig = mConfig->getConfig("Hole");
	std::array<int, 4> holeBoundary;
	std::vector<int> temp = TPlotter::getIntegerSetFromString(holeConfig.find("boundary"));
	std::copy_n(temp.begin(), 4, holeBoundary.begin());

	int holeThreshold = stoi(holeConfig.find("threshold"));
	mHole = new TCollimatorHole(holeBoundary, holeThreshold);

	CppConfigDictionary rulerConfig = mConfig->getConfig("Ruler");
	std::array<int, 2> rulerBoundary;
	std::vector<int> rulerTemp = TPlotter::getIntegerSetFromString(rulerConfig.find("boundary"));
	std::copy_n(rulerTemp.begin(), 2, rulerBoundary.begin());
	int rulerThreshold = stoi(rulerConfig.find("threshold"));

	mRuler = new TCollimatorRuler(rulerBoundary, rulerThreshold);

	std::array<int, 2> distanceCut;
	std::vector<int> tempCut = TPlotter::getIntegerSetFromString(rulerConfig.find("distance_cut"));
	std::copy_n(tempCut.begin(), 2, distanceCut.begin());

	mRuler->setDistanceCut(distanceCut);

	std::array<int, 2> scaleCut;
	std::vector<int> tempScale = TPlotter::getIntegerSetFromString(rulerConfig.find("scale_cut"));
	std::copy_n(tempScale.begin(), 2, scaleCut.begin());
	mRuler->setScaleCut(scaleCut);
}

TCollimator::~TCollimator() {
	if ( mIsMap ) {
		delete mRedMap;
		delete mGreenMap;
		delete mBlueMap;
	}
}

void TCollimator::getComponent() {
	int nRow = getNRow();
	int nColumn = getNColumn();

	mIsMap = true;
	mRedMap = new TH2I("redMap", "Red Component; Column; Row", nColumn, 0, nColumn, nRow, 0, nRow);
	mGreenMap = new TH2I("greenMap", "Green Component; Column; Row", nColumn, 0, nColumn, nRow, 0, nRow);
	mBlueMap = new TH2I("blueMap", "Blue Component; Column; Row", nColumn, 0, nColumn, nRow, 0, nRow);

	for ( int iRow = 0; iRow < nRow; iRow++ ) {
		for ( int iColumn = 0; iColumn < nColumn; iColumn++ ) {
			cv::Vec3b pixel = getImage().at<cv::Vec3b>(iRow, iColumn);
			mRedMap->Fill(iColumn, iRow, pixel[0]);
			mGreenMap->Fill(iColumn, iRow, pixel[1]);
			mBlueMap->Fill(iColumn, iRow, pixel[2]);
		}
	}
}

void TCollimator::saveComponent() {
	int canvasWidth = static_cast<int>(getNColumn() * 1.25);
	int canvasHeight = static_cast<int>(getNRow() * 1.25);

	gStyle->SetOptStat(0);

	TCanvas* redCanvas = new TCanvas("redCanvas", "", canvasWidth, canvasHeight);
	gStyle->SetPalette(N_COLOUR_STEP, getRGBSet()[0].data());
	mRedMap->Draw("COLZ");
	redCanvas->SaveAs(static_cast<TString>(mOutputDirectory / "red_component.png"));

	TCanvas* greenCanvas = new TCanvas("greenCanvas", "", canvasWidth, canvasHeight);
	gStyle->SetPalette(N_COLOUR_STEP, getRGBSet()[1].data());
	mGreenMap->Draw("COLZ");
	greenCanvas->SaveAs(static_cast<TString>(mOutputDirectory / "green_component.png"));

	TCanvas* blueCanvas = new TCanvas("blueCanvas", "", canvasWidth, canvasHeight);
	gStyle->SetPalette(N_COLOUR_STEP, getRGBSet()[2].data());
	mBlueMap->Draw("COLZ");
	blueCanvas->SaveAs(static_cast<TString>(mOutputDirectory / "blue_component.png"));
}

void TCollimator::getHole() {
	mHole->getHole(getImage());
}

void TCollimator::drawHole() {
	std::filesystem::path holePath = mOutputDirectory;
	holePath /= mConfig->getConfig("Hole_plot").find("name");
	mHole->drawHole(holePath);
}

void TCollimator::getRuler() {
	mRuler->getRuler(getImage());
}

void TCollimator::drawRuler() {
	std::filesystem::path rulerPath = mOutputDirectory;
	rulerPath /= mConfig->getConfig("Ruler_plot").find("name");
	mRuler->drawRuler(rulerPath);
}

void TCollimator::getArea() {
	std::array<int, 2> hole = mHole->getSize();
	std::array<double, 2> ruler = mRuler->getInterval();

	double pixelLength = 1. / ruler[0];
	double pixelLengthStdev = 1. * ruler[1] / std::pow(ruler[0], 2);

	double pixelArea = std::pow(pixelLength, 2);
	double pixelAreaStdev = pixelLength * 2 * pixelLengthStdev;

	double holeArea = hole[0] * pixelArea;
	double holeAreaUpperStdev = sqrt(pow(pixelArea * hole[1], 2) + pow(hole[0] * pixelAreaStdev, 2));
	double holeAreaLowerStdev = sqrt(pow(pixelArea * hole[2], 2) + pow(hole[0] * pixelAreaStdev, 2));

	std::array<double, 2> upper = getEffectiveNumber(holeArea, holeAreaUpperStdev);
	std::array<double, 2> lower = getEffectiveNumber(holeArea, holeAreaLowerStdev);

	std::cout << "The Area of collimator hole is " << upper[0] << " +- " << upper[1] << std::endl;
}

void TCollimator::drawRow(const int iRow) {
	std::array<std::vector<int>, 3> rowData = getRow(iRow);

	TGraph* red = new TGraph();
	TGraph* green = new TGraph();
	TGraph* blue = new TGraph();

	for ( int i = 0; i < rowData[0].size(); i++ ) {
		red->SetPoint(i, i, rowData[0][i]);
		green->SetPoint(i, i, rowData[1][i]);
		blue->SetPoint(i, i, rowData[2][i]);
	}

	TCanvas* canvas = new TCanvas("rowCanvas", "", getNRow() * 1.25, 1000);

	red->SetLineColor(kRed);
	red->Draw("AL");
	green->SetLineColor(kGreen);
	green->Draw("SAME L");
	blue->SetLineColor(kBlue);
	blue->Draw("SAME L");

	canvas->SaveAs(static_cast<TString>(mOutputDirectory / "row.png"));
}