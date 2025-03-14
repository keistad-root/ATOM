#ifndef __TGEANTANALYSIS__
#define __TGEANTANALYSIS__

#include<filesystem>
#include<unordered_map>
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
	TGeantAnalysis(const CppConfigFile& config);
	~TGeantAnalysis();
private:
	CppConfigFile mConfig;

	TFile* mPrimaryFile;
	TFile* mIncidentFile;
	TFile* mSecondaryFile;
	TTree* mPrimaryTree;
	TTree* mIncidentTree;
	TTree* mSecondaryTree;
	std::filesystem::path mOutputPath;

	TPrimaryAnalysisTuple mPrimaryTuple;
	TIncidentAnalysisTuple mIncidentTuple;
	std::vector<TIncidentAnalysisTuple> mIncidentSet;
	std::vector<TSecondaryAnalysisTuple> mSecondarySet;
	TSecondaryAnalysisTuple mSecondaryTuple;
	std::unordered_map<std::string, TH1D*> m1DHistograms;
	std::unordered_map<std::string, TH2D*> m2DHistograms;
	std::array<int, 4> mEntry;

	Double_t depositEnergyMetal = 0.;
	Double_t depositEnergyEpitaxial = 0.;
	Double_t depositEnergySubstrate = 0.;

	bool isPrimary = false, isIncident = false, isSecondary = false;

public:
	void setHistograms();
	void readTree();
	void saveFile();
	const std::array<int, 4>& getEntry() const { return mEntry; };
private:
	void readIncidentFile();
	void readPrimaryFile();
	void readSecondaryFile();
	void readPrimaryTree();
	void readIncidentTree();
	void readSecondaryTree();
	int getNDouble(std::vector<std::pair<Double_t, Double_t>> position);
	void fillIncidentHistograms();
	void fillPrimaryHistograms();
	void fillSecondaryHistograms();
};
#endif