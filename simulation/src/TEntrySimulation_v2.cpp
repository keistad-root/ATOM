#include "TEntrySimulation_v2.h"

TAdvancedEntrySimulation::TAdvancedEntrySimulation(double interval) {
	std::clog << "The lattice interval is " << interval << "mm" << std::endl;
	setInterval(interval);
}

void TAdvancedEntrySimulation::setInterval(double interval) {
	mInterval = interval;
}

void TAdvancedEntrySimulation::setSource(double diskRadius) {
	mSource.radius = diskRadius;
	for ( double yStep = -mSource.radius; yStep < mSource.radius + mInterval; yStep += mInterval ) {
		for ( double xStep = -mSource.radius; xStep < mSource.radius + mInterval; xStep += mInterval ) {
			if ( pow(xStep, 2) + pow(yStep, 2) < pow(mSource.radius, 2) ) {
				mSource.sInitPoints.push_back({xStep, yStep});
			}
		}
	}
}

void TAdvancedEntrySimulation::setCollimator(const std::array<double, 3>& upperCenter, const double upperRadius, const std::array<double, 3>& lowerCenter, const double lowerRadius) {
	mCollimator.sUpperCenter[0] = upperCenter[0];
	mCollimator.sUpperCenter[1] = upperCenter[1];
	mCollimator.sUpperCenter[2] = upperCenter[2];

	mCollimator.sUpperRadius = upperRadius;

	mCollimator.sLowerCenter[0] = lowerCenter[0];
	mCollimator.sLowerCenter[1] = lowerCenter[1];
	mCollimator.sLowerCenter[2] = lowerCenter[2];

	mCollimator.sLowerRadius = lowerRadius;
}


void TAdvancedEntrySimulation::setDetector(const std::array<double, 3>& center, const double width, const double height) {
	detector.sCenter[0] = center[0];
	detector.sCenter[1] = center[1];
	detector.sCenter[2] = center[2];

	detector.sDimension[0] = width;
	detector.sDimension[1] = height;
}

void TAdvancedEntrySimulation::extractTrack(int angleDivide) {
	mHitPoint.clear();
	double phi = 0.;
	double theta = 0.;
	for ( const std::array<double, 2>&initPoint : mSource.sInitPoints ) {
		for ( int iTheta = 0; iTheta < angleDivide + 1; iTheta++ ) {
			theta = iTheta * (TMath::Pi() / angleDivide);
			for ( int iPhi = 0; iPhi < angleDivide; iPhi++ ) {
				phi = iPhi * (2 * TMath::Pi() / angleDivide);
				double detectorX = initPoint[0] - detector.sCenter[2] * cos(phi) * tan(theta);
				double detectorY = initPoint[1] - detector.sCenter[2] * sin(phi) * tan(theta);
				if ( detector.isBelong(detectorX, detectorY) && tan(theta) < 0 ) {
					double collimatorUpperX = initPoint[0] - mCollimator.sUpperCenter[2] * cos(phi) * tan(theta);
					double collimatorUpperY = initPoint[1] - mCollimator.sUpperCenter[2] * sin(phi) * tan(theta);
					double collimatorLowerX = initPoint[0] - mCollimator.sLowerCenter[2] * cos(phi) * tan(theta);
					double collimatorLowerY = initPoint[1] - mCollimator.sLowerCenter[2] * sin(phi) * tan(theta);

					mHitPoint.push_back({collimatorUpperX, collimatorUpperY, collimatorLowerX, collimatorLowerY, detectorX, detectorY, sin(theta) * (TMath::Pi() / angleDivide) * (2 * TMath::Pi() / angleDivide)});
				}
			}
			phi = 0.;
		}
	}
}

double TAdvancedEntrySimulation::calculateEntry() {
	double entry = 0.;
	// TCanvas* mapCan = new TCanvas("cMap", "", 1500, 1000);
	// TH2D* detectorMap = new TH2D("dMap", "Detector Map; Rows [mm]; Columns [mm]", 256, -15., 15., 128, -7.5, 7.5);
	for ( const std::array<double, 7>&hitPoint : mHitPoint ) {
		if ( mCollimator.isBelong(hitPoint[0], hitPoint[1], hitPoint[2], hitPoint[3]) ) {
			entry += hitPoint[6];
			// detectorMap->Fill(hitPoint[4] - detector.sCenter[0], hitPoint[5] - detector.sCenter[1], hitPoint[6]);
		}
	}
	// detectorMap->Draw("COLZ");
	// mapCan->SaveAs("/mnt/homes/ychoi/PIDproject/plots/final/entry_test/comparison/map.png");
	return entry / (4 * TMath::Pi()) / mSource.sInitPoints.size();
}