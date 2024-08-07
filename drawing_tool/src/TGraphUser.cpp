#include "TGraphUser.h"

std::unordered_map<std::string, Color_t> colourSet = {{"red", kRed}, {"green", kGreen + 2}, {"blue", kBlue}, {"magenta", kMagenta}, {"cyan", kCyan}, {"orange", kOrange + 1}, {"yellow", kYellow + 1}};

/**
 * @brief Construct a new TGraphUser::TGraphUser object
 *
 * @param config
*/
TGraphUser::TGraphUser(const CppConfigDictionary& config) {
	mFileName = config.find("file_name");
	mMultiGraph = new TMultiGraph();
	setCanvas(config);
	setLegend(config);
}

void TGraphUser::Save(const std::filesystem::path& outputPath) {
	mCanvas->cd();
	mMultiGraph->SetTitle(title + ";" + x_title + ";" + y_title);
	mMultiGraph->GetXaxis()->SetLabelOffset(canvasOffsets[0]);
	mMultiGraph->GetYaxis()->SetLabelOffset(canvasOffsets[1]);
	mMultiGraph->GetXaxis()->SetTitleOffset(canvasOffsets[2]);
	mMultiGraph->GetYaxis()->SetTitleOffset(canvasOffsets[3]);
	mMultiGraph->Draw("AL");
	mCanvas->SetMargin(canvasMargins[0], canvasMargins[1], canvasMargins[2], canvasMargins[3]);
	mLegend->Draw();
	std::filesystem::path output = outputPath;
	output /= mFileName;
	savePath = output;
	mCanvas->SaveAs(savePath);
}

void TGraphUser::AddGraph(TGraph* graph) {
	mLegend->AddEntry(graph, "Graph", "lp");
	mMultiGraph->Add(graph);
}

void TGraphUser::AddGraph(TGraph* graph, const CppConfigDictionary& graphConfig) {
	TString legendLabel = graphConfig.hasKey("legend_label") ? graphConfig.find("legend_label") : "graph";

	Style_t lineStyle = graphConfig.hasKey("line_style") ? stof(graphConfig.find("line_style")) : mUniversialLineStyle;
	Size_t lineWidth = graphConfig.hasKey("line_width") ? stof(graphConfig.find("line_width")) : mUniversiallineWidth;
	Color_t lineColour = graphConfig.hasKey("line_colour") ? colourSet.find(graphConfig.find("line_colour"))->second : 0;
	graph->SetLineStyle(lineStyle);
	graph->SetLineWidth(lineWidth);
	graph->SetLineColor(lineColour);

	Style_t markerStyle = graphConfig.hasKey("marker_style") ? stof(graphConfig.find("marker_style")) : mUniversialMarkerStyle;
	Size_t markerSize = graphConfig.hasKey("marker_size") ? stof(graphConfig.find("marker_size")) : mUniversialMarkerSize;
	Color_t markerColour = graphConfig.hasKey("marker_colour") ? colourSet.find(graphConfig.find("marker_colour"))->second : 0;
	graph->SetMarkerStyle(markerStyle);
	graph->SetMarkerSize(markerSize);
	graph->SetMarkerColor(markerColour);

	TString graphOption = graphConfig.hasKey("graph_option") ? graphConfig.find("graph_option") : "pl";
	mLegend->AddEntry(graph, legendLabel, graphOption);
	mMultiGraph->Add(graph, graphOption);
}


/**
 * @brief Set Canvas Attributes
 * @details	The following attributes are adjusted.
 * - Main title, x-axis and y-axis titles.
 * - Margin between canvas border and plot border
 * @param config
*/
void TGraphUser::setCanvas(const CppConfigDictionary& config) {
	Int_t canvasRow = config.hasKey("canvas_row") ? stoi(config.find("canvas_row")) : 1000;
	Int_t canvasWidth = config.hasKey("canvas_width") ? stoi(config.find("canvas_width")) : 1000;

	mCanvas = new TCanvas();
	mCanvas->SetCanvasSize(canvasRow, canvasWidth);

	// Set Canvas Titles
	if ( config.hasKey("titles") ) {
		CppConfigDictionary titles = config.getSubConfig("titles");
		title = titles.hasKey("title") ? titles.find("title") : "";
		x_title = titles.hasKey("x_title") ? titles.find("x_title") : "";
		y_title = titles.hasKey("y_title") ? titles.find("y_title") : "";
	}

	// Set Canvas Margin
	if ( config.hasKey("canvas_margin") ) {
		CppConfigDictionary canvasMargin = config.getSubConfig("canvas_margin");
		Float_t marginLeft = canvasMargin.hasKey("left") ? stod(canvasMargin.find("left")) : .1;
		Float_t marginRight = canvasMargin.hasKey("right") ? stod(canvasMargin.find("right")) : .1;
		Float_t marginTop = canvasMargin.hasKey("top") ? stod(canvasMargin.find("top")) : .1;
		Float_t marginBottom = canvasMargin.hasKey("bottom") ? stod(canvasMargin.find("bottom")) : .1;
		canvasMargins = {marginLeft, marginRight, marginBottom, marginTop};
	} else {
		canvasMargins = {.1, .1, .1, .1};
	}

	// Set Canvas Offsets
	if ( config.hasKey("offsets") ) {
		CppConfigDictionary canvasOffset = config.getSubConfig("offsets");
		Float_t xLabelOffset = canvasOffset.hasKey("x_label") ? stod(canvasOffset.find("x_label")) : .01;
		Float_t yLabelOffset = canvasOffset.hasKey("y_label") ? stod(canvasOffset.find("y_label")) : .01;
		Float_t xTitleOffset = canvasOffset.hasKey("x_title") ? stod(canvasOffset.find("x_title")) : 1.;
		Float_t yTitleOffset = canvasOffset.hasKey("y_title") ? stod(canvasOffset.find("y_title")) : 1.;
		canvasOffsets = {xLabelOffset, yLabelOffset, xTitleOffset, yTitleOffset};
	} else {
		canvasOffsets = {.01, .01, 1., 1.};
	}

	mUniversialMarkerStyle = config.hasKey("marker_style") ? stoi(config.find("marker_style")) : 1;
	mUniversialMarkerSize = config.hasKey("marker_size") ? stoi(config.find("marker_size")) : 1;
	mUniversialLineStyle = config.hasKey("line_style") ? stoi(config.find("line_style")) : 1;
	mUniversiallineWidth = config.hasKey("line_width") ? stoi(config.find("line_width")) : 1;
}

/**
 * @brief Set Legend Attributes
 * @details
 *
*/
void TGraphUser::setLegend(const CppConfigDictionary& config) {
	if ( config.hasKey("legend") ) {
		CppConfigDictionary legendConfig = config.getSubConfig("legend");
		mLegendTitle = legendConfig.hasKey("title") ? legendConfig.find("title") : "";
		Float_t xMin = legendConfig.hasKey("x_min") ? stof(legendConfig.find("x_min")) : 0.7;
		Float_t yMin = legendConfig.hasKey("y_min") ? stof(legendConfig.find("y_min")) : 0.7;
		Float_t xMax = legendConfig.hasKey("x_max") ? stof(legendConfig.find("x_max")) : 0.9;
		Float_t yMax = legendConfig.hasKey("y_max") ? stof(legendConfig.find("y_max")) : 0.9;
		mLegendPoints = {xMin, yMin, xMax, yMax};
	} else {
		mLegendTitle = "Legend";
		mLegendPoints = {0.7, 0.7, 0.9, 0.9};
	}
	mLegend = new TLegend(mLegendPoints[0], mLegendPoints[1], mLegendPoints[2], mLegendPoints[3]);
	mLegend->SetHeader(mLegendTitle, "C");
}
