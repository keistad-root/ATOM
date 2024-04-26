#define __TCLUSTERSHAPE_HEADER__
#include "TClusterShape.h"


/**
 * @brief Construct a new TClusterShape::TClusterShape object
 * @details It takes cluster set and cluster size for analysing here.
 * The role of this construct is extracting cluster from cluster set according to cluster size.
 * @param clusters
 * @param clusterSize
 * @warning
 * @bug
 * @todo
 * @see TCluster::getSize()
 */

TClusterShape::TClusterShape() : fBits(kNotDeleted) { }

TClusterShape::TClusterShape(const int clusterSize, const std::vector<TCluster*>& clusters) : mClusterSize(clusterSize), fBits(kNotDeleted) {
	mClusterOriginSet.assign(clusters.begin(), clusters.end());
}

// TClusterShape::TClusterShape(const std::vector<TCluster*> clusters, const int clusterSize) : mClusterSize(clusterSize) {
// 	// 	for ( TCluster* cluster : clusters ) {
// 	// 		if ( cluster->getSize() == clusterSize ) {
// 	// 			mClusterWithN.push_back(cluster);
// 	// 		}
// 	// 	}
// }

TClusterShape::TClusterShape(const std::vector<TCluster*> clusters) {
	// mClusterWithN.assign(clusters.begin(), clusters.end());
}

TClusterShape::~TClusterShape() {
	for ( TShapeInfo shapeInfo : mClusterShapeInfos ) {
		delete shapeInfo.mClusterMap;
		delete shapeInfo.mClusterMatrix;

	}
}

/**
 * @brief Specificate the clusters according to their shapes.
 * @details This function checks homeomorphism of cluster and store the shape informations.
 * By the pixel structure, there are 8 types for homeomorphism.
 * @return void
 * @warning
 * @bug
 * @todo
 * @see TCluster::getShape(), TMatrix2D::hasHomeomorphism
*/
void TClusterShape::identifyShapes() {
	bool isFirst = true;
	int iShape = 0;
	for ( TCluster* cluster : mClusterOriginSet ) {
		if ( isFirst ) {
			TShapeInfo shapeInfo;
			shapeInfo.mPresidentCluster = cluster;
			shapeInfo.mEntry = 1;
			shapeInfo.iShape = iShape;
			iShape++;
			calClusterInfo(shapeInfo, cluster);
			shapeInfo.mClusterMap = clusterMap(shapeInfo.mClusterMatrix);
			mClusterShapeInfos.push_back(shapeInfo);
			std::vector<TCluster*> newClusterSet;
			mClusterSameSizeSet.insert_or_assign(iShape, newClusterSet);
			isFirst = false;
			continue;
		}
		TMatrix2D<int>* checkingCluster = clusterMatrix(cluster);
		bool isHomoemorphismExist = false;
		for ( TShapeInfo& shapeInfo : mClusterShapeInfos ) {
			TMatrix2D<int>* comparedCluster = shapeInfo.mClusterMatrix;
			if ( comparedCluster->hasHomeomorphism(*checkingCluster) ) {
				shapeInfo.mEntry++;
				isHomoemorphismExist = true;
				// mClusterSameSizeSet.find(shapeInfo.iShape)->second.push_back(cluster);
				break;
			}
		}

		if ( !isHomoemorphismExist ) {
			TShapeInfo shapeInfo;
			shapeInfo.mPresidentCluster = cluster;
			shapeInfo.mEntry = 1;
			shapeInfo.iShape = iShape;
			iShape++;
			calClusterInfo(shapeInfo, cluster);
			shapeInfo.mClusterMap = clusterMap(shapeInfo.mClusterMatrix);
			mClusterShapeInfos.push_back(shapeInfo);
			std::vector<TCluster*> newClusterSet;
			mClusterSameSizeSet.insert_or_assign(iShape, newClusterSet);
		}
	}
}
/**
 * @brief Sorting shapes according to the entry.
 * @details It sorts cluster information structs with same shapes.
 * The number of clusters having same shapes is saved in cluster information.
 * It is used for sort criteria.
 * @param descend
 * @return void
 * @warning
 * @bug
 * @todo More strict criteria is needed. If they have same entry, then it cannot be sorted and saved by coming order.
 * @see
*/
void TClusterShape::sortShapes(bool descend) {
	std::sort(mClusterShapeInfos.begin(), mClusterShapeInfos.end(),
			  [ ](TShapeInfo& a, TShapeInfo& b) {
				  if ( a.mEntry != b.mEntry ) {
					  return a.mEntry > b.mEntry;
				  } else if ( a.mClusterMatrix->getNRow() != b.mClusterMatrix->getNRow() ) {
					  return a.mClusterMatrix->getNRow() < b.mClusterMatrix->getNRow();
				  } else {
					  return a.mClusterMatrix->getNColumn() < b.mClusterMatrix->getNColumn();
				  }
			  }
	);
}

/**
 * @brief Drawing and saving shape image.
 * @details It visulaizes shape informations form matrix form.
 * `TImage` class is used for saving plots.
 * The extra pixel on border of plot is drawn for readability.
 * @param clusterMatrix
 * @return TImage*
 * @warning
 * @bug The canvases and histograms have same name with each other.
 * @todo Avoiding same name problem.
 * @see
*/

TH2I* TClusterShape::clusterMap(const TMatrix2D<int>* clusterMatrix) {
	static int numbering;
	int nRow = clusterMatrix->getNRow();
	int nColumn = clusterMatrix->getNColumn();

	TH2I* map = new TH2I(Form("_%d", numbering), "", nRow + 2, 0, nRow + 2, nColumn + 2, 0, nColumn + 2);
	for ( int iRow = 0; iRow < nRow; iRow++ ) {
		for ( int iColumn = 0; iColumn < nColumn; iColumn++ ) {
			if ( clusterMatrix->getElement(iRow, iColumn) == 1 ) {
				map->Fill(iRow + 1, iColumn + 1);
			}
		}
	}

	map->GetXaxis()->SetNdivisions(nRow + 2, 0, 0, true);
	for ( int i = 1; i <= map->GetNbinsX(); ++i ) {
		map->GetXaxis()->SetBinLabel(i, "");
	}
	for ( int i = 1; i <= map->GetNbinsY(); ++i ) {
		map->GetYaxis()->SetBinLabel(i, "");
	}
	map->GetYaxis()->SetNdivisions(nColumn + 2, 0, 0, true);
	map->GetZaxis()->SetNdivisions(0, 0, 0, true);
	map->SetStats(0);

	numbering++;
	return map;
}


void TClusterShape::calClusterInfo(TShapeInfo& shapeInfo, TCluster* cluster) {
	int xBinN = cluster->getMaxX() - cluster->getMinX();
	int yBinN = cluster->getMaxY() - cluster->getMinY();
	bool longHeight = yBinN > xBinN ? true : false;
	shapeInfo.mLongBinN = longHeight ? yBinN : xBinN;
	shapeInfo.mShortBinN = longHeight ? xBinN : yBinN;

	shapeInfo.mClusterMatrix = new TMatrix2D<int>(shapeInfo.mLongBinN + 1, shapeInfo.mShortBinN + 1);
	if ( longHeight ) {
		for ( const std::pair<int, int>& pixel : cluster->getPixels() ) {
			shapeInfo.mClusterMatrix->setElement(pixel.second - cluster->getMinY(), pixel.first - cluster->getMinX(), 1);
		}
	} else {
		for ( const std::pair<int, int>& pixel : cluster->getPixels() ) {
			shapeInfo.mClusterMatrix->setElement(pixel.first - cluster->getMinX(), pixel.second - cluster->getMinY(), 1);
		}
	}

}

TMatrix2D<int>* TClusterShape::clusterMatrix(const TCluster* cluster) {
	int xBinN = cluster->getMaxX() - cluster->getMinX();
	int yBinN = cluster->getMaxY() - cluster->getMinY();
	TMatrix2D<int>* matrix = new TMatrix2D<int>(xBinN + 1, yBinN + 1);
	for ( const std::pair<int, int>& pixel : cluster->getPixels() ) {
		matrix->setElement(pixel.first - cluster->getMinX(), pixel.second - cluster->getMinY(), 1);
	}
	return matrix;
}

/**
 * @brief Setter of `mClusterSize` member
 * @param clusterSize
 * @return void
*/
void TClusterShape::setClusterSize(const int clusterSize) {
	mClusterSize = clusterSize;
}
/**
 * @brief Getter of `mClusterShapeInfos` member
 * @return const std::vector<TShapeInfo>&
*/
const std::vector<TShapeInfo>& TClusterShape::getClusterShapeInfos() const {
	return mClusterShapeInfos;
}
/**
 * @brief Getter of `mClusterSize` member
 * @return const int
*/
const int TClusterShape::getClusterSize() const {
	return mClusterSize;
}

const std::unordered_map<int, std::vector<TCluster*>>& TClusterShape::getClusterSameSizeSet() const {
	return mClusterSameSizeSet;
}
