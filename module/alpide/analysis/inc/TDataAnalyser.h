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
	TDataAnalyser(const CppConfigFile& config);
	~TDataAnalyser();
private:
	CppConfigFile mConfig;
	std::unique_ptr<TFile> mInputFile;
	std::unique_ptr<TFile> mOutputFile;

	std::vector<TALPIDEEvent*> mEventSet;
	std::vector<TCluster*> mClusterSet;
	std::vector<TClusterShape*> mShapeSet;

	std::array<std::array<bool, 512>, 1024> mMaskingMap;
public:
	void openInputFile();
	void openOutputFile();

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