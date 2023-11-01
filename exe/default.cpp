#include <iostream>
#include <string>

#include "TFile.h"
#include "Decoder.h"

void extract_name_volt(TString chip_ID) {
    
}

void raw_to_root(std::string file_in, TFile* outputFile, Bool_t mux=kFALSE) {
    TDecoder* dec = new TDecoder(file_in+"raw",mux);

}

void decoder(std::string file_in, TString file_out="data/apts") {
    TFile* outputFile = new TFile(file_out+".root","RECREATE");
    inputJSON* jsonFile = new inputJSON(file_in+".json");

    TString chip_ID = jsonFile->findValue("chip_ID");
    extract_name_volt(chip_ID);
    Bool_t mux = kFALSE;

    raw_to_root(file_in, outputFile, mux);

    delete jsonFile;
    delete outputFile;
}


int main() {
    decoder("data/source/apts_20230130_172940");
}