#include "TClusterN.h"

TClusterN::TClusterN(const std::vector<TCluster*> clusters, const int clusterSize) : mClusterSize(clusterSize) {
	for ( TCluster* cluster : clusters ) {
		if ( cluster->getSize() == clusterSize ) {
			mNCluster.push_back(cluster);
		}
	}
}

void TClusterN::calShapeSpecies() {
	bool isFirst = true;
	for ( TCluster* cluster : mNCluster ) {
		if ( isFirst ) {
			mClustersWithShape.push_back({cluster, 1});
			isFirst = false;
			continue;
		}
		TMatrix2D<int> checkingCluster = cluster->getShape();
		bool isHomoemorphismExist = false;
		for ( auto iter = mClustersWithShape.begin(); iter != mClustersWithShape.end(); iter++ ) {
			TMatrix2D<int> comparedCluster = iter->first->getShape();
			if ( comparedCluster.hasHomeomorphism(checkingCluster) ) {
				iter->second++;
				isHomoemorphismExist = true;
				break;
			}
		}
		if ( !isHomoemorphismExist ) {
			mClustersWithShape.push_back({cluster, 1});
		}
	}
	std::sort(mClustersWithShape.begin(), mClustersWithShape.end(),
			  [ ](std::pair<TCluster*, int>& a, std::pair<TCluster*, int>& b) {
				  return a.second > b.second;
			  });
}


void TClusterN::setClusterSize(const int clusterSize) {
	mClusterSize = clusterSize;
}

const std::vector<std::pair<TCluster*, int>>& TClusterN::getClustersWithShape() const {
	return mClustersWithShape;
}

const int TClusterN::getClusterSize() const {
	return mClusterSize;
}