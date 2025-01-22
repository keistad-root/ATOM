#include "TGeantComparison.h"

const int REF_PHI = 11;

TGeantComparison::TGeantComparison(const CppConfigFile& config) : TPlotter(), mConfig(config) {
	mOutputPath = mConfig.getConfig("File").find("output_directory");
	std::vector<std::string> fileTagList = mConfig.getConfig("FileList").getKeyList();
	// for ( const auto& fileTag : fileTagList ) {
	// 	TString filePath = mConfig.getConfig("FileList").find(fileTag);
	// 	TFile* file = new TFile(filePath, "READ");
	// 	mFileMap.insert_or_assign(fileTag, file);
	// }
}

TGeantComparison::~TGeantComparison() { }

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

	std::unique_ptr<TGraph> graph = std::make_unique<TGraph>();
	for ( int i = 0; i < interestData.size(); i++ ) {
		double xValue = interestData[i][0];
		double yValue = interestData[i][1] / referenceData[1];
		graph->SetPoint(i, xValue, yValue);
	}

	std::unique_ptr<TCanvas> canvas = std::make_unique<TCanvas>();
	savePlot(canvas, graph, mConfig.getConfig("NormalizedPlot"));
	setCanvasAttribute(canvas, mConfig.getConfig("NormalizedPlot"));
	saveCanvas(canvas.get(), mOutputPath, mConfig.getConfig("NormalizedPlot"));
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

	std::unique_ptr<TCanvas> canvas = std::make_unique<TCanvas>();
	savePlot(canvas.get(), resultHist, mConfig.getConfig("EpitaxialRatio"));
	setCanvasAttribute(canvas, mConfig.getConfig("EpitaxialRatio"));
	saveCanvas(canvas.get(), mOutputPath, mConfig.getConfig("EpitaxialRatio"));
}
