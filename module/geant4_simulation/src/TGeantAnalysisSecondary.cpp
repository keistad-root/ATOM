#include "TGeantAnalysisSecondary.h"

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TPlotter.h"
#include "TH1.h"
#include "TH2.h"

#include "TVolumeID.h"

TGeantAnalysisSecondary::TGeantAnalysisSecondary(const CppConfigFile& config) : mConfig(config) { }

TGeantAnalysisSecondary::~TGeantAnalysisSecondary() { }

void TGeantAnalysisSecondary::openFile() {
	static int iFile = 0;
	std::filesystem::path inputFileName = mConfig.getConfig("FILE").find("SIMULATION_FILE");
	TString inputFileNameStr = std::string(inputFileName.stem()) + "_" + std::to_string(iFile) + std::string(inputFileName.extension());

	mInputFile = new TFile(inputFileName, "READ");
	mInputTree = static_cast<TTree*>(mInputFile->Get("trackTree"));

	mInputTree->SetBranchAddress("eventID", &mInputTuple.eventID);
	mInputTree->SetBranchAddress("trackID", &mInputTuple.trackID);
	mInputTree->SetBranchAddress("particleID", &mInputTuple.particleID);
	mInputTree->SetBranchAddress("initialVolumeID", &mInputTuple.initialVolumeID);
	mInputTree->SetBranchAddress("initX", &mInputTuple.initialPosition[0]);
	mInputTree->SetBranchAddress("initY", &mInputTuple.initialPosition[1]);
	mInputTree->SetBranchAddress("initZ", &mInputTuple.initialPosition[2]);
	mInputTree->SetBranchAddress("initPX", &mInputTuple.initialMomentum[0]);
	mInputTree->SetBranchAddress("initPY", &mInputTuple.initialMomentum[1]);
	mInputTree->SetBranchAddress("initPZ", &mInputTuple.initialMomentum[2]);
	mInputTree->SetBranchAddress("initKineticEnergy", &mInputTuple.initialKineticEnergy);
	mInputTree->SetBranchAddress("finalVolumeID", &mInputTuple.finalVolumeID);
	mInputTree->SetBranchAddress("finalX", &mInputTuple.finalPosition[0]);
	mInputTree->SetBranchAddress("finalY", &mInputTuple.finalPosition[1]);
	mInputTree->SetBranchAddress("finalZ", &mInputTuple.finalPosition[2]);
	mInputTree->SetBranchAddress("finalPX", &mInputTuple.finalMomentum[0]);
	mInputTree->SetBranchAddress("finalPY", &mInputTuple.finalMomentum[1]);
	mInputTree->SetBranchAddress("finalPZ", &mInputTuple.finalMomentum[2]);
	mInputTree->SetBranchAddress("finalKineticEnergy", &mInputTuple.finalKineticEnergy);
}

void TGeantAnalysisSecondary::setHistograms() {
	std::vector<CppConfigDictionary> configList = mConfig.getConfigList();
	for ( const CppConfigDictionary& config : configList ) {
		std::string_view key = config.getConfigName();
		if ( config.hasKey("type") && config.find("type") == "1H" ) {
			TH1D* hist = TPlotter::init1DHist(config);
			m1DHistograms.insert_or_assign(key, hist);
		}
		if ( config.hasKey("type") && config.find("type") == "2H" ) {
			TH2D* hist = TPlotter::init2DHist(config);
			m2DHistograms.insert_or_assign(key, hist);
		}
	}
}

void TGeantAnalysisSecondary::readTree() {
	Int_t nEntries = mInputTree->GetEntries();
	for ( Int_t i = 0; i < nEntries; i++ ) {
		mInputTree->GetEntry(i);
		if ( mInputTuple.particleID != 0 ) {
			fillSecondaryHistograms();
		}
	}
}

void TGeantAnalysisSecondary::fillSecondaryHistograms() {
	for ( const auto& [key, hist] : m1DHistograms ) {
		if ( key == "SecondaryParticle" ) {
			hist->Fill(mInputTuple.particleID);
		}
		if ( key == "SecondaryInALPIDEVolume" ) {
			hist->Fill(mInputTuple.initialVolumeID);
		}
		if ( key == "SecondaryInALPIDEDecayVolume" ) {
			hist->Fill(mInputTuple.finalVolumeID);
		}
	}
}

void TGeantAnalysisSecondary::saveFile() {
	std::filesystem::path outputFilePath = mConfig.getConfig("FILE").find("PLOT_FILE");
	TString outputFileName = std::string(outputFilePath);
	TFile outputFile(outputFileName, "RECREATE");
	for ( const auto& [key, hist] : m1DHistograms ) {
		hist->Write(key.data());
	}
	for ( const auto& [key, hist] : m2DHistograms ) {
		hist->Write(key.data());
	}
	outputFile.Close();
}