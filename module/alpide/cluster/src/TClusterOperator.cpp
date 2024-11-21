#include "TCluster.h"

bool TCluster::operator==(const TCluster& cluster) const {
	int aX = mMaxX - mMinX;
	int aY = mMaxY - mMinY;
	int bX = cluster.mMaxX - cluster.mMinX;
	int bY = cluster.mMaxY - cluster.mMinY;
	if ( mPixels.size() != cluster.mPixels.size() ) {
		return false;
	}
	if ( !(((aX == bX) && (aY == bY)) || ((aX == bY) && (aY == bX))) ) {
		return false;
	}
	return true;
}

bool TCluster::operator!=(const TCluster& cluster) const {
	int aX = mMaxX - mMinX;
	int aY = mMaxY - mMinY;
	int bX = cluster.mMaxX - cluster.mMinX;
	int bY = cluster.mMaxY - cluster.mMinY;

	if ( mPixels.size() != cluster.mPixels.size() ) {
		return true;
	}

	if ( !(((aX == bX) && (aY == bY)) || ((aX == bY) && (aY == bX))) ) {
		return true;
	}
	return true;
}