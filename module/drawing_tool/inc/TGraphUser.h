#ifndef __TGRAPHUSER__
#define __TGRAPHUSER__

#include "CppConfigFile.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TMultiGraph.h"
#include<filesystem>

/**
 * @brief The general tools for drawing TGraph Class with config file
 *
*/
class TGraphUser : public TGraph {
private:
	TCanvas* mCanvas; // Canvas for graph
	TLegend* mLegend; // Legend
	TMultiGraph* mMultiGraph; // Multi-graph
private:
	TString savePath; // Save path and file name of graph
	std::string mFileName;

	TString title, x_title, y_title; // Graph title, x-axis and y-axis titles
	std::vector<std::tuple<TGraph*, Style_t, Size_t, Style_t, Size_t>> graphSetAndAttribute; // Graph, marker style, marker size, line style, line size

	std::array<Float_t, 4> canvasMargins; // Left, Right, Bottom, Top
	std::array<Float_t, 4> canvasOffsets;

	Style_t mUniversialMarkerStyle;
	Size_t mUniversialMarkerSize;
	Style_t mUniversialLineStyle;
	Size_t mUniversiallineWidth;

	TString mLegendTitle; // Title of legend
	std::array<Float_t, 4> mLegendPoints; // The cornor points of legend
public:
	TGraphUser(const CppConfigDictionary& config);
	void AddGraph(TGraph* graph);
	void AddGraph(TGraph* graph, const CppConfigDictionary& graphConfig);
	void Save(const std::filesystem::path& outputPath = ".");
private:
	void setCanvas(const CppConfigDictionary& config);
	void setLegend(const CppConfigDictionary& config);

};


#endif