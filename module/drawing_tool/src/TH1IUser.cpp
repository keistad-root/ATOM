#include "TH1IUser.h"

TH1IUser::TH1IUser(const CppConfigDictionary& config, const std::vector<std::pair<std::string, std::vector<int>>>& data) : mConfig(config) {
	getFilePath();
	getTitle();
	for ( const std::pair<std::string, std::vector<int>>& d : data ) {
		dataList.push_back({d.first, d.second});
	}
	fillData();
	drawCanvas();
}

void TH1IUser::getFilePath() {
	std::filesystem::path outputPath = mConfig.hasKey("output_path") ? mConfig.find("output_path") : ".";
	if ( mConfig.hasKey("subdirectory") ) {
		outputPath /= mConfig.find("subdirectory");
	}
	std::string fileName = mConfig.hasKey("filename") ? mConfig.find("filename") + ".png" : "histogram.png";
	outputPath /= fileName;
	if ( mConfig.hasKey("extension") ) {
		outputPath.replace_extension(mConfig.find("extension"));
	}
	mFilePath = std::string(outputPath);
}

void TH1IUser::getTitle() {
	std::string title = mConfig.hasKey("title") ? mConfig.find("title") : "";
	std::string xTitle = mConfig.hasKey("x_title") ? mConfig.find("x_title") : "";
	std::string yTitle = mConfig.hasKey("y_title") ? mConfig.find("y_title") : "";

	mTitle = title + ";" + xTitle + ";" + yTitle;
}

void TH1IUser::fillData() {
	for ( const auto& data : dataList ) {
		Int_t xMin = mConfig.hasKey("x_min") ? stoi(mConfig.find("x_min")) : 1000;
		Int_t xMax = mConfig.hasKey("x_max") ? stoi(mConfig.find("x_max")) : 0;
		Int_t nBin = mConfig.hasKey("n_bin") ? stoi(mConfig.find("n_bin")) : xMax - xMin;
		TH1I* tempHist = new TH1I(static_cast<TString>("th1i_" + std::to_string(iHist)), mTitle, nBin, xMin, xMax);
		iHist++;
		int i = 0;
		for ( const int value : data.second ) {
			tempHist->SetBinContent(i, value);
			i++;
		}
		histList.push_back({data.first, tempHist});
	}
}

void TH1IUser::drawCanvas() {
	int canWidth = mConfig.hasKey("canvas_width") ? stoi(mConfig.find("canvas_width")) : 1500;
	int canHeight = mConfig.hasKey("canvas_height") ? stoi(mConfig.find("canvas_height")) : 1000;
	can = new TCanvas("can", "", canWidth, canHeight);
	for ( const auto& hist : histList ) {
		hist.second->Draw();
	}
	can->SaveAs(mFilePath);
}