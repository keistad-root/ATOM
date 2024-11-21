#include "TCollimator.h"

TCollimatorHole::TCollimatorHole(const std::array<int, 4>& boundaries, const int threshold) {
	mBoundaries = boundaries;
	mThreshold = threshold;

	mHole = new TH2I("hist", "Hole; Column; Row", mBoundaries[1] - mBoundaries[0], mBoundaries[0], mBoundaries[1], mBoundaries[3] - mBoundaries[2], mBoundaries[2], mBoundaries[3]);
}

void TCollimatorHole::getHole(const cv::Mat& image) {
	for ( int y = mBoundaries[2]; y < mBoundaries[3]; y++ ) {
		for ( int x = mBoundaries[0]; x < mBoundaries[1]; x++ ) {
			cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);
			int dRed = pixel[0] > mThreshold ? 1 : 0;
			int dGreen = pixel[1] > mThreshold ? 1 : 0;
			int dBlue = pixel[2] > mThreshold ? 1 : 0;

			if ( dRed + dGreen + dBlue > 0 ) {
				mHole->SetBinContent(x - mBoundaries[0], y - mBoundaries[2], 3);
			}
		}
	}
}

std::array<int, 3> TCollimatorHole::getBoundaryNumber() {
	std::vector<std::array<int, 2>> mBound;
	for ( int y = 1; y < mHole->GetNbinsX() - 1; y++ ) {
		for ( int x = 1; x < mHole->GetNbinsY() - 1; x++ ) {
			if ( mHole->GetBinContent(x, y) == 3 ) {
				if ( mHole->GetBinContent(x - 1, y) == 0 ) {
					mHole->SetBinContent(x, y, 2);
					continue;
				} else if ( mHole->GetBinContent(x + 1, y) == 0 ) {
					mHole->SetBinContent(x, y, 2);
					continue;
				} else if ( mHole->GetBinContent(x, y - 1) == 0 ) {
					mHole->SetBinContent(x, y, 2);
					continue;
				} else if ( mHole->GetBinContent(x, y + 1) == 0 ) {
					mHole->SetBinContent(x, y, 2);
					continue;
				} else if ( mHole->GetBinContent(x + 1, y - 1) == 0 ) {
					mHole->SetBinContent(x, y, 2);
					continue;
				} else if ( mHole->GetBinContent(x + 1, y + 1) == 0 ) {
					mHole->SetBinContent(x, y, 2);
					continue;
				} else if ( mHole->GetBinContent(x - 1, y - 1) == 0 ) {
					mHole->SetBinContent(x, y, 2);
					continue;
				} else if ( mHole->GetBinContent(x - 1, y + 1) == 0 ) {
					mHole->SetBinContent(x, y, 2);
					continue;
				}
			}
		}
	}
	for ( int y = 1; y < mHole->GetNbinsX() - 1; y++ ) {
		for ( int x = 1; x < mHole->GetNbinsY() - 1; x++ ) {
			if ( mHole->GetBinContent(x, y) == 0 ) {
				if ( mHole->GetBinContent(x - 1, y) == 2 ) {
					mHole->SetBinContent(x, y, 1);
					continue;
				} else if ( mHole->GetBinContent(x + 1, y) == 2 ) {
					mHole->SetBinContent(x, y, 1);
					continue;
				} else if ( mHole->GetBinContent(x, y - 1) == 2 ) {
					mHole->SetBinContent(x, y, 1);
					continue;
				} else if ( mHole->GetBinContent(x, y + 1) == 2 ) {
					mHole->SetBinContent(x, y, 1);
					continue;
				} else if ( mHole->GetBinContent(x + 1, y - 1) == 2 ) {
					mHole->SetBinContent(x, y, 1);
					continue;
				} else if ( mHole->GetBinContent(x + 1, y + 1) == 2 ) {
					mHole->SetBinContent(x, y, 1);
					continue;
				} else if ( mHole->GetBinContent(x - 1, y - 1) == 2 ) {
					mHole->SetBinContent(x, y, 1);
					continue;
				} else if ( mHole->GetBinContent(x - 1, y + 1) == 2 ) {
					mHole->SetBinContent(x, y, 1);
					continue;
				}
			}
		}
	}
	int nInnerBound = 0;
	int nOuterBound = 0;
	int nSize = 0;

	for ( int y = 0; y < mHole->GetNbinsX(); y++ ) {
		for ( int x = 0; x < mHole->GetNbinsY(); x++ ) {
			if ( mHole->GetBinContent(x, y) == 3 ) {
				nSize++;
			}
			if ( mHole->GetBinContent(x, y) == 2 ) {
				nSize++;
				nInnerBound++;
			}
			if ( mHole->GetBinContent(x, y) == 1 ) {
				nOuterBound++;
			}
		}
	}

	return {nSize, nInnerBound, nOuterBound};
}


void TCollimatorHole::drawHole(std::filesystem::path savePath) {
	int canvasWidth = static_cast<int>((mBoundaries[1] - mBoundaries[0]) * 12.5);
	int canvasHeight = static_cast<int>((mBoundaries[3] - mBoundaries[2]) * 12.5);

	// TGraph* holeGraph = new TGraph();
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
	mRuler = new TH2I("ruler", "Ruler ; Column; Row", image.cols, 0, image.cols, image.rows, 0, image.rows);

	// TGraph* graph = new TGraph();

	// for ( int x = 0; x < image.cols; x++ ) {
	// 	int red = 0;
	// 	int green = 0;
	// 	int blue = 0;
	// 	for ( int y = mBoundaries[0]; y < mBoundaries[1]; y++ ) {
	// 		cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);
	// 		red += pixel[0];
	// 		green += pixel[1];
	// 		blue += pixel[2];
	// 	}
	// 	graph->SetPoint(x, x, red);
	// }
	// TCanvas* canvas = new TCanvas("sumCanvas", "", 1500, 1000);
	// graph->Draw("AL");
	// canvas->SaveAs("Data/temp.png");

	for ( int y = mBoundaries[0]; y < mBoundaries[1]; y++ ) {
		for ( int x = 0; x < image.cols; x++ ) {
			cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);
			int dRed = pixel[0] < mThreshold ? 1 : 0;
			int dGreen = pixel[1] < mThreshold ? 1 : 0;
			int dBlue = pixel[2] < mThreshold ? 1 : 0;

			if ( (dRed + dGreen + dBlue > 1) && outLine ) {
				mRuler->Fill(x, y);
				inLine = true;
				outLine = false;
			}
			if ( (dRed + dGreen + dBlue < 1) && inLine ) {
				inLine = false;
				outLine = true;
			}
		}
	}
}

void TCollimatorRuler::setDistanceCut(std::array<int, 2> distanceCut) {
	mDistanceCut = distanceCut;
}

void TCollimatorRuler::drawRuler(std::filesystem::path savePath) {
	int canvasWidth = static_cast<int>(mRuler->GetNbinsX() * 12.5);
	int canvasHeight = static_cast<int>(mRuler->GetNbinsY() * 12.5);

	// TGraph* holeGraph = new TGraph();
	gStyle->SetOptStat(0);
	TCanvas* canvas = new TCanvas("rulerCanvas", "", canvasWidth, canvasHeight);
	mRuler->SetTitle("Hole; Column; Row");
	mRuler->Draw("COLZ");
	canvas->SetGrid();
	canvas->SaveAs(static_cast<TString>(std::string(savePath)));
}

std::array<double, 2> TCollimatorRuler::getInterval() {
	double interval;
	double stdev;
	int nInterval = 0;
	int nAnormal = 0;
	for ( int y = mBoundaries[0]; y < mBoundaries[1]; y++ ) {
		int rulerX = 0;
		for ( int x = 0; x < mRuler->GetNbinsX(); x++ ) {
			if ( mRuler->GetBinContent(x, y) == 1 ) {
				int distance = x - rulerX;
				if ( rulerX != 0 && distance < mDistanceCut[1] && distance > mDistanceCut[0] ) {
					stdev += std::pow(distance, 2);
					interval += distance;
					nInterval++;
				} else if ( rulerX != 0 ) {
					nAnormal++;
				}
				rulerX = x;
			}
		}
	}
	interval /= nInterval;
	stdev /= nInterval;
	stdev -= pow(interval, 2);

	std::cout << static_cast<double>(nAnormal) / (nInterval + nAnormal) << std::endl;

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
	std::array<int, 3> hole = mHole->getBoundaryNumber();
	std::array<double, 2> ruler = mRuler->getInterval();

	double pixelLength = 1. / ruler[0];
	double pixelLengthStdev = ruler[1] / std::pow(ruler[0], 2);

	double pixelArea = std::pow(pixelLength, 2);
	double pixelAreaStdev = pixelLength * 2 * pixelLengthStdev;

	double holeArea = hole[0] * pixelArea;
	double holeAreaUpperStdev = sqrt(pow(pixelArea * hole[1], 2) + pow(hole[0] * pixelAreaStdev, 2));
	double holeAreaLowerStdev = sqrt(pow(pixelArea * hole[2], 2) + pow(hole[0] * pixelAreaStdev, 2));

	std::array<double, 2> upper = getEffectiveNumber(holeArea, holeAreaUpperStdev);
	std::array<double, 2> lower = getEffectiveNumber(holeArea, holeAreaLowerStdev);

	std::cout << "The pixel area is " << getEffectiveNumber(pixelArea, pixelAreaStdev)[0] << " +- " << getEffectiveNumber(pixelArea, pixelAreaStdev)[1] << std::endl;
	std::cout << "The pixel of collimator hole is " << getEffectiveNumber(hole[0], hole[1])[0] << " +- " << getEffectiveNumber(hole[0], hole[1])[1] << std::endl;
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