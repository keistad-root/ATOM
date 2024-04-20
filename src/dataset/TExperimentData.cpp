#include "TExperimentData.h"

TExperimentData::TExperimentData() : fBits(kNotDeleted) { }

TExperimentData::~TExperimentData() {
	for ( TALPIDEEvent* event : mEvents ) {
		if ( event != nullptr && !event->IsDestructed() ) {
			delete event;
			event = nullptr;
		}
	}
	for ( TCluster* cluster : mClusters ) {
		if ( cluster != nullptr && !cluster->IsDestructed() ) {
			delete cluster;
			cluster = nullptr;
		}
	}
	for ( TALPIDEEvent* maskedEvent : mMaskedEvents ) {
		if ( maskedEvent != nullptr && !maskedEvent->IsDestructed() ) {
			delete maskedEvent;
			maskedEvent = nullptr;
		}
	}
	for ( TCluster* maskedCluster : mMaskedClusters ) {
		if ( maskedCluster != nullptr && !maskedCluster->IsDestructed() ) {
			delete maskedCluster;
			maskedCluster = nullptr;
		}
	}
	for ( TALPIDEEvent* noiseEvent : mNoiseEvents ) {
		if ( noiseEvent != nullptr && !noiseEvent->IsDestructed() ) {
			delete noiseEvent;
			noiseEvent = nullptr;
		}
	}
	for ( TCluster* noiseCluster : mNoiseClusters ) {
		if ( noiseCluster != nullptr && !noiseCluster->IsDestructed() ) {
			delete noiseCluster;
			noiseCluster = nullptr;
		}
	}
}


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

void TExperimentData::setNoiseEvents(const std::vector<TALPIDEEvent*>&& noiseEvents) {
	mNoiseEvents = noiseEvents;
}

void TExperimentData::setNoiseClusters(const std::vector<TCluster*>&& noiseClusters) {
	mNoiseClusters = noiseClusters;
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

const std::vector<TALPIDEEvent*> TExperimentData::getNoiseEvents() const {
	return mNoiseEvents;
}

const std::vector<TCluster*> TExperimentData::getNoiseClusters() const {
	return mNoiseClusters;
}