#ifndef __TGEANTANALYSISSECONDARY__
#define __TGEANTANALYSISSECONDARY__

#include "CppConfigFile.h"
#include "TGeantTuple.h"
#include<unordered_map>
#include<string_view>

class TFile;
class TTree;
class TH1D;
class TH2D;

class TGeantAnalysisSecondary {
public:
	TGeantAnalysisSecondary(const CppConfigFile& config);
	~TGeantAnalysisSecondary();
private:
	CppConfigFile mConfig;

	TFile* mInputFile;
	TTree* mInputTree;

	TSecondaryAnalysisTuple mInputTuple;
	std::unordered_map<std::string, TH1D*> m1DHistograms;
	std::unordered_map<std::string, TH2D*> m2DHistograms;
public:
	void openFile();
	void setHistograms();
	void readTree();
	void fillSecondaryHistograms();
	void saveFile();
};

#endif