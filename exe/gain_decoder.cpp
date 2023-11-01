#include <iostream>

#include "TROOT.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TColor.h"
#include "TLegend.h"
#include "TStyle.h"

#include "myjson.h"
#include "Decoder.h"

const Int_t colorList[16] = {kRed, kRed+2, kOrange+4, kOrange+7, kYellow+1, kGreen,kGreen+2,kGreen+4,kCyan,kCyan+2,kBlue,kBlue+2,kBlue+4,kMagenta,kMagenta+2,kPink-8};

void raw_to_root(std::string file_in, TFile* outputFile, Bool_t mux=kFALSE) {
    TDecoder* dec = new TDecoder(file_in+".raw",mux);
    std::vector<Int_t**> allData;
    
    TTree* tree = new TTree("data", "APTS data");
    Int_t nev;
    Int_t nframe;
    Int_t signal[4][4];

    tree->Branch("Number of Event",&nev,"nev/I");
    tree->Branch("Number of Frame",&nframe,"nf/I");
    tree->Branch("signal",&signal,"sig[4][4]/I");

    nev = 0;
    while (!dec->is_done()) {
        Int_t** data = dec->get_next_event();
        for (Int_t frame = 0; frame < 200; frame++){
            nframe=frame;
            for (Int_t y = 0; y < 4; y++) {
                for (Int_t x = 0; x < 4; x++) {
                    signal[x][y]=data[frame][y+x*4];
                }
            }
            tree->Fill();
        }
        nev++;
    }
    tree->Write();
    delete dec;
}

void analysis_gain(std::string file_in, TString file_out="data/apts_gain") {
    Bool_t mux = kFALSE;
    TFile* outputFile = new TFile(file_out+".root","RECREATE");
    raw_to_root(file_in, outputFile, mux);
    outputFile->Close();
    delete outputFile;
}

std::vector<Double_t> derivative(Double_t arrX[], Double_t arrY[], Int_t size){
    std::vector<Double_t> der_arr;
    Int_t npoints = 0;
    Double_t num = 0;
    Int_t den = 0;
    der_arr.push_back((arrY[0]-arrY[1])/(arrX[0]-arrY[1]));
    for (Int_t i = 1; i < size-1; i++) {
        npoints = 0;
        num = 0;
        den = 0;
        if (i < 5) {
            npoints=i;
        } else if (i >= size-5){
            npoints = size-i-1;
        } else {
            npoints = 5;
        }
        for (Int_t n = 0; n < npoints; n++) {
            num += arrY[i-(n+1)]-arrY[i+(n+1)];
            den += arrX[i-(n+1)]-arrX[i+(n+1)];
        }
        der_arr.push_back(num/den);
    }
    der_arr.push_back((arrY[size-1]-arrY[size-2])/(arrX[size-1]-arrX[size-2]));

    return der_arr;
}

void drawGainInfo(std::string file_in, TString file_in2){
    inputJSON* jsonFile = new inputJSON(file_in+".json");

    TFile* inputFile = new TFile(file_in2+".root","READ");
    TTree* tree = (TTree*) inputFile->Get("data;1");
    Int_t nev;
    Int_t nframe;
    Int_t signal[4][4]={{0,},};

    tree->SetBranchAddress("Number of Event",&nev);
    tree->SetBranchAddress("Number of Frame",&nframe);
    tree->SetBranchAddress("signal",&signal);

    Double_t baseline_all[16][89] = {{0,},};
    Int_t ntrg_vres = jsonFile->findValue("ntrg_vres").Atoi();
    std::cout.precision(13);

    for (Int_t i = 0; i < 89; i++) {
        for (Int_t j = 0; j < ntrg_vres; j++){
            if (j == 49) continue;
            for (Int_t k = 0; k < 200; k++) {
                tree->GetEntry(i*ntrg_vres*200+j*200+k);
                for (Int_t xy = 0; xy < 16; xy++) {
                    baseline_all[xy][i]+=signal[xy%4][int(floor(xy/4.))]/((ntrg_vres-1)*200.0);
                }
            }
        }
    }
    delete tree;
    inputFile->Close();
    delete inputFile;

    TFile* analysedFile = new TFile(file_in2+"_analysed.root","RECREATE");
    TTree* vresTree = new TTree("Vreset","Vreset Value");
    Double_t X_vres[89];
    Int_t vres;
    vresTree->Branch("Vreset",&vres,"vres/I");
    for (Int_t i = 20; i < 901; i+=10) {
        vres = i;
        X_vres[i/10-2]=i;
        vresTree->Fill();
    }
    vresTree->Write();

    TTree* dataTree = new TTree("data","Baseline value");
    Double_t value[89];

    dataTree->Branch("Baseline",value,"vres[89]/I");
    for(Int_t i = 0; i < 16; i++) {
        for (Int_t j = 0; j < 89; j++) {
            value[j] = baseline_all[i][j];
        }
        dataTree->Fill();
    }
    dataTree->Write();

    delete vresTree;
    delete dataTree;
    analysedFile->Close();
    delete analysedFile;

    std::vector<Double_t> baseline_der;

    TCanvas* can = new TCanvas("can","Gain Info.",1800,900);
    can->cd();
    TPad* pad1 = new TPad("pad1","baseline",0,0,0.5,1);
    pad1->Draw();
    TPad* pad2 = new TPad("pad2","derivative",0.5,0,1,1);
    pad2->Draw();
    pad1->cd();
    TMultiGraph* mg = new TMultiGraph();
    TGraph* baselineGraph[16];
    for (Int_t i = 0; i < 16; i++) {
        baselineGraph[i]=new TGraph();
        for (Int_t j =0; j < 89; j++) {
            baselineGraph[i]->SetPoint(j,X_vres[j],baseline_all[i][j]*0.0381);
        }
        baselineGraph[i]->SetMarkerSize(2);
        baselineGraph[i]->SetMarkerColor(colorList[i]);
        mg->Add(baselineGraph[i]);
    }
    mg->SetTitle("Baseline vs. V_RESET");
    mg->GetXaxis()->SetTitle("V_RESET [mV]");
    mg->GetYaxis()->SetTitle("Baseline [mV]");
    mg->Draw("AP");
    pad1->SetLeftMargin(0.13);
    pad2->cd();
    TMultiGraph* mg2 = new TMultiGraph();
    TGraph* derivativeGraph[16];
    TLegend* legend = new TLegend(0.4,0.1,0.9,0.4);
    legend->SetHeader("Pixel Coordinate", "C");
    legend->SetNColumns(4);
    for (Int_t i =0; i < 16; i++) {
        derivativeGraph[i] = new TGraph();
        baseline_der = derivative(X_vres,baseline_all[i],89);
        for (Int_t j = 1; j < 89; j++) {
            derivativeGraph[i]->SetPoint(j-1,X_vres[j],baseline_der[j]*0.0381);
        }
        derivativeGraph[i]->SetLineColor(colorList[i]);
        derivativeGraph[i]->SetLineWidth(2);
        TString forLegend = Form("%d %d",int(floor(i/4)),i%4);
        legend->AddEntry(derivativeGraph[i],forLegend,"L");
        mg2->Add(derivativeGraph[i]);
    }
    mg2->Draw("AL");
    mg2->SetTitle("Derivative vs. V_RESET");
    mg2->GetXaxis()->SetTitle("V_RESET [mV]");
    mg2->GetYaxis()->SetTitle("Derivative");
    legend->Draw();
    can->SaveAs(file_in2+"_control_plot.pdf");
}

int main() {
    std::cout << "Gain decoding..." << std::endl;

    std::string path = "data/";
    analysis_gain(path+"gain/apts_gain_20230130_172940");
    drawGainInfo(path+"gain/apts_gain_20230130_172940","data/apts_gain");

    return 0;
}