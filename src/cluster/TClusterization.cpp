#include "TClusterization.h"

TClusterization::TClusterization(const std::vector<TALPIDEEvent*>& events) {
	for ( auto& event : events ) {
		mEvents.push_back(event);
	}
}

TClusterization::~TClusterization() { }

void TClusterization::removeConsecutionPixels() {
	std::vector<std::pair<int, int>> preDeletedPixel;
	std::vector<std::pair<int, int>> currentDeletedPixel;
	for ( int iEvent = 1; iEvent < mEvents.size(); iEvent++ ) {
		for ( int iPixel = mEvents[iEvent]->getData().size() - 1; iPixel > -1; iPixel-- ) {

			if ( std::find(mEvents[iEvent - 1]->getData().begin(), mEvents[iEvent - 1]->getData().end(), mEvents[iEvent]->getData()[iPixel]) != mEvents[iEvent - 1]->getData().end() || (!preDeletedPixel.empty() && find(preDeletedPixel.begin(), preDeletedPixel.end(), mEvents[iEvent]->getData()[iPixel]) != preDeletedPixel.end()) ) {
				currentDeletedPixel.push_back(mEvents[iEvent]->getData()[iPixel]);
				mEvents[iEvent]->removePixel(mEvents[iEvent]->getData()[iPixel]);
			}
		}
		preDeletedPixel.clear();
		preDeletedPixel = std::move(currentDeletedPixel);
		currentDeletedPixel.clear();
	}
}

void TClusterization::addNewCluster(const std::pair<int, int>& pixel, std::vector<TCluster*>& clusters, int iEvent, int iTime) {
	TCluster* cluster = new TCluster(iEvent, iTime);
	cluster->AddPixel(pixel);
	clusters.push_back(cluster);
}

void TClusterization::removeIndependentCluster(const std::pair<int, int>& pixel, std::vector<TCluster*>& clusters) {
	if ( clusters[0]->getMaxY() + 2 < pixel.second ) {
		int cut = 0;
		for ( const TCluster* cluster : clusters ) {
			if ( cluster->getMaxY() + 2 < pixel.second ) {
				cut++;
			} else break;
		}
		mClusters.insert(mClusters.end(), clusters.begin(), clusters.begin() + cut);
		clusters.erase(clusters.begin(), clusters.begin() + cut);
	}
}

bool TClusterization::clusterLitmusTest(const std::pair<int, int>& pixel, std::vector<TCluster*>& clusters) {
	bool clusterExist = false;
	int iCluster = 0;
	int iOrigin = 0;
	for ( TCluster*& clustered : clusters ) {
		if ( clustered->isNeighbour(pixel) ) {
			if ( clusterExist == false ) {
				clustered->AddPixel(pixel);
				clusterExist = true;
				iOrigin = iCluster;
			} else {
				clusters[iOrigin]->AddCluster(*clustered);
				clusters.erase(clusters.begin() + iCluster);
				break;
			}
		}
		iCluster++;
	}

	return clusterExist;
}

void TClusterization::clusterize() {
	std::cout << "Generate clusters from events..." << std::endl;

	ProgressBar* pBar = new ProgressBar(mEvents.size());

	for ( TALPIDEEvent*& event : mEvents ) {
		pBar->printProgress();
		std::vector<TCluster*> clusterCandidate;
		bool isFirst = true;
		for ( const std::pair<int, int>& pixel : event->getData() ) {
			if ( isFirst ) {
				addNewCluster(pixel, clusterCandidate, event->getEvent(), event->getTime());
				isFirst = false;
				continue;
			}
			removeIndependentCluster(pixel, clusterCandidate);
			bool clusterExist = clusterLitmusTest(pixel, clusterCandidate);
			if ( !clusterExist ) {
				addNewCluster(pixel, clusterCandidate, event->getEvent(), event->getTime());
			} // Make new cluster if no relation with any cluster candidates.
		}
		if ( !clusterCandidate.empty() ) {
			mClusters.insert(mClusters.end(), clusterCandidate.begin(), clusterCandidate.end());
			clusterCandidate.clear();
		} // Move remained cluster candidates to cluster
	}
	delete pBar;
	pBar = nullptr;
	for ( TCluster* cluster : mClusters ) {
		cluster->calMembers();
	}
}

const std::vector<TCluster*>& TClusterization::getClusters() const {
	return mClusters;
}