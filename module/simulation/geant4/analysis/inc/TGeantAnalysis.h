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

class TGeantAnalysis : public TPlotter {
public:
	TGeantAnalysis();
	~TGeantAnalysis();

private:
	std::unique_ptr<TFile> mInputFile;
	std::unique_ptr<TTree> mIncidentTree;
	std::filesystem::path mOutputPath;

	TIncidentAnalysisTuple mIncidentTuple;
	std::unordered_map<std::string_view, std::unique_ptr<TH1D>> m1DHistograms;
	std::unordered_map<std::string_view, std::unique_ptr<TH2D>> m2DHistograms;

public:
	void readInputFile(std::filesystem::path inputFilePath);
	void setHistograms(const std::vector<CppConfigDictionary>& configList);
	void readTree();
	void fillHistograms();
	void saveFile(std::filesystem::path outputFilePath);
	int getNDouble(std::vector<std::pair<Double_t, Double_t>> position);
};
#endif