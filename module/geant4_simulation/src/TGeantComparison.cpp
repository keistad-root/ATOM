#include "TGeantComparison.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"

const int REF_PHI = 11;

TGeantComparison::TGeantComparison(const CppConfigFile& config) : mConfig(config) {
	mOutputPath = mConfig.getConfig("File").find("output_directory");
	std::vector<std::string> fileTagList = mConfig.getConfig("FileList").getKeyList();
	// for ( const auto& fileTag : fileTagList ) {
	// 	TString filePath = mConfig.getConfig("FileList").find(fileTag);
	// 	TFile* file = new TFile(filePath, "READ");
	// 	mFileMap.insert_or_assign(fileTag, file);
	// }
}

TGeantComparison::~TGeantComparison() { }

void TGeantComparison::getComparedPlot(const std::string& configName) {
	io::CSVReader<3> dataCSV(mConfig.getConfig("File").find("data_file"));

	std::string xValue = mConfig.getConfig(configName).find("x_value");
	std::string yValue = mConfig.getConfig(configName).find("y_value");

	dataCSV.read_header(io::ignore_extra_column, "Tag", xValue, yValue);

	std::vector<std::string> interestSet;
	std::string interestString = mConfig.getConfig(configName).find("interest");
	std::stringstream interestStream(interestString);
	std::string interest;

	while ( std::getline(interestStream, interest, ' ') ) {
		interestSet.push_back(interest);
	}

	std::string tag;
	double x, y;

	TGraph* graph = new TGraph();
	while ( dataCSV.read_row(tag, x, y) ) {
		if ( std::find(interestSet.begin(), interestSet.end(), tag) != interestSet.end() ) {
			graph->SetPoint(graph->GetN(), x, y);
		}
	}

	TF1* fitFunc = new TF1("fitFunc", "[0]*x + [1]", 0.9, 1.1);
	graph->Fit("fitFunc", "R");

	TText* slopeText = new TText(.55, .11, TString::Format("Slope: %.2f", fitFunc->GetParameter(0)));
	slopeText->SetNDC();
	slopeText->SetTextSize(1. / 16);

	TCanvas* canvas = new TCanvas();
	TPlotter::drawPlot(canvas, graph, mConfig.getConfig(configName));
	fitFunc->Draw("same");
	slopeText->Draw();
	TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig(configName));
}

void TGeantComparison::getPlotNormalized() {
	io::CSVReader<3> dataCSV(mConfig.getConfig("File").find("data_file"));

	std::string xValue = mConfig.getConfig("NormalizedPlot").find("x_value");
	std::string yValue = mConfig.getConfig("NormalizedPlot").find("y_value");

	dataCSV.read_header(io::ignore_extra_column, "Tag", xValue, yValue);

	std::vector<std::array<double, 2>> interestData;
	std::array<double, 2> referenceData;

	std::string tag;
	double x, y;

	std::vector<std::string> interestSet;
	std::string interestString = mConfig.getConfig("NormalizedPlot").find("interest");
	std::stringstream interestStream(interestString);
	std::string interest;

	while ( std::getline(interestStream, interest, ' ') ) {
		interestSet.push_back(interest);
	}

	while ( dataCSV.read_row(tag, x, y) ) {
		if ( std::find(interestSet.begin(), interestSet.end(), tag) != interestSet.end() ) {
			interestData.push_back({x, y});
		} else if ( tag == mConfig.getConfig("NormalizedPlot").find("reference") ) {
			referenceData = {x, y};
		}
	}

	TGraph* graph = new TGraph();
	for ( long unsigned int i = 0; i < interestData.size(); i++ ) {
		double xValue = interestData[i][0];
		double yValue = interestData[i][1] / referenceData[1];
		graph->SetPoint(i, xValue, yValue);
	}

	TCanvas* canvas = new TCanvas();
	TPlotter::drawPlot(canvas, graph, mConfig.getConfig("NormalizedPlot"));
	TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("NormalizedPlot"));
}


void TGeantComparison::getDividePlot() {
	TH2D* interestHist = (TH2D*) mFileMap.find(mConfig.getConfig("EpitaxialRatio").find("interest_plot"))->second->Get("AlphaCorrelationDepositEnergyEpitaxialAndIncidentAngle");
	TH2D* referenceHist = (TH2D*) mFileMap.find(mConfig.getConfig("EpitaxialRatio").find("reference_plot"))->second->Get("AlphaCorrelationDepositEnergyEpitaxialAndIncidentAngle");

	TH2D* resultHist = new TH2D("resultHist", "", 90, 0, 90, 600, 0, 6);
	for ( int i = 1; i < 90; i++ ) {
		for ( int j = 1; j < 600; j++ ) {
			double interestValue = interestHist->GetBinContent(i, j);
			double referenceValue = referenceHist->GetBinContent(i, j);
			int resultValue = interestValue / referenceValue;
			if ( (int) referenceValue != 0 ) {
				resultHist->SetBinContent(i, j, resultValue);
			}
		}
	}

	TCanvas* canvas = new TCanvas();
	TPlotter::drawPlot(canvas, resultHist, mConfig.getConfig("EpitaxialRatio"));
	TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("EpitaxialRatio"));
}
