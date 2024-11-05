#ifndef __TDATAANALYSER__
#define __TDATAANALYSER__

// Basic header
#include<fstream>

// ROOT header
#include "TFile.h"
#include "TTree.h"

// User header
#include "TALPIDEEvent.h"
#include "TCluster.h"
#include "TClusterization.h"
#include "CppConfigFile.h"
#include "cpptqdm.h"


class TDataAnalyser {
public:
	TDataAnalyser(CppConfigFile* config);
	~TDataAnalyser();
private:
	CppConfigFile* mConfig;

	TFile* mInputFile;
	TFile* mOutputFile;

	std::vector<TALPIDEEvent*> mEventSet;
	std::vector<TCluster*> mClusterSet;

	std::array<std::array<bool, 512>, 1024> mMaskingMap;
public:
	void extractEvent();
	void excludeHotPixel();
	void extractHotPixel();
	void extractCluster();
	void saveEvent();
	void saveCluster();
};

#endif