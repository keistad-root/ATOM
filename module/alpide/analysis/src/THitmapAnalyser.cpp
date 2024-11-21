#define __TANALYSER_HEADERS__

#include "TAnalyser.h"


void TAnalyser::getHitmaps() {
	CONSTANT::ALPIDE alpideConstants;
	Int_t rawBins = alpideConstants.ROW;
	Int_t rawMin = 0;
	Int_t rawMax = alpideConstants.ROW;
	Int_t columnBins = alpideConstants.COLUMN;
	Int_t columnMin = 0;
	Int_t columnMax = alpideConstants.COLUMN;

	mOriginHitmap = new TH2D("originHitmap", "Origin Hitmap", rawBins, rawMin, rawMax, columnBins, columnMin, columnMax);
	for ( const TALPIDEEvent* event : mOriginEventSet ) {
		for ( const std::pair<int, int>& pixel : event->getData() ) {
			mOriginHitmap->Fill(pixel.first, pixel.second);
		}
	}
	mHitmapMaskedNoisemap = new TH2D("highHitCutNoisemap", "", rawBins, rawMin, rawMax, columnBins, columnMin, columnMax);
	mHitmapMaskedHitmap = new TH2D("highHitCutHitmap", "", rawBins, rawMin, rawMax, columnBins, columnMin, columnMax);

	doMasking();

	for ( const TALPIDEEvent* event : mHHCutEventSet ) {
		for ( const std::pair<int, int>& pixel : event->getData() ) {
			mHitmapMaskedHitmap->Fill(pixel.first, pixel.second);
		}
	}

	for ( const TALPIDEEvent* event : mHHCutNoiseEventSet ) {
		for ( const std::pair<int, int>& pixel : event->getData() ) {
			mHitmapMaskedNoisemap->Fill(pixel.first, pixel.second);
		}
	}
	if ( mConfigFile.hasConfig("Origin_Hitmap") ) getOriginHitmap();
	if ( mConfigFile.hasConfig("High_Hit_Cut_Hitmap") ) getHitmapMaskedHitmap();
	if ( mConfigFile.hasConfig("Hit_Masked_Noisemap") ) getHitmapMaskedNoisemap();
}


void TAnalyser::getOriginHitmap() {
	CppConfigDictionary mOriginHitmapConfig = mConfigFile.getConfig("Origin_Hitmap");

	std::string plotTitle = mOriginHitmapConfig.hasKey("title") ? mOriginHitmapConfig.find("title") : "";
	mOriginHitmap->SetTitle(static_cast<TString>(plotTitle));
	CONSTANT::HITMAP hitmapConstants;
	if ( mOriginHitmapConfig.hasKey("canvas_width") ) {
		hitmapConstants.CANVAS_WIDTH = stoi(mOriginHitmapConfig.find("canvas_width"));
	}
	// Int_t canWidth = mOriginHitmapConfig.hasKey("canvas_width") ? stoi(mOriginHitmapConfig.find("canvas_width")) : 2500;
	Int_t canHeight = mOriginHitmapConfig.hasKey("canvas_height") ? stoi(mOriginHitmapConfig.find("canvas_height")) : 1000;
	TCanvas* canvas = new TCanvas("ohc", "", hitmapConstants.CANVAS_WIDTH, canHeight);

	Double_t leftMargin = mOriginHitmapConfig.hasKey("left_margin") ? stod(mOriginHitmapConfig.find("left_margin")) : .07;
	Double_t rightMargin = mOriginHitmapConfig.hasKey("right_margin") ? stod(mOriginHitmapConfig.find("right_margin")) : .35;
	Double_t topMargin = mOriginHitmapConfig.hasKey("top_margin") ? stod(mOriginHitmapConfig.find("top_margin")) : .08;
	Double_t bottomMargin = mOriginHitmapConfig.hasKey("bottom_margin") ? stod(mOriginHitmapConfig.find("bottom_margin")) : .12;
	canvas->SetMargin(leftMargin, rightMargin, bottomMargin, topMargin);

	Double_t xTitleOffset = mOriginHitmapConfig.hasKey("x_title_offset") ? stod(mOriginHitmapConfig.find("x_title_offset")) : 1.4;
	Double_t xLabelOffset = mOriginHitmapConfig.hasKey("x_label_offset") ? stod(mOriginHitmapConfig.find("x_label_offset")) : 0.003;
	Double_t yTitleOffset = mOriginHitmapConfig.hasKey("y_title_offset") ? stod(mOriginHitmapConfig.find("y_title_offset")) : 1.;
	Double_t yLabelOffset = mOriginHitmapConfig.hasKey("y_label_offset") ? stod(mOriginHitmapConfig.find("y_label_offset")) : 0.;
	mOriginHitmap->GetXaxis()->SetTitleOffset(xTitleOffset);
	mOriginHitmap->GetXaxis()->SetLabelOffset(xLabelOffset);
	mOriginHitmap->GetYaxis()->SetTitleOffset(yTitleOffset);
	mOriginHitmap->GetYaxis()->SetLabelOffset(yLabelOffset);

	std::string originHitmapFileName = mOriginHitmapConfig.hasKey("filename") ? mOriginHitmapConfig.find("filename") : "origin_hitmap.png";
	std::filesystem::path tempPath = (mOutputPath / originHitmapFileName);
	std::filesystem::create_directories(tempPath.parent_path());
	std::filesystem::path colzPath = (tempPath.parent_path() / (tempPath.stem().string() + "_colz")).replace_extension(tempPath.extension());
	mOriginHitmap->Draw("COLZ");
	if ( isExpSetting ) {
		mExpSettingLegend->Draw("SAME");
	}
	canvas->SaveAs(static_cast<TString>(colzPath));

	std::filesystem::path legoPath = (tempPath.parent_path() / (tempPath.stem().string() + "_lego")).replace_extension(tempPath.extension());
	mOriginHitmap->Draw("LEGO0");
	if ( isExpSetting ) {
		mExpSettingLegend->Draw("SAME");
	}
	canvas->SaveAs(static_cast<TString>(legoPath));

	if ( isGraphFile ) {
		mGraphFile->cd();
		mOriginHitmap->Write();
	}

	delete canvas;
	canvas = nullptr;
}

void TAnalyser::getHitmapMaskedHitmap() {
	CppConfigDictionary config = mConfigFile.getConfig("High_Hit_Cut_Hitmap");

	std::string plotTitle = config.hasKey("title") ? config.find("title") : "";
	mHitmapMaskedHitmap->SetTitle(static_cast<TString>(plotTitle));

	Int_t canWidth = config.hasKey("canvas_width") ? stoi(config.find("canvas_width")) : 2500;
	Int_t canHeight = config.hasKey("canvas_height") ? stoi(config.find("canvas_height")) : 1000;
	TCanvas* canvas = new TCanvas("ohc", "", canWidth, canHeight);

	Double_t leftMargin = config.hasKey("left_margin") ? stod(config.find("left_margin")) : .7;
	Double_t rightMargin = config.hasKey("right_margin") ? stod(config.find("right_margin")) : .35;
	Double_t topMargin = config.hasKey("top_margin") ? stod(config.find("top_margin")) : .08;
	Double_t bottomMargin = config.hasKey("bottom_margin") ? stod(config.find("bottom_margin")) : .12;
	canvas->SetMargin(leftMargin, rightMargin, bottomMargin, topMargin);

	Double_t xTitleOffset = config.hasKey("x_title_offset") ? stod(config.find("x_title_offset")) : 1.4;
	Double_t xLabelOffset = config.hasKey("x_label_offset") ? stod(config.find("x_label_offset")) : 0.003;
	Double_t yTitleOffset = config.hasKey("y_title_offset") ? stod(config.find("y_title_offset")) : 1.;
	Double_t yLabelOffset = config.hasKey("y_label_offset") ? stod(config.find("y_label_offset")) : 0.;
	mHitmapMaskedHitmap->GetXaxis()->SetTitleOffset(xTitleOffset);
	mHitmapMaskedHitmap->GetXaxis()->SetLabelOffset(xLabelOffset);
	mHitmapMaskedHitmap->GetYaxis()->SetTitleOffset(yTitleOffset);
	mHitmapMaskedHitmap->GetYaxis()->SetLabelOffset(yLabelOffset);

	std::string originHitmapFileName = config.hasKey("filename") ? config.find("filename") : "hit_masked_hitmap.png";
	std::filesystem::path tempPath = (mOutputPath / originHitmapFileName);
	std::filesystem::create_directories(tempPath.parent_path());
	std::filesystem::path colzPath = (tempPath.parent_path() / (tempPath.stem().string() + "_colz")).replace_extension(tempPath.extension());
	mHitmapMaskedHitmap->Draw("COLZ");
	mExpSettingLegend->Draw("SAME");
	canvas->SaveAs(static_cast<TString>(colzPath));

	std::filesystem::path legoPath = (tempPath.parent_path() / (tempPath.stem().string() + "_lego")).replace_extension(tempPath.extension());
	mHitmapMaskedHitmap->Draw("LEGO0");
	mExpSettingLegend->Draw("SAME");
	canvas->SaveAs(static_cast<TString>(legoPath));

	if ( isGraphFile ) {
		mGraphFile->cd();
		mHitmapMaskedHitmap->Write();
	}

	delete canvas;
	canvas = nullptr;
}

void TAnalyser::getHitmapMaskedNoisemap() {
	CppConfigDictionary config = mConfigFile.getConfig("High_Hit_Cut_Noise_Hitmap");

	std::string plotTitle = config.hasKey("title") ? config.find("title") : "";
	mHitmapMaskedNoisemap->SetTitle(static_cast<TString>(plotTitle));

	Int_t canWidth = config.hasKey("canvas_width") ? stoi(config.find("canvas_width")) : 2500;
	Int_t canHeight = config.hasKey("canvas_height") ? stoi(config.find("canvas_height")) : 1000;
	TCanvas* canvas = new TCanvas("ohc", "", canWidth, canHeight);

	Double_t leftMargin = config.hasKey("left_margin") ? stod(config.find("left_margin")) : .7;
	Double_t rightMargin = config.hasKey("right_margin") ? stod(config.find("right_margin")) : .35;
	Double_t topMargin = config.hasKey("top_margin") ? stod(config.find("top_margin")) : .08;
	Double_t bottomMargin = config.hasKey("bottom_margin") ? stod(config.find("bottom_margin")) : .12;
	canvas->SetMargin(leftMargin, rightMargin, bottomMargin, topMargin);

	Double_t xTitleOffset = config.hasKey("x_title_offset") ? stod(config.find("x_title_offset")) : 1.4;
	Double_t xLabelOffset = config.hasKey("x_label_offset") ? stod(config.find("x_label_offset")) : 0.003;
	Double_t yTitleOffset = config.hasKey("y_title_offset") ? stod(config.find("y_title_offset")) : 1.;
	Double_t yLabelOffset = config.hasKey("y_label_offset") ? stod(config.find("y_label_offset")) : 0.;
	mHitmapMaskedNoisemap->GetXaxis()->SetTitleOffset(xTitleOffset);
	mHitmapMaskedNoisemap->GetXaxis()->SetLabelOffset(xLabelOffset);
	mHitmapMaskedNoisemap->GetYaxis()->SetTitleOffset(yTitleOffset);
	mHitmapMaskedNoisemap->GetYaxis()->SetLabelOffset(yLabelOffset);

	std::string originHitmapFileName = config.hasKey("filename") ? config.find("filename") : "hit_masked_hitmap.png";
	std::filesystem::path tempPath = (mOutputPath / originHitmapFileName);
	std::filesystem::create_directories(tempPath.parent_path());
	std::filesystem::path colzPath = (tempPath.parent_path() / (tempPath.stem().string() + "_colz")).replace_extension(tempPath.extension());
	mHitmapMaskedNoisemap->Draw("COLZ");
	mExpSettingLegend->Draw("SAME");
	canvas->SaveAs(static_cast<TString>(colzPath));

	std::filesystem::path legoPath = (tempPath.parent_path() / (tempPath.stem().string() + "_lego")).replace_extension(tempPath.extension());
	mHitmapMaskedNoisemap->Draw("LEGO0");
	mExpSettingLegend->Draw("SAME");
	canvas->SaveAs(static_cast<TString>(legoPath));

	if ( isGraphFile ) {
		mGraphFile->cd();
		mHitmapMaskedNoisemap->Write();
	}

	delete canvas;
	canvas = nullptr;
}