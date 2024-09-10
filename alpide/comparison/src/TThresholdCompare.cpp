#define __TTHRESHOLDCOMPARE_HEADER__
#include "TThresholdCompare.h"

TThresholdCompare::TThresholdCompare(const CppConfigFile& config) {
	mDataConfig = config.getConfig("Data");
	mThresholdConfig = config.getConfig("Threshold");
	mErrorConfig = config.getConfig("Error");
}

void TThresholdCompare::getFile() {
	for ( const CppConfigDictionary& dataConfig : mDataConfig.getSubConfigSet() ) {
		TFile* tFile = new TFile(static_cast<TString>(dataConfig.find("file")), "READ");
		TH1D* tThreshold = (TH1D*) tFile->Get("threshold");
		TH1D* tError = (TH1D*) tFile->Get("errDist");
		mThresholdSet.push_back({dataConfig, tFile, tThreshold, tError});
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

	for ( const std::tuple<CppConfigDictionary, TFile*, TH1D*, TH1D*>& dataSet : mThresholdSet ) {
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

	for ( const std::tuple<CppConfigDictionary, TFile*, TH1D*, TH1D*>& dataSet : mThresholdSet ) {
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