#ifndef __TH1IUSER__
#define __TH1IUSER__

#include "TH1I.h"
#include "TCanvas.h"
#include "TAxis.h"

#include "CppConfigFile.h"

#include<vector>
// #include<pair>
#include<filesystem>


class TH1IUser : public TH1I {
private:
	std::vector<std::pair<std::string, std::vector<int>>> dataList;
	std::vector<std::pair<std::string, TH1I*>> histList;
	CppConfigDictionary mConfig;
	TString mFilePath = "";
	TString mTitle = "";

	int iHist = 0;

	TCanvas* can;
public:
	TH1IUser(const CppConfigDictionary& config, const std::vector<std::pair<std::string, std::vector<int>>>& data);
private:
	void getFilePath();
	void getTitle();
	void fillData();
	void drawCanvas();
};


#endif