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

struct TEventInformation {
	int eventID;
	int particleID;
	int initialVolumeID;
	std::array<double, 3> incidentPosition;
	double incidentMomentum;
	double incidentKineticEnergy;
	double incidentAngle;

	int nSecondaryParticle;
	std::array<double, 4> totalDepositEnergy;
	std::array<double, 4> alphaDepositEnergy;
	std::array<double, 4> electronDepositEnergy;
	std::array<double, 4> gammaDepositEnergy;

};

class TGeantAnalysis {
public:
	TGeantAnalysis(const CppConfigFile& config);
	~TGeantAnalysis();
private:
	CppConfigFile mConfig;

	TFile* mPrimaryFile;
	TFile* mIncidentFile;
	TTree* mPrimaryTree;
	TTree* mIncidentTree;
	std::filesystem::path mOutputPath;

	TPrimaryAnalysisTuple mPrimaryTuple;
	TIncidentAnalysisTuple mIncidentTuple;
	std::unordered_map<std::string, TH1D*> m1DHistograms;
	std::unordered_map<std::string, TH2D*> m2DHistograms;
	std::array<int, 4> mEntry;
	std::array<double, 2> mRoi;

	bool isPrimary = false, isIncident = false, isSecondary = false;
	bool isRoi = false;

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
	int getNDouble(std::vector<std::pair<Double_t, Double_t>> position);

	void fill1DHistograms(std::string_view name, double value);
	void fill2DHistograms(std::string_view name, double x, double y);
	bool isFromOutside();
	bool isFromALPIDE();
};
#endif