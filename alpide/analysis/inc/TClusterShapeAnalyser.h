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
#include "TGraph.h"
#include "TF1.h"
#include "TLegend.h"

#include "cpptqdm.h"
#include "CppConfigFile.h"

#include "TClusterShape.h"
#include "TCluster.h"
#include "TClusterDivideData.h"
#include "TExperimentData.h"
#include "TMatrix2D.h"
#endif

#include <vector>
#include <unordered_map>

#include "TClusterAnalyser.h"

class TClusterShape;

class TClusterShapeAnalyser : protected TClusterAnalyser {
private:
	std::unordered_map<std::string, std::vector<TClusterShape*>> mClusterShapeSet;
	std::unordered_map<std::string, int> mNTotalShapeSet;
	std::unordered_map<std::string, int> mMaxModeSet;
public:
	TClusterShapeAnalyser(const TClusterAnalyser& analyser);
	~TClusterShapeAnalyser();
	void doShaping(std::string_view typeName, const std::vector<int>& clusterSizeRange);
	void saveIndividualShapes(std::string_view typeName, const CppConfigDictionary config);
	void saveSameSizeInfos(std::string_view typeName, const CppConfigDictionary config);
	void saveSameSizeShapes(std::string_view typeName, const CppConfigDictionary config);
	void saveTotalShapes(std::string_view typeName, const CppConfigDictionary config);
	void saveSameSizeShapeEntry(std::string_view typeName, const CppConfigDictionary config);
	void saveTotalShapeEntry(std::string_view typeName, const CppConfigDictionary config);

private:
	unsigned int fBits;
public:
	enum {
		kNotDeleted = 0x02000000
	};
	bool IsDestructed() const { return !TestBit(kNotDeleted); }
	bool TestBit(unsigned int f) const { return (bool) ((fBits & f) != 0); }
};

#endif