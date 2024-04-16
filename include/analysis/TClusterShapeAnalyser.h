/**
 * @file TClusterShapeAnalyser.h
 * @author Yongjun Choi (yochoi@cern.ch)
 * @brief Tools for analysing and drawing cluster shape
 * @version 0.1
 * @date 16-04-2024
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __TCLUSTERSHAPEANALYSER__
#define __TCLUSTERSHAPEANALYSER__

#include "TAnalyser.h"
#include "TClusterAnalyser.h"
#include "TClusterShape.h"

class TClusterShapeAnalyser : protected TClusterAnalyser {
private:
	std::vector<TClusterShape*> clusterShapes;
public:
	TClusterShapeAnalyser(const TClusterAnalyser& analyser);
	void doShaping(int lower, int upper);
	void saveIndividualShapes();
	void saveSameSizeShapes();
	void saveTotalShapes();
	void saveDetailedInformationOfShapes();
	void saveSameSizeShapeEntry();
	void saveTotalShapeEntry();
};

#endif