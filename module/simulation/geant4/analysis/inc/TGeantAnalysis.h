#ifndef __TGEANTANALYSIS__
#define __TGEANTANALYSIS__

#include<iostream>

#include "TStyle.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TMath.h"
#include "CppConfigFile.h"
#include<filesystem>

class TGeantAnalysis {
public:
	TGeantAnalysis();
	TGeantAnalysis(const CppConfigFile& fileName);
	~TGeantAnalysis();

private:
	std::filesystem::path mOutputPath;

	TFile* mFile = nullptr;

	TTree* mInitTree = nullptr;

public:
	void readInit();
	void readALPIDE();
	void readIncident();
};

#endif