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
	std::vector<TALPIDEEvent*> mNoiseEvents;
	std::vector<TCluster*> mNoiseClusters;

public:
	TExperimentData();
	~TExperimentData();

	void setEvents(const std::vector<TALPIDEEvent*>&& events);
	void setClusters(const std::vector<TCluster*>&& clusters);
	void setMaskedEvents(const std::vector<TALPIDEEvent*>&& maskedEvents);
	void setMaskedClusters(const std::vector<TCluster*>&& maskedClusters);
	void setNoiseEvents(const std::vector<TALPIDEEvent*>&& noiseEvents);
	void setNoiseClusters(const std::vector<TCluster*>&& noiseClusters);
	const std::vector<TALPIDEEvent*> getEvents() const;
	const std::vector<TCluster*> getClusters() const;
	const std::vector<TALPIDEEvent*> getMaskedEvents() const;
	const std::vector<TCluster*> getMaskedClusters() const;
	const std::vector<TALPIDEEvent*> getNoiseEvents() const;
	const std::vector<TCluster*> getNoiseClusters() const;

private:
	uint fBits;
public:
	enum {
		kNotDeleted = 0x02000000
	};
	bool IsDestructed() const { return !TestBit(kNotDeleted); }
	bool TestBit(uint f) const { return (bool) ((fBits & f) != 0); }
};

#endif