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
TClusterShape::TClusterShape(const std::vector<TCluster*> clusters, const int clusterSize) : mClusterSize(clusterSize) {
	for ( TCluster* cluster : clusters ) {
		if ( cluster->getSize() == clusterSize ) {
			mClusterWithN.push_back(cluster);
		}
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
	for ( TCluster* cluster : mClusterWithN ) {
		if ( isFirst ) {
			TShapeInfo shapeInfo;
			shapeInfo.mPresidentCluster = cluster;
			shapeInfo.mEntry = 1;
			shapeInfo.mClusterImage = clusterImage(cluster->getShape());
			mClusterShapeInfos.push_back(shapeInfo);
			isFirst = false;
			continue;
		}
		TMatrix2D<int> checkingCluster = cluster->getShape();
		bool isHomoemorphismExist = false;
		for ( TShapeInfo& shapeInfo : mClusterShapeInfos ) {
			TMatrix2D<int> comparedCluster = shapeInfo.mPresidentCluster->getShape();
			if ( comparedCluster.hasHomeomorphism(checkingCluster) ) {
				shapeInfo.mEntry++;
				isHomoemorphismExist = true;
				break;
			}
		}

		if ( !isHomoemorphismExist ) {
			TShapeInfo shapeInfo;
			shapeInfo.mPresidentCluster = cluster;
			shapeInfo.mEntry = 1;
			shapeInfo.mClusterImage = clusterImage(cluster->getShape());
			mClusterShapeInfos.push_back(shapeInfo);
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
				  return a.mEntry > b.mEntry;
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
TImage* TClusterShape::clusterImage(const TMatrix2D<int>& clusterMatrix) {
	static int numbering = 0;
	// Creating new image.
	TImage* image = TImage::Create();

	// Getting row and column length.
	int nRow = clusterMatrix.getNRow();
	int nColumn = clusterMatrix.getNColumn();

	// Defining new canvas for saving to image. The name and title is set ans "temp". The size of one pixel is set as 100. The margin is removed for compact image and grid is added for readability.
	TCanvas* shapeCan = new TCanvas(Form("%d", numbering), "temp", 100 * (nRow + 2), 100 * (nColumn + 2));
	shapeCan->SetMargin(0, 0, 0, 0);
	shapeCan->SetGrid();

	// Making histogram for saving matrix information. The extra pixels are added as a border.
	TH2I* shapeMap = new TH2I(Form("%d", numbering), "", nRow + 2, 0, nRow + 2, nColumn + 2, 0, nColumn + 2);
	// The pixel is filled if the element is 1.
	for ( int iRow = 0; iRow < nRow; iRow++ ) {
		for ( int iColumn = 0; iColumn < nColumn; iColumn++ ) {
			if ( clusterMatrix.getElement(iRow, iColumn) == 1 ) {
				shapeMap->Fill(iRow + 1, iColumn + 1);
			}
		}
	}
	// Remove sub ticks. Sub ticks make the plot confusing.
	shapeMap->GetXaxis()->SetNdivisions(nRow + 2, 0, 0, true);
	shapeMap->GetYaxis()->SetNdivisions(nColumn + 2, 0, 0, true);
	shapeMap->GetZaxis()->SetNdivisions(0, 0, 0, true);
	// Remove legend. The legend information is useless in here.
	shapeMap->SetStats(0);
	// The histogram is drown with colz. By this, The fired pixels are represented as yellow box and others are white.
	shapeMap->Draw("COLZ");

	// The canvas is saved in image.
	image->FromPad(shapeCan);
	// return inner image.
	numbering++;
	return image;
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