/**
 * @file TThresholdAnalyser.h
 * @author Yongjun Choi (yochoi@cern.ch)
 * @brief The tools for threshold analysis
 * @version 0.1
 * @date 2024-04-13
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __TTHRESHOLDANALYSER__
#define __TTHRESHOLDANALYSER__

 // Basic header
#include<iostream>
#include<sstream>
#include<vector>
#include<array>
#include<fstream>
#include<unordered_map>

// ROOT header
#include "TFile.h"
#include "TTree.h"
#include "TObjString.h"

// User header
#include "cpptqdm.h"
#include "TThreshold.h"
#include "CppConfigFile.h"

class TThresholdAnalyser {
private:
	std::ifstream mFile; /** Dat file */
	std::ifstream mCfgFile;
	TObjString chipID;

	std::filesystem::path mDataPath;
	TFile* mThresholdFile;

	CppConfigFile mConfig;

	std::unordered_map<std::string, int> mSetting;

	std::vector<TThreshold*> mThresholds; /**< The array in which the informations about threshold of ALPIDE are stored */
public:
	TThresholdAnalyser(const CppConfigFile& mConfig);
	~TThresholdAnalyser();

	void getThreshold();
	void setConfig();

	void saveThresholdData();
};

#endif