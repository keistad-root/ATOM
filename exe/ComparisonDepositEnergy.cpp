#include<filesystem>

#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TStyle.h"

void drawPlot(int length) {
	std::filesystem::path phi2Path = "/mnt/homes/ychoi/entry_test/geant4/plot/incident_plot_" + std::to_string(length) + "mm_2phi_alpha_noscreen.root";
	std::filesystem::path phi3Path = "/mnt/homes/ychoi/entry_test/geant4/plot/incident_plot_" + std::to_string(length) + "mm_3phi_alpha_noscreen.root";
	std::filesystem::path phi4Path = "/mnt/homes/ychoi/entry_test/geant4/plot/incident_plot_" + std::to_string(length) + "mm_4phi_alpha_noscreen.root";
	std::filesystem::path phi7Path = "/mnt/homes/ychoi/entry_test/geant4/plot/incident_plot_" + std::to_string(length) + "mm_7phi_alpha_noscreen.root";
	std::filesystem::path phiRefPath = "/mnt/homes/ychoi/entry_test/geant4/plot/incident_plot_" + std::to_string(length) + "mm_nocollimator_alpha_noscreen.root";

	TFile* phi2File = new TFile(phi2Path.c_str(), "READ");
	TFile* phi3File = new TFile(phi3Path.c_str(), "READ");
	TFile* phi4File = new TFile(phi4Path.c_str(), "READ");
	TFile* phi7File = new TFile(phi7Path.c_str(), "READ");
	TFile* phiRefFile = new TFile(phiRefPath.c_str(), "READ");

	TH1D* phi2DepositEnergy = static_cast<TH1D*>(phi2File->Get("AlphaDepositEnergyEpitaxial"));
	TH1D* phi3DepositEnergy = static_cast<TH1D*>(phi3File->Get("AlphaDepositEnergyEpitaxial"));
	TH1D* phi4DepositEnergy = static_cast<TH1D*>(phi4File->Get("AlphaDepositEnergyEpitaxial"));
	TH1D* phi7DepositEnergy = static_cast<TH1D*>(phi7File->Get("AlphaDepositEnergyEpitaxial"));
	TH1D* phiRefDepositEnergy = static_cast<TH1D*>(phiRefFile->Get("AlphaDepositEnergyEpitaxial"));

	for ( int i = 0; i < phiRefDepositEnergy->GetNbinsX(); i++ ) {
		phi2DepositEnergy->SetBinContent(i + 1, phi2DepositEnergy->GetBinContent(i + 1) / phiRefDepositEnergy->GetBinContent(i + 1));
		phi3DepositEnergy->SetBinContent(i + 1, phi3DepositEnergy->GetBinContent(i + 1) / phiRefDepositEnergy->GetBinContent(i + 1));
		phi4DepositEnergy->SetBinContent(i + 1, phi4DepositEnergy->GetBinContent(i + 1) / phiRefDepositEnergy->GetBinContent(i + 1));
		phi7DepositEnergy->SetBinContent(i + 1, phi7DepositEnergy->GetBinContent(i + 1) / phiRefDepositEnergy->GetBinContent(i + 1));
	}
	TCanvas* phi2Canvas = new TCanvas("phi2Canvas", "", 1000, 1000);
	phi2DepositEnergy->SetTitle("Ratio of Alpha Deposit Energy in Epitaxial; Energy [MeV]; Ratio To Reference");
	phi2DepositEnergy->SetEntries(phi2DepositEnergy->GetEffectiveEntries());
	phi2DepositEnergy->SetMinimum(0.001);
	phi2DepositEnergy->SetMaximum(1);
	phi2DepositEnergy->SetLineWidth(4);
	phi2DepositEnergy->Draw();
	phi2Canvas->SetLogy();
	phi2Canvas->SetGrid();
	phi2Canvas->SaveAs(static_cast<TString>("L" + std::to_string(length) + "phi2DepositEnergy.png"));
	delete phi2Canvas;
	TCanvas* phi3Canvas = new TCanvas("phi3Canvas", "", 1000, 1000);
	phi3DepositEnergy->SetTitle("Ratio of Alpha Deposit Energy in Epitaxial; Energy [MeV]; Ratio To Reference");
	phi3DepositEnergy->SetEntries(phi3DepositEnergy->GetEffectiveEntries());
	phi3DepositEnergy->SetMinimum(0.001);
	phi3DepositEnergy->SetMaximum(1);
	phi3DepositEnergy->SetLineWidth(4);
	phi3DepositEnergy->Draw();
	phi3Canvas->SetLogy();
	phi3Canvas->SetGrid();
	phi3Canvas->SaveAs(static_cast<TString>("L" + std::to_string(length) + "phi3DepositEnergy.png"));
	delete phi3Canvas;
	TCanvas* phi4Canvas = new TCanvas("phi4Canvas", "", 1000, 1000);
	phi4DepositEnergy->SetTitle("Ratio of Alpha Deposit Energy in Epitaxial; Energy [MeV]; Ratio To Reference");
	phi4DepositEnergy->SetEntries(phi4DepositEnergy->GetEffectiveEntries());
	phi4DepositEnergy->SetMinimum(0.001);
	phi4DepositEnergy->SetMaximum(1);
	phi4DepositEnergy->SetLineWidth(4);
	phi4DepositEnergy->Draw();
	phi4Canvas->SetLogy();
	phi4Canvas->SetGrid();
	phi4Canvas->SaveAs(static_cast<TString>("L" + std::to_string(length) + "phi4DepositEnergy.png"));
	delete phi4Canvas;
	TCanvas* phi7Canvas = new TCanvas("phi7Canvas", "", 1000, 1000);
	phi7DepositEnergy->SetTitle("Ratio of Alpha Deposit Energy in Epitaxial; Energy [MeV]; Ratio To Reference");
	phi7DepositEnergy->SetEntries(phi7DepositEnergy->GetEffectiveEntries());
	phi7DepositEnergy->SetMinimum(0.001);
	phi7DepositEnergy->SetMaximum(1);
	phi7DepositEnergy->SetLineWidth(4);
	phi7DepositEnergy->Draw();
	phi7Canvas->SetLogy();
	phi7Canvas->SetGrid();
	phi7Canvas->SaveAs(static_cast<TString>("L" + std::to_string(length) + "phi7DepositEnergy.png"));
	delete phi7Canvas;
}


int main() {
	// gStyle->SetOptStat(0);
	drawPlot(1);
	drawPlot(3);
	drawPlot(7);
	drawPlot(20);
	return 0;
}