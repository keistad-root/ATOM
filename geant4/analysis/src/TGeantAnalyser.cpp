#include "TGeantAnalyser.h"

// TGeantAnalyser::TGeantAnalyser(TString inputFile) : TAnalyser(inputFile) {}
// TGeantAnalyser::TGeantAnalyser(TFile* inputFile) : TAnalyser(inputFile) {}

// TGeantAnalyser::~TGeantAnalyser() {}

// void TGeantAnalyser::refineData() {
	// Float_t Eloss[6] = {1,1,1,1,1,1};
	// std::array<Double_t,3> position;
	// bool pass[6] = {false,false,false,false,false,false};
	// std::array<Double_t, 2> angle;
	// Double_t minX=3., minZ=3., maxX=-3., maxZ=-3.;
	// Int_t cntParticle = 0;

	// tTrack->SetBranchAddress("posX",&position[0]);
	// tTrack->SetBranchAddress("posY",&position[1]);
	// tTrack->SetBranchAddress("posZ",&position[2]);
	// tTrack->SetBranchAddress("Eloss",&Eloss);
	// tTrack->SetBranchAddress("Passed",&pass);
	// tTrack->SetBranchAddress("dirTheta",&angle[0]);
	// tTrack->SetBranchAddress("dirPhi",&angle[1]);

	// for (int entry = 0; entry < tTrack->GetEntries(); entry++) {
	//     tTrack->GetEntry(entry);
	//     if ((pass[3] || pass[4]) && TMath::Abs(position[0]) < 15. && TMath::Abs(position[2]) < 6.5) {
	//         clusterMap->Fill(position[0], position[2]);
	//         doubleClusterMap->Fill(position[0],position[2]);
	//         angleMap->Fill(angle[0], angle[1]);
	//         minX=TMath::Min(minX,position[0]);
	//         minZ=TMath::Min(minZ,position[2]);
	//         maxX=TMath::Max(maxX,position[0]);
	//         maxZ=TMath::Max(maxZ,position[2]);
	//         cntParticle++;
	//     }
	//     eLossHist[0]->Fill(Eloss[0]);
	//     eLossHist[1]->Fill(Eloss[1]);
	//     eLossHist[2]->Fill(Eloss[2]);
	//     eLossHist[3]->Fill(Eloss[3]);
	//     eLossHist[4]->Fill(Eloss[4]);
	//     eLossHist[5]->Fill(Eloss[5]);
	//     if (pass[3] && pass[4]) {
	//         eDep->Fill(Eloss[3]+Eloss[4]);
	//     }
	// }
	// Double_t area = TMath::Pi() * pow(((maxX - minX) + (maxZ - minZ) )/4,2);

	// clusterDensity = (0.00025 * 4300 * cntParticle / area) / tTrack -> GetEntries();
	// Double_t probability = (pow(30./32.,2)-TMath::Pi()*(0.09991))/pow(30./32.,2);
	// Double_t density = 0.;
	// for (int x = 0; x < 32; x++) {
	//     for (int y = 0; y < 16; y++) {
	//         density = (0.0025 * 4300 * doubleClusterMap->GetBinContent(x,y) / pow(30./32.,2)) / tTrack->GetEntries();
	//         doubleClusterMap->SetBinContent(x,y,1-pow(probability, density));
	//         if (doubleClusterMap->GetBinContent(x,y) > 1.) {
	//             std::cout << density << std::endl;
	//         }
	//     }
	// }
// }

// void TGeantAnalyser::saveEnergyLossDistribution(std::string_view title) {
	// for (std::unique_ptr<TH1D>& hist : eLossHist) {
	//     std::unique_ptr<TCanvas> canvase(new TCanvas("can","can",1610,1000));
	//     hist->SetBinContent(1,0);
	//     hist->Draw();
	//     canvase->SaveAs(static_cast<TString>(title));
	// }
// }

// void TGeantAnalyser::saveTotalDepositEnergyInALPIDE(std::string_view title) {
	// std::unique_ptr<TCanvas> can(new TCanvas("cmap","Simulated cluster map",1000,500));
	// eDep->Draw();
	// can->SaveAs(static_cast<const TString>(title));
// }

// void TGeantAnalyser::saveClustermap(std::string_view title) {
	// std::unique_ptr<TCanvas> can(new TCanvas("cmap","Simulated cluster map",1000,500));
	// clusterMap->Draw();
	// can->SaveAs(static_cast<const TString>(title));
// }

// void TGeantAnalyser::saveAnglemap(std::string_view title) {
	// std::unique_ptr<TCanvas> can(new TCanvas("cmap","Simulated cluster map",1000,500));
	// angleMap->Draw();
	// can->SaveAs(static_cast<const TString>(title));
// }

// void TGeantAnalyser::saveDoubleClusterFrequencymap(std::string_view title) {
	// std::unique_ptr<TCanvas> can(new TCanvas("cmap","Simulated cluster map",1000,500));
	// // doubleClusterMap->SetMaximum(1.);
	// doubleClusterMap->SetStats(0);
	// doubleClusterMap->Draw("COLZ");
	// can->SaveAs(static_cast<const TString>(title));
// }

// const double TGeantAnalyser::getClusterDensity() const {
	// return clusterDensity;
// }
