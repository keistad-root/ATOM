/**
 * @file TClusterization.h
 * @author Yongjun Choi (yochoi@cern.ch)
 * @brief Class for clusterizing events on a chip
 * @version 0.1
 * @date 13-04-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __TCLUSTERIZATION__
#define __TCLUSTERIZATION__

#ifdef __TCLUSTERIZATION_HEADER__
#include "cpptqdm.h"

#include "TALPIDEEvent.h"
#include "TCluster.h"
#endif

#include <vector>

class TALPIDEEvent;
class TCluster;

/**
 * @class TClusterization
 * @brief Class of tools for clusterizing events for single event
 * @details It takes a role to clusterize an event.
 */
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