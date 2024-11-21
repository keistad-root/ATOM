#define __TTHRESHOLDCOMPARE_HEADER__
#include "TThresholdCompare.h"

std::array<int, 6> stoTime(const std::string& timeStr) {
	std::istringstream str(timeStr);
	int year, month, day, hour, minute, second;
	str >> year >> month >> day >> hour >> minute >> second;
	return {year, month, day, hour, minute, second};
}

TString getTitle(const std::string& titleStr) {
	int firstSplit = titleStr.find("\" \"");
	int secondSplit = titleStr.find("\" \"", titleStr.find("\" \"") + 1);
	std::string mainTitle = titleStr.substr(1, firstSplit - 1);
	std::string xTitle = titleStr.substr(firstSplit + 3, secondSplit - firstSplit - 3);
	std::string yTitle = titleStr.substr(secondSplit + 3, titleStr.size() - secondSplit - 4);
	return mainTitle + ";" + xTitle + ";" + yTitle;
}

TThresholdCompare::TThresholdCompare(CppConfigFile& config) {
	mDataConfig = config.getConfig("Data");
	mThresholdConfig = config.getConfig("Threshold");
	mErrorConfig = config.getConfig("Error");
	mQualityConfig = config.getConfig("Quality");
	mThresholdValConfig = config.getConfig("ThresholdValue");
	mNoiseValConfig = config.getConfig("NoiseValue");
	mQualityValConfig = config.getConfig("QualityValue");
	mEntryConfig = config.getConfig("Entry");
	mStdevConfig = config.getConfig("Stdev");
	defineColour();
}

void TThresholdCompare::getFile() {
	for ( const CppConfigDictionary& dataConfig : mDataConfig.getSubConfigSet() ) {
		TFile* tFile = new TFile(static_cast<TString>(dataConfig.find("file")), "READ");
		TH1D* tThreshold = (TH1D*) tFile->Get("thresholdDistribution");
		TH1D* tError = (TH1D*) tFile->Get("noiseDistribution");
		TH1D* tQuality = (TH1D*) tFile->Get("qualityDistribution");
		mThresholdSet.push_back({dataConfig, tFile, tThreshold, tError, tQuality});
	}
}

void TThresholdCompare::mergeThreshold() {
	Int_t canvasWidth = mThresholdConfig.hasKey("canvas_width") ? stoi(mThresholdConfig.find("canvas_width")) : 2000;
	Int_t canvasHeight = mThresholdConfig.hasKey("canvas_height") ? stoi(mThresholdConfig.find("canvas_height")) : 1000;

	TCanvas* thrCan = new TCanvas("thrCan", "", canvasWidth, canvasHeight);

	Double_t legendX1 = mThresholdConfig.hasKey("legend_x1") ? stod(mThresholdConfig.find("legend_x1")) : .7;
	Double_t legendX2 = mThresholdConfig.hasKey("legend_x2") ? stod(mThresholdConfig.find("legend_x2")) : .9;
	Double_t legendY1 = mThresholdConfig.hasKey("legend_y1") ? stod(mThresholdConfig.find("legend_y1")) : .7;
	Double_t legendY2 = mThresholdConfig.hasKey("legend_y2") ? stod(mThresholdConfig.find("legend_y2")) : .9;
	TString legendTitle = mThresholdConfig.hasKey("legend_title") ? mThresholdConfig.find("legend_title") : "";
	TLegend* thrLegend = new TLegend(legendX1, legendY1, legendX2, legendY2, legendTitle);

	Int_t yMax = mThresholdConfig.hasKey("y_max") ? stoi(mThresholdConfig.find("y_max")) : 10000;

	for ( const std::tuple<CppConfigDictionary, TFile*, TH1D*, TH1D*, TH1D*>& dataSet : mThresholdSet ) {
		CppConfigDictionary tConfig = std::get<0>(dataSet);
		TH1D* tThreshold = std::get<2>(dataSet);
		Int_t lineColour = tConfig.hasKey("line_colour") ? getColour(tConfig.find("line_colour")) : kWhite;
		tThreshold->SetLineColor(lineColour);
		Int_t lineWidth = tConfig.hasKey("line_width") ? stod(tConfig.find("line_width")) : 1;
		Int_t lineStyle = tConfig.hasKey("line_style") ? stoi(tConfig.find("line_style")) : 1;
		tThreshold->SetLineStyle(lineStyle);
		tThreshold->SetLineWidth(lineWidth);
		tThreshold->SetStats(0);
		TString tLegend = tConfig.find("legend");
		thrLegend->AddEntry(tThreshold, tLegend, "l");
		tThreshold->SetMaximum(yMax);
		tThreshold->Draw("SAME");
	}

	thrLegend->Draw();
	std::filesystem::path outputPath = mThresholdConfig.hasKey("output_file") ? mThresholdConfig.find("output_file") : "Data/threshold_comparison.png";
	std::filesystem::create_directories(outputPath.parent_path());
	thrCan->SaveAs(static_cast<TString>(outputPath));

	delete thrCan;
	thrCan = nullptr;
}

void TThresholdCompare::mergeError() {
	Int_t canvasWidth = mErrorConfig.hasKey("canvas_width") ? stoi(mErrorConfig.find("canvas_width")) : 2000;
	Int_t canvasHeight = mErrorConfig.hasKey("canvas_height") ? stoi(mErrorConfig.find("canvas_height")) : 1000;

	TCanvas* thrCan = new TCanvas("thrCan", "", canvasWidth, canvasHeight);

	Double_t legendX1 = mErrorConfig.hasKey("legend_x1") ? stod(mErrorConfig.find("legend_x1")) : .7;
	Double_t legendX2 = mErrorConfig.hasKey("legend_x2") ? stod(mErrorConfig.find("legend_x2")) : .9;
	Double_t legendY1 = mErrorConfig.hasKey("legend_y1") ? stod(mErrorConfig.find("legend_y1")) : .7;
	Double_t legendY2 = mErrorConfig.hasKey("legend_y2") ? stod(mErrorConfig.find("legend_y2")) : .9;
	TString legendTitle = mErrorConfig.hasKey("legend_title") ? mErrorConfig.find("legend_title") : "";
	TLegend* thrLegend = new TLegend(legendX1, legendY1, legendX2, legendY2, legendTitle);

	Int_t yMax = mErrorConfig.hasKey("y_max") ? stoi(mErrorConfig.find("y_max")) : 10000;

	for ( const std::tuple<CppConfigDictionary, TFile*, TH1D*, TH1D*, TH1D*>& dataSet : mThresholdSet ) {
		CppConfigDictionary tConfig = std::get<0>(dataSet);
		TH1D* tError = std::get<3>(dataSet);
		Int_t lineColour = tConfig.hasKey("line_colour") ? getColour(tConfig.find("line_colour")) : kWhite;
		tError->SetLineColor(lineColour);
		Int_t lineWidth = tConfig.hasKey("line_width") ? stod(tConfig.find("line_width")) : 1;
		Int_t lineStyle = tConfig.hasKey("line_style") ? getColour(tConfig.find("line_style")) : 1;
		tError->SetLineWidth(lineWidth);
		tError->SetLineStyle(lineStyle);
		tError->SetStats(0);
		TString tLegend = tConfig.find("legend");
		thrLegend->AddEntry(tError, tLegend, "l");
		tError->SetMaximum(yMax);
		tError->Draw("SAME");
	}

	thrLegend->Draw();
	std::filesystem::path outputPath = mErrorConfig.hasKey("output_file") ? mErrorConfig.find("output_file") : "Data/threshold_comparison.png";
	std::filesystem::create_directories(outputPath.parent_path());
	thrCan->SaveAs(static_cast<TString>(outputPath));

	delete thrCan;
	thrCan = nullptr;
}

void TThresholdCompare::mergeQuality() {
	Int_t canvasWidth = mQualityConfig.hasKey("canvas_width") ? stoi(mQualityConfig.find("canvas_width")) : 2000;
	Int_t canvasHeight = mQualityConfig.hasKey("canvas_height") ? stoi(mQualityConfig.find("canvas_height")) : 1000;

	TCanvas* thrCan = new TCanvas("thrCan", "", canvasWidth, canvasHeight);

	Double_t legendX1 = mQualityConfig.hasKey("legend_x1") ? stod(mQualityConfig.find("legend_x1")) : .7;
	Double_t legendX2 = mQualityConfig.hasKey("legend_x2") ? stod(mQualityConfig.find("legend_x2")) : .9;
	Double_t legendY1 = mQualityConfig.hasKey("legend_y1") ? stod(mQualityConfig.find("legend_y1")) : .7;
	Double_t legendY2 = mQualityConfig.hasKey("legend_y2") ? stod(mQualityConfig.find("legend_y2")) : .9;
	TString legendTitle = mQualityConfig.hasKey("legend_title") ? mQualityConfig.find("legend_title") : "";
	TLegend* thrLegend = new TLegend(legendX1, legendY1, legendX2, legendY2, legendTitle);

	Int_t yMax = mQualityConfig.hasKey("y_max") ? stoi(mQualityConfig.find("y_max")) : 10000;

	for ( const std::tuple<CppConfigDictionary, TFile*, TH1D*, TH1D*, TH1D*>& dataSet : mThresholdSet ) {
		CppConfigDictionary tConfig = std::get<0>(dataSet);
		TH1D* tQuality = std::get<4>(dataSet);
		Int_t lineColour = tConfig.hasKey("line_colour") ? getColour(tConfig.find("line_colour")) : kWhite;
		tQuality->SetLineColor(lineColour);
		Int_t lineWidth = tConfig.hasKey("line_width") ? stod(tConfig.find("line_width")) : 1;
		Int_t lineStyle = tConfig.hasKey("line_style") ? stoi(tConfig.find("line_style")) : 1;
		tQuality->SetLineStyle(lineStyle);
		tQuality->SetLineWidth(lineWidth);
		tQuality->SetStats(0);
		TString tLegend = tConfig.find("legend");
		thrLegend->AddEntry(tQuality, tLegend, "l");
		tQuality->SetMaximum(yMax);
		tQuality->Draw("SAME");
	}

	thrLegend->Draw();
	std::filesystem::path outputPath = mQualityConfig.hasKey("output_file") ? mQualityConfig.find("output_file") : "Data/threshold_comparison.png";
	std::filesystem::create_directories(outputPath.parent_path());
	thrCan->SaveAs(static_cast<TString>(outputPath));

	delete thrCan;
	thrCan = nullptr;
}

void TThresholdCompare::compareThresholdValue() {
	Int_t canvasWidth = mThresholdValConfig.hasKey("canvas_width") ? stoi(mThresholdValConfig.find("canvas_width")) : 2000;
	Int_t canvasHeight = mThresholdValConfig.hasKey("canvas_height") ? stoi(mThresholdValConfig.find("canvas_height")) : 1000;

	TCanvas* thrValCan = new TCanvas("thrValCan", "", canvasWidth, canvasHeight);
	TGraphErrors* thrValGraph = new TGraphErrors();
	int iGraph = 0;
	for ( const std::tuple<CppConfigDictionary, TFile*, TH1D*, TH1D*, TH1D*>& dataSet : mThresholdSet ) {
		CppConfigDictionary tConfig = std::get<0>(dataSet);

		TH1D* tThreshold = std::get<2>(dataSet);
		std::array<int, 6> timeArray = stoTime(tConfig.find("time"));
		TDatime* dateX = new TDatime(timeArray[0], timeArray[1], timeArray[2], timeArray[3], timeArray[4], timeArray[5]);
		thrValGraph->SetPoint(iGraph, dateX->Convert(), tThreshold->GetMean());
		thrValGraph->SetPointError(iGraph, 0, tThreshold->GetStdDev());
		iGraph++;
	}
	Int_t lineColour = mThresholdValConfig.hasKey("line_colour") ? getColour(mThresholdValConfig.find("line_colour")) : kWhite;
	Int_t lineWidth = mThresholdValConfig.hasKey("line_width") ? stod(mThresholdValConfig.find("line_width")) : 1;
	Int_t lineStyle = mThresholdValConfig.hasKey("line_style") ? getColour(mThresholdValConfig.find("line_style")) : 1;
	thrValGraph->SetLineStyle(lineStyle);
	thrValGraph->SetLineColor(lineColour);
	thrValGraph->SetLineWidth(lineWidth);

	Int_t markerColour = mThresholdValConfig.hasKey("marker_colour") ? getColour(mThresholdValConfig.find("marker_colour")) : kWhite;
	Int_t markerWidth = mThresholdValConfig.hasKey("marker_size") ? stod(mThresholdValConfig.find("marker_size")) : 1;
	Int_t markerStyle = mThresholdValConfig.hasKey("marker_style") ? getColour(mThresholdValConfig.find("marker_style")) : 1;
	thrValGraph->SetMarkerStyle(markerStyle);
	thrValGraph->SetMarkerSize(markerWidth);
	thrValGraph->SetMarkerColor(markerColour);

	TString thrValTitle = mThresholdValConfig.hasKey("title") ? getTitle(mThresholdValConfig.find("title")) : "";
	thrValGraph->SetTitle(thrValTitle);

	thrValGraph->GetXaxis()->SetTimeDisplay(1);
	Double_t xAxisLabelSize = mThresholdValConfig.hasKey("x_axis_label_size") ? stod(mThresholdValConfig.find("x_axis_label_size")) : .1;
	thrValGraph->GetXaxis()->SetLabelSize(xAxisLabelSize);
	Double_t yAxisLabelSize = mThresholdValConfig.hasKey("y_axis_label_size") ? stod(mThresholdValConfig.find("y_axis_label_size")) : .1;
	thrValGraph->GetYaxis()->SetLabelSize(yAxisLabelSize);

	Double_t xAxisOffset = mThresholdValConfig.hasKey("x_axis_offset") ? stod(mThresholdValConfig.find("x_axis_offset")) : .1;
	thrValGraph->GetXaxis()->SetTitleOffset(xAxisOffset);
	Double_t yAxisOffset = mThresholdValConfig.hasKey("y_axis_offset") ? stod(mThresholdValConfig.find("y_axis_offset")) : .1;
	thrValGraph->GetYaxis()->SetTitleOffset(yAxisOffset);


	Double_t xAxisTitleSize = mThresholdValConfig.hasKey("x_axis_title_size") ? stod(mThresholdValConfig.find("x_axis_title_size")) : .1;
	thrValGraph->GetXaxis()->SetTitleSize(xAxisTitleSize);
	Double_t yAxisTitleSize = mThresholdValConfig.hasKey("y_axis_title_size") ? stod(mThresholdValConfig.find("y_axis_title_size")) : .1;
	thrValGraph->GetYaxis()->SetTitleSize(yAxisTitleSize);

	thrValGraph->GetXaxis()->SetTimeFormat("%Y-%m-%d");
	thrValGraph->Draw("APL");

	Double_t leftMargin = mThresholdValConfig.hasKey("left_margin") ? stod(mThresholdValConfig.find("left_margin")) : .1;
	Double_t rightMargin = mThresholdValConfig.hasKey("right_margin") ? stod(mThresholdValConfig.find("right_margin")) : .1;
	Double_t topMargin = mThresholdValConfig.hasKey("top_margin") ? stod(mThresholdValConfig.find("top_margin")) : .1;
	Double_t bottomMargin = mThresholdValConfig.hasKey("bottom_margin") ? stod(mThresholdValConfig.find("bottom_margin")) : .1;
	thrValCan->SetMargin(leftMargin, rightMargin, bottomMargin, topMargin);



	std::filesystem::path outputPath = mThresholdValConfig.hasKey("output_file") ? mThresholdValConfig.find("output_file") : "Data/threshold_value_comparison.png";
	std::filesystem::create_directories(outputPath.parent_path());
	thrValCan->SaveAs(static_cast<TString>(outputPath));
}

void TThresholdCompare::compareNoiseValue() {
	Int_t canvasWidth = mNoiseValConfig.hasKey("canvas_width") ? stoi(mNoiseValConfig.find("canvas_width")) : 2000;
	Int_t canvasHeight = mNoiseValConfig.hasKey("canvas_height") ? stoi(mNoiseValConfig.find("canvas_height")) : 1000;

	TCanvas* noiseValCan = new TCanvas("noiseValCan", "", canvasWidth, canvasHeight);
	TGraphErrors* noiseValGraph = new TGraphErrors();
	int iGraph = 0;
	for ( const std::tuple<CppConfigDictionary, TFile*, TH1D*, TH1D*, TH1D*>& dataSet : mThresholdSet ) {
		CppConfigDictionary tConfig = std::get<0>(dataSet);

		TH1D* tNoise = std::get<3>(dataSet);
		std::array<int, 6> timeArray = stoTime(tConfig.find("time"));
		TDatime* dateX = new TDatime(timeArray[0], timeArray[1], timeArray[2], timeArray[3], timeArray[4], timeArray[5]);
		noiseValGraph->SetPoint(iGraph, dateX->Convert(), tNoise->GetMean());
		noiseValGraph->SetPointError(iGraph, 0, tNoise->GetStdDev());
		iGraph++;
	}
	Int_t lineColour = mNoiseValConfig.hasKey("line_colour") ? getColour(mNoiseValConfig.find("line_colour")) : kWhite;
	Int_t lineWidth = mNoiseValConfig.hasKey("line_width") ? stod(mNoiseValConfig.find("line_width")) : 1;
	Int_t lineStyle = mNoiseValConfig.hasKey("line_style") ? getColour(mNoiseValConfig.find("line_style")) : 1;
	noiseValGraph->SetLineStyle(lineStyle);
	noiseValGraph->SetLineColor(lineColour);
	noiseValGraph->SetLineWidth(lineWidth);

	Int_t markerColour = mNoiseValConfig.hasKey("marker_colour") ? getColour(mNoiseValConfig.find("marker_colour")) : kWhite;
	Int_t markerWidth = mNoiseValConfig.hasKey("marker_size") ? stod(mNoiseValConfig.find("marker_size")) : 1;
	Int_t markerStyle = mNoiseValConfig.hasKey("marker_style") ? getColour(mNoiseValConfig.find("marker_style")) : 1;
	noiseValGraph->SetMarkerStyle(markerStyle);
	noiseValGraph->SetMarkerSize(markerWidth);
	noiseValGraph->SetMarkerColor(markerColour);

	TString noiseValTitle = mNoiseValConfig.hasKey("title") ? getTitle(mNoiseValConfig.find("title")) : "";
	noiseValGraph->SetTitle(noiseValTitle);

	Double_t xAxisLabelSize = mNoiseValConfig.hasKey("x_axis_label_size") ? stod(mNoiseValConfig.find("x_axis_label_size")) : .1;
	noiseValGraph->GetXaxis()->SetLabelSize(xAxisLabelSize);
	Double_t yAxisLabelSize = mNoiseValConfig.hasKey("y_axis_label_size") ? stod(mNoiseValConfig.find("y_axis_label_size")) : .1;
	noiseValGraph->GetYaxis()->SetLabelSize(yAxisLabelSize);

	Double_t xAxisOffset = mNoiseValConfig.hasKey("x_axis_offset") ? stod(mNoiseValConfig.find("x_axis_offset")) : .1;
	noiseValGraph->GetXaxis()->SetTitleOffset(xAxisOffset);
	Double_t yAxisOffset = mNoiseValConfig.hasKey("y_axis_offset") ? stod(mNoiseValConfig.find("y_axis_offset")) : .1;
	noiseValGraph->GetYaxis()->SetTitleOffset(yAxisOffset);


	Double_t xAxisTitleSize = mNoiseValConfig.hasKey("x_axis_title_size") ? stod(mNoiseValConfig.find("x_axis_title_size")) : .1;
	noiseValGraph->GetXaxis()->SetTitleSize(xAxisTitleSize);
	Double_t yAxisTitleSize = mNoiseValConfig.hasKey("y_axis_title_size") ? stod(mNoiseValConfig.find("y_axis_title_size")) : .1;
	noiseValGraph->GetYaxis()->SetTitleSize(yAxisTitleSize);


	noiseValGraph->GetXaxis()->SetTimeDisplay(1);
	noiseValGraph->GetXaxis()->SetTimeFormat("%Y-%m-%d");
	noiseValGraph->Draw("APL");

	Double_t leftMargin = mNoiseValConfig.hasKey("left_margin") ? stod(mNoiseValConfig.find("left_margin")) : .1;
	Double_t rightMargin = mNoiseValConfig.hasKey("right_margin") ? stod(mNoiseValConfig.find("right_margin")) : .1;
	Double_t topMargin = mNoiseValConfig.hasKey("top_margin") ? stod(mNoiseValConfig.find("top_margin")) : .1;
	Double_t bottomMargin = mNoiseValConfig.hasKey("bottom_margin") ? stod(mNoiseValConfig.find("bottom_margin")) : .1;
	noiseValCan->SetMargin(leftMargin, rightMargin, bottomMargin, topMargin);

	std::filesystem::path outputPath = mNoiseValConfig.hasKey("output_file") ? mNoiseValConfig.find("output_file") : "Data/Noise_value_comparison.png";
	std::filesystem::create_directories(outputPath.parent_path());
	noiseValCan->SaveAs(static_cast<TString>(outputPath));
}


void TThresholdCompare::compareQualityValue() {
	Int_t canvasWidth = mQualityValConfig.hasKey("canvas_width") ? stoi(mQualityValConfig.find("canvas_width")) : 2000;
	Int_t canvasHeight = mQualityValConfig.hasKey("canvas_height") ? stoi(mQualityValConfig.find("canvas_height")) : 1000;

	TCanvas* qualityValCan = new TCanvas("qualityValCan", "", canvasWidth, canvasHeight);
	TGraphErrors* qualityValGraph = new TGraphErrors();
	int iGraph = 0;
	for ( const std::tuple<CppConfigDictionary, TFile*, TH1D*, TH1D*, TH1D*>& dataSet : mThresholdSet ) {
		CppConfigDictionary tConfig = std::get<0>(dataSet);

		TH1D* tNoise = std::get<4>(dataSet);
		std::array<int, 6> timeArray = stoTime(tConfig.find("time"));
		TDatime* dateX = new TDatime(timeArray[0], timeArray[1], timeArray[2], timeArray[3], timeArray[4], timeArray[5]);
		qualityValGraph->SetPoint(iGraph, dateX->Convert(), tNoise->GetMean());
		qualityValGraph->SetPointError(iGraph, 0, tNoise->GetStdDev());
		iGraph++;
	}
	Int_t lineColour = mQualityValConfig.hasKey("line_colour") ? getColour(mQualityValConfig.find("line_colour")) : kWhite;
	Int_t lineWidth = mQualityValConfig.hasKey("line_width") ? stod(mQualityValConfig.find("line_width")) : 1;
	Int_t lineStyle = mQualityValConfig.hasKey("line_style") ? getColour(mQualityValConfig.find("line_style")) : 1;
	qualityValGraph->SetLineStyle(lineStyle);
	qualityValGraph->SetLineColor(lineColour);
	qualityValGraph->SetLineWidth(lineWidth);

	Int_t markerColour = mQualityValConfig.hasKey("marker_colour") ? getColour(mQualityValConfig.find("marker_colour")) : kWhite;
	Int_t markerWidth = mQualityValConfig.hasKey("marker_size") ? stod(mQualityValConfig.find("marker_size")) : 1;
	Int_t markerStyle = mQualityValConfig.hasKey("marker_style") ? getColour(mQualityValConfig.find("marker_style")) : 1;
	qualityValGraph->SetMarkerStyle(markerStyle);
	qualityValGraph->SetMarkerSize(markerWidth);
	qualityValGraph->SetMarkerColor(markerColour);

	TString noiseValTitle = mQualityValConfig.hasKey("title") ? getTitle(mQualityValConfig.find("title")) : "";
	qualityValGraph->SetTitle(noiseValTitle);


	Double_t xAxisLabelSize = mQualityValConfig.hasKey("x_axis_label_size") ? stod(mQualityValConfig.find("x_axis_label_size")) : .1;
	qualityValGraph->GetXaxis()->SetLabelSize(xAxisLabelSize);
	Double_t yAxisLabelSize = mQualityValConfig.hasKey("y_axis_label_size") ? stod(mQualityValConfig.find("y_axis_label_size")) : .1;
	qualityValGraph->GetYaxis()->SetLabelSize(yAxisLabelSize);

	Double_t xAxisOffset = mQualityValConfig.hasKey("x_axis_offset") ? stod(mQualityValConfig.find("x_axis_offset")) : .1;
	qualityValGraph->GetXaxis()->SetTitleOffset(xAxisOffset);
	Double_t yAxisOffset = mQualityValConfig.hasKey("y_axis_offset") ? stod(mQualityValConfig.find("y_axis_offset")) : .1;
	qualityValGraph->GetYaxis()->SetTitleOffset(yAxisOffset);


	Double_t xAxisTitleSize = mQualityValConfig.hasKey("x_axis_title_size") ? stod(mQualityValConfig.find("x_axis_title_size")) : .1;
	qualityValGraph->GetXaxis()->SetTitleSize(xAxisTitleSize);
	Double_t yAxisTitleSize = mQualityValConfig.hasKey("y_axis_title_size") ? stod(mQualityValConfig.find("y_axis_title_size")) : .1;
	qualityValGraph->GetYaxis()->SetTitleSize(yAxisTitleSize);


	qualityValGraph->GetXaxis()->SetTimeDisplay(1);
	qualityValGraph->GetXaxis()->SetTimeFormat("%Y-%m-%d");
	qualityValGraph->Draw("APL");


	Double_t leftMargin = mQualityValConfig.hasKey("left_margin") ? stod(mQualityValConfig.find("left_margin")) : .1;
	Double_t rightMargin = mQualityValConfig.hasKey("right_margin") ? stod(mQualityValConfig.find("right_margin")) : .1;
	Double_t topMargin = mQualityValConfig.hasKey("top_margin") ? stod(mQualityValConfig.find("top_margin")) : .1;
	Double_t bottomMargin = mQualityValConfig.hasKey("bottom_margin") ? stod(mQualityValConfig.find("bottom_margin")) : .1;
	qualityValCan->SetMargin(leftMargin, rightMargin, bottomMargin, topMargin);

	std::filesystem::path outputPath = mQualityValConfig.hasKey("output_file") ? mQualityValConfig.find("output_file") : "Data/Noise_value_comparison.png";
	std::filesystem::create_directories(outputPath.parent_path());
	qualityValCan->SaveAs(static_cast<TString>(outputPath));
}


void TThresholdCompare::compareEntry() {
	Int_t canvasWidth = mEntryConfig.hasKey("canvas_width") ? stoi(mEntryConfig.find("canvas_width")) : 2000;
	Int_t canvasHeight = mEntryConfig.hasKey("canvas_height") ? stoi(mEntryConfig.find("canvas_height")) : 1000;

	TCanvas* thrValCan = new TCanvas("entValCan", "", canvasWidth, canvasHeight);
	TGraphErrors* thrValGraph = new TGraphErrors();
	int iGraph = 0;
	for ( const std::tuple<CppConfigDictionary, TFile*, TH1D*, TH1D*, TH1D*>& dataSet : mThresholdSet ) {
		CppConfigDictionary tConfig = std::get<0>(dataSet);

		TH1D* tThreshold = std::get<2>(dataSet);
		std::array<int, 6> timeArray = stoTime(tConfig.find("time"));
		TDatime* dateX = new TDatime(timeArray[0], timeArray[1], timeArray[2], timeArray[3], timeArray[4], timeArray[5]);
		thrValGraph->SetPoint(iGraph, dateX->Convert(), tThreshold->GetEntries());
		thrValGraph->SetPointError(iGraph, 0, 0);
		iGraph++;
	}
	Int_t lineColour = mEntryConfig.hasKey("line_colour") ? getColour(mEntryConfig.find("line_colour")) : kWhite;
	Int_t lineWidth = mEntryConfig.hasKey("line_width") ? stod(mEntryConfig.find("line_width")) : 1;
	Int_t lineStyle = mEntryConfig.hasKey("line_style") ? getColour(mEntryConfig.find("line_style")) : 1;
	thrValGraph->SetLineStyle(lineStyle);
	thrValGraph->SetLineColor(lineColour);
	thrValGraph->SetLineWidth(lineWidth);

	Int_t markerColour = mEntryConfig.hasKey("marker_colour") ? getColour(mEntryConfig.find("marker_colour")) : kWhite;
	Int_t markerWidth = mEntryConfig.hasKey("marker_size") ? stod(mEntryConfig.find("marker_size")) : 1;
	Int_t markerStyle = mEntryConfig.hasKey("marker_style") ? getColour(mEntryConfig.find("marker_style")) : 1;
	thrValGraph->SetMarkerStyle(markerStyle);
	thrValGraph->SetMarkerSize(markerWidth);
	thrValGraph->SetMarkerColor(markerColour);

	TString thrValTitle = mEntryConfig.hasKey("title") ? getTitle(mEntryConfig.find("title")) : "";
	thrValGraph->SetTitle(thrValTitle);

	thrValGraph->GetXaxis()->SetTimeDisplay(1);
	thrValGraph->GetXaxis()->SetTimeFormat("%Y-%m-%d");
	thrValGraph->Draw("APL");

	std::filesystem::path outputPath = mEntryConfig.hasKey("output_file") ? mEntryConfig.find("output_file") : "Data/threshold_value_comparison.png";
	std::filesystem::create_directories(outputPath.parent_path());
	thrValCan->SaveAs(static_cast<TString>(outputPath));
}


void TThresholdCompare::compareStdDev() {
	Int_t canvasWidth = mStdevConfig.hasKey("canvas_width") ? stoi(mStdevConfig.find("canvas_width")) : 2000;
	Int_t canvasHeight = mStdevConfig.hasKey("canvas_height") ? stoi(mStdevConfig.find("canvas_height")) : 1000;

	TCanvas* thrValCan = new TCanvas("stdevValCan", "", canvasWidth, canvasHeight);
	TGraphErrors* thrValGraph = new TGraphErrors();
	int iGraph = 0;
	for ( const std::tuple<CppConfigDictionary, TFile*, TH1D*, TH1D*, TH1D*>& dataSet : mThresholdSet ) {
		CppConfigDictionary tConfig = std::get<0>(dataSet);

		TH1D* tThreshold = std::get<2>(dataSet);
		std::array<int, 6> timeArray = stoTime(tConfig.find("time"));
		TDatime* dateX = new TDatime(timeArray[0], timeArray[1], timeArray[2], timeArray[3], timeArray[4], timeArray[5]);
		thrValGraph->SetPoint(iGraph, dateX->Convert(), tThreshold->GetStdDev());
		thrValGraph->SetPointError(iGraph, 0, 0);
		iGraph++;
	}
	Int_t lineColour = mStdevConfig.hasKey("line_colour") ? getColour(mStdevConfig.find("line_colour")) : kWhite;
	Int_t lineWidth = mStdevConfig.hasKey("line_width") ? stoi(mStdevConfig.find("line_width")) : 1;
	Int_t lineStyle = mStdevConfig.hasKey("line_style") ? getColour(mStdevConfig.find("line_style")) : 1;
	thrValGraph->SetLineStyle(lineStyle);
	thrValGraph->SetLineColor(lineColour);
	thrValGraph->SetLineWidth(lineWidth);

	Int_t markerColour = mStdevConfig.hasKey("marker_colour") ? getColour(mStdevConfig.find("marker_colour")) : kWhite;
	Int_t markerWidth = mStdevConfig.hasKey("marker_size") ? stoi(mStdevConfig.find("marker_size")) : 1;
	Int_t markerStyle = mStdevConfig.hasKey("marker_style") ? getColour(mStdevConfig.find("marker_style")) : 1;
	thrValGraph->SetMarkerStyle(markerStyle);
	thrValGraph->SetMarkerSize(markerWidth);
	thrValGraph->SetMarkerColor(markerColour);

	TString thrValTitle = mStdevConfig.hasKey("title") ? getTitle(mStdevConfig.find("title")) : "";
	thrValGraph->SetTitle(thrValTitle);

	thrValGraph->GetXaxis()->SetTimeDisplay(1);
	thrValGraph->GetXaxis()->SetTimeFormat("%Y-%m-%d");
	thrValGraph->Draw("APL");

	std::filesystem::path outputPath = mStdevConfig.hasKey("output_file") ? mStdevConfig.find("output_file") : "Data/threshold_value_comparison.png";
	std::filesystem::create_directories(outputPath.parent_path());
	thrValCan->SaveAs(static_cast<TString>(outputPath));
}