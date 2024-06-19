#ifndef __TENTRYSIMULATION__
#define __TENTRYSIMULATION__

#include <cmath>
#include <iostream>

#include "TMath.h"
#include "TGraph2D.h"
#include "TCanvas.h"

struct TDisk {
	double radius;
	double coordZ;
	bool isBelong(double x, double y) {
		int intX = round(x * 10);
		int intY = round(y * 10);
		if ( pow(intX, 2) + pow(intY, 2) < pow(int(radius * 10), 2) ) {
			return true;
		} else {
			return false;
		}
	}
};

struct TDetector {
	double width, height;
	double coordZ;
	bool isBelong(double x, double y) {
		if ( (abs(x) < width / 2) && (abs(y) < height / 2) ) {
			return true;
		} else {
			return false;
		}
	}
};

class TEntrySimulation {
private:
	TDisk source;
	TDisk upperDisk;
	TDisk lowerDisk;
	TDetector detector;
public:
	void setInitGeometry(double diskRadius, double upperDiskCoordZ, double lowerDiskCoordZ, double detectorWidth, double detectorHeight, double detectorCoordZ);
	void setSource(double sourceRadius);
	void setCollimator(double diskRadius, double upperDiskCoordZ, double lowerDiskCoordZ);
	void setDetector(double detectorWidth, double detectorHeight, double detectorCoordZ);

	double doCount();
};



#endif