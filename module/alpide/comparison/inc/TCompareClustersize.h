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

class TCompareClustersize : public TPlotter {
public:
	TCompareClustersize(const CppConfigFile& config);
private:
	CppConfigFile mConfig;
	std::filesystem::path mOutputPath;
	std::unordered_map<std::string, TH1D*> mGraphFileSet;
	std::vector<CppConfigDictionary> mPlotDictionary;
	std::vector<std::pair<CppConfigDictionary, TH1D*>> mGraphSet;
	CppConfigDictionary mRegionDictionary;
public:
	TH1D* getClustersize(const std::string& file);

	TH1D* subtractClustersize(const std::string& graph1, const std::string& graph2);

	void setGraphSet();
	void drawClustersize();
	void drawRegion();
};

#endif