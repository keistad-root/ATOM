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

#ifdef __TCLUSTERSHAPEANALYSER_HEADER__ 
#include <iostream>

#include "TCanvas.h"
#include "TH2I.h"
#include "TText.h"
#include "TLine.h"
#include "TPaveText.h"

#include "cpptqdm.h"

#include "TClusterShape.h"
#include "TExperimentData.h"
#include "TMatrix2D.h"
#endif

#include <vector>

#include "TClusterAnalyser.h"

class TClusterShape;



class TClusterShapeAnalyser : protected TClusterAnalyser {
private:
	std::vector<TClusterShape*> mClusterShapes;
	std::vector<TClusterShape*> mMaskedClusterShapes;
	std::vector<TClusterShape*> mNoisePixelClusterShapes;

	int mNClusterShapes, mNMaskedClusterShapes, mNNoisePixelClusterShapes;
	int mClusterMode, mMaskedClusterMode, mNoisePixelClusterMode;
	int mNTotalShapes;
	int mMode;
	std::string mDataSetName;
public:
	TClusterShapeAnalyser(const TClusterAnalyser& analyser);
	~TClusterShapeAnalyser();
	void doShaping(int lower, int upper);
	void doShaping(const std::vector<int>& clusterSizeSet);

	void saveIndividualShapes();
	void saveSameSizeShapes();
	void saveTotalShapes();
	void saveDetailedInformationOfShapes();
	void saveSameSizeShapeEntry();
	void saveTotalShapeEntry();
};

#endif