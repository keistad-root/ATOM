/**
 * @file TClusterShape.h
 * @author Yongjun Choi (yochoi@cern.ch)
 * @brief Tools for extracting cluster shape image
 * @version 0.1
 * @date 16-04-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __TCLUSTERSHAPE__
#define __TCLUSTERSHAPE__

#ifdef __TCLUSTERSHAPE_HEADER__
#include "TCluster.h"
#include "TH2I.h"
#include "TMatrix2D.h"
#endif

#include<vector>

class TH2I;

class TCluster;
template<typename T> class TMatrix2D;

/**
 * @struct TShapeInfo
 * @brief The information set stucture for clusters that having homeomorphism shape.
 * @details It stores on cluster for extracting basic cluster informations.
 * And cluster image and the number of homeomorphism clusters are saved.
 * @warning
 * @bug
 * @todo Add struct member if needed.
 */
struct TShapeInfo {
	TCluster* mPresidentCluster;
	TMatrix2D<int>* mClusterMatrix;
	TH2I* mClusterMap;
	int mEntry;
	int mShortBinN;
	int mLongBinN;
};

/**
 * @class TClusterShape
 * @brief Class for extracting cluster shape information with same cluster size
 * @details It collects clusters having same cluster size.
 * And specificate the clusters according to their shapes.
 * The image and other informations can be extract by `TShapeInfo` struct.
 * @warning
 * @bug
 * @todo It can be modified if needed.
 */
class TClusterShape {
private:
	int mClusterSize; /**< The cluster size of this shape*/
	std::vector<TCluster*> mClusterWithN; /**< The set of clusters extracting from all clusters if their size is mClusterSize */
	std::vector<TShapeInfo> mClusterShapeInfos; /**< The set of cluster shape informations */
public:
	TClusterShape() = default;
	TClusterShape(const std::vector<TCluster*> clusters, const int clusterSize);
	~TClusterShape();

	void identifyShapes();
	void sortShapes(bool descend = true);
	void calClusterInfo(TShapeInfo& shapeInfo, TCluster* cluster);
	TMatrix2D<int>* clusterMatrix(const TCluster* cluster);
	TH2I* clusterMap(const TMatrix2D<int>* clusterMatrix);

	const std::vector<TShapeInfo>& getClusterShapeInfos() const;

	void setClusterSize(const int ClusterSize);
	const int getClusterSize() const;
};

#endif