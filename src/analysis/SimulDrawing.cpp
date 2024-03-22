#include "SimulDrawing.h"

SimulDrawing::SimulDrawing(std::unique_ptr<TFile>& inputFile) {
    mInputFile = std::move(inputFile);
    tSource.reset(static_cast<TTree*>(mInputFile->Get("tSource;1")));
    tTrack.reset(static_cast<TTree*>(mInputFile->Get("tTrack;1")));
    
    TString histTitle[6] = {"air", "stand", "aluminium screen", "ALPIDE circuit part", "ALPIDE other part", "carrier board"};
    for (int iHist = 0; iHist < 6; iHist++) {
        TString histName = Form("hist%d",iHist);
        eLossHist[iHist].reset(new TH1D(histName, "Enegy loss in " + histTitle[iHist] + "; Energy [MeV]; d(Entry)/d(Energy)",60,0,6));
    }
    eDep.reset(new TH1D("eDep","Energy deposited to circuit and body of ALPIDE", 60,0,6));
    clusterMap.reset(new TH2D("clustermap", "Cluster map; x[mm]; y[mm]", 1024, -15., 15., 512, -7.5, 7.5));
    angleMap.reset(new TH2D("angle", "Angle Map; $\\phi$; $\\theta$", 100,0.,1.,100,0.,1.));
    doubleClusterMap.reset(new TH2D("double", "Frequency for doubled cluster; x[mm]; y[mm]",32,-15.,15.,16,-7.5,7.5));
}

void SimulDrawing::refineData() {
    Float_t Eloss[6] = {1,1,1,1,1,1};
    std::array<Double_t,3> position;
    bool pass[6] = {false,false,false,false,false,false};
    std::array<Double_t, 2> angle;
    Double_t minX=3., minZ=3., maxX=-3., maxZ=-3.;
    Int_t cntParticle = 0;

    tTrack->SetBranchAddress("posX",&position[0]);
    tTrack->SetBranchAddress("posY",&position[1]);
    tTrack->SetBranchAddress("posZ",&position[2]);
    tTrack->SetBranchAddress("Eloss",&Eloss);
    tTrack->SetBranchAddress("Passed",&pass);
    tTrack->SetBranchAddress("dirTheta",&angle[0]);
    tTrack->SetBranchAddress("dirPhi",&angle[1]);

    for (int entry = 0; entry < tTrack->GetEntries(); entry++) {
        tTrack->GetEntry(entry);
        if ((pass[3] || pass[4]) && TMath::Abs(position[0]) < 15. && TMath::Abs(position[2]) < 6.5) {
            clusterMap->Fill(position[0], position[2]);
            doubleClusterMap->Fill(position[0],position[2]);
            angleMap->Fill(angle[0], angle[1]);
            minX=TMath::Min(minX,position[0]);
            minZ=TMath::Min(minZ,position[2]);
            maxX=TMath::Max(maxX,position[0]);
            maxZ=TMath::Max(maxZ,position[2]);
            cntParticle++;
        }
        eLossHist[0]->Fill(Eloss[0]);
        eLossHist[1]->Fill(Eloss[1]);
        eLossHist[2]->Fill(Eloss[2]);
        eLossHist[3]->Fill(Eloss[3]);
        eLossHist[4]->Fill(Eloss[4]);
        eLossHist[5]->Fill(Eloss[5]);
        if (pass[3] && pass[4]) {
            eDep->Fill(Eloss[3]+Eloss[4]);
        }
    }
    Double_t area = TMath::Pi() * pow(((maxX - minX) + (maxZ - minZ) )/4,2);

    clusterDensity = (0.00025 * 4300 * cntParticle / area) / tTrack -> GetEntries();
    Double_t probability = (pow(30./32.,2)-TMath::Pi()*(0.09991))/pow(30./32.,2);
    Double_t density = 0.;
    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 16; y++) {
            density = (0.0025 * 4300 * doubleClusterMap->GetBinContent(x,y) / pow(30./32.,2)) / tTrack->GetEntries();
            doubleClusterMap->SetBinContent(x,y,1-pow(probability, density));
            if (doubleClusterMap->GetBinContent(x,y) > 1.) {
                std::cout << density << std::endl;
            }
        }
    }
}

void SimulDrawing::saveEnergyLossDistribution(std::string_view title) {
    for (std::unique_ptr<TH1D>& hist : eLossHist) {
        std::unique_ptr<TCanvas> canvase(new TCanvas("can","can",1610,1000));
        hist->SetBinContent(1,0);
        hist->Draw();
        canvase->SaveAs(static_cast<TString>(title));
    }
}

void SimulDrawing::saveTotalDepositEnergyInALPIDE(std::string_view title) {
    std::unique_ptr<TCanvas> can(new TCanvas("cmap","Simulated cluster map",1000,500));
    eDep->Draw();
    can->SaveAs(static_cast<const TString>(title));
}

void SimulDrawing::saveClustermap(std::string_view title) {
    std::unique_ptr<TCanvas> can(new TCanvas("cmap","Simulated cluster map",1000,500));
    clusterMap->Draw();
    can->SaveAs(static_cast<const TString>(title));
}

void SimulDrawing::saveAnglemap(std::string_view title) {
    std::unique_ptr<TCanvas> can(new TCanvas("cmap","Simulated cluster map",1000,500));
    angleMap->Draw();
    can->SaveAs(static_cast<const TString>(title));
}

void SimulDrawing::saveDoubleClusterFrequencymap(std::string_view title) {
    std::unique_ptr<TCanvas> can(new TCanvas("cmap","Simulated cluster map",1000,500));
    // doubleClusterMap->SetMaximum(1.);
    doubleClusterMap->SetStats(0);
    doubleClusterMap->Draw("COLZ");
    can->SaveAs(static_cast<const TString>(title));
}

const double SimulDrawing::getClusterDensity() const {
    return clusterDensity;
}
