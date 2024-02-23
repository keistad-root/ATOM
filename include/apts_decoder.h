#ifndef __APTS_DECODER_H__
#define __APTS_DECODER_H__

#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

const Int_t APTS_PIXEL_ADC_MAPPING[16] = {2,1,5,0,4,8,9,12,13,14,10,15,11,7,6,3};
const Int_t APTS_MUX_PIXEL_ADC_MAPPING[16] = {3,2,1,0,4,8,9,5,12,13,14,15,10,11,6,7};

class TDecoder {
private:
    TString file_in_;
    std::ifstream rawFile_;
    uint8_t* binary_data_;
    Int_t data_index_;
    Int_t len_data_;
    Int_t event_num_;
    Int_t len_event_data_;
    Bool_t mux_;
    Int_t** event_data_;
    TFile* outputFile_;
    TTree* tree;
    
public:
    TDecoder();
    TDecoder(TString file_in, Bool_t mux = kFALSE);
    TDecoder(TString file_in, TString file_out, Bool_t mux = kFALSE);
    TDecoder(TString file_in, TFile* outputFile, Bool_t mux = kFALSE);
    ~TDecoder();
    void OpenBinary();
    void OpenBinary(TString file_in);
    void ReadBinary();
    Bool_t IsDone();
    Int_t** GetNextEvent();
    void DecodeAptsEvent();
    Int_t* to_binary(Int_t temp1d[], Int_t num1);
    Int_t to_decimal(Int_t num1,Int_t num2,Int_t num3,Int_t num4,Int_t num5,Int_t num6,Int_t num7,Int_t num8,Int_t num9,Int_t num10,Int_t num11,Int_t num12,Int_t num13,Int_t num14,Int_t num15,Int_t num16);
    void raw_to_root();
    void CloseOutputFile();
};

#endif