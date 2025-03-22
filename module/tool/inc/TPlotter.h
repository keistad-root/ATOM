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

#include<filesystem>

#include "TString.h"

#include "CppConfigDictionary.h"

class TH1;
class TH1D;
class TH2;
class TH2D;
class TGraph;
class TMultiGraph;
class TCanvas;
class TLegend;
class TGraphErrors;
class TF1;

class TPlotter {
public:
	static TCanvas* initCanvas(const CppConfigDictionary& config);
	static TLegend* initLegend(const CppConfigDictionary& config);
	static TH1D* init1DHist(const CppConfigDictionary& config);
	static TH2D* init2DHist(const CppConfigDictionary& config);
	static TF1* initFunction(const CppConfigDictionary& config);
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

	static void setAttribute(TGraphErrors* graph, const CppConfigDictionary& config);
	static void setLineStyle(TGraphErrors* plot, const CppConfigDictionary& config);
	static void setLineColour(TGraphErrors* plot, const CppConfigDictionary& config);
	static void setLineWidth(TGraphErrors* plot, const CppConfigDictionary& config);
	static void setMarkerStyle(TGraphErrors* plot, const CppConfigDictionary& config);
	static void setMarkerSize(TGraphErrors* plot, const CppConfigDictionary& config);
	static void setMarkerColour(TGraphErrors* plot, const CppConfigDictionary& config);

	static void drawPlot(TCanvas* canvas, TH1* plot, const CppConfigDictionary& config, TString drawType);
	static void drawPlot(TCanvas* canvas, TH2* plot, const CppConfigDictionary& config, TString drawType);
	static void drawPlot(TCanvas* canvas, TGraph* plot, const CppConfigDictionary& config, TString drawType);
	static void drawPlot(TCanvas* canvas, TGraphErrors* plot, const CppConfigDictionary& config, TString drawType);
	static void drawPlot(TCanvas* canvas, TMultiGraph* plot, const CppConfigDictionary& config, TString drawType);

	static void setCanvasAttribute(TCanvas* canvas, const CppConfigDictionary& config);
	static void setTitle(TH1* plot, const CppConfigDictionary& config);
	static void setRange(TH1* plot, const CppConfigDictionary& config);
	static void setTitle(TH2* plot, const CppConfigDictionary& config);
	static void setRange(TH2* plot, const CppConfigDictionary& config);
	static void setTitle(TGraph* plot, const CppConfigDictionary& config);
	static void setRange(TGraph* plot, const CppConfigDictionary& config);
	static void setTitle(TMultiGraph* plot, const CppConfigDictionary& config);
	static void setRange(TMultiGraph* plot, const CppConfigDictionary& config);
	static void setMargin(TCanvas* canvas, const CppConfigDictionary& config);
	static void setLogScale(TCanvas* canvas, const CppConfigDictionary& config);
	static void setGrid(TCanvas* canvas, const CppConfigDictionary& config);
	static void saveFitLegend(TLegend* legend, TF1* fitFunc);

	static void saveLegend(TCanvas* canvas, TLegend* legend);
	static void saveCanvas(TCanvas* canvas, std::filesystem::path path, const CppConfigDictionary& config);

	static TString getTitle(std::string_view titleStr);

	static const std::vector<int> getIntegerSetFromString(const std::string& rangeStr);
	static const std::vector<double> getDoubleSetFromString(const std::string& rangeStr);

	static void setRightAxis(TH1* plot, const CppConfigDictionary& config);
	static void setRightAxis(TGraph* plot, const CppConfigDictionary& config);

	// Canvas property
};


#endif