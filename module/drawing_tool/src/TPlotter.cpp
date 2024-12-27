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
}

void TPlotter::initHist(TH1* hist, const CppConfigDictionary& config) {
	std::vector<double> set = {1, 0, 1};
	if ( config.hasKey("bins") ) {
		set = getDoubleSetFromString(config.find("bins"));
	}
	hist->SetBins(static_cast<int>(set[0]), set[1], set[2]);
}

void TPlotter::initHist(std::unique_ptr<TH1D>& hist, const CppConfigDictionary& config) {
	std::vector<double> set = {1, 0, 1};
	if ( config.hasKey("bins") ) {
		set = getDoubleSetFromString(config.find("bins"));
	}
	hist->SetBins(static_cast<int>(set[0]), set[1], set[2]);
	if ( config.hasKey("name") ) {
		hist->SetName(static_cast<TString>(config.find("name")));
	}
}

void TPlotter::initHist(TH2* hist, const CppConfigDictionary& config) {
	std::vector<double> set = {1, 0, 1, 1, 0, 1};
	if ( config.hasKey("bins") ) {
		set = getDoubleSetFromString(config.find("bins"));
	}
	hist->SetBins(static_cast<int>(set[0]), set[1], set[2], static_cast<int>(set[3]), set[4], set[5]);
}

void TPlotter::initHist(std::unique_ptr<TH2D>& hist, const CppConfigDictionary& config) {
	std::vector<double> set = {1, 0, 1, 1, 0, 1};
	if ( config.hasKey("bins") ) {
		set = getDoubleSetFromString(config.find("bins"));
	}
	hist->SetBins(static_cast<int>(set[0]), set[1], set[2], static_cast<int>(set[3]), set[4], set[5]);
	if ( config.hasKey("name") ) {
		hist->SetName(static_cast<TString>(config.find("name")));
	}
}

void TPlotter::savePlot(TCanvas* canvas, TH1* plot, const CppConfigDictionary& config) {
	setTitle(plot, config);
	setXRange(plot, config);
	setYRange(plot, config);
	setAttribute(plot, config);

	plot->Draw("SAME HIST");
	setMargin(canvas, config);
}

void TPlotter::savePlot(TCanvas* canvas, TH2* plot, const CppConfigDictionary& config) {
	setTitle(plot, config);
	setXRange(plot, config);
	setYRange(plot, config);
	setZRange(plot, config);
	setAttribute(plot, config);

	drawPlot(canvas, plot, "COLZ");
	setMargin(canvas, config);
}

void TPlotter::savePlot(TCanvas* canvas, TMultiGraph* plot, const CppConfigDictionary& config) {
	setTitle(plot, config);
	setXRange(plot, config);
	setYRange(plot, config);

	plot->Draw("APL");
	setMargin(canvas, config);
}


void TPlotter::savePlot(TCanvas* canvas, TH1* plot, const std::string& configName) {
	setTitle(plot, mConfig->getConfig(configName));
	setXRange(plot, mConfig->getConfig(configName));
	setYRange(plot, mConfig->getConfig(configName));
	setAttribute(plot, mConfig->getConfig(configName));

	plot->Draw("COLZ0");
	setMargin(canvas, mConfig->getConfig(configName));
}

void TPlotter::addLegend(TCanvas* canvas, TLegend*& legend, const CppConfigDictionary& config) {
	std::vector<double> position = {0, 0, 0, 0};
	if ( config.hasKey("legend_position") ) {
		position = getDoubleSetFromString(config.find("legend_position"));
	}
	legend = new TLegend(position[0], position[1], position[2], position[3]);
	gStyle->SetOptStat(0);
	if ( config.hasKey("legend_title") ) {
		legend->SetHeader(static_cast<TString>(config.find("legend_title")), "C");
	}
}

// void TPlotter::savePlot(TH1* plot, const std::string& configName) {
// 	TCanvas* canvas = new TCanvas("canvas", "", CANVAS_WIDTH, CANVAS_HEIGHT);
// 	setTitle(plot, mConfig->getConfig(configName));
// 	// setBins(plot, mConfig->getConfig(configName));
// 	setXRange(plot, mConfig->getConfig(configName));
// 	setYRange(plot, mConfig->getConfig(configName));

// 	if ( mConfig->getConfig(configName).hasKey("type") ) {
// 		TString type = mConfig->getConfig(configName).find("type");
// 		plot->Draw(type);
// 	} else {
// 		plot->Draw();
// 	}
// 	setMargin(canvas, mConfig->getConfig(configName));
// 	saveCanvas(canvas, mOutputPath, mConfig->getConfig(configName));

// 	delete canvas;
// 	canvas = nullptr;
// }

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


// void TPlotter::savePlot(TH2* plot, const std::string& configName) {
// 	TCanvas* canvas = new TCanvas("canvas", "", CANVAS_WIDTH, CANVAS_HEIGHT);
// 	setTitle(plot, mConfig->getConfig(configName));
// 	setXRange(plot, mConfig->getConfig(configName));
// 	setYRange(plot, mConfig->getConfig(configName));
// 	setZRange(plot, mConfig->getConfig(configName));

// 	if ( mConfig->getConfig(configName).hasKey("type") ) {
// 		TString type = mConfig->getConfig(configName).find("type");
// 		plot->Draw(type);
// 	} else {
// 		plot->Draw();
// 	}

// 	saveCanvas(canvas, mOutputPath, mConfig->getConfig(configName));

// 	delete canvas;
// 	canvas = nullptr;
// }

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



void TPlotter::saveLegend(TCanvas* canvas, TLegend* legend) {
	canvas->cd();
	legend->Draw("SAME");
}

void TPlotter::setCanvasAttribute(TCanvas* canvas, const CppConfigDictionary& config) {
	TObject* firstObject = canvas->GetListOfPrimitives()->At(0);
	if ( firstObject->InheritsFrom("TH1") ) {
		TH1* hist = static_cast<TH1*>(firstObject);
		setTitle(hist, config);
		setXRange(hist, config);
		setYRange(hist, config);
	} else if ( firstObject->InheritsFrom("TH2") ) {
		TH2* hist = static_cast<TH2*>(firstObject);
		setTitle(hist, config);
		setXRange(hist, config);
		setYRange(hist, config);
		setZRange(hist, config);
	} else if ( firstObject->InheritsFrom("TGraph") ) {
		TGraph* graph = static_cast<TGraph*>(firstObject);
		setTitle(graph, config);
		setXRange(graph, config);
		setYRange(graph, config);
	} else if ( firstObject->InheritsFrom("TMultiGraph") ) {
		TMultiGraph* multiGraph = static_cast<TMultiGraph*>(firstObject);
		setTitle(multiGraph, config);
		setXRange(multiGraph, config);
		setYRange(multiGraph, config);
	}
	if ( config.hasKey("margin") ) {
		std::vector<double> marginSet = getDoubleSetFromString(config.find("margin"));
		canvas->SetMargin(marginSet[0], marginSet[1], marginSet[2], marginSet[3]);
	}
	if ( config.hasKey("logx") && config.find("logx") == "true" ) {
		canvas->SetLogx();
	}
	if ( config.hasKey("logy") && config.find("logy") == "true" ) {
		canvas->SetLogy();
	}
	if ( config.hasKey("grid") && config.find("grid") == "true" ) {
		canvas->SetGrid();
	}
}


void TPlotter::setCanvasAttribute(std::unique_ptr<TCanvas>& canvas, const CppConfigDictionary& config) {
	TObject* firstObject = canvas->GetListOfPrimitives()->At(0);
	if ( firstObject->InheritsFrom("TH1") ) {
		TH1* hist = static_cast<TH1*>(firstObject);
		setTitle(hist, config);
		setXRange(hist, config);
		setYRange(hist, config);
	} else if ( firstObject->InheritsFrom("TH2") ) {
		TH2* hist = static_cast<TH2*>(firstObject);
		setTitle(hist, config);
		setXRange(hist, config);
		setYRange(hist, config);
		setZRange(hist, config);
	} else if ( firstObject->InheritsFrom("TGraph") ) {
		TGraph* graph = static_cast<TGraph*>(firstObject);
		setTitle(graph, config);
		setXRange(graph, config);
		setYRange(graph, config);
	} else if ( firstObject->InheritsFrom("TMultiGraph") ) {
		TMultiGraph* multiGraph = static_cast<TMultiGraph*>(firstObject);
		setTitle(multiGraph, config);
		setXRange(multiGraph, config);
		setYRange(multiGraph, config);
	}
	if ( config.hasKey("margin") ) {
		std::vector<double> marginSet = getDoubleSetFromString(config.find("margin"));
		canvas->SetMargin(marginSet[0], marginSet[1], marginSet[2], marginSet[3]);
	}
	if ( config.hasKey("logx") && config.find("logx") == "true" ) {
		canvas->SetLogx();
	}
	if ( config.hasKey("logy") && config.find("logy") == "true" ) {
		canvas->SetLogy();
	}
	if ( config.hasKey("grid") && config.find("grid") == "true" ) {
		canvas->SetGrid();
	}
}

void TPlotter::saveCanvas(TCanvas* canvas, std::filesystem::path path, const CppConfigDictionary& config) {
	if ( config.hasKey("logx") && config.find("logx") == "true" ) {
		canvas->SetLogx();
	}
	if ( config.hasKey("logy") && config.find("logy") == "true" ) {
		canvas->SetLogy();
	}
	if ( config.hasKey("logz") && config.find("logz") == "true" ) {
		canvas->SetLogz();
	}
	if ( config.hasKey("grid") && config.find("grid") == "true" ) {
		canvas->SetGrid();
	}
	if ( config.hasKey("canvas_size") ) {
		std::vector<double> size = getDoubleSetFromString(config.find("canvas_size"));
		canvas->SetCanvasSize(size[0], size[1]);
	}
	std::string name = config.hasKey("name") ? config.find("name") : "filename";
	std::string extension = config.hasKey("extension") ? config.find("extension") : EXTENSION;
	TString savePath = static_cast<TString>((path / name).replace_extension(extension));
	canvas->SaveAs(savePath);
}