#include "TTrack.h"

TTrack::TTrack(int x, int y, int z, double theta, double phi) : mPoint({x, y, z}), mAngle({theta, phi}) {
	mWeight = sin(theta);
}

std::array<int, 2> TTrack::getXYposition(const int z) {
	int x = mPoint[0] - z * cos(mAngle[1]) * tan(mAngle[0]);
	int y = mPoint[1] - z * sin(mAngle[1]) * tan(mAngle[0]);
	return {x, y};
}

double TTrack::getWeight() {
	return mWeight;
}