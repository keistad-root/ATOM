#include "TSimulDetector.h"

TSimulDetector::TSimulDetector() : mCenter({0, 0, 0}), mDimension({30, 15}) { }

TSimulDetector::TSimulDetector(double x, double y, double z, double width, double height) {
	mCenter = {static_cast<int>(x / grid), static_cast<int>(y / grid), static_cast<int>(z / grid)};
	mDimension = {static_cast<int>(width / grid), static_cast<int>(height / grid)};
}

bool TSimulDetector::isInclude(TTrack* track) {
	std::array<int, 2> XY = track->getXYposition(mCenter[2]);
	bool underX = (XY[0] >= (mCenter[0] - (mDimension[0] / 2.)));
	bool overX = (XY[0] <= (mCenter[0] + (mDimension[0] / 2.)));
	bool underY = (XY[1] >= (mCenter[1] - (mDimension[1] / 2.)));
	bool overY = (XY[1] <= (mCenter[1] + (mDimension[1] / 2.)));

	// std::cout << underX << " " << underY << " " << overX << " " << overY << std::endl;
	// if ( underX ) { return true; } else { return false; }
	if ( underX && underY && overX && overY ) { return true; } else { return false; }
	return false;
}

void TSimulDetector::saveDetector() {
	TGraph* graph = new TGraph();

	int i = 0;
	for ( int x = 0; x < 1024; x++ ) {
		for ( int y = 0; y < 512; y++ ) {
			double rX = (mCenter[0] - (mDimension[0] / 2.) + mDimension[0] * (x / 1024.)) * grid;
			double rY = (mCenter[1] - (mDimension[1] / 2.) + mDimension[1] * (y / 512.)) * grid;
			graph->SetPoint(i, rX, rY);
			i++;
		}
	}

	TCanvas* canvas = new TCanvas("dCan", "", 3000, 1500);
	graph->SetTitle("Coordinates in detector; x[mm]; y[mm]");
	graph->Draw("AP");

	canvas->SaveAs("Data/Detector.png");
}