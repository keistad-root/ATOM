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

void TEntrySimulation::setDetector(double detectorWidth, double detectorHeight, double detectorCoordZ) {
	detector.width = detectorWidth;
	detector.height = detectorHeight;
	detector.coordZ = detectorCoordZ;
}

double TEntrySimulation::doCount() {
	int nCount = 0;
	int totCount = 0;
	double effAngle = 0.;

	double sourceStep = source.radius / 25.;
	int iPoint = 0;
	for ( double x = -source.radius; x < source.radius + sourceStep; x += sourceStep ) {
		for ( double y = -source.radius; y < source.radius + sourceStep; y += sourceStep ) {
			if ( source.isBelong(x, y) ) {
				for ( double phi = 0.; phi < 2 * TMath::Pi(); phi += (TMath::Pi() / 180.) ) {
					for ( double theta = (TMath::Pi() / 2.); theta < TMath::Pi() * (179. / 180); theta += (TMath::Pi() / 180.) ) {
						std::cout << theta << " ";
						totCount++;
						if ( upperDisk.isBelong(x + upperDisk.coordZ * cos(phi) / tan(theta), y + upperDisk.coordZ * sin(phi) / tan(theta)) ) {
							if ( lowerDisk.isBelong(x + lowerDisk.coordZ * cos(phi) / tan(theta), y + lowerDisk.coordZ * sin(phi) / tan(theta)) ) {
								if ( detector.isBelong(x + detector.coordZ * cos(phi) / tan(theta), y + detector.coordZ * sin(phi) / tan(theta)) ) {
									nCount++;
								}
							}
						}
					}
					break;
				}
				std::cout << std::endl;
				exit(0);
				effAngle += static_cast<double>(nCount) / totCount;
				nCount = 0;
				totCount = 0;
				iPoint++;
			}
		}
	}
	std::cout << std::endl;
	// effAngle = static_cast<double>(nCount) / totCount;
	effAngle /= iPoint;
	return effAngle;
}