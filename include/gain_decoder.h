#ifndef __GAIN_DECODER__
#define __GAIN_DECODER__

#include "TFile.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TMultiGraph.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TAxis.h"

#include "apts_decoder.h"
#include "myjson.h"

const Int_t colorList[16] = {kRed, 
                             kRed+2, 
                             kOrange+4, 
                             kOrange+7, 
                             kYellow+1, 
                             kGreen,
                             kGreen+2,
                             kGreen+4,
                             kCyan,
                             kCyan+2,
                             kBlue,
                             kBlue+2,
                             kBlue+4,
                             kMagenta,
                             kMagenta+2,
                             kPink-8};


class GainDecoder : public TDecoder {
private:
    TString file_out_decoded_;
    TString file_out_analysed_;
    TString file_out_control_plot_;
    TFile* decodedFile;
    TFile* analysedFile;

    Double_t baseline_all[16][89];
    Double_t baselineX[89];
    std::vector<Double_t> baseline_der;

public:
    GainDecoder(TString file_in, TString file_out_decoded, TString file_out_analysed, TString file_out_control_plot, Bool_t mux = kFALSE);
    GainDecoder(TString file_in, TString file_out, Bool_t mux = kFALSE);
    void OpenOutputFiles();
    void OpenOutputFiles(TString file_out);
    void OpenOutputFiles(TString file_out_decoded, TString file_out_analysed);
    void MakeDecodedFile();
    std::vector<Double_t> derivative(Double_t arrX[], Double_t arrY[], Int_t size);
    void CalAnalysedData(Int_t ntrg_vres);
    void MakeAnalysedData();
    void DrawControlPlot();
};

#endif