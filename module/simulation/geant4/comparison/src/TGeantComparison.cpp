#include "TGeantComparison.h"

TGeantComparison::TGeantComparison(const CppConfigFile& config) : TPlotter(), mConfig(config) {
	mOutputPath = mConfig.getConfig("File").find("output_directory");
	std::vector<std::string> fileTagList = mConfig.getConfig("FileList").getKeyList();
	for ( const auto& fileTag : fileTagList ) {
		TString filePath = mConfig.getConfig("FileList").find(fileTag);
		TFile* file = new TFile(filePath, "READ");
		mFileMap.insert_or_assign(fileTag, file);
	}
}

TGeantComparison::~TGeantComparison() { }

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
