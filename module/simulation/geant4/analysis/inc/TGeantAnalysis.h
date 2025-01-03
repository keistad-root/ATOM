#ifndef __TGEANTANALYSIS__
#define __TGEANTANALYSIS__

#include<filesystem>
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"

#include "TPlotter.h"
#include "CppConfigFile.h"
#include "cpptqdm.h"

#include "TGeantTuple.h"
#include "TParticleID.h"
#include "TVolumeID.h"

class TGeantAnalysis : public TPlotter {
public:
	TGeantAnalysis();
	~TGeantAnalysis();

private:
	std::unique_ptr<TFile> mPrimaryFile;
	std::unique_ptr<TFile> mIncidentFile;
	std::unique_ptr<TTree> mPrimaryTree;
	std::unique_ptr<TTree> mIncidentTree;
	std::filesystem::path mOutputPath;

	TPrimaryAnalysisTuple mPrimaryTuple;
	TIncidentAnalysisTuple mIncidentTuple;
	std::unordered_map<std::string_view, std::unique_ptr<TH1D>> m1DHistograms;
	std::unordered_map<std::string_view, std::unique_ptr<TH2D>> m2DHistograms;

public:
	void readIncidentFile(std::filesystem::path inputFilePath);
	void readPrimaryFile(std::filesystem::path inputFilePath);
	void setHistograms(const std::vector<CppConfigDictionary>& configList);
	void readTree();
	void fillIncidentHistograms();
	void fillPrimaryHistograms();
	void saveFile(std::filesystem::path outputFilePath);
	int getNDouble(std::vector<std::pair<Double_t, Double_t>> position);
};
#endif