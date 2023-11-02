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
#include "gain_decoder.h"
#include "pitch_clock.h"

int main() {
    TClock* clock = new TClock();

    std::cout << "Gain decoding..." << std::endl;
    TString path = "data/";
    inputJSON* json = new inputJSON(std::string(path.Data())+"gain/apts_gain_20230130_172940.json");
    GainDecoder* dec = new GainDecoder(path+"gain/apts_gain_20230130_172940.raw", path+"apts_gain");

    std::cout << "Decoded file generated..." << std::endl;
    dec->MakeDecodedFile();
    clock->Measure();
    
    std::cout << "Analysed file generated..." << std::endl;
    dec->CalAnalysedData(json->findValue("ntrg_vres").Atoi());
    dec->MakeAnalysedData();
    clock->Measure();

    std::cout << "Control plots are being drawn..." << std::endl;
    dec->DrawControlPlot();
    clock->EndProgram();

    return 0;
}