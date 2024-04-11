/**
 * @file TAnalyser.h
 * @author Yongjun Choi (yochoi@cern.ch)
 * @brief The class for controlling ROOT and config file when analysing
 * @version 0.1
 * @date 2024-04-11
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __TANALYSER__
#define __TANALYSER__

#include "Headers.h"
#include "TKey.h"
#include "cppconfig.h"

class TAnalyser {
private:
	TFile* mInputFile = nullptr;
	Configuration* mConfFile = nullptr;
	std::unordered_map<std::string, int> mConfigSetting;
public:
	TAnalyser() = delete;
	TAnalyser(TString inputFileTitle);
	TAnalyser(TFile& inputFile);
	TAnalyser(TFile* inputFile);
	~TAnalyser();

	void setInputFile(TString inputFileTitle);
	void setInputFile(TFile* inputFile);
	void setInputFile(TFile& inputFile);

	void setConfFile(std::string_view configFileTitle);
	void setConfFile(Configuration* config);
	const Configuration* getConfFile() const;
	const std::unordered_map<std::string, int> getConfigSetting() const;
	TTree* openTree(std::string treeName);
};

#endif