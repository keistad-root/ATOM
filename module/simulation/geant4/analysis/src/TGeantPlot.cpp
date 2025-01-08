#include "TGeantPlot.h"

#include "TFile.h"
#include "TList.h"
#include "TKey.h"

TGeantPlot::~TGeantPlot() { }

void TGeantPlot::readInputFile(std::filesystem::path inputFilePath) {
	TString inputFileName = std::string(inputFilePath);
	mInputFile = std::make_unique<TFile>(inputFileName, "READ");
}

void TGeantPlot::getHistorams() {
	TList* keyList = mInputFile->GetListOfKeys();
	TIter next(keyList);
	TKey* key;

	while ( (key = (TKey*) next()) ) {
		std::unique_ptr<TObject> objPtr(key->ReadObj());
		if ( objPtr->InheritsFrom(TH1D::Class()) ) {
			std::string keyName = objPtr->GetName();
			m1DHistograms.insert_or_assign(keyName, std::make_unique<TH1D>(*dynamic_cast<TH1D*>(objPtr.get())));
		}
		if ( objPtr->InheritsFrom(TH2D::Class()) ) {
			std::string keyName = objPtr->GetName();
			m2DHistograms.insert_or_assign(keyName, std::make_unique<TH2D>(*dynamic_cast<TH2D*>(objPtr.get())));
		}
	}
}

void TGeantPlot::setOutputDirectory(std::string_view outputDirectory) {
	mOutputDirectory = outputDirectory;
}

void TGeantPlot::saveHistorams(const std::vector<CppConfigDictionary>& configList) {
	for ( const auto& [key, hist] : m1DHistograms ) {
		for ( const CppConfigDictionary& config : configList ) {
			if ( key == config.getConfigName() ) {
				std::unique_ptr<TCanvas> canvas = std::make_unique<TCanvas>();
				savePlot(canvas.get(), hist.get(), config);
				setCanvasAttribute(canvas, config);
				if ( key == "IncidentParticle" ) {
					for ( int i = 0; i < hist->GetNbinsX(); i++ ) {
						hist->GetXaxis()->SetBinLabel(i + 1., mParticleName[i]);
					}
				}
				if ( key == "IncidentVolume" || key == "AlphaIncidentVolume" || key == "ElectronIncidentVolume" ) {
					for ( int i = 0; i < hist->GetNbinsX(); i++ ) {
						hist->GetXaxis()->SetBinLabel(i + 1., mVolumeName[i]);
					}
				}
				saveCanvas(canvas.get(), mOutputDirectory, config);
			}
		}
	}
	for ( const auto& [key, hist] : m2DHistograms ) {
		for ( const CppConfigDictionary& config : configList ) {
			if ( key == config.getConfigName() ) {
				std::unique_ptr<TCanvas> canvas = std::make_unique<TCanvas>();
				savePlot(canvas.get(), hist.get(), config);
				setCanvasAttribute(canvas, config);
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
					TColor::InvertPalette();
				}
				saveCanvas(canvas.get(), mOutputDirectory, config);

			}
		}
	}
}

void TGeantPlot::getEntries() {
	std::unique_ptr<TH1D>& hist = m1DHistograms.find("AlphaDepositEnergyEpitaxial")->second;
	std::unique_ptr<TH1D>& metalAlphaHist = m1DHistograms.find("AlphaDepositEnergyMetal")->second;
	std::unique_ptr<TH1D>& metalElectronHist = m1DHistograms.find("ElectronDepositEnergyMetal")->second;
	int nCS4 = 0;
	int nCS12 = 0;
	int nCS32 = 0;
	double energyPerPixel = 3.75 / 32;
	for ( int i = 0; i < hist->GetNbinsX(); i++ ) {
		if ( hist->GetBinLowEdge(i + 1) < energyPerPixel * 4 ) {
			nCS4 += hist->GetBinContent(i + 1);
		} else if ( hist->GetBinLowEdge(i + 1) < energyPerPixel * 12 ) {
			nCS12 += hist->GetBinContent(i + 1);
		} else {
			nCS32 += hist->GetBinContent(i + 1);
		}
	}
	std::cout << metalAlphaHist->GetEffectiveEntries() << "\t" << metalElectronHist->GetEffectiveEntries() << "\t" << hist->GetEffectiveEntries() << std::endl;
	// std::cout << nCS4 << "\t" << nCS12 << "\t" << nCS32 << std::endl;
	int maxBin = 0;
	for ( int i = 1; i <= m1DHistograms["AlphaIncidentAngle"]->GetNbinsX(); i++ ) {
		if ( m1DHistograms["AlphaIncidentAngle"]->GetBinContent(i) > 1 ) {
			maxBin = i;
		}
	}
	// std::cout << maxBin << std::endl;
}