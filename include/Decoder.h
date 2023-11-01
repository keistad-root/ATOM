#ifndef __DECODER_H__
#define __DECODER_H__

#include <iostream>
#include <fstream>
#include <algorithm>
#include "TROOT.h"
#include "TFile.h"

const Int_t APTS_PIXEL_ADC_MAPPING[16] = {2,1,5,0,4,8,9,12,13,14,10,15,11,7,6,3};
const Int_t APTS_MUX_PIXEL_ADC_MAPPING[16] = {3,2,1,0,4,8,9,5,12,13,14,15,10,11,6,7};

class TDecoder {
private:
    std::ifstream rawFile;
    uint8_t* data;
    Int_t i;
    Int_t _len_data;
    Int_t iev;
    Bool_t _mux;

    
public:
    TDecoder(std::string file_in, Bool_t mux = kFALSE);
    ~TDecoder();
    Bool_t is_done();
    Int_t** get_next_event();
    Int_t** decode_apts_event(Int_t length);
    Int_t* to_binary(Int_t arr[], Int_t num1);
    Int_t to_decimal(Int_t num1,Int_t num2,Int_t num3,Int_t num4,Int_t num5,Int_t num6,Int_t num7,Int_t num8,Int_t num9,Int_t num10,Int_t num11,Int_t num12,Int_t num13,Int_t num14,Int_t num15,Int_t num16);
};

#endif