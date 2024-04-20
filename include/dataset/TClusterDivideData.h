#ifndef __TCLUSTERDIVIDEDATA__
#define __TCLUSTERDIVIDEDATA__

#include "Headers.h"
#include "TCluster.h"

class TClusterDivideData {
private:
	std::unordered_map<int, std::vector<TCluster*>> mClusterData;
public:
	TClusterDivideData(const std::vector<TCluster*>& clusters);
	const std::vector<TCluster*>& getClusterOfSize(const int clusterSize);
};




#endif