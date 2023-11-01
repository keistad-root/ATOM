#include <iostream>
#include <string>
#include <ctime>

#include "TFile.h"
#include "TTree.h"
#include "Decoder.h"
#include "myjson.h"

TString* extract_name_volt(TString chip_ID) {
    TString str[2] = {"a","b"};
    return str;
}

void raw_to_root(std::string file_in, TFile* outputFile, Bool_t mux=kFALSE) {
    TDecoder* dec = new TDecoder(file_in+".raw",mux);
    std::vector<Int_t**> allData;
    
    TTree* tree = new TTree("data", "APTS data");
    Int_t nev;
    Int_t nframe;
    Int_t signal[4][4];

    tree->Branch("Number of Event",&nev,"nev/I");
    tree->Branch("Number of Frame",&nframe,"nf/I");
    tree->Branch("signal",signal,"sig[4][4]/I");

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

void decoder(std::string file_in, TString file_out="data/apts") {
    TFile* outputFile = new TFile(file_out+".root","RECREATE");
    inputJSON* jsonFile = new inputJSON(file_in+".json");
    TString chip_ID = jsonFile->findValue("chip_ID");
    TString* nameVolt = extract_name_volt(chip_ID);
    Bool_t mux = kFALSE;

    raw_to_root(file_in, outputFile, mux);

    delete jsonFile;
    outputFile->Close();
    delete outputFile;
}

int main() {
    clock_t start, finish;
    double duration;

    start = clock();
    
    std::cout << "Source Decoding..." << std::endl;

    TString path = "/home/ychoi/Data/APTS/input/Group/Pusan/Vbb_1.2/";
    decoder("/home/ychoi/Data/APTS/input/Group/Pusan/Vbb_1.2/source/apts_20230130_172940");

    finish = clock();

    duration = (double)(finish-start) / CLOCKS_PER_SEC;
    std::cout << duration << "s" << std::endl;

    return 0;
}