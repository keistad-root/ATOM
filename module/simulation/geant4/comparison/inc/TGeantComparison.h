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
#include "TF1.h"
#include "TText.h"

class TGeantComparison : public TPlotter {
public:
	TGeantComparison(const CppConfigFile& config);
	~TGeantComparison();
private:
	CppConfigFile mConfig;
	std::filesystem::path mOutputPath;

	std::unordered_map<std::string, TFile*> mFileMap;
public:
	void getPlotNormalized();
	void getComparedPlot();

	void getDividePlot();

};

#endif