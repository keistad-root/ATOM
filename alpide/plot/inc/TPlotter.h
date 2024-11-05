/**
 * @file TPlotter.h
 * @author Yongjun Choi (yochoi@cern.ch)
 * @brief Basic tools for plot
 * @version 0.1
 * @date 27-09-2024
 *
 * @copyright Copyright (c) 2024
 *
*/

#ifndef __TPLOTTER__
#define __TPLOTTER__

#include<array>
#include<sstream>
#include<filesystem>
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "CppConfigFile.h"

const int CANVAS_WIDTH = 2000;
const int CANVAS_HEIGHT = 1000;

const std::string EXTENSION = "png";

class TPlotter {
public:
	TPlotter(const CppConfigFile* config);
private:
	const CppConfigFile* mConfig;
	std::filesystem::path mOutputPath;

public:
	void savePlot(TH1* plot, const std::string& configName);
	void savePlot(TH2* plot, const std::string& configName);

	const std::vector<int> getIntegerSetFromString(const std::string& rangeStr);
	const std::vector<double> getDoubleSetFromString(const std::string& rangeStr);

	void setXBin(TH1* hist, const CppConfigDictionary& config);
	void setYBin(TH1* hist, const CppConfigDictionary& config);
	void setXBin(TH2* hist, const CppConfigDictionary& config);
	void setYBin(TH2* hist, const CppConfigDictionary& config);
	void setZBin(TH2* hist, const CppConfigDictionary& config);

	void setBins(TH1* hist, const CppConfigDictionary& config);
	void setBins(TH2* hist, const CppConfigDictionary& config);

	void setXRange(TH1* hist, const CppConfigDictionary& config);
	void setXRange(TH2* hist, const CppConfigDictionary& config);
	void setYRange(TH1* hist, const CppConfigDictionary& config);
	void setYRange(TH2* hist, const CppConfigDictionary& config);
	void setZRange(TH2* hist, const CppConfigDictionary& config);

	TString getTitle(const std::string& titleStr);
	void setTitle(TH1* hist, const CppConfigDictionary& config);
	void setTitle(TH2* hist, const CppConfigDictionary& config);

	void setMargin(TCanvas* hist, const CppConfigDictionary& config);

	void saveCanvas(TCanvas* canvas, std::filesystem::path path, const CppConfigDictionary& config);
};

#endif