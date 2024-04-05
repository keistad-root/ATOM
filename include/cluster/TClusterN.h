#ifndef __TCLUSTERN__
#define __TCLUSTERN__

#include "Headers.h"
#include "TCluster.h"
#include "TClusterAnalyser.h"

class TClusterN {
private:
    int mClusterSize;
    std::vector<TCluster*> mNCluster;
    std::unordered_map<TCluster*, int> mClustersWithShape;
public:
    TClusterN(const std::vector<TCluster*> clusters, const int clusterSize);

    void calShapeSpecies();

    const std::unordered_map<TCluster*, int>& getClustersWithShape() const;

    void setClusterSize(const int ClusterSize);
    const int getClusterSize() const;
};

#endif