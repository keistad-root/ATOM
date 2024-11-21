#include "TPlotter.h"

TPlotter::TPlotter(const CppConfigFile* config) : mConfig(config) {
	// gStyle->SetOptStat(0);
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
	defineColour();
}

void TPlotter::savePlot(TCanvas* canvas, TH1* plot, const CppConfigDictionary& config) {
	setTitle(plot, config);
	// setBins(plot, config);
	setXRange(plot, config);
	setYRange(plot, config);
	setLineColour(plot, config);

	plot->Draw("SAME HIST");
	setMargin(canvas, config);
}


void TPlotter::savePlot(TCanvas* canvas, TH1* plot, const std::string& configName) {
	setTitle(plot, mConfig->getConfig(configName));
	setXRange(plot, mConfig->getConfig(configName));
	setYRange(plot, mConfig->getConfig(configName));
	setLineColour(plot, mConfig->getConfig(configName));

	plot->Draw("COLZ0");
	setMargin(canvas, mConfig->getConfig(configName));
}

void TPlotter::savePlot(TH1* plot, const std::string& configName) {
	TCanvas* canvas = new TCanvas("canvas", "", CANVAS_WIDTH, CANVAS_HEIGHT);
	setTitle(plot, mConfig->getConfig(configName));
	// setBins(plot, mConfig->getConfig(configName));
	setXRange(plot, mConfig->getConfig(configName));
	setYRange(plot, mConfig->getConfig(configName));

	if ( mConfig->getConfig(configName).hasKey("type") ) {
		TString type = mConfig->getConfig(configName).find("type");
		plot->Draw(type);
	} else {
		plot->Draw();
	}
	setMargin(canvas, mConfig->getConfig(configName));
	saveCanvas(canvas, mOutputPath, mConfig->getConfig(configName));

	delete canvas;
	canvas = nullptr;
}

void TPlotter::savePlot(TCanvas* canvas, TH2* plot, const std::string& configName) {
	setTitle(plot, mConfig->getConfig(configName));
	setXRange(plot, mConfig->getConfig(configName));
	setYRange(plot, mConfig->getConfig(configName));
	setZRange(plot, mConfig->getConfig(configName));

	if ( mConfig->getConfig(configName).hasKey("type") ) {
		TString type = mConfig->getConfig(configName).find("type");
		plot->Draw(type);
	} else {
		plot->Draw();
	}
}


void TPlotter::savePlot(TH2* plot, const std::string& configName) {
	TCanvas* canvas = new TCanvas("canvas", "", CANVAS_WIDTH, CANVAS_HEIGHT);
	setTitle(plot, mConfig->getConfig(configName));
	setXRange(plot, mConfig->getConfig(configName));
	setYRange(plot, mConfig->getConfig(configName));
	setZRange(plot, mConfig->getConfig(configName));

	if ( mConfig->getConfig(configName).hasKey("type") ) {
		TString type = mConfig->getConfig(configName).find("type");
		plot->Draw(type);
	} else {
		plot->Draw();
	}

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

void TPlotter::saveCanvas(TCanvas* canvas, std::filesystem::path path, const CppConfigDictionary& config) {
	if ( config.hasKey("logy") && config.find("logy") == "true" ) {
		canvas->SetLogy();
	}
	// gStyle->SetOptStat(0);

	std::string name = config.hasKey("name") ? config.find("name") : "file";
	TString savePath = static_cast<TString>((path / name).replace_extension(EXTENSION));
	canvas->SaveAs(savePath);
}