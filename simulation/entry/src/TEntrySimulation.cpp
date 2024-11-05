#include "TEntrySimulation.h"

void TEntrySimulation::setInitGeometry(double diskRadius, double upperDiskCoordZ, double lowerDiskCoordZ, double detectorWidth, double detectorHeight, double detectorCoordZ) {
	upperDisk.radius = diskRadius;
	upperDisk.coordZ = upperDiskCoordZ;
	lowerDisk.radius = diskRadius;
	lowerDisk.coordZ = lowerDiskCoordZ;
	detector.width = detectorWidth;
	detector.height = detectorHeight;
	detector.coordZ = detectorCoordZ;
}

void TEntrySimulation::setSource(double sourceRadius) {
	source.radius = sourceRadius;
	source.coordZ = 0.;
}

void TEntrySimulation::setCollimator(double diskRadius, double upperDiskCoordZ, double lowerDiskCoordZ) {
	upperDisk.radius = diskRadius;
	upperDisk.coordZ = upperDiskCoordZ;
	lowerDisk.radius = diskRadius;
	lowerDisk.coordZ = lowerDiskCoordZ;
}

void TEntrySimulation::setDetector(double detectorWidth, double detectorHeight, double detectorCoordX, double detectorCoordY, double detectorCoordZ) {
	detector.width = detectorWidth;
	detector.height = detectorHeight;
	detector.coordX = detectorCoordX;
	detector.coordY = detectorCoordY;
	detector.coordZ = detectorCoordZ;
}

double TEntrySimulation::doCount() {
	double nCount = 0;
	double totCount = 0;
	double effAngle = 0.;
	double sourceStep = source.radius / 25.;
	int iPoint = 0;
	for ( double x = -source.radius; x < source.radius + 2 * sourceStep; x += sourceStep ) {
		for ( double y = -source.radius; y < source.radius + 2 * sourceStep; y += sourceStep ) {
			if ( source.isBelong(x, y) ) {
				for ( double phi = 0.; phi < 2 * TMath::Pi() - (TMath::Pi() / 360.); phi += (TMath::Pi() / 180.) ) {
					for ( double theta = (TMath::Pi() / 2.); theta < TMath::Pi() - (TMath::Pi() / 360.); theta += (TMath::Pi() / 180.) ) {
						totCount += TMath::Sin(theta);
						if ( upperDisk.isBelong(x - upperDisk.coordZ * cos(phi) * tan(theta), y - upperDisk.coordZ * sin(phi) * tan(theta)) ) {
							if ( lowerDisk.isBelong(x - lowerDisk.coordZ * cos(phi) * tan(theta), y - lowerDisk.coordZ * sin(phi) * tan(theta)) ) {
								if ( detector.isBelong(x - detector.coordZ * cos(phi) * tan(theta), y - detector.coordZ * sin(phi) * tan(theta)) ) {
									nCount += TMath::Sin(theta);
								}
							}
						}
					}
				}
				effAngle += static_cast<double>(nCount) / totCount;
				nCount = 0;
				totCount = 0;
				iPoint++;
			}
		}
	}
	// effAngle = static_cast<double>(nCount) / totCount;
	effAngle /= iPoint;
	return effAngle;
}