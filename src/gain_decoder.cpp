#include "gain_decoder.h"

GainDecoder::GainDecoder(TString file_in, TString file_out_decoded, TString file_out_analysed, TString file_out_control_plot, Bool_t mux) 
: TDecoder(file_in, file_out_decoded, mux), file_out_decoded_{file_out_decoded}, file_out_analysed_{file_out_analysed}, file_out_control_plot_{file_out_control_plot} {
}

GainDecoder::GainDecoder(TString file_in, TString file_out, Bool_t mux) 
: TDecoder(file_in, file_out+"_decoded.root", mux), file_out_decoded_{file_out+"_decoded.root"}, file_out_analysed_{file_out+"_analysed.root"}, file_out_control_plot_{file_out+"_control_plot.pdf"} {    
}

void GainDecoder::OpenOutputFiles() {
    analysedFile = new TFile(file_out_analysed_,"RECREATE");
}

void GainDecoder::OpenOutputFiles(TString file_out) {
    file_out_analysed_ = file_out + "_analysed.root";
    analysedFile = new TFile(file_out_analysed_,"RECREATE");
}

void GainDecoder::OpenOutputFiles(TString file_out_decoded, TString file_out_analysed) {
    file_out_analysed_ = file_out_analysed;
    analysedFile = new TFile(file_out_analysed_,"RECREATE");
}

void GainDecoder::MakeDecodedFile() {
    raw_to_root();
    CloseOutputFile();
}

std::vector<Double_t> GainDecoder::derivative(Double_t arrX[], Double_t arrY[], Int_t size){
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

void GainDecoder::CalAnalysedData(Int_t ntrg_vres) {
    decodedFile = new TFile(file_out_decoded_,"READ");
    TTree* tree = (TTree*) decodedFile->Get("data;1");

    Int_t nev;
    Int_t nframe;
    Int_t signal[4][4]={{0,},};

    tree->SetBranchAddress("Number of Event",&nev);
    tree->SetBranchAddress("Number of Frame",&nframe);
    tree->SetBranchAddress("signal",&signal);

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
    decodedFile->Close();
}

void GainDecoder::MakeAnalysedData() {
    OpenOutputFiles();
    TTree* vresTree = new TTree("Vreset","Vreset Value");
    Int_t vres;
    vresTree->Branch("Vreset",&vres,"vres/I");
    for (Int_t i = 20; i < 901; i+=10) {
        vres = i;
        baselineX[i/10-2]=i;
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
}

void GainDecoder::DrawControlPlot() {
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
            baselineGraph[i]->SetPoint(j,baselineX[j],baseline_all[i][j]*0.0381);
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
        baseline_der = derivative(baselineX,baseline_all[i],89);
        for (Int_t j = 1; j < 89; j++) {
            derivativeGraph[i]->SetPoint(j-1,baselineX[j],baseline_der[j]*0.0381);
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
    can->SaveAs(file_out_control_plot_);

    delete pad1;
    delete pad2;
    can->Close();
    delete can;
    delete mg;
    delete legend;
    delete mg2;
}