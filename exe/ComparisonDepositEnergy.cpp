#include<filesystem>

#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TLegend.h"

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

	TGraph* graph[4] = {new TGraph(), new TGraph(), new TGraph(), new TGraph()};
	graph[0]->SetPoint(0, 1, 0.0093);
	graph[0]->SetPoint(1, 3, 0);
	graph[0]->SetPoint(2, 7, 0);
	graph[0]->SetPoint(3, 20, 0);
	graph[1]->SetPoint(0, 1, 0.035);
	graph[1]->SetPoint(1, 3, 0.018);
	graph[1]->SetPoint(2, 7, 0.25);
	graph[1]->SetPoint(3, 20, 0.17);
	graph[2]->SetPoint(0, 1, 0.19);
	graph[2]->SetPoint(1, 3, 0.039);
	graph[2]->SetPoint(2, 7, 0.5);
	graph[2]->SetPoint(3, 20, 0.25);
	graph[3]->SetPoint(0, 1, 0.55);
	graph[3]->SetPoint(1, 3, 0.056);
	graph[3]->SetPoint(2, 20, 0.084);
	TLegend* legend = new TLegend(0.5, 0.7, 0.7, 0.9);
	legend->AddEntry(graph[0], "#phi2", "p");
	legend->AddEntry(graph[1], "#phi3", "p");
	legend->AddEntry(graph[2], "#phi4", "p");
	legend->AddEntry(graph[3], "#phi7", "p");

	TCanvas* canvas = new TCanvas("canvas", "", 1000, 1000);
	graph[0]->SetMarkerColor(kRed);
	graph[0]->SetMarkerStyle(21);
	graph[0]->SetMarkerSize(2);
	graph[0]->SetMaximum(1);
	graph[0]->Draw("AP");
	graph[1]->SetMarkerColor(kBlue);
	graph[1]->SetMarkerStyle(21);
	graph[1]->SetMarkerSize(2);
	graph[1]->Draw("P");
	graph[2]->SetMarkerColor(kMagenta);
	graph[2]->SetMarkerStyle(21);
	graph[2]->SetMarkerSize(2);
	graph[2]->Draw("P");
	graph[3]->SetMarkerColor(kGreen + 3);
	graph[3]->SetMarkerStyle(21);
	graph[3]->SetMarkerSize(2);
	graph[3]->Draw("P");

	legend->Draw("SAME");
	canvas->SetGrid();
	canvas->SaveAs("DepositEnergyRatio.png");

	return 0;
}