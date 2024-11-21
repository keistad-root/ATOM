#ifndef __TCLUSTERDIVIDEDATA__
#define __TCLUSTERDIVIDEDATA__

#ifdef __TCLUSTERDIVIDEDATA_HEADER__
#include <iostream>
#include "TCluster.h"
#endif

#include<unordered_map>
#include<vector>

class TCluster;

class TClusterDivideData {
private:
	std::unordered_map<int, std::vector<TCluster*>> mClusterData;
public:
	TClusterDivideData(const std::vector<TCluster*>& clusters);
	TClusterDivideData(const TClusterDivideData& copy);
	const std::vector<TCluster*>& getClusterOfSize(const int clusterSize);
};

#endif