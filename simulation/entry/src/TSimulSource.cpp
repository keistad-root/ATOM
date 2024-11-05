#include "TSimulSource.h"

TSimulSource::TSimulSource() : mCenter({0, 0, 0}), mRadius(0) { }

TSimulSource::TSimulSource(double x, double y, double z, double radius, int step) : mStep(step) {
	mCenter = {static_cast<int>(x / grid), static_cast<int>(y / grid), static_cast<int>(z / grid)};
	mRadius = static_cast<int>(radius / grid);
}

void TSimulSource::extractCoordinate() {
	for ( int y = -mRadius; y < mRadius + 1; y += mStep ) {
		for ( int x = -mRadius; x < mRadius + 1; x += mStep ) {
			if ( pow(x - mCenter[0], 2) + pow(y - mCenter[1], 2) <= pow(mRadius, 2) ) {
				mCoordinate.push_back({x, y});
			}
		}
	}
}

const std::vector<std::pair<int, int>>& TSimulSource::getSourceCoordinate() const {
	return mCoordinate;
}

void TSimulSource::saveCoordinate() {
	TGraph* graph = new TGraph();

	for ( int i = 0; i < mCoordinate.size(); i++ ) {
		graph->SetPoint(i, mCoordinate[i].first * grid, mCoordinate[i].second * grid);
	}

	TCanvas* canvas = new TCanvas("sCan", "", 1500, 1500);
	graph->SetTitle("Coordinates in source; x[mm]; y[mm]");
	graph->Draw("AP");

	canvas->SaveAs("Data/Source.png");
}