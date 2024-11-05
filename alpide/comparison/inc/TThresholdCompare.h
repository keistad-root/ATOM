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
#include<array>
#include<memory>
#include<filesystem>

#include "TGraphErrors.h"
#include "TDatime.h"

class TThreshold;
class TFile;
class TH1D;

#include "CppConfigFile.h"

class TThresholdCompare {
private:
	std::vector<std::tuple<CppConfigDictionary, TFile*, TH1D*, TH1D*, TH1D*>> mThresholdSet;
	CppConfigDictionary mDataConfig;
	CppConfigDictionary mThresholdConfig;
	CppConfigDictionary mErrorConfig;
	CppConfigDictionary mQualityConfig;
	CppConfigDictionary mThresholdValConfig;
	CppConfigDictionary mNoiseValConfig;
	CppConfigDictionary mQualityValConfig;
	CppConfigDictionary mEntryConfig;
	CppConfigDictionary mStdevConfig;

public:
	TThresholdCompare(CppConfigFile& config);

	void getFile();

	void mergeThreshold();
	void mergeError();
	void mergeQuality();
	void compareThresholdValue();
	void compareNoiseValue();
	void compareQualityValue();
	void compareEntry();
	void compareStdDev();
};

#endif