#define __TANALYSER_HEADERS__

#include "TAnalyser.h"


void TAnalyser::getOriginClustermap() {
	CppConfigDictionary config = mConfigFile.getConfig("Origin_Clustermap");

	std::string plotTitle = config.hasKey("title") ? config.find("title") : "";
	mOriginClustermap->SetTitle(static_cast<TString>(plotTitle));

	Int_t canWidth = config.hasKey("canvas_width") ? stoi(config.find("canvas_width")) : 2500;
	Int_t canHeight = config.hasKey("canvas_height") ? stoi(config.find("canvas_height")) : 1000;
	TCanvas* canvas = new TCanvas("occ", "", canWidth, canHeight);

	Double_t leftMargin = config.hasKey("left_margin") ? stod(config.find("left_margin")) : .7;
	Double_t rightMargin = config.hasKey("right_margin") ? stod(config.find("right_margin")) : .35;
	Double_t topMargin = config.hasKey("top_margin") ? stod(config.find("top_margin")) : .08;
	Double_t bottomMargin = config.hasKey("bottom_margin") ? stod(config.find("bottom_margin")) : .12;
	canvas->SetMargin(leftMargin, rightMargin, bottomMargin, topMargin);

	Double_t xTitleOffset = config.hasKey("x_title_offset") ? stod(config.find("x_title_offset")) : 1.4;
	Double_t xLabelOffset = config.hasKey("x_label_offset") ? stod(config.find("x_label_offset")) : 0.003;
	Double_t yTitleOffset = config.hasKey("y_title_offset") ? stod(config.find("y_title_offset")) : 1.;
	Double_t yLabelOffset = config.hasKey("y_label_offset") ? stod(config.find("y_label_offset")) : 0.;
	mOriginClustermap->GetXaxis()->SetTitleOffset(xTitleOffset);
	mOriginClustermap->GetXaxis()->SetLabelOffset(xLabelOffset);
	mOriginClustermap->GetYaxis()->SetTitleOffset(yTitleOffset);
	mOriginClustermap->GetYaxis()->SetLabelOffset(yLabelOffset);

	std::string originHitmapFileName = config.hasKey("filename") ? config.find("filename") : "origin_clustermap.png";
	std::filesystem::path tempPath = (mOutputPath / originHitmapFileName);
	std::filesystem::create_directories(tempPath.parent_path());
	std::filesystem::path colzPath = (tempPath.parent_path() / (tempPath.stem().string() + "_colz")).replace_extension(tempPath.extension());
	mOriginClustermap->Draw("COLZ");
	mExpSettingLegend->Draw("SAME");
	canvas->SaveAs(static_cast<TString>(colzPath));

	std::filesystem::path legoPath = (tempPath.parent_path() / (tempPath.stem().string() + "_lego")).replace_extension(tempPath.extension());
	mOriginClustermap->Draw("LEGO0");
	mExpSettingLegend->Draw("SAME");
	canvas->SaveAs(static_cast<TString>(legoPath));

	if ( isGraphFile ) {
		mGraphFile->cd();
		mOriginClustermap->Write();
	}

	delete canvas;
	canvas = nullptr;
}

void TAnalyser::getHHCutClustermap() {
	CppConfigDictionary config = mConfigFile.getConfig("High_Hit_Cut_Clustermap");

	std::string plotTitle = config.hasKey("title") ? config.find("title") : "";
	mHHCutClustermap->SetTitle(static_cast<TString>(plotTitle));

	Int_t canWidth = config.hasKey("canvas_width") ? stoi(config.find("canvas_width")) : 2500;
	Int_t canHeight = config.hasKey("canvas_height") ? stoi(config.find("canvas_height")) : 1000;
	TCanvas* canvas = new TCanvas("occ", "", canWidth, canHeight);

	Double_t leftMargin = config.hasKey("left_margin") ? stod(config.find("left_margin")) : .7;
	Double_t rightMargin = config.hasKey("right_margin") ? stod(config.find("right_margin")) : .35;
	Double_t topMargin = config.hasKey("top_margin") ? stod(config.find("top_margin")) : .08;
	Double_t bottomMargin = config.hasKey("bottom_margin") ? stod(config.find("bottom_margin")) : .12;
	canvas->SetMargin(leftMargin, rightMargin, bottomMargin, topMargin);

	Double_t xTitleOffset = config.hasKey("x_title_offset") ? stod(config.find("x_title_offset")) : 1.4;
	Double_t xLabelOffset = config.hasKey("x_label_offset") ? stod(config.find("x_label_offset")) : 0.003;
	Double_t yTitleOffset = config.hasKey("y_title_offset") ? stod(config.find("y_title_offset")) : 1.;
	Double_t yLabelOffset = config.hasKey("y_label_offset") ? stod(config.find("y_label_offset")) : 0.;
	mHHCutClustermap->GetXaxis()->SetTitleOffset(xTitleOffset);
	mHHCutClustermap->GetXaxis()->SetLabelOffset(xLabelOffset);
	mHHCutClustermap->GetYaxis()->SetTitleOffset(yTitleOffset);
	mHHCutClustermap->GetYaxis()->SetLabelOffset(yLabelOffset);

	std::string originHitmapFileName = config.hasKey("filename") ? config.find("filename") : "High_Hit_Cut_Clustermap.png";
	std::filesystem::path tempPath = (mOutputPath / originHitmapFileName);
	std::filesystem::create_directories(tempPath.parent_path());
	std::filesystem::path colzPath = (tempPath.parent_path() / (tempPath.stem().string() + "_colz")).replace_extension(tempPath.extension());
	mHHCutClustermap->Draw("COLZ");
	mExpSettingLegend->Draw("SAME");
	canvas->SaveAs(static_cast<TString>(colzPath));

	std::filesystem::path legoPath = (tempPath.parent_path() / (tempPath.stem().string() + "_lego")).replace_extension(tempPath.extension());
	mHHCutClustermap->Draw("LEGO0");
	mExpSettingLegend->Draw("SAME");
	canvas->SaveAs(static_cast<TString>(legoPath));

	if ( isGraphFile ) {
		mGraphFile->cd();
		mHHCutClustermap->Write();
	}

	delete canvas;
	canvas = nullptr;
}

void TAnalyser::getHHHCCutClustermap() {
	CppConfigDictionary config = mConfigFile.getConfig("High_Hit_N_Cluster_Cut_Clustermap");

	std::string plotTitle = config.hasKey("title") ? config.find("title") : "";
	mHHHCCutClustermap->SetTitle(static_cast<TString>(plotTitle));

	Int_t canWidth = config.hasKey("canvas_width") ? stoi(config.find("canvas_width")) : 2500;
	Int_t canHeight = config.hasKey("canvas_height") ? stoi(config.find("canvas_height")) : 1000;
	TCanvas* canvas = new TCanvas("hhhcc", "", canWidth, canHeight);

	Double_t leftMargin = config.hasKey("left_margin") ? stod(config.find("left_margin")) : .07;
	Double_t rightMargin = config.hasKey("right_margin") ? stod(config.find("right_margin")) : .35;
	Double_t topMargin = config.hasKey("top_margin") ? stod(config.find("top_margin")) : .08;
	Double_t bottomMargin = config.hasKey("bottom_margin") ? stod(config.find("bottom_margin")) : .12;
	canvas->SetMargin(leftMargin, rightMargin, bottomMargin, topMargin);

	Double_t xTitleOffset = config.hasKey("x_title_offset") ? stod(config.find("x_title_offset")) : 1.4;
	Double_t xLabelOffset = config.hasKey("x_label_offset") ? stod(config.find("x_label_offset")) : 0.003;
	Double_t yTitleOffset = config.hasKey("y_title_offset") ? stod(config.find("y_title_offset")) : 1.;
	Double_t yLabelOffset = config.hasKey("y_label_offset") ? stod(config.find("y_label_offset")) : 0.;
	mHHHCCutClustermap->GetXaxis()->SetTitleOffset(xTitleOffset);
	mHHHCCutClustermap->GetXaxis()->SetLabelOffset(xLabelOffset);
	mHHHCCutClustermap->GetYaxis()->SetTitleOffset(yTitleOffset);
	mHHHCCutClustermap->GetYaxis()->SetLabelOffset(yLabelOffset);
	mHHHCCutClustermap->RebinX(4);
	mHHHCCutClustermap->RebinY(4);

	std::string originHitmapFileName = config.hasKey("filename") ? config.find("filename") : "High_Hit_N_Cluster_Cut_Clustermap.png";
	std::filesystem::path tempPath = (mOutputPath / originHitmapFileName);
	std::filesystem::create_directories(tempPath.parent_path());
	std::filesystem::path colzPath = (tempPath.parent_path() / (tempPath.stem().string() + "_colz")).replace_extension(tempPath.extension());
	mHHHCCutClustermap->Draw("COLZ");
	mExpSettingLegend->Draw("SAME");
	canvas->SaveAs(static_cast<TString>(colzPath));

	std::filesystem::path legoPath = (tempPath.parent_path() / (tempPath.stem().string() + "_lego")).replace_extension(tempPath.extension());
	mHHHCCutClustermap->Draw("LEGO0");
	mExpSettingLegend->Draw("SAME");
	canvas->SaveAs(static_cast<TString>(legoPath));

	if ( isGraphFile ) {
		mGraphFile->cd();
		mHHHCCutClustermap->Write();
	}

	delete canvas;
	canvas = nullptr;
}