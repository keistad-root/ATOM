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
		if ( pow(x, 2) + pow(y, 2) < pow(radius, 2) ) {
			return true;
		} else {
			return false;
		}
	}
};

struct TDetector {
	double width, height;
	double coordX, coordY, coordZ;
	bool isBelong(double x, double y) {
		if ( (abs(x - coordX) < width / 2) && (abs(y - coordY) < height / 2) ) {
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
	void setDetector(double detectorWidth, double detectorHeight, double detectorCoordX, double detectorCoordY, double detectorCoordZ);

	double doCount();
};



#endif