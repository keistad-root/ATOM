/**
 * @file TClusterization.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-04-06
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __TCLUSTERIZATION__
#define __TCLUSTERIZATION__

#include "Headers.h"
#include "TCluster.h"
#include "TALPIDEEvent.h"

#include "cpptqdm.h"

class TClusterization {
	std::vector<TALPIDEEvent*> mEvents;
	std::vector<TCluster*> mClusters;
public:
	TClusterization() = delete;
	TClusterization(const std::vector<TALPIDEEvent*>& events);
	~TClusterization();

	void removeConsecutionPixels(); // Remove later pixels if it fired continuously.

	void addNewCluster(const std::pair<int, int>& pixel, std::vector<TCluster*>& clusters, int iEvent, int iTime);

	void removeIndependentCluster(const std::pair<int, int>& pixel, std::vector<TCluster*>& clusters);

	bool clusterLitmusTest(const std::pair<int, int>& pixel, std::vector<TCluster*>& clusters);
	void clusterize();
	const std::vector<TCluster*>& getClusters() const;
};

#endif