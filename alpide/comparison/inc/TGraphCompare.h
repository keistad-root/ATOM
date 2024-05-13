#ifndef __TGRAPHCOMPARE__
#define __TGRAPHCOMPARE__

#ifdef __TGRAPHCOMPARE_HEADER__
#include <iostream>
#include<sstream>
#include "Rtypes.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TString.h"
#include "TColor.h"
#include "TLegend.h"
#include "CppConfigFile.h"
#endif

#include <unordered_map>
#include <vector>

class TFile;
class TH1D;

class Configurable;

class TGraphCompare {
private:
	std::unordered_map<std::string, TFile*> mGraphFileSet;
	std::vector<TH1D*> mClusterSizeSet;
	std::vector<std::string> mGraphInfoSet;
public:
	TGraphCompare(const std::vector<std::string>& graphFilePath);
	void TCompareClusterSize(std::string_view typeName, const CppConfigDictionary config);
	// void TCompareClusterSizeRatio(std::string_view typeName, const Configurable* config);
	// void TCompareClusterSize3D(std::string_view typeName, const Configurable* config);
	TH1D* getClustersizeHistogram(std::string_view pathInRoot, std::string operation);

};

#endif

// struct TGraphObjects {
// 	std::string name;
// 	std::string operation;
// 	std::string legend;
// 	double weighting;
// 	bool operator < (const TGraphObjects& other) const { return true; }
// 	bool operator==(const TGraphObjects& other) const { return name == other.name; }
// 	TH1D* getClustersizeHistogram(std::string_view pathInRoot, const std::unordered_map<std::string, TFile*>& graphFileSet);
// };