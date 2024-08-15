/**
 * This simulation is counting entry ratio by
 */

#ifndef __TENTRYSIMULATION__
#define __TENTRYSIMULATION__

#include <cmath>
#include <iostream>

#include "TMath.h"
#include "TGraph2D.h"
#include "TCanvas.h"
#include "TH2D.h"

struct TSource {
	std::array<double, 3> sCenter = {0, 0, 0};
	double radius;
	std::vector<std::array<double, 2>> sInitPoints;
};

struct TDetector {
	std::array<double, 3> sCenter = {0, 0, 0};
	std::array<double, 2> sDimension = {30, 15};
	bool isBelong(double x, double y) {
		if ( (abs(x - sCenter[0]) < (sDimension[0] / 2)) && (abs(y - sCenter[1]) < (sDimension[1] / 2)) ) {
			return true;
		} else {
			return false;
		}
	}
};

struct TCollimator {
	std::array<double, 3> sUpperCenter = {0, 0, 0};
	std::array<double, 3> sLowerCenter = {0, 0, 0};
	double sUpperRadius = 3.5;
	double sLowerRadius = 3.5;
	bool isBelong(double upperX, double upperY, double lowerX, double lowerY) {
		if ( (pow(upperX, 2) + pow(upperY, 2) < pow(sUpperRadius, 2)) && (pow(lowerX, 2) + pow(lowerY, 2) < pow(sLowerRadius, 2)) ) {
			return true;
		} else {
			return false;
		}
	}
};

class TAdvancedEntrySimulation {
private:
	double mInterval = 1.; /** =1mm */
	std::vector<std::array<double, 7>> mHitPoint; /** Hit points. Three points in a tarck.*/

	TSource mSource;
	TCollimator mCollimator;
	TDetector detector;
public:
	TAdvancedEntrySimulation(double interval);
	void setInterval(double interval);
	void setSource(double diskRadius);
	void setCollimator(const std::array<double, 3>& upperCenter, const double upperRadius, const std::array<double, 3>& lowerCenter, const double lowerRadius);
	void setDetector(const std::array<double, 3>& center, const double width = 30., const double height = 15.);
	void extractTrack(int angleDivide);
	double calculateEntry();
};



#endif