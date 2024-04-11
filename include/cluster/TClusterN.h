/**
 * @file TClusterN.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-04-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __TCLUSTERN__
#define __TCLUSTERN__

#include "Headers.h"
#include "TCluster.h"

class TClusterN {
private:
	int mClusterSize;
	std::vector<TCluster*> mNCluster;
	std::vector<std::pair<TCluster*, int>> mClustersWithShape;
public:
	TClusterN() = default;
	TClusterN(const std::vector<TCluster*> clusters, const int clusterSize);

	void calShapeSpecies();

	const std::vector<std::pair<TCluster*, int>>& getClustersWithShape() const;

	void setClusterSize(const int ClusterSize);
	const int getClusterSize() const;
};

#endif