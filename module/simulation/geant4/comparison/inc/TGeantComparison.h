#ifndef __TGEANTCOMPARISON__
#define __TGEANTCOMPARISON__

#include<filesystem>

#include "TPlotter.h"
#include "CppConfigFile.h"

struct IncidentInfo {
	Int_t length;
	Int_t width;
	Int_t eventID;
	Int_t particleID;
	Double_t x;
	Double_t y;
};

class TGeantComparison : public TPlotter {
public:
	TGeantComparison(const CppConfigFile& config);
	~TGeantComparison();
private:
	CppConfigFile mConfig;
	std::filesystem::path mOutputPath;
	std::vector<IncidentInfo> mIncidentInfo;
public:

};

#endif