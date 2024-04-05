#include "TClusterN.h"

TClusterN::TClusterN(const std::vector<TCluster*> clusters, const int clusterSize) : mClusterSize(clusterSize) {
    for (TCluster* cluster : clusters) {
        if (cluster->getSize() == clusterSize) {
            mNCluster.push_back(cluster);
        }
    }
}

void TClusterN::calShapeSpecies() {
    bool isFirst = true;
    std::cout << mClusterSize << " " << mNCluster.size() << std::endl;
    for (TCluster* cluster : mNCluster) {
        if (isFirst) {
            mClustersWithShape.insert({cluster, 1});
            isFirst = false;
            continue;
        }
        TMatrix2D<int> checkingCluster = cluster->getShape();
        bool isHomoemorphismExist = false;
        for (auto iter = mClustersWithShape.begin(); iter != mClustersWithShape.end(); iter++) {
            TMatrix2D<int> comparedCluster = iter->first->getShape();
            if (comparedCluster.hasHomeomorphism(checkingCluster)) {
                iter->second++;
                isHomoemorphismExist = true;
                break;
            } 
            // std::cout << checkingCluster << std::endl;
            // std::cout << comparedCluster << std::endl;
            // std::cout << "====================================" << std::endl;
        }
        if (!isHomoemorphismExist) {
            mClustersWithShape.insert({cluster, 1});
        }
    }
}

void TClusterN::setClusterSize(const int clusterSize) {
    mClusterSize = clusterSize;
}

const std::unordered_map<TCluster*, int>& TClusterN::getClustersWithShape() const {
    return mClustersWithShape;
}

const int TClusterN::getClusterSize() const {
    return mClusterSize;
}