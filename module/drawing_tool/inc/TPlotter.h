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
#include "TLegend.h"
#include "CppConfigFile.h"
#include "TColourUser.h"
#include "TStyle.h"
#include "TMultiGraph.h"
#include "TGraph.h"


const int CANVAS_WIDTH = 2000;
const int CANVAS_HEIGHT = 1000;

const std::string EXTENSION = "png";

class TPlotter {
public:
	TPlotter() = default;
	TPlotter(const CppConfigFile* config);
private:
	const CppConfigFile* mConfig;
	std::filesystem::path mOutputPath;

public:
	template <typename T> void setAttribute(T* plot, const CppConfigDictionary& config);

	template <typename T> void setLineStyle(T* plot, const CppConfigDictionary& config);
	template <typename T> void setLineColour(T* plot, const CppConfigDictionary& config);
	template <typename T> void setLineWidth(T* plot, const CppConfigDictionary& config);
	template <typename T> void setMarkerStyle(T* plot, const CppConfigDictionary& config);
	template <typename T> void setMarkerColour(T* plot, const CppConfigDictionary& config);
	template <typename T> void setMarkerSize(T* plot, const CppConfigDictionary& config);

	template <typename T> void drawPlot(TCanvas* canvas, T* plot, TString drawType = "SAME");
	void setCanvasAttribute(TCanvas* canvas, const CppConfigDictionary& config);
	void setCanvasAttribute(std::unique_ptr<TCanvas>& canvas, const CppConfigDictionary& config);

	void initHist(TH1* hist, const CppConfigDictionary& config);
	void initHist(std::unique_ptr<TH1D>& hist, const CppConfigDictionary& config);
	void initHist(TH2* hist, const CppConfigDictionary& config);
	void initHist(std::unique_ptr<TH2D>& hist, const CppConfigDictionary& config);


	void savePlot(TCanvas* canvas, TH1* plot, const CppConfigDictionary& config);
	void savePlot(TCanvas* canvas, TH2* plot, const CppConfigDictionary& config);
	void savePlot(TCanvas* canvas, TH1* plot, const std::string& configName);
	void savePlot(TCanvas* canvas, TH2* plot, const std::string& configName);
	void savePlot(TCanvas* canvas, TMultiGraph* plot, const CppConfigDictionary& config);
	// void savePlot(TH1* plot, const std::string& configName);
	// void savePlot(TH2* plot, const std::string& configName);

	void addLegend(TCanvas* canvas, TLegend*& legend, const CppConfigDictionary& config);

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




	template <typename T> void setTitle(T* hist, const CppConfigDictionary& config);

	// Canvas property
	void setMargin(TCanvas* hist, const CppConfigDictionary& config);
	void saveLegend(TCanvas* canvas, TLegend* legend);
	void saveCanvas(TCanvas* canvas, std::filesystem::path path, const CppConfigDictionary& config);
};

template <typename T> void TPlotter::setAttribute(T* plot, const CppConfigDictionary& config) {
	setLineStyle(plot, config);
	setLineColour(plot, config);
	setLineWidth(plot, config);
	setMarkerStyle(plot, config);
	setMarkerColour(plot, config);
	setMarkerSize(plot, config);
}

template <typename T> void TPlotter::setLineStyle(T* hist, const CppConfigDictionary& config) {
	if ( config.hasKey("line_style") ) {
		hist->SetLineStyle(stoi(config.find("line_style")));
	}
}

template <typename T> void TPlotter::setLineColour(T* hist, const CppConfigDictionary& config) {
	if ( config.hasKey("line_colour") ) {
		hist->SetLineColor(TColourUser::getColour(config.find("line_colour")));
	}
}

template <typename T> void TPlotter::setLineWidth(T* hist, const CppConfigDictionary& config) {
	if ( config.hasKey("line_width") ) {
		hist->SetLineWidth(stoi(config.find("line_width")));
	}
}

template <typename T> void TPlotter::setMarkerStyle(T* hist, const CppConfigDictionary& config) {
	if ( config.hasKey("marker_style") ) {
		hist->SetMarkerStyle(stoi(config.find("marker_style")));
	}
}

template <typename T> void TPlotter::setMarkerColour(T* hist, const CppConfigDictionary& config) {
	if ( config.hasKey("marker_colour") ) {
		hist->SetMarkerColor(TColourUser::getColour(config.find("marker_colour")));
	}
}

template <typename T> void TPlotter::setMarkerSize(T* hist, const CppConfigDictionary& config) {
	if ( config.hasKey("marker_size") ) {
		hist->SetMarkerSize(stoi(config.find("marker_size")));
	}
}

template <typename T> void TPlotter::drawPlot(TCanvas* canvas, T* plot, TString drawType) {
	canvas->cd();
	plot->Draw(drawType);
}


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
void TPlotter::setTitle(T* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("title") ) {
		TString title = getTitle(config.find("title"));
		plot->SetTitle(title);
	}
	if ( config.hasKey("title_offset") ) {
		std::vector<double> offset = getDoubleSetFromString(config.find("title_offset"));
		plot->GetXaxis()->SetTitleOffset(offset[0]);
		plot->GetYaxis()->SetTitleOffset(offset[1]);
	}
}

#endif