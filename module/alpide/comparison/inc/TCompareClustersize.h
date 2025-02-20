#ifndef __TCOMPARECLUSTERSIZE__
#define __TCOMPARECLUSTERSIZE__

#include<filesystem>
#include<unordered_map>

#include "TH1.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"

#include "TLegend.h"

#include "TPlotter.h"
#include "CppConfigFile.h"

class TClusterInfo {
public:
	TClusterInfo(std::string_view tag, const CppConfigDictionary& config);
	~TClusterInfo();
private:
	std::string mTag;
	CppConfigDictionary mConfig;
	TH1D* mClusterSizeHistogram;
public:
	TH1D* setClusterSizeHistogram(std::string_view name);
	TH1D* getClusterSizeHistogram() { return mClusterSizeHistogram; }
	const CppConfigDictionary& getConfig() const { return mConfig; }
};

class TCompareClustersize : public TPlotter {
public:
	TCompareClustersize(const CppConfigFile& config);
private:
	CppConfigFile mConfig;
	std::filesystem::path mOutputPath;
	std::vector<TClusterInfo> mClusterInfo;
public:
	void drawClustersize();
};

#endif