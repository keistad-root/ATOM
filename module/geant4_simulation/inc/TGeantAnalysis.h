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

class TH1D;
class TH2D;

class TGeantAnalysis {
public:
	TGeantAnalysis();
	~TGeantAnalysis();

private:
	TFile* mPrimaryFile;
	TFile* mIncidentFile;
	TFile* mSecondaryFile;
	TTree* mPrimaryTree;
	TTree* mIncidentTree;
	TTree* mSecondaryTree;
	std::filesystem::path mOutputPath;

	TPrimaryAnalysisTuple mPrimaryTuple;
	TIncidentAnalysisTuple mIncidentTuple;
	TSecondaryAnalysisTuple mSecondaryTuple;
	std::unordered_map<std::string_view, TH1D*> m1DHistograms;
	std::unordered_map<std::string_view, TH2D*> m2DHistograms;
	std::array<int, 4> mEntry;

public:
	void readIncidentFile(std::filesystem::path inputFilePath);
	void readPrimaryFile(std::filesystem::path inputFilePath);
	void readSecondaryFile(std::filesystem::path inputFilePath);
	void setHistograms(const std::vector<CppConfigDictionary>& configList);
	void readTree();
	void fillIncidentHistograms();
	void fillPrimaryHistograms();
	void fillSecondaryHistograms();
	void saveFile(std::filesystem::path outputFilePath);
	int getNDouble(std::vector<std::pair<Double_t, Double_t>> position);
	const std::array<int, 4>& getEntry() const { return mEntry; };
};
#endif