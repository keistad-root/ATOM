#include "TSimulCollimator.h"

TSimulCollimator::TSimulCollimator(double uX, double uY, double uZ, double uRadius, double lX, double lY, double lZ, double lRadius) {
	mUpperCenter = {static_cast<int>(uX / grid), static_cast<int>(uY / grid), static_cast<int>(uZ / grid)};
	mLowerCenter = {static_cast<int>(lX / grid), static_cast<int>(lY / grid), static_cast<int>(lZ / grid)};

	mUpperRadius = static_cast<int>(uRadius / grid);
	mLowerRadius = static_cast<int>(lRadius / grid);
}

bool TSimulCollimator::isInclude(TTrack* track) {
	std::array<int, 2> uXY = track->getXYposition(mUpperCenter[2]);
	std::array<int, 2> lXY = track->getXYposition(mLowerCenter[2]);

	bool upper = pow(uXY[0] - mUpperCenter[0], 2) + pow(uXY[1] - mUpperCenter[1], 2) < pow(mUpperRadius, 2);
	bool lower = pow(lXY[0] - mLowerCenter[0], 2) + pow(lXY[1] - mLowerCenter[1], 2) < pow(mLowerRadius, 2);

	if ( upper && lower ) {
		// if ( upper ) {
		return true;
	} else {
		return false;
	}
	return false;
}