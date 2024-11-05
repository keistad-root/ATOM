#include "TCollimator.h"

TCollimator::TCollimator(const std::string file) : JPEGTool(file) { }

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

void TCollimator::saveComponent(const std::string savePath) {
	int canvasWidth = static_cast<int>(getNColumn() * 1.25);
	int canvasHeight = static_cast<int>(getNRow() * 1.25);

	gStyle->SetOptStat(0);

	TCanvas* redCanvas = new TCanvas("redCanvas", "", canvasWidth, canvasHeight);
	gStyle->SetPalette(N_COLOUR_STEP, getRGBSet()[0].data());
	mRedMap->Draw("COLZ");
	redCanvas->SaveAs(static_cast<TString>(savePath + "red_component.png"));

	TCanvas* greenCanvas = new TCanvas("greenCanvas", "", canvasWidth, canvasHeight);
	gStyle->SetPalette(N_COLOUR_STEP, getRGBSet()[1].data());
	mGreenMap->Draw("COLZ");
	greenCanvas->SaveAs(static_cast<TString>(savePath + "green_component.png"));

	TCanvas* blueCanvas = new TCanvas("blueCanvas", "", canvasWidth, canvasHeight);
	gStyle->SetPalette(N_COLOUR_STEP, getRGBSet()[2].data());
	mBlueMap->Draw("COLZ");
	blueCanvas->SaveAs(static_cast<TString>(savePath + "blue_component.png"));
}

std::array<int, 2> TCollimator::getBoxEndRow(const int row) {
	std::array<std::vector<int>, 3> array = getRow(row);

	int lMax = 0;
	int min = 0;
	int rMax = 0;

	for ( int i = 0; i < getNColumn(); i++ ) {
		int value = array[0][i] - array[2][i];
		if ( i < array[0].size() / 2 ) {
			lMax = std::max(lMax, value);
		} else {
			rMax = std::max(rMax, value);
		}
		min = std::min(min, value);
	}

	int initPoint = 0;
	for ( int i = 0; i < array[0].size() / 2; i++ ) {
		if ( array[0][i] - array[2][i] > ((lMax - min) * mBoxRatioLeft) ) {
			initPoint = i;
		}
	}

	int endPoint = 0;
	for ( int i = array[0].size() / 2; i < array[0].size(); i++ ) {
		if ( array[0][i] - array[2][i] > ((rMax - min) * mBoxRatioRight) ) {
			endPoint = i;
			break;
		}
	}

	return {initPoint, endPoint};
}


std::array<int, 2> TCollimator::getBoxEndColumn(const int column) {
	std::array<std::vector<int>, 3> array = getColumn(column);

	int lMax = 0;
	int min = 0;
	int rMax = 0;

	for ( int i = 0; i < getNRow(); i++ ) {
		int value = array[0][i] - array[2][i];
		if ( i < array[0].size() / 2 ) {
			lMax = std::max(lMax, value);
		} else {
			rMax = std::max(rMax, value);
		}
		min = std::min(min, value);
	}

	int initPoint = 0;
	for ( int i = 0; i < array[0].size() / 2; i++ ) {
		if ( array[0][i] - array[2][i] > ((lMax - min) * mBoxRatioBottom) ) {
			initPoint = i;
		}
	}

	int endPoint = 0;
	for ( int i = array[0].size() / 2; i < array[0].size(); i++ ) {
		if ( array[0][i] - array[2][i] > ((rMax - min) * mBoxRatioTop) ) {
			endPoint = i;
			break;
		}
	}

	return {initPoint, endPoint};
}

void TCollimator::getBox() {
	for ( int i = mBoxBottom; i < mBoxTop; i++ ) {
		mBox.push_back({getBoxEndRow(i)[0], i});
		mBox.push_back({getBoxEndRow(i)[1], i});
	}
	for ( int i = mBoxLeft; i < mBoxRight; i++ ) {
		mBox.push_back({i, getBoxEndColumn(i)[0]});
		mBox.push_back({i, getBoxEndColumn(i)[1]});
	}
}

void TCollimator::drawBox() {
	int canvasWidth = static_cast<int>(getNColumn() * 1.25);
	int canvasHeight = static_cast<int>(getNRow() * 1.25);

	TGraph* boxGraph = new TGraph();
	int iPixel = 0;
	for ( const std::array<int, 2>&pixel : mBox ) {
		boxGraph->SetPoint(iPixel, pixel[0], pixel[1]);
		iPixel++;
	}
	TCanvas* canvas = new TCanvas("boxCanvas", "", canvasWidth, canvasHeight);
	boxGraph->GetXaxis()->SetLimits(0, getNColumn());
	boxGraph->SetMinimum(0);
	boxGraph->SetMaximum(getNRow());
	boxGraph->SetMarkerStyle(21);
	boxGraph->SetMarkerSize(2);
	boxGraph->SetTitle("Box; Column; Row");
	boxGraph->Draw("AP");
	canvas->SaveAs("Data/box.png");
}

std::array<int, 2> TCollimator::getHoleEndRow(const int row) {
	std::array<std::vector<int>, 3> array = getRow(row);
	std::vector<int> highValues;

	for ( int i = mHoleLeft; i < mHoleRight; i++ ) {
		if ( (array[0][i] > mHoleThreshold) && (array[1][i] > mHoleThreshold) && (array[2][i] > mHoleThreshold) ) {
			highValues.push_back(i);
		}
	}
	if ( highValues.size() > 2 ) {
		return {highValues[0], highValues.back()};
	} else {
		return {0, 0};
	}
}

std::array<int, 2> TCollimator::getHoleEndColumn(const int column) {
	std::array<std::vector<int>, 3> array = getColumn(column);
	std::vector<int> highValues;

	for ( int i = mHoleBottom; i < mHoleTop; i++ ) {
		if ( (array[0][i] > mHoleThreshold) && (array[1][i] > mHoleThreshold) && (array[2][i] > mHoleThreshold) ) {
			highValues.push_back(i);
		}
	}
	if ( highValues.size() > 2 ) {
		return {highValues[0], highValues.back()};
	} else {
		return {0, 0};
	}
}

void TCollimator::getHole() {
	for ( int y = mHoleBottom; y < mHoleTop; y++ ) {
		for ( int x = mHoleLeft; x < mHoleRight; x++ ) {
			cv::Vec3b pixel = getImage().at<cv::Vec3b>(y, x);
			if ( (pixel[0] > mHoleThreshold) && (pixel[1] > mHoleThreshold) && (pixel[2] > mHoleThreshold) ) {
				mHole.push_back({x, y});
			}
		}
	}


	// for ( int i = mHoleBottom; i < mHoleTop; i++ ) {
	// 	std::array<int, 2> ends = getHoleEndRow(i);
	// 	if ( ends[0] != 0 && ends[1] != 0 ) {
	// 		for ( int j = ends[0]; j < ends[1] + 1; j++ ) {
	// 			mHole.push_back({j, i});
	// 		}
	// 	}
	// 	// if ( ends[0] != 0 ) {
	// 	// 	mHole.push_back({ends[0], i});
	// 	// }
	// 	// if ( ends[1] != 0 ) {
	// 	// 	mHole.push_back({ends[1], i});
	// 	// }
	// }

	// for ( int i = mHoleLeft; i < mHoleRight; i++ ) {
	// 	std::array<int, 2> ends = getHoleEndColumn(i);
	// 	if ( ends[0] != 0 && ends[1] != 0 ) {
	// 		for ( int j = ends[0]; j < ends[1] + 1; j++ ) {
	// 			mHole.push_back({i, j});
	// 		}
	// 	}
	// 	// if ( ends[0] != 0 ) {
	// 	// 	mHole.push_back({i, ends[0]});
	// 	// }
	// 	// if ( ends[1] != 0 ) {
	// 	// 	mHole.push_back({i, ends[1]});
	// 	// }
	// }
	sortHole();
}

void TCollimator::drawHole() {
	int canvasWidth = static_cast<int>((mHoleRight - mHoleLeft) * 12.5);
	int canvasHeight = static_cast<int>((mHoleTop - mHoleBottom) * 12.5);

	// TGraph* holeGraph = new TGraph();
	gStyle->SetOptStat(0);
	TH2I* holeHist = new TH2I("hist", "Hole; Column; Row", mHoleRight - mHoleLeft, mHoleLeft, mHoleRight, mHoleTop - mHoleBottom, mHoleBottom, mHoleTop);
	int iPixel = 0;
	for ( const std::array<int, 2>&pixel : mHole ) {
		// holeGraph->SetPoint(iPixel, pixel[0], pixel[1]);
		// iPixel++;
		holeHist->Fill(pixel[0], pixel[1]);
	}
	TCanvas* canvas = new TCanvas("holeCanvas", "", canvasWidth, canvasHeight);
	// holeGraph->GetXaxis()->SetLimits(0, getNColumn());
	// holeGraph->SetMinimum(0);
	// holeGraph->SetMaximum(getNRow());
	holeHist->SetMarkerStyle(21);
	holeHist->SetMarkerSize(1);
	holeHist->SetTitle("Hole; Column; Row");
	holeHist->Draw("AP");
	canvas->SaveAs("Data/hole.png");
}

void TCollimator::drawTotal() {
	int canvasWidth = static_cast<int>(getNColumn() * 1.25);
	int canvasHeight = static_cast<int>(getNRow() * 1.25);
	TGraph* boxGraph = new TGraph();
	int iPixel = 0;
	for ( const std::array<int, 2>&pixel : mBox ) {
		boxGraph->SetPoint(iPixel, pixel[0], pixel[1]);
		iPixel++;
	}

	TGraph* holeGraph = new TGraph();
	iPixel = 0;
	for ( const std::array<int, 2>&pixel : mHole ) {
		holeGraph->SetPoint(iPixel, pixel[0], pixel[1]);
		iPixel++;
	}
	TCanvas* canvas = new TCanvas("boxCanvas", "", canvasWidth, canvasHeight);
	boxGraph->GetXaxis()->SetLimits(0, getNColumn());
	boxGraph->SetMinimum(0);
	boxGraph->SetMaximum(getNRow());
	boxGraph->SetMarkerStyle(21);
	boxGraph->SetMarkerSize(2);
	boxGraph->SetTitle("Total; Column; Row");
	boxGraph->Draw("AP");
	holeGraph->SetMarkerStyle(21);
	holeGraph->SetMarkerSize(1);
	holeGraph->Draw("SAME P");
	canvas->SaveAs("Data/total.png");
}

void TCollimator::sortHole() {
	std::sort(mHole.begin(), mHole.end(), [ ](std::array<int, 2> a, std::array<int, 2> b) {return a[0] < b[0]; });

}