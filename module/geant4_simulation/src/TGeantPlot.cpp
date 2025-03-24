#include "TGeantPlot.h"

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TList.h"
#include "TKey.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TString.h"
#include "TStyle.h"

std::vector<std::string> volumePlot = {"IN_VOL", "AlphaIncidentVolume", "ElectronIncidentVolume", "ElectronFinalVolume", "SecondaryInALPIDEVolume", "SecondaryInALPIDEDecayVolume", "SecondaryInitVolume", "SecondaryFinalVolume", "GammaIncidentVolume"};
std::vector<std::string> particleNamePlot = {"IN_PA", "SE_PA_IN_ALPIDE", "SecondaryParticle", "SecondaryMotherParticleInALPIDE", "MotherDoughterCorrelationInALPIDE"};

void TGeantPlot::readInputFile(std::filesystem::path inputFilePath) {
	TString inputFileName = std::string(inputFilePath);
	mInputFile = new TFile(inputFileName, "READ");
}

void TGeantPlot::getHistorams() {
	TList* keyList = mInputFile->GetListOfKeys();
	TIter next(keyList);
	TKey* key;

	while ( (key = (TKey*) next()) ) {
		TObject* objPtr = (key->ReadObj());
		if ( objPtr->InheritsFrom(TH1D::Class()) ) {
			std::string keyName = key->GetName();
			m1DHistograms.insert_or_assign(keyName, static_cast<TH1D*>(objPtr));
		}
		if ( objPtr->InheritsFrom(TH2D::Class()) ) {
			std::string keyName = key->GetName();
			m2DHistograms.insert_or_assign(keyName, static_cast<TH2D*>(objPtr));
		}
	}
}

void TGeantPlot::setOutputDirectory(std::string_view outputDirectory) {
	mOutputDirectory = outputDirectory;
}

void TGeantPlot::saveHistorams(const std::vector<CppConfigDictionary>& configList) {
	gStyle->SetOptStat(0);
	for ( const auto& [key, hist] : m1DHistograms ) {
		for ( const CppConfigDictionary& config : configList ) {
			if ( key == config.getConfigName() ) {
				TCanvas* canvas = TPlotter::initCanvas(config);
				TLegend* legend = TPlotter::initLegend(config);
				TPlotter::drawPlot(canvas, hist, config);
				hist->SetEntries(hist->GetEffectiveEntries());
				legend->AddEntry(hist, Form("Entries: %.1f", hist->GetEffectiveEntries()), "");
				legend->AddEntry(hist, Form("Mean: %.3f", hist->GetMean()), "");
				legend->AddEntry(hist, Form("Std Dev: %.3f", hist->GetStdDev()), "");
				legend->AddEntry(hist, Form("Underflow: %.1f", hist->GetBinContent(0)), "");
				legend->AddEntry(hist, Form("Overflow: %.1f", hist->GetBinContent(hist->GetNbinsX() + 1)), "");
				if ( std::find(particleNamePlot.begin(), particleNamePlot.end(), key) != particleNamePlot.end() ) {
					for ( int i = 0; i < hist->GetNbinsX(); i++ ) {
						hist->GetXaxis()->SetBinLabel(i + 1., mParticleName[i]);
					}
				}
				if ( std::find(volumePlot.begin(), volumePlot.end(), key) != volumePlot.end() ) {
					for ( int i = 0; i < hist->GetNbinsX(); i++ ) {
						hist->GetXaxis()->SetBinLabel(i + 1., mVolumeName[i]);
					}
				}
				TPlotter::saveLegend(canvas, legend);
				TPlotter::saveCanvas(canvas, mOutputDirectory, config);
			}
		}
	}
	for ( const auto& [key, hist] : m2DHistograms ) {
		for ( const CppConfigDictionary& config : configList ) {
			if ( key == config.getConfigName() ) {
				TCanvas* canvas = TPlotter::initCanvas(config);
				TLegend* legend = TPlotter::initLegend(config);
				TPlotter::drawPlot(canvas, hist, config);
				legend->AddEntry(hist, Form("Entries: %.1f", hist->GetEffectiveEntries()), "");
				legend->AddEntry(hist, Form("Mean x: %.3f", hist->GetMean(1)), "");
				legend->AddEntry(hist, Form("Mean y: %.3f", hist->GetMean(2)), "");
				legend->AddEntry(hist, Form("Std Dev x: %.3f", hist->GetStdDev(1)), "");
				legend->AddEntry(hist, Form("Std Dev y: %.3f", hist->GetStdDev(2)), "");
				if ( std::find(particleNamePlot.begin(), particleNamePlot.end(), key) != particleNamePlot.end() ) {
					for ( int i = 0; i < hist->GetNbinsX(); i++ ) {
						hist->GetXaxis()->SetBinLabel(i + 1., mParticleName[i]);
						hist->GetYaxis()->SetBinLabel(i + 1., mParticleName[i]);
					}
				}
				if ( key == "ElctronIncidentXYWithElectrode" ) {
					TGraph* electrode = new TGraph();
					for ( int i = 0; i < 10; i++ ) {
						for ( int j = 0; j < 10; j++ ) {
							electrode->SetPoint(electrode->GetN(), -0.126 + i * 0.028, -0.126 + j * 0.028);
						}
					}
					electrode->SetMarkerStyle(20);
					electrode->SetMarkerSize(1);
					electrode->SetMarkerColor(kRed);
					electrode->Draw("P");
					// canvas->SetGrayscale();
					// TColor::InvertPalette();
				}
				TPlotter::saveLegend(canvas, legend);
				TPlotter::saveCanvas(canvas, mOutputDirectory, config);

			}
		}
	}
}

void TGeantPlot::getEntries() {
	// TH1D* hist = m1DHistograms.find("AlphaDepositEnergyEpitaxial")->second;
	// TH1D* metalAlphaHist = m1DHistograms.find("AlphaDepositEnergyMetal")->second;
	// TH1D* metalElectronHist = m1DHistograms.find("ElectronDepositEnergyMetal")->second;

	// int maxBin = 0;
	// for ( int i = 1; i <= m1DHistograms["AlphaIncidentAngle"]->GetNbinsX(); i++ ) {
	// 	if ( m1DHistograms["AlphaIncidentAngle"]->GetBinContent(i) > 1 ) {
	// 		maxBin = i;
	// 	}
	// }
	// std::cout << maxBin << std::endl;
}