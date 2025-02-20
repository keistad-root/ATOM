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
#include "TGaxis.h"
#include "TFrame.h"


const int CANVAS_WIDTH = 2000;
const int CANVAS_HEIGHT = 1000;

const std::string EXTENSION = "png";

class TPlotter {
public:
	TPlotter() = default;
	~TPlotter() = default;
public:
	static void setAttribute(TH1* plot, const CppConfigDictionary& config);
	static void setLineColour(TH1* plot, const CppConfigDictionary& config);
	static void setLineWidth(TH1* plot, const CppConfigDictionary& config);
	static void setLineStyle(TH1* plot, const CppConfigDictionary& config);

	static void setAttribute(TH2* plot, const CppConfigDictionary& config);

	static void setAttribute(TGraph* graph, const CppConfigDictionary& config);
	static void setLineStyle(TGraph* plot, const CppConfigDictionary& config);
	static void setLineColour(TGraph* plot, const CppConfigDictionary& config);
	static void setLineWidth(TGraph* plot, const CppConfigDictionary& config);
	static void setMarkerStyle(TGraph* plot, const CppConfigDictionary& config);
	static void setMarkerSize(TGraph* plot, const CppConfigDictionary& config);
	static void setMarkerColour(TGraph* plot, const CppConfigDictionary& config);

	static void drawPlot(TCanvas* canvas, TH1* plot, TString drawType);
	static void drawPlot(TCanvas* canvas, TH2* plot, TString drawType);
	static void drawPlot(TCanvas* canvas, TGraph* plot, TString drawType);
	static void drawPlot(TCanvas* canvas, TMultiGraph* plot, TString drawType);

	static void setCanvasAttribute(TCanvas* canvas, const CppConfigDictionary& config);
	static void setTitle(TH1* plot, CppConfigDictionary& config);
	static void setRange(TH1* plot, CppConfigDictionary& config);

	void initHist(TH1* hist, const CppConfigDictionary& config);
	void initHist(std::unique_ptr<TH1D>& hist, const CppConfigDictionary& config);
	void initHist(TH2* hist, const CppConfigDictionary& config);
	void initHist(std::unique_ptr<TH2D>& hist, const CppConfigDictionary& config);


	void savePlot(TCanvas* canvas, TH1* plot, const CppConfigDictionary& config);
	void savePlot(TCanvas* canvas, TH2* plot, const CppConfigDictionary& config);
	void savePlot(TCanvas* canvas, TH1* plot, const std::string& configName);
	void savePlot(TCanvas* canvas, TH2* plot, const std::string& configName);
	void savePlot(std::unique_ptr<TCanvas>& canvas, std::unique_ptr<TGraph>& plot, const CppConfigDictionary& config);
	void savePlot(TCanvas* canvas, TMultiGraph* plot, const CppConfigDictionary& config);
	void savePlot(TCanvas* canvas, TGraph* plot, const CppConfigDictionary& config);
	// void savePlot(TH1* plot, const std::string& configName);
	// void savePlot(TH2* plot, const std::string& configName);

	void addLegend(TCanvas* canvas, TLegend*& legend, const CppConfigDictionary& config);

	static const std::vector<int> getIntegerSetFromString(const std::string& rangeStr);
	static const std::vector<double> getDoubleSetFromString(const std::string& rangeStr);
	TString getTitle(const std::string& titleStr);

	// Getter

	// Plot property
	template <typename T> void setXBin(T* hist, const CppConfigDictionary& config);
	template <typename T> void setYBin(T* hist, const CppConfigDictionary& config);
	template <typename T> void setZBin(T* hist, const CppConfigDictionary& config);
	template <typename T> void setBins(T* hist, const CppConfigDictionary& config);

	template <typename T> void setXRange(T* hist, const CppConfigDictionary& config);
	template <typename T> void setYRange(T* hist, const CppConfigDictionary& config);
	template <typename T> void setZRange(T* hist, const CppConfigDictionary& config);

	template <typename T> void setRightAxis(T* hist, const CppConfigDictionary& config);

	template <typename T> void setTitle(T* hist, const CppConfigDictionary& config);

	// Canvas property
	void setMargin(TCanvas* hist, const CppConfigDictionary& config);
	void saveLegend(TCanvas* canvas, TLegend* legend);
	void saveCanvas(TCanvas* canvas, std::filesystem::path path, const CppConfigDictionary& config);
};


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

template <typename T> void TPlotter::setRightAxis(T* hist, const CppConfigDictionary& config) {
	if ( config.hasKey("right_axis") && config.find("right_axis") == "false" ) {
	} else {
		double xmax = hist->GetXaxis()->GetXmax();
		double ymin = hist->GetMinimum();
		double ymax = hist->GetMaximum() * 1.05;
		if ( config.hasKey("y_range") ) {
			std::vector<double> range = getDoubleSetFromString(config.find("y_range"));
			ymin = range[0];
			ymax = range[1];
		} else if ( config.hasKey("logy") && config.find("logy") == "true" ) {
			double minNonZero = std::numeric_limits<double>::max();
			for ( int i = 1; i <= hist->GetNbinsX(); ++i ) {
				double binContent = hist->GetBinContent(i);
				if ( binContent > 0 && binContent < minNonZero ) {
					minNonZero = binContent;
				}
			}
			ymin = minNonZero / 1.5;
			ymax = hist->GetMaximum() * 1.5;
			hist->GetYaxis()->SetRangeUser(ymin, ymax);
		} else {
			ymin = hist->GetMinimum();
			ymax = hist->GetMaximum() * 1.1;
			hist->GetYaxis()->SetRangeUser(ymin, ymax);
		}
		TGaxis* axis;
		if ( config.hasKey("logy") && config.find("logy") == "true" ) {
			axis = new TGaxis(xmax, ymin, xmax, ymax, ymin, ymax, 510, "+G");
		} else {
			axis = new TGaxis(xmax, ymin, xmax, ymax, ymin, ymax, 510, "+L");
		}
		axis->Draw();
	}
}


#endif