/**
 * @file TExperimentData.h
 * @author Yongjun Choi (yochoi@cern.ch)
 * @brief
 * @version 0.1
 * @date 16-04-2024
 *
 * @copyright Copyright (c) 2024
 *
*/

#ifndef __TEXPERIMENTDATA__
#define __TEXPERIMENTDATA__

#include "Headers.h"
#include "TALPIDEEvent.h"
#include "TCluster.h"

class TExperimentData {
private:
	std::vector<TALPIDEEvent*> mEvents;
	std::vector<TCluster*> mClusters;
	std::vector<TALPIDEEvent*> mMaskedEvents;
	std::vector<TCluster*> mMaskedClusters;
public:
	TExperimentData() = default;

	void setEvents(const std::vector<TALPIDEEvent*>&& events);
	void setClusters(const std::vector<TCluster*>&& clusters);
	void setMaskedEvents(const std::vector<TALPIDEEvent*>&& maskedEvents);
	void setMaskedClusters(const std::vector<TCluster*>&& maskedClusters);
	const std::vector<TALPIDEEvent*> getEvents() const;
	const std::vector<TCluster*> getClusters() const;
	const std::vector<TALPIDEEvent*> getMaskedEvents() const;
	const std::vector<TCluster*> getMaskedClusters() const;
};

#endif