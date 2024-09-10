#define __TANALYSER_HEADERS__

#include "TAnalyser.h"


void TAnalyser::getOriginClustersize() {
	CppConfigDictionary config = mConfigFile.getConfig("Origin_Clustersize");

	std::string plotTitle = config.hasKey("title") ? config.find("title") : "";
	mOriginClustersize->SetTitle(static_cast<TString>(plotTitle));

	Int_t canWidth = config.hasKey("canvas_width") ? stoi(config.find("canvas_width")) : 2500;
	Int_t canHeight = config.hasKey("canvas_height") ? stoi(config.find("canvas_height")) : 1000;
	TCanvas* canvas = new TCanvas("originClustersizeCanvas", "", canWidth, canHeight);

	Double_t leftMargin = config.hasKey("left_margin") ? stod(config.find("left_margin")) : .07;
	Double_t rightMargin = config.hasKey("right_margin") ? stod(config.find("right_margin")) : .35;
	Double_t topMargin = config.hasKey("top_margin") ? stod(config.find("top_margin")) : .08;
	Double_t bottomMargin = config.hasKey("bottom_margin") ? stod(config.find("bottom_margin")) : .12;
	canvas->SetMargin(leftMargin, rightMargin, bottomMargin, topMargin);

	Double_t xTitleOffset = config.hasKey("x_title_offset") ? stod(config.find("x_title_offset")) : 1.4;
	Double_t xLabelOffset = config.hasKey("x_label_offset") ? stod(config.find("x_label_offset")) : 0.003;
	Double_t yTitleOffset = config.hasKey("y_title_offset") ? stod(config.find("y_title_offset")) : 1.;
	Double_t yLabelOffset = config.hasKey("y_label_offset") ? stod(config.find("y_label_offset")) : 0.;
	mOriginClustersize->GetXaxis()->SetTitleOffset(xTitleOffset);
	mOriginClustersize->GetXaxis()->SetLabelOffset(xLabelOffset);
	mOriginClustersize->GetYaxis()->SetTitleOffset(yTitleOffset);
	mOriginClustersize->GetYaxis()->SetLabelOffset(yLabelOffset);

	std::string originHitmapFileName = config.hasKey("filename") ? config.find("filename") : "High_Hit_N_Cluster_Cut_Clustermap.png";
	std::filesystem::path tempPath = (mOutputPath / originHitmapFileName);
	std::filesystem::create_directories(tempPath.parent_path());
	mOriginClustersize->Draw();
	mExpSettingLegend->Draw("SAME");
	canvas->SetLogy();
	canvas->SaveAs(static_cast<TString>(tempPath));

	if ( isGraphFile ) {
		mGraphFile->cd();
		mOriginClustersize->Write();
	}

	delete canvas;
	canvas = nullptr;
}

void TAnalyser::getHHCutClustersize() {
	CppConfigDictionary config = mConfigFile.getConfig("High_Hit_Cut_Clustersize");

	std::string plotTitle = config.hasKey("title") ? config.find("title") : "";
	mHHCutClustersize->SetTitle(static_cast<TString>(plotTitle));

	Int_t canWidth = config.hasKey("canvas_width") ? stoi(config.find("canvas_width")) : 2500;
	Int_t canHeight = config.hasKey("canvas_height") ? stoi(config.find("canvas_height")) : 1000;
	TCanvas* canvas = new TCanvas("originClustersizeCanvas", "", canWidth, canHeight);

	Double_t leftMargin = config.hasKey("left_margin") ? stod(config.find("left_margin")) : .07;
	Double_t rightMargin = config.hasKey("right_margin") ? stod(config.find("right_margin")) : .35;
	Double_t topMargin = config.hasKey("top_margin") ? stod(config.find("top_margin")) : .08;
	Double_t bottomMargin = config.hasKey("bottom_margin") ? stod(config.find("bottom_margin")) : .12;
	canvas->SetMargin(leftMargin, rightMargin, bottomMargin, topMargin);

	Double_t xTitleOffset = config.hasKey("x_title_offset") ? stod(config.find("x_title_offset")) : 1.4;
	Double_t xLabelOffset = config.hasKey("x_label_offset") ? stod(config.find("x_label_offset")) : 0.003;
	Double_t yTitleOffset = config.hasKey("y_title_offset") ? stod(config.find("y_title_offset")) : 1.;
	Double_t yLabelOffset = config.hasKey("y_label_offset") ? stod(config.find("y_label_offset")) : 0.;
	mHHCutClustersize->GetXaxis()->SetTitleOffset(xTitleOffset);
	mHHCutClustersize->GetXaxis()->SetLabelOffset(xLabelOffset);
	mHHCutClustersize->GetYaxis()->SetTitleOffset(yTitleOffset);
	mHHCutClustersize->GetYaxis()->SetLabelOffset(yLabelOffset);

	std::string HHCutFileName = config.hasKey("filename") ? config.find("filename") : "High_Hit_N_Cluster_Cut_Clustermap.png";
	std::filesystem::path tempPath = (mOutputPath / HHCutFileName);
	std::filesystem::create_directories(tempPath.parent_path());
	mHHCutClustersize->Draw();
	mExpSettingLegend->Draw("SAME");
	canvas->SetLogy();
	canvas->SaveAs(static_cast<TString>(tempPath));

	if ( isGraphFile ) {
		mGraphFile->cd();
		mHHCutClustersize->Write();
	}

	delete canvas;
	canvas = nullptr;
}

void TAnalyser::getHHHCCutClustersize() {
	CppConfigDictionary config = mConfigFile.getConfig("High_Hit_N_Cluster_Cut_Clustersize");

	std::string plotTitle = config.hasKey("title") ? config.find("title") : "";
	mHHHCCutClustersize->SetTitle(static_cast<TString>(plotTitle));

	Int_t canWidth = config.hasKey("canvas_width") ? stoi(config.find("canvas_width")) : 2500;
	Int_t canHeight = config.hasKey("canvas_height") ? stoi(config.find("canvas_height")) : 1000;
	TCanvas* canvas = new TCanvas("originClustersizeCanvas", "", canWidth, canHeight);

	Double_t leftMargin = config.hasKey("left_margin") ? stod(config.find("left_margin")) : .07;
	Double_t rightMargin = config.hasKey("right_margin") ? stod(config.find("right_margin")) : .35;
	Double_t topMargin = config.hasKey("top_margin") ? stod(config.find("top_margin")) : .08;
	Double_t bottomMargin = config.hasKey("bottom_margin") ? stod(config.find("bottom_margin")) : .12;
	canvas->SetMargin(leftMargin, rightMargin, bottomMargin, topMargin);

	Double_t xTitleOffset = config.hasKey("x_title_offset") ? stod(config.find("x_title_offset")) : 1.4;
	Double_t xLabelOffset = config.hasKey("x_label_offset") ? stod(config.find("x_label_offset")) : 0.003;
	Double_t yTitleOffset = config.hasKey("y_title_offset") ? stod(config.find("y_title_offset")) : 1.;
	Double_t yLabelOffset = config.hasKey("y_label_offset") ? stod(config.find("y_label_offset")) : 0.;
	mHHHCCutClustersize->GetXaxis()->SetTitleOffset(xTitleOffset);
	mHHHCCutClustersize->GetXaxis()->SetLabelOffset(xLabelOffset);
	mHHHCCutClustersize->GetYaxis()->SetTitleOffset(yTitleOffset);
	mHHHCCutClustersize->GetYaxis()->SetLabelOffset(yLabelOffset);

	std::string HHHCCutFileName = config.hasKey("filename") ? config.find("filename") : "High_Hit_N_Cluster_Cut_Clustermap.png";
	std::filesystem::path tempPath = (mOutputPath / HHHCCutFileName);
	std::filesystem::create_directories(tempPath.parent_path());
	mHHHCCutClustersize->Draw();
	mExpSettingLegend->Draw("SAME");
	canvas->SetLogy();
	canvas->SaveAs(static_cast<TString>(tempPath));

	if ( isGraphFile ) {
		mGraphFile->cd();
		mHHHCCutClustersize->Write();
	}

	delete canvas;
	canvas = nullptr;
}

