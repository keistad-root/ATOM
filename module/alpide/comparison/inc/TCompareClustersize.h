#ifndef __TCOMPARECLUSTERSIZE__
#define __TCOMPARECLUSTERSIZE__

#include<string>
#include<vector>
#include "CppConfigFile.h"

class TH1D;

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

	bool isInsideRegion(const std::vector<double>& roi, double roiTheta, double x, double y);
};

class TCompareClustersize {
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