#define __TCLUSTERDIVIDEDATA_HEADER__
#include "TClusterDivideData.h"

TClusterDivideData::TClusterDivideData(const std::vector<TCluster*>& clusters) {
	for ( TCluster* cluster : clusters ) {
		int clusterSize = cluster->getSize();
		if ( mClusterData.count(clusterSize) ) {
			mClusterData.find(clusterSize)->second.push_back(cluster);
		} else {
			std::vector<TCluster*> set;
			set.push_back(cluster);
			mClusterData.insert_or_assign(clusterSize, set);
		}
	}
}

TClusterDivideData::TClusterDivideData(const TClusterDivideData& copy) : mClusterData(copy.mClusterData) { }


const std::vector<TCluster*>& TClusterDivideData::getClusterOfSize(const int clusterSize) {
	try {
		if ( !mClusterData.count(clusterSize) ) throw clusterSize;
		return mClusterData.find(clusterSize)->second;
	} catch ( int clusterSize ) {
		std::cerr << "There are no clusters having cluster Size " + std::to_string(clusterSize) << std::endl;
		std::vector<TCluster*>* empty;
		return *empty;
	}
}