#ifndef __TTHRPEDE__
#define __TTHRPEDE__

#include "CppConfigFile.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TCanvas.h"

class TThrPede {
public:
	TThrPede(CppConfigFile* config);
	~TThrPede();
private:
	CppConfigFile* mConfig;

	TFile* mThresholdFile;
	TFile* mPedestalFile;

	std::array<double, 1024 * 512> mThresholdSet;
	std::array<int, 1024 * 512> mPedestalSet;

public:
	void getThresholdInfo();
	void getPedestalInfo();

	void getCorrelation();
};

#endif