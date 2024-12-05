#ifndef __TDATAANALYSER__
#define __TDATAANALYSER__

// Basic header
#include<fstream>
#include<unordered_map>
#include<map>
// ROOT header
#include "TFile.h"
#include "TTree.h"
#include "TH2.h"

// User header
#include "TALPIDEEvent.h"
#include "TCluster.h"
#include "TClusterization.h"
#include "TClusterShape.h"
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
	std::vector<TClusterShape*> mShapeSet;

	std::array<std::array<bool, 512>, 1024> mMaskingMap;
public:
	void extractEvent();
	void excludeHotPixel();
	void extractHotPixel();
	void extractCluster();
	void extractShape();
	void saveEvent();
	void saveCluster();
	void saveShape();
};

#endif