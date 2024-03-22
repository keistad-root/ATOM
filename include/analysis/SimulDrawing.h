#ifndef __SIMULDRAWING__
#define __SIMULDRAWING__

#include <iostream>
#include <algorithm>

#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TPad.h"

class SimulDrawing {
private:
    std::unique_ptr<TFile> mInputFile;
    std::unique_ptr<TTree> tSource;
    std::unique_ptr<TTree> tTrack;

    std::unique_ptr<TH1D> eLossHist[6];
    std::unique_ptr<TH1D> eDep;
    std::unique_ptr<TH2D> clusterMap;
    std::unique_ptr<TH2D> angleMap;
    std::unique_ptr<TH2D> doubleClusterMap;
    double clusterDensity;
public:
    SimulDrawing(std::unique_ptr<TFile>& inputFile);
    void refineData();
    void saveEnergyLossDistribution(std::string_view output_file_Name);
    void saveTotalDepositEnergyInALPIDE(std::string_view output_file_Name);
    void saveClustermap(std::string_view output_file_Name);
    void saveAnglemap(std::string_view output_file_Name);
    void saveDoubleClusterFrequencymap(std::string_view output_file_Name);
    const double getClusterDensity() const;
};

#endif