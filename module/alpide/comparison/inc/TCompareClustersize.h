#ifndef __TCOMPARECLUSTERSIZE__
#define __TCOMPARECLUSTERSIZE__

#include<filesystem>
#include<unordered_map>

#include "TH1.h"
#include "TFile.h"
#include "TTree.h"

#include "TPlotter.h"
#include "CppConfigFile.h"

class TCompareClustersize : public TPlotter {
public:
	TCompareClustersize(const CppConfigFile* config);
private:
	std::unordered_map<std::string, TH1D*> mGraphFileSet;
	std::vector<CppConfigDictionary> mPlotDictionary;
public:
	TH1D* getClustersize(const std::string& file);

	TH1D* subtractClustersize(const std::string& graph1, const std::string& graph2);

	void drawClustersize();
};

#endif