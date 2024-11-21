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
#include<typeinfo>
#include<iostream>
#include<filesystem>

#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "CppConfigFile.h"
#include "TColourUser.h"
#include "TStyle.h"

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
	void savePlot(TCanvas* canvas, TH1* plot, const CppConfigDictionary& config);
	void savePlot(TCanvas* canvas, TH1* plot, const std::string& configName);
	void savePlot(TCanvas* canvas, TH2* plot, const std::string& configName);
	void savePlot(TH1* plot, const std::string& configName);
	void savePlot(TH2* plot, const std::string& configName);

	static const std::vector<int> getIntegerSetFromString(const std::string& rangeStr);
	static const std::vector<double> getDoubleSetFromString(const std::string& rangeStr);
	TString getTitle(const std::string& titleStr);

	// Getter
	std::filesystem::path getOutputPath() { return mOutputPath; }
	const CppConfigFile* getMainConfig() { return mConfig; }

	// Plot property
	template <typename T> void setXBin(T* hist, const CppConfigDictionary& config);
	template <typename T> void setYBin(T* hist, const CppConfigDictionary& config);
	template <typename T> void setZBin(T* hist, const CppConfigDictionary& config);
	template <typename T> void setBins(T* hist, const CppConfigDictionary& config);

	template <typename T> void setXRange(T* hist, const CppConfigDictionary& config);
	template <typename T> void setYRange(T* hist, const CppConfigDictionary& config);
	template <typename T> void setZRange(T* hist, const CppConfigDictionary& config);

	template <typename T> void setLineColour(T* hist, const CppConfigDictionary& config);

	template <typename T> void setTitle(T* hist, const CppConfigDictionary& config);

	// Canvas property
	void setMargin(TCanvas* hist, const CppConfigDictionary& config);
	void saveCanvas(TCanvas* canvas, std::filesystem::path path, const CppConfigDictionary& config);
};

template <typename T>
void TPlotter::setXBin(T* hist, const CppConfigDictionary& config) {
	std::string typeName = std::string(typeid(T).name());
	if ( config.hasKey("x_bins") ) {
		std::vector<double> set = getDoubleSetFromString(config.find("x_bins"));
		if ( typeName.find("TH1") != std::string::npos ) {
			hist->SetBins(static_cast<int>(set[2]), set[0], set[1]);
		} else if ( typeName.find("TH2") != std::string::npos ) {
			hist->SetBins(static_cast<int>(set[2]), set[0], set[1], hist->GetNbinsY(), hist->GetYaxis()->GetXmin(), hist->GetYaxis()->GetXmax());
		}
	}
}

template <typename T>
void TPlotter::setYBin(T* hist, const CppConfigDictionary& config) {
	if ( config.hasKey("y_bins") ) {
		std::vector<double> set = getDoubleSetFromString(config.find("y_bins"));
		hist->SetBins(static_cast<int>(set[2]), set[0], set[1], hist->GetNbinsY(), hist->GetYaxis()->GetXmin(), hist->GetYaxis()->GetXmax());
	}
}

template <typename T>
void TPlotter::setZBin(T* hist, const CppConfigDictionary& config) {
	if ( config.hasKey("z_bins") ) {
		std::vector<double> set = getDoubleSetFromString(config.find("z_bins"));
		hist->SetBins(hist->GetNbinsX(), hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax(), hist->GetNbinsY(), hist->GetYaxis()->GetXmin(), hist->GetYaxis()->GetXmax(), static_cast<int>(set[2]), set[0], set[1]);
	}
}


template <typename T>
void TPlotter::setBins(T* hist, const CppConfigDictionary& config) {
	std::string typeName = std::string(typeid(T).name());
	setXBin(hist, config);
	setYBin(hist, config);
	if ( typeName.find("TH2") != std::string::npos ) {
		setZBin(hist, config);
	}
}

template <typename T>
void TPlotter::setXRange(T* hist, const CppConfigDictionary& config) {
	if ( config.hasKey("x_range") ) {
		std::vector<double> range = getDoubleSetFromString(config.find("x_range"));
		hist->GetXaxis()->SetRangeUser(range[0], range[1]);
	}
}

template <typename T>
void TPlotter::setYRange(T* hist, const CppConfigDictionary& config) {
	if ( config.hasKey("y_range") ) {
		std::vector<double> range = getDoubleSetFromString(config.find("y_range"));
		hist->GetYaxis()->SetRangeUser(range[0], range[1]);
	}
}

template <typename T>
void TPlotter::setZRange(T* hist, const CppConfigDictionary& config) {
	if ( config.hasKey("z_range") ) {
		std::vector<double> range = getDoubleSetFromString(config.find("z_range"));
		hist->GetZaxis()->SetRangeUser(range[0], range[1]);
	}
}

template <typename T>
void TPlotter::setTitle(T* hist, const CppConfigDictionary& config) {
	if ( config.hasKey("title") ) {
		TString title = getTitle(config.find("title"));
		hist->SetTitle(title);
	}
}

template <typename T> void TPlotter::setLineColour(T* hist, const CppConfigDictionary& config) {
	if ( config.hasKey("line_colour") ) {
		hist->SetLineColor(getColour(config.find("line_colour")));
	}
}


#endif