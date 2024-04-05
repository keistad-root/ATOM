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

    void addNewCluster(const std::array<int, 2>& pixel, std::vector<TCluster*>& clusters, int iEvent, int iTime);

    void removeIndependentCluster(const std::array<int, 2>& pixel, std::vector<TCluster*>& clusters);

    bool clusterLitmusTest(const std::array<int,2>& pixel, std::vector<TCluster*>& clusters);
    void clusterize();
    const std::vector<TCluster*>& getClusters() const;
};

#endif