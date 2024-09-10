#ifndef __TTHRESHOLDCOMPARE__
#define __TTHRESHOLDCOMPARE__

#ifdef __TTHRESHOLDCOMPARE_HEADER__
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TColourUser.h"
#include "TText.h"
#include "TLegend.h"
#endif

#include<vector>
#include<memory>
#include<filesystem>

class TThreshold;
class TFile;
class TH1D;

#include "CppConfigFile.h"

class TThresholdCompare {
private:
	std::vector<std::tuple<CppConfigDictionary, TFile*, TH1D*, TH1D*>> mThresholdSet;
	CppConfigDictionary mDataConfig;
	CppConfigDictionary mThresholdConfig;
	CppConfigDictionary mErrorConfig;
public:
	TThresholdCompare(const CppConfigFile& config);

	void getFile();

	void mergeThreshold();
	void mergeError();

};

#endif