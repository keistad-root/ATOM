#include <iostream>
#include <string>
#include <ctime>

#include "TFile.h"

#include "decoder.h"
#include "myjson.h"
#include "pitch_clock.h"

TString* extract_name_volt(TString chip_ID) {
    TString str[2] = {"a","b"};
    return str;
}

void decoder(TString file_in, TString file_out="data/apts") {
    TFile* outputFile = new TFile(file_out+".root","RECREATE");
    inputJSON* jsonFile = new inputJSON(std::string(file_in.Data())+".json");
    TString chip_ID = jsonFile->findValue("chip_ID");
    TString* nameVolt = extract_name_volt(chip_ID);
    Bool_t mux = kFALSE;
    TDecoder* dec = new TDecoder(file_in+".raw",outputFile,mux);

    dec -> raw_to_root();

    delete jsonFile;
    outputFile->Close();
    delete outputFile;
    delete dec;
}

int main() {
    TClock* clock = new TClock();
    std::cout << "Source Decoding..." << std::endl;

    TString path = "data/";
    decoder(path + "source/apts_20230130_172940");

    clock->EndProgram();
    return 0;
}