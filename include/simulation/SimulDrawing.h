#ifndef __SIMULDRAWING__
#define __SIMULDRAWING__

#include <iostream>
#include <algorithm>

#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TPad.h"

class SimulDrawing {
private:
    std::unique_ptr<TFile> mInputFile;
    std::unique_ptr<TTree> tSource;
    std::unique_ptr<TTree> tTrack;

    std::unique_ptr<TH1D> eLossHist[6];
public:
    SimulDrawing(std::unique_ptr<TFile>& inputFile);

    void getEnergyLoss();
    void drawEnergyLossHistogram();
    void getDensity();
};

#endif