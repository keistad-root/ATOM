#include<array>
#include<sstream>
#include<typeinfo>
#include<iostream>

#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TColourUser.h"
#include "TStyle.h"
#include "TMultiGraph.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGaxis.h"
#include "TFrame.h"
#include "TF1.h"

#include "TColourUser.h"
#include "TPlotter.h"

#include<ctime>

/**
 * @brief Initializing canvas
 * @details Initialize canvas with informations given by config.
 * Canvas size is set to 1000x1000 by default.
 * The canvas size in config file can be set by "CANVAS_SIZE" key.
 * @param canvas
 * @param config
*/
TCanvas* TPlotter::initCanvas(const CppConfigDictionary& config) {
	std::vector<double> size = {1000, 1000};
	static int iCanvas = 0;
	if ( config.hasKey("CANVAS_SIZE") ) {
		size = getDoubleSetFromString(config.find("CANVAS_SIZE"));
	}
	TCanvas* canvas = new TCanvas(Form("canvas_%d", iCanvas), "", size[0], size[1]);
	iCanvas++;

	return canvas;
}

TH1D* TPlotter::init1DHist(const CppConfigDictionary& config) {
	static int i1DHist = 0;
	std::vector<double> bin = {1, 0, 1};
	if ( config.hasKey("BIN") ) {
		bin = getDoubleSetFromString(config.find("BIN"));
	}
	TH1D* hist = new TH1D(Form("hist1d_%d", i1DHist), "", static_cast<int>(bin[0]), bin[1], bin[2]);
	i1DHist++;
	return hist;
}

TH2D* TPlotter::init2DHist(const CppConfigDictionary& config) {
	static int i2DHist = 0;
	std::vector<double> bin = {1, 0, 1, 1, 0, 1};
	if ( config.hasKey("BIN") ) {
		bin = getDoubleSetFromString(config.find("BIN"));
	}
	TH2D* hist = new TH2D(Form("hist2d_%d", i2DHist), "", static_cast<int>(bin[0]), bin[1], bin[2], static_cast<int>(bin[3]), bin[4], bin[5]);
	i2DHist++;
	return hist;
}

TF1* TPlotter::initFunction(const CppConfigDictionary& config) {
	static int iFunction = 0;
	std::vector<double> range = {0, 1};
	if ( config.hasKey("FIT_RANGE") ) {
		range = getDoubleSetFromString(config.find("FIT_RANGE"));
	}
	TString formula = config.hasKey("FIT_FUNC") ? config.find("FIT_FUNC").substr(1, config.find("FIT_FUNC").size() - 2) : "x";
	TF1* func = new TF1(Form("func_%d", iFunction), formula, range[0], range[1]);
	if ( config.hasKey("FIT_PAR") ) {
		std::vector<double> par = getDoubleSetFromString(config.find("FIT_PAR"));
		for ( int i = 0; i < par.size(); i++ ) {
			func->SetParameter(i, par[i]);
		}
	}
	iFunction++;
	return func;
}

// Set TH1 attributes
void TPlotter::setAttribute(TH1* plot, const CppConfigDictionary& config) {
	setLineColour(plot, config);
	setLineWidth(plot, config);
	setLineStyle(plot, config);
}

void TPlotter::setLineColour(TH1* plot, const CppConfigDictionary& config) {
	Color_t lineColour;
	if ( config.hasKey("LINE_COLOUR") ) {
		lineColour = TColourUser::getColour(config.find("LINE_COLOUR"));
		plot->SetLineColor(lineColour);
	} else {
		lineColour = TColourUser::getColour("blue");
		plot->SetLineColor(lineColour);
	}
}

void TPlotter::setLineWidth(TH1* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("LINE_WIDTH") ) {
		Width_t lineWidth = stod(config.find("LINE_WIDTH"));
		plot->SetLineWidth(lineWidth);
	} else {
		plot->SetLineWidth(2);
	}
}

void TPlotter::setLineStyle(TH1* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("LINE_STYLE") ) {
		Style_t lineStyle = static_cast<short>(stoi(config.find("LINE_STYLE")));
		plot->SetLineStyle(lineStyle);
	}
}

// Set TH2 attributes
void TPlotter::setAttribute(TH2* plot, const CppConfigDictionary& config) { }

void TPlotter::setAttribute(TGraph* graph, const CppConfigDictionary& config) {
	setLineColour(graph, config);
	setLineWidth(graph, config);
	setLineStyle(graph, config);
	setMarkerStyle(graph, config);
	setMarkerSize(graph, config);
	setMarkerColour(graph, config);
}

void TPlotter::setLineStyle(TGraph* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("LINE_STYLE") ) {
		Style_t lineStyle = stoi(config.find("LINE_STYLE"));
		plot->SetLineStyle(lineStyle);
	}
}

void TPlotter::setLineColour(TGraph* plot, const CppConfigDictionary& config) {
	Color_t lineColour;
	if ( config.hasKey("LINE_COLOUR") ) {
		lineColour = TColourUser::getColour(config.find("LINE_COLOUR"));
	} else {
		lineColour = TColourUser::getColour("blue");
	}
	plot->SetLineColor(lineColour);
}

void TPlotter::setLineWidth(TGraph* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("LINE_WIDTH") ) {
		Width_t lineWidth = stod(config.find("LINE_WIDTH"));
		plot->SetLineWidth(lineWidth);
	} else {
		plot->SetLineWidth(2);
	}
}

void TPlotter::setMarkerStyle(TGraph* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("MARKER_STYLE") ) {
		Style_t markerStyle = static_cast<short>(stoi(config.find("MARKER_STYLE")));
		plot->SetMarkerStyle(markerStyle);
	}
}

void TPlotter::setMarkerSize(TGraph* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("MARKER_SIZE") ) {
		Size_t markerSize = stof(config.find("MARKER_SIZE"));
		plot->SetMarkerSize(markerSize);
	}
}

void TPlotter::setMarkerColour(TGraph* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("MARKER_COLOUR") ) {
		Color_t markerColour = TColourUser::getColour(config.find("MARKER_COLOUR"));
		plot->SetMarkerColor(markerColour);
	}
}


void TPlotter::setAttribute(TGraphErrors* graph, const CppConfigDictionary& config) {
	setLineColour(graph, config);
	setLineWidth(graph, config);
	setLineStyle(graph, config);
	setMarkerStyle(graph, config);
	setMarkerSize(graph, config);
	setMarkerColour(graph, config);
}

void TPlotter::setLineStyle(TGraphErrors* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("LINE_STYLE") ) {
		Style_t lineStyle = stoi(config.find("LINE_STYLE"));
		plot->SetLineStyle(lineStyle);
	}
}

void TPlotter::setLineColour(TGraphErrors* plot, const CppConfigDictionary& config) {
	Color_t lineColour;
	if ( config.hasKey("LINE_COLOUR") ) {
		lineColour = TColourUser::getColour(config.find("LINE_COLOUR"));
	} else {
		lineColour = TColourUser::getColour("blue");
	}
	plot->SetLineColor(lineColour);
}

void TPlotter::setLineWidth(TGraphErrors* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("LINE_WIDTH") ) {
		Width_t lineWidth = stod(config.find("LINE_WIDTH"));
		plot->SetLineWidth(lineWidth);
	} else {
		plot->SetLineWidth(2);
	}
}

void TPlotter::setMarkerStyle(TGraphErrors* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("MARKER_STYLE") ) {
		Style_t markerStyle = static_cast<short>(stoi(config.find("MARKER_STYLE")));
		plot->SetMarkerStyle(markerStyle);
	}
}

void TPlotter::setMarkerSize(TGraphErrors* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("MARKER_SIZE") ) {
		Size_t markerSize = stof(config.find("MARKER_SIZE"));
		plot->SetMarkerSize(markerSize);
	}
}

void TPlotter::setMarkerColour(TGraphErrors* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("MARKER_COLOUR") ) {
		Color_t markerColour = TColourUser::getColour(config.find("MARKER_COLOUR"));
		plot->SetMarkerColor(markerColour);
	}
}

void TPlotter::drawPlot(TCanvas* canvas, TH1* plot, const CppConfigDictionary& config) {
	canvas->cd();
	setAttribute(plot, config);
	TString drawOption = config.hasKey("DRAW_OPTION") ? config.find("DRAW_OPTION") : "HISTE";
	plot->Draw(drawOption);
}

void TPlotter::drawPlot(TCanvas* canvas, TH2* plot, const CppConfigDictionary& config) {
	canvas->cd();
	setAttribute(plot, config);
	TString drawOption = config.hasKey("DRAW_OPTION") ? config.find("DRAW_OPTION") : "COLZ0";
	plot->Draw(drawOption);
}

void TPlotter::drawPlot(TCanvas* canvas, TGraph* plot, const CppConfigDictionary& config) {
	canvas->cd();
	setAttribute(plot, config);
	TString drawOption = config.hasKey("DRAW_OPTION") ? config.find("DRAW_OPTION") : "APL";
	plot->Draw(drawOption);
}

void TPlotter::drawPlot(TCanvas* canvas, TGraphErrors* plot, const CppConfigDictionary& config) {
	canvas->cd();
	setAttribute(plot, config);
	TString drawOption = config.hasKey("DRAW_OPTION") ? config.find("DRAW_OPTION") : "APLE";
	plot->Draw(drawOption);
}

void TPlotter::drawPlot(TCanvas* canvas, TMultiGraph* plot, const CppConfigDictionary& config) {
	canvas->cd();
	TString drawOption = config.hasKey("DRAW_OPTION") ? config.find("DRAW_OPTION") : "APLE";
	plot->Draw(drawOption);
}

void TPlotter::setCanvasAttribute(TCanvas* canvas, const CppConfigDictionary& config) {
	TObject* firstObject = canvas->GetListOfPrimitives()->At(0);
	if ( firstObject->InheritsFrom("TFrame") ) {
		firstObject = canvas->GetListOfPrimitives()->At(1);
	}
	if ( firstObject->InheritsFrom("TH1") && !firstObject->InheritsFrom("TH2") ) {
		TH1* hist = static_cast<TH1*>(firstObject);
		setTitle(hist, config);
		setRange(hist, config);
		setRightAxis(hist, config);
	} else if ( firstObject->InheritsFrom("TH2") ) {
		TH2* hist = static_cast<TH2*>(firstObject);
		setTitle(hist, config);
		setRange(hist, config);
	} else if ( firstObject->InheritsFrom("TGraph") ) {
		TGraph* graph = static_cast<TGraph*>(firstObject);
		setTitle(graph, config);
		setRange(graph, config);
		setRightAxis(graph, config);
	} else if ( firstObject->InheritsFrom("TMultiGraph") ) {
		TMultiGraph* multiGraph = static_cast<TMultiGraph*>(firstObject);
		setTitle(multiGraph, config);
		setRange(multiGraph, config);
	}
	setMargin(canvas, config);
	setLogScale(canvas, config);
	setGrid(canvas, config);
}

void TPlotter::setTitle(TH1* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("TITLE") ) {
		TString title = getTitle(config.find("TITLE"));
		plot->SetTitle(title);
	}
}

void TPlotter::setRange(TH1* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("X_RANGE") ) {
		std::vector<double> range = getDoubleSetFromString(config.find("X_RANGE"));
		plot->GetXaxis()->SetRangeUser(range[0], range[1]);
	}
	if ( config.hasKey("Y_RANGE") ) {
		std::vector<double> range = getDoubleSetFromString(config.find("Y_RANGE"));
		plot->GetYaxis()->SetRangeUser(range[0], range[1]);
	}
}

void TPlotter::setTitle(TH2* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("TITLE") ) {
		TString title = getTitle(config.find("TITLE"));
		plot->SetTitle(title);
	}
}

void TPlotter::setRange(TH2* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("X_RANGE") ) {
		std::vector<double> range = getDoubleSetFromString(config.find("X_RANGE"));
		plot->GetXaxis()->SetRangeUser(range[0], range[1]);
	}
	if ( config.hasKey("Y_RANGE") ) {
		std::vector<double> range = getDoubleSetFromString(config.find("Y_RANGE"));
		plot->GetYaxis()->SetRangeUser(range[0], range[1]);
	}
	if ( config.hasKey("Z_RANGE") ) {
		std::vector<double> range = getDoubleSetFromString(config.find("Z_RANGE"));
		plot->GetZaxis()->SetRangeUser(range[0], range[1]);
	}
}

void TPlotter::setTitle(TGraph* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("TITLE") ) {
		TString title = getTitle(config.find("TITLE"));
		plot->SetTitle(title);
	}
}

void TPlotter::setRange(TGraph* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("X_RANGE") ) {
		std::vector<double> range = getDoubleSetFromString(config.find("X_RANGE"));
		plot->GetXaxis()->SetRangeUser(range[0], range[1]);
	}
	if ( config.hasKey("Y_RANGE") ) {
		std::vector<double> range = getDoubleSetFromString(config.find("Y_RANGE"));
		plot->GetYaxis()->SetRangeUser(range[0], range[1]);
	}
}

void TPlotter::setTitle(TMultiGraph* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("TITLE") ) {
		TString title = getTitle(config.find("TITLE"));
		plot->SetTitle(title);
	}
}

void TPlotter::setRange(TMultiGraph* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("X_RANGE") ) {
		std::vector<double> range = getDoubleSetFromString(config.find("X_RANGE"));
		plot->GetXaxis()->SetRangeUser(range[0], range[1]);
	}
	if ( config.hasKey("Y_RANGE") ) {
		std::vector<double> range = getDoubleSetFromString(config.find("Y_RANGE"));
		plot->GetYaxis()->SetRangeUser(range[0], range[1]);
	}
}

void TPlotter::setMargin(TCanvas* canvas, const CppConfigDictionary& config) {
	if ( config.hasKey("MARGIN") ) {
		std::vector<double> marginSet = getDoubleSetFromString(config.find("MARGIN"));
		canvas->SetMargin(marginSet[0], marginSet[1], marginSet[2], marginSet[3]);
	}
}

void TPlotter::setLogScale(TCanvas* canvas, const CppConfigDictionary& config) {
	if ( config.hasKey("LOG_X") && config.find("LOG_X") == "true" ) {
		canvas->SetLogx();
	}
	if ( config.hasKey("LOG_Y") && config.find("LOG_Y") == "true" ) {
		canvas->SetLogy();
	}
	if ( config.hasKey("LOG_Z") && config.find("LOG_Z") == "true" ) {
		canvas->SetLogz();
	}
}

void TPlotter::setGrid(TCanvas* canvas, const CppConfigDictionary& config) {
	if ( config.hasKey("GRID") && config.find("GRID") == "false" ) {

	} else {
		canvas->SetGrid();
	}
}

void TPlotter::saveFitLegend(TLegend* legend, TF1* fitFunc) {
	legend->SetMargin(0.05);
	legend->AddEntry(static_cast<TObject*>(nullptr), fitFunc->GetFormula()->GetExpFormula(), "");
	legend->AddEntry(static_cast<TObject*>(nullptr), Form("#chi^{2}/NDoF: %.2f", fitFunc->GetChisquare() / fitFunc->GetNDF()), "");
	for ( int i = 0; i < fitFunc->GetNpar(); i++ ) {
		legend->AddEntry(static_cast<TObject*>(nullptr), Form("[%d]: %.2f #pm %.2f", i, fitFunc->GetParameter(i), fitFunc->GetParError(i)), "");
	}
}

TString TPlotter::getTitle(std::string_view titleStr) {
	std::string str = std::string(titleStr);
	int firstSplit = str.find("\" \"");
	int secondSplit = str.find("\" \"", titleStr.find("\" \"") + 1);
	std::string mainTitle = str.substr(1, firstSplit - 1);
	std::string xTitle = str.substr(firstSplit + 3, secondSplit - firstSplit - 3);
	std::string yTitle = str.substr(secondSplit + 3, titleStr.size() - secondSplit - 4);
	return mainTitle + ";" + xTitle + ";" + yTitle;
}

TLegend* TPlotter::initLegend(const CppConfigDictionary& config) {
	std::vector<double> position = {.6, .7, .9, .9};
	if ( config.hasKey("LEGEND_POSITION") ) {
		position = getDoubleSetFromString(config.find("LEGEND_POSITION"));
	}
	TLegend* legend = new TLegend(position[0], position[1], position[2], position[3]);
	if ( config.hasKey("LEGEND_TITLE") ) {
		legend->SetHeader(static_cast<TString>(config.find("LEGEND_TITLE")), "C");
	}
	return legend;
}


void TPlotter::saveLegend(TCanvas* canvas, TLegend* legend) {
	canvas->cd();
	legend->Draw("SAME");
}

void TPlotter::saveCanvas(TCanvas* canvas, std::filesystem::path path, const CppConfigDictionary& config) {
	setCanvasAttribute(canvas, config);

	std::string name = config.hasKey("NAME") ? config.find("NAME") : "filename";
	std::string extension = config.hasKey("EXTENSION") ? config.find("EXTENSION") : "png";
	time_t now = time(NULL);
	struct tm* pnow = localtime(&now);

	std::ostringstream date;
	date << std::setw(2) << std::setfill('0') << pnow->tm_year - 100;
	date << std::setw(2) << std::setfill('0') << pnow->tm_mon + 1;
	date << std::setw(2) << std::setfill('0') << pnow->tm_mday;
	date << "_";
	date << std::setw(2) << std::setfill('0') << pnow->tm_hour;
	date << std::setw(2) << std::setfill('0') << pnow->tm_min;
	date << std::setw(2) << std::setfill('0') << pnow->tm_sec;
	name += "_" + date.str();

	std::filesystem::path savePath = (path / name).replace_extension(extension);
	std::filesystem::create_directories(savePath.parent_path());
	canvas->SaveAs(static_cast<TString>(savePath));
}

const std::vector<int> TPlotter::getIntegerSetFromString(const std::string& numStr) {
	std::vector<int> temp;
	std::stringstream str(numStr);
	int token;

	while ( str >> token ) {
		temp.push_back(token);
	}
	return temp;
}

const std::vector<double> TPlotter::getDoubleSetFromString(const std::string& numStr) {
	std::vector<double> temp;
	std::stringstream str(numStr);
	double token;

	while ( str >> token ) {
		temp.push_back(token);
	}
	return temp;
}

void TPlotter::setRightAxis(TH1* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("RIGHT_AXIS") && config.find("RIGHT_AXIS") == "false" ) {
	} else {
		double xmax = plot->GetXaxis()->GetXmax();
		double ymin = plot->GetMinimum();
		double ymax = plot->GetMaximum() * 1.05;
		if ( config.hasKey("Y_RANGE") ) {
			std::vector<double> range = getDoubleSetFromString(config.find("Y_RANGE"));
			ymin = range[0];
			ymax = range[1];
		} else if ( config.hasKey("LOG_Y") && config.find("LOG_Y") == "true" ) {
			double minNonZero = std::numeric_limits<double>::max();
			for ( int i = 1; i <= plot->GetNbinsX(); ++i ) {
				double binContent = plot->GetBinContent(i);
				if ( binContent > 0 && binContent < minNonZero ) {
					minNonZero = binContent;
				}
			}
			ymin = minNonZero / 1.5;
			ymax = plot->GetMaximum() * 1.5;
			plot->GetYaxis()->SetRangeUser(ymin, ymax);
		} else {
			ymin = plot->GetMinimum();
			ymax = plot->GetMaximum() * 1.1;
			plot->GetYaxis()->SetRangeUser(ymin, ymax);
		}
		TGaxis* axis;
		if ( config.hasKey("LOG_Y") && config.find("LOG_Y") == "true" ) {
			axis = new TGaxis(xmax, ymin, xmax, ymax, ymin, ymax, 510, "+LG");
			axis->SetTextFont(gStyle->GetTextFont());
			axis->SetLabelFont(gStyle->GetLabelFont("X"));
		} else {
			axis = new TGaxis(xmax, ymin, xmax, ymax, ymin, ymax, 510, "+L");
			axis->SetTextFont(gStyle->GetTextFont());
			axis->SetLabelFont(gStyle->GetLabelFont("X"));
		}
		axis->Draw();
	}
}

void TPlotter::setRightAxis(TGraph* plot, const CppConfigDictionary& config) {
	if ( config.hasKey("RIGHT_AXIS") && config.find("RIGHT_AXIS") == "false" ) {
	} else {
		double xmax = plot->GetXaxis()->GetXmax();
		double ymin = plot->GetMinimum();
		double ymax = plot->GetMaximum() * 1.05;
		if ( config.hasKey("Y_RANGE") ) {
			std::vector<double> range = getDoubleSetFromString(config.find("Y_RANGE"));
			ymin = range[0];
			ymax = range[1];
		} else if ( config.hasKey("LOG_Y") && config.find("LOG_Y") == "true" ) {
			*std::min_element(plot->GetY(), plot->GetY() + plot->GetN());
			ymin = *std::min_element(plot->GetY(), plot->GetY() + plot->GetN()) > 0 ? *std::min_element(plot->GetY(), plot->GetY() + plot->GetN()) : .1;
			ymax = *std::max_element(plot->GetY(), plot->GetY() + plot->GetN()) * 1.5;
			plot->SetMinimum(ymin);
			plot->SetMaximum(ymax);
		} else {
			ymin = *std::min_element(plot->GetY(), plot->GetY() + plot->GetN()) * .9;
			ymax = *std::max_element(plot->GetY(), plot->GetY() + plot->GetN()) * 1.1;
			plot->SetMinimum(ymin);
			plot->SetMaximum(ymax);
		}
		TGaxis* axis;
		if ( config.hasKey("LOG_Y") && config.find("LOG_Y") == "true" ) {
			axis = new TGaxis(xmax, ymin, xmax, ymax, ymin, ymax, 510, "+LG");
			axis->SetTextFont(gStyle->GetTextFont());
			axis->SetLabelFont(gStyle->GetLabelFont("X"));
		} else {
			axis = new TGaxis(xmax, ymin, xmax, ymax, ymin, ymax, 510, "+L");
			axis->SetTextFont(gStyle->GetTextFont());
			axis->SetLabelFont(gStyle->GetLabelFont("X"));
		}
		axis->Draw();
	}
}