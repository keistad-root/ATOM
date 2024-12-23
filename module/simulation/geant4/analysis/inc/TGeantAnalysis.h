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
#include "cpptqdm.h"
#include<filesystem>

#include "TPlotter.h"
#include "TParticleID.h"
#include "TVolumeID.h"

struct trackTuple {
	Int_t eventID;
	Int_t trackID;
	Int_t parentID;
	Int_t particleID;
	Double_t initX, initY, initZ;
	Double_t initPX, initPY, initPZ;
	Double_t initKineticEnergy;
	Int_t initVolumeID;
	Double_t finalX, finalY, finalZ;
	Double_t finalPX, finalPY, finalPZ;
	Double_t finalKineticEnergy;
	Int_t finalVolumeID;
};

struct incidentTuple {
	Int_t eventID = 0;
	Int_t trackID = 0;
	Int_t particleID = 0;
	Int_t initVolumeID = 0;
	Double_t depositEnergy[3] = {0, 0, 0};
	Double_t position[3] = {0, 0, 0};
	Double_t momentum[3] = {0, 0, 0};
	Double_t globalTime = 0;
	Double_t localTime = 0;
	Double_t kineticEnergy = 0;
};

class TGeantAnalysis : public TPlotter {
public:
	TGeantAnalysis(const CppConfigFile& fileName);
	~TGeantAnalysis();

private:
	std::filesystem::path mOutputPath;
	CppConfigFile mConfig;
	ProgressBar* mProgressBar;

	TFile* mFile = nullptr;
	TFile* mIncidentFile = nullptr;
	std::vector<trackTuple> mTrackTupleSet;
	std::vector<incidentTuple> mIncidentTupleSet;

	std::vector<std::string> mParticleList = {"unknown", "#alpha", "e^{-}", "#gamma", "p^{+}", "n^{0}", "^{12}C", "^{13}C", "N", "^{16}O", "^{18}O", "Si"};

	std::vector<std::string> mVolumeList = {"None", "ALPIDEMetal", "ALPIDEEpitaxial", "ALPIDESubstrate", "Collimator", "Screen", "World"};
	std::vector<std::string> mConfigList;

	std::unordered_map<std::string, TObject*> mHistograms;

public:
	void readTrack();

	void setHistograms();
	void getTrackInformations();
	void getIncidentInformations();
	void getTrackInformations(const trackTuple& track);
	void getIncidentInformations(const incidentTuple& incident);
	void saveHistograms();
};

#endif