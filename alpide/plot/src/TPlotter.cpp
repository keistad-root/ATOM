#include "TPlotter.h"

TPlotter::TPlotter(const CppConfigFile* config) : mConfig(config) {
	try {
		if ( !mConfig->hasConfig("File") ) throw std::string("File");
		CppConfigDictionary fileConfig = mConfig->getConfig("File");
		if ( !fileConfig.hasKey("output_directory") ) throw std::string("output_directory");
		mOutputPath = fileConfig.find("output_directory");
		std::filesystem::create_directories(mOutputPath);
	} catch ( std::string config ) {
		std::cerr << "Config " << config << " is necessary" << std::endl;
		exit(1);
	}
}

void TPlotter::savePlot(TH1* plot, const std::string& configName) {
	TCanvas* canvas = new TCanvas("canvas", "", CANVAS_WIDTH, CANVAS_HEIGHT);
	setTitle(plot, mConfig->getConfig(configName));
	setXRange(plot, mConfig->getConfig(configName));
	setYRange(plot, mConfig->getConfig(configName));

	plot->Draw("COLZ");
	setMargin(canvas, mConfig->getConfig(configName));
	saveCanvas(canvas, mOutputPath, mConfig->getConfig(configName));

	delete canvas;
	canvas = nullptr;
}

void TPlotter::savePlot(TH2* plot, const std::string& configName) {
	TCanvas* canvas = new TCanvas("canvas", "", CANVAS_WIDTH, CANVAS_HEIGHT);
	setTitle(plot, mConfig->getConfig(configName));
	setXRange(plot, mConfig->getConfig(configName));
	setYRange(plot, mConfig->getConfig(configName));
	setZRange(plot, mConfig->getConfig(configName));

	plot->Draw("LEGO0");

	saveCanvas(canvas, mOutputPath, mConfig->getConfig(configName));

	delete canvas;
	canvas = nullptr;
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

void TPlotter::setXBin(TH1* hist, const CppConfigDictionary& config) {
	if ( config.hasKey("x_bins") ) {
		std::vector<double> set = getDoubleSetFromString(config.find("x_bins"));
		hist->SetBins(static_cast<int>(set[2]), set[0], set[1]);
	}
}
void TPlotter::setXBin(TH2* hist, const CppConfigDictionary& config) {
	if ( config.hasKey("x_bins") ) {
		std::vector<double> set = getDoubleSetFromString(config.find("x_bins"));
		hist->SetBins(static_cast<int>(set[2]), set[0], set[1], hist->GetNbinsY(), hist->GetYaxis()->GetXmin(), hist->GetYaxis()->GetXmax());
	}
}
void TPlotter::setYBin(TH1* hist, const CppConfigDictionary& config) {
	if ( config.hasKey("y_bins") ) {
		std::vector<double> set = getDoubleSetFromString(config.find("y_bins"));
		hist->SetBins(hist->GetNbinsX(), hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax(), static_cast<int>(set[2]), set[0], set[1]);
	}
}
void TPlotter::setYBin(TH2* hist, const CppConfigDictionary& config) {
	if ( config.hasKey("y_bins") ) {
		std::vector<double> set = getDoubleSetFromString(config.find("y_bins"));
		hist->SetBins(hist->GetNbinsX(), hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax(), static_cast<int>(set[2]), set[0], set[1]);
	}
}
void TPlotter::setZBin(TH2* hist, const CppConfigDictionary& config) {
	if ( config.hasKey("z_bins") ) {
		std::vector<double> set = getDoubleSetFromString(config.find("z_bins"));
		hist->SetBins(hist->GetNbinsX(), hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax(), hist->GetNbinsY(), hist->GetYaxis()->GetXmin(), hist->GetYaxis()->GetXmax(), static_cast<int>(set[2]), set[0], set[1]);
	}
}

void TPlotter::setBins(TH1* hist, const CppConfigDictionary& config) {
	setXBin(hist, config);
	setYBin(hist, config);
}
void TPlotter::setBins(TH2* hist, const CppConfigDictionary& config) {
	setXBin(hist, config);
	setYBin(hist, config);
	setZBin(hist, config);
}


void TPlotter::setXRange(TH1* hist, const CppConfigDictionary& config) {
	std::vector<int> range = {0, 50};
	if ( config.hasKey("x_range") ) {
		range = getIntegerSetFromString(config.find("x_range"));
		hist->GetXaxis()->SetRangeUser(range[0], range[1]);
	}
}

void TPlotter::setXRange(TH2* hist, const CppConfigDictionary& config) {
	std::vector<int> range = {0, 50};
	if ( config.hasKey("x_range") ) {
		range = getIntegerSetFromString(config.find("x_range"));
		hist->GetXaxis()->SetRangeUser(range[0], range[1]);
	}
}

void TPlotter::setYRange(TH1* hist, const CppConfigDictionary& config) {
	std::vector<int> range = {0, 50};
	if ( config.hasKey("y_range") ) {
		range = getIntegerSetFromString(config.find("y_range"));
		hist->GetYaxis()->SetRangeUser(range[0], range[1]);
	}
}

void TPlotter::setYRange(TH2* hist, const CppConfigDictionary& config) {
	std::vector<int> range = {0, 50};
	if ( config.hasKey("y_range") ) {
		range = getIntegerSetFromString(config.find("y_range"));
		hist->GetYaxis()->SetRangeUser(range[0], range[1]);
	}
}

void TPlotter::setZRange(TH2* hist, const CppConfigDictionary& config) {
	std::vector<int> range = {0, 50};
	if ( config.hasKey("z_range") ) {
		range = getIntegerSetFromString(config.find("z_range"));
		hist->GetZaxis()->SetRangeUser(range[0], range[1]);
	}
}

void TPlotter::setMargin(TCanvas* canvas, const CppConfigDictionary& config) {
	if ( config.hasKey("margin") ) {
		std::vector<double> marginSet = getDoubleSetFromString(config.find("margin"));
		canvas->SetMargin(marginSet[0], marginSet[1], marginSet[2], marginSet[3]);
	}
}









TString TPlotter::getTitle(const std::string& titleStr) {
	int firstSplit = titleStr.find("\" \"");
	int secondSplit = titleStr.find("\" \"", titleStr.find("\" \"") + 1);
	std::string mainTitle = titleStr.substr(1, firstSplit - 1);
	std::string xTitle = titleStr.substr(firstSplit + 3, secondSplit - firstSplit - 3);
	std::string yTitle = titleStr.substr(secondSplit + 3, titleStr.size() - secondSplit - 4);
	return mainTitle + ";" + xTitle + ";" + yTitle;
}

void TPlotter::setTitle(TH1* hist, const CppConfigDictionary& config) {
	if ( config.hasKey("title") ) {
		TString title = getTitle(config.find("title"));
		hist->SetTitle(title);
	}
}

void TPlotter::setTitle(TH2* hist, const CppConfigDictionary& config) {
	if ( config.hasKey("title") ) {
		TString title = getTitle(config.find("title"));
		hist->SetTitle(title);
	}
}

void TPlotter::saveCanvas(TCanvas* canvas, std::filesystem::path path, const CppConfigDictionary& config) {
	std::string name = config.hasKey("name") ? config.find("name") : "file";
	TString savePath = static_cast<TString>((path / name).replace_extension(EXTENSION));
	canvas->SaveAs(savePath);
}