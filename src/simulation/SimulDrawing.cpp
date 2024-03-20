#include "SimulDrawing.h"

SimulDrawing::SimulDrawing(std::unique_ptr<TFile>& inputFile) {
    mInputFile = std::move(inputFile);
    tSource.reset(static_cast<TTree*>(mInputFile->Get("tSource;1")));
    tTrack.reset(static_cast<TTree*>(mInputFile->Get("tTrack;1")));
    
    for (int iHist = 0; iHist < 6; iHist++) {
        TString histName = Form("hist%d",iHist);
        eLossHist[iHist].reset(new TH1D(histName,histName,60,0,6));
    }
}

void SimulDrawing::getEnergyLoss() {
    Float_t Eloss[6] = {1,1,1,1,1,1};
    tTrack->SetBranchAddress("Eloss",&Eloss);
    for (int entry = 0; entry < tTrack->GetEntries(); entry++) {
        tTrack->GetEntry(entry);
        eLossHist[0]->Fill(Eloss[0]);
        eLossHist[1]->Fill(Eloss[1]);
        eLossHist[2]->Fill(Eloss[2]);
        eLossHist[3]->Fill(Eloss[3]);
        eLossHist[4]->Fill(Eloss[4]);
        eLossHist[5]->Fill(Eloss[5]);
    }
}

void SimulDrawing::drawEnergyLossHistogram() {
    std::unique_ptr<TCanvas> can(new TCanvas("can","can",1800,900));
    std::unique_ptr<TPad> pad[6];
    Float_t border[6][4] = {{ .0,  .5, .33, 1.},
                            {.33,  .5, .66, 1.},
                            {.66,  .5,  1., 1.},
                            { .0,  0., .33, .5},
                            {.33,  0., .66, .5},
                            {.66,  0.,  1., .5}};
    for (int iPad = 0; iPad < 6; iPad++) {
        TString padName = Form("pad%d",iPad);
        pad[iPad].reset(new TPad(padName,padName,border[iPad][0],border[iPad][1],border[iPad][2],border[iPad][3]));
        pad[iPad]->Draw();
    }

    TString histTitle[6] = {"air", "stand", "aluminium screen", "ALPIDE circuit part", "ALPIDE other part", "carrier board"};
    for (int iPad = 0; iPad < 6; iPad++) {
        pad[iPad]->cd();
        eLossHist[iPad]->SetBinContent(1,0);
        eLossHist[iPad]->SetTitle("Enegy loss in " +histTitle[iPad]);
        eLossHist[iPad]->GetXaxis()->SetTitle("Energy [MeV]");
        eLossHist[iPad]->GetYaxis()->SetTitle("dEntry/dEnergy");
        eLossHist[iPad]->Draw();
    }
    can->SaveAs("data/ElossHist.pdf");
}

void SimulDrawing::getDensity() {
    std::array<Double_t,3> position;
    bool pass[6] = {false,false,false,false,false,false};

    tTrack->SetBranchAddress("Passed",&pass);

    tTrack->SetBranchAddress("posX",&position[0]);
    tTrack->SetBranchAddress("posY",&position[1]);
    tTrack->SetBranchAddress("posZ",&position[2]);

    Double_t minX=3., minZ=3., maxX=-3., maxZ=-3.;
    Int_t cntParticle;
    for (int entry = 0; entry < tTrack->GetEntries(); entry++) {
        tTrack->GetEntry(entry);
        if ((pass[3] || pass[4])) {
            minX=TMath::Min(minX,position[0]);
            minZ=TMath::Min(minZ,position[2]);
            maxX=TMath::Max(maxX,position[0]);
            maxZ=TMath::Max(maxZ,position[2]);
            cntParticle++;
        }
    };
    Double_t area = TMath::Pi() * pow(((maxX - minX) + (maxZ - minZ) )/4,2);
    std::cout << "Area: " << area << "mm^2" << std::endl;
    std::cout << "Density: " << 4300*cntParticle/area/tTrack->GetEntries()*0.00025 << "/mm^2 per 1 strobe(250us)" << std::endl;
    std::cout << "Ratio: " << 1 - pow(0.99875,4300*cntParticle/area/tTrack->GetEntries()*0.00025) << std::endl;
}