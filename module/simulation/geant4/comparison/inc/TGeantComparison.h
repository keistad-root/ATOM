#ifndef __TGEANTCOMPARISON__
#define __TGEANTCOMPARISON__

#include<filesystem>
#include<csv.h>
#include<sstream>

#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TGraphErrors.h"
#include "TPlotter.h"
#include "CppConfigFile.h"

struct FileInfo {
	Int_t length;
	Int_t width;
	Double_t area;
	std::string file;
	Int_t nAlpha = 0;
	Int_t nElectron = 0;
	Int_t nDouble = 0;
	bool isEpitaxial = false;
	bool isMetal = false;
};

struct ExpInfo {
	Int_t length;
	Int_t width;
	Double_t area;
	Double_t areaError;
	Double_t regionA;
	Double_t regionB;
	Double_t regionC;
	Double_t regionD;
};

class TGeantComparison : public TPlotter {
public:
	TGeantComparison(const CppConfigFile& config);
	~TGeantComparison();
private:
	CppConfigFile mConfig;
	std::filesystem::path mOutputPath;
	std::vector<FileInfo> mReference;
	std::vector<FileInfo> mFileInfo;
	std::vector<ExpInfo> mExpInfo;

	TH1* mDistanceHist[3];

	const Int_t activity = 4300;
	const Double_t time = 0.025;
	const Int_t eventSize = activity * time;
public:
	void getIncidentInfo(FileInfo& fileInfo);
	Int_t getDoubleClusterNum(std::vector<std::pair<Double_t, Double_t>> position);
	void getDistance(TH1* hist, std::vector<std::pair<Double_t, Double_t>> position);

	void drawNAlphaGraph();
	void drawNElectronGraph();
	void drawAlphaElectronRatioGraph();

	void drawNDoubleGraph();
	void drawDoubleSingleRatioGraph();

	void compareAlpha();
	void compareElectron();
	void compareDouble();
};

#endif