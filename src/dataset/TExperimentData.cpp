#include "TExperimentData.h"

void TExperimentData::setEvents(const std::vector<TALPIDEEvent*>&& events) {
	mEvents = events;
}

void TExperimentData::setClusters(const std::vector<TCluster*>&& clusters) {
	mClusters = clusters;
}

void TExperimentData::setMaskedEvents(const std::vector<TALPIDEEvent*>&& maskedEvents) {
	mMaskedEvents = maskedEvents;
}

void TExperimentData::setMaskedClusters(const std::vector<TCluster*>&& maskedClusters) {
	mMaskedClusters = maskedClusters;
}

const std::vector<TALPIDEEvent*> TExperimentData::getEvents() const {
	return mEvents;
}

const std::vector<TCluster*> TExperimentData::getClusters() const {
	return mClusters;
}

const std::vector<TALPIDEEvent*> TExperimentData::getMaskedEvents() const {
	return mMaskedEvents;
}

const std::vector<TCluster*> TExperimentData::getMaskedClusters() const {
	return mMaskedClusters;
}