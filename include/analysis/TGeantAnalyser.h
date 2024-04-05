#ifndef __TGEANTANALYSER__
#define __TGEANTANALYSER__

#include "Headers.h"
#include "TAnalyser.h"

class TGeantAnalyser : public TAnalyser {
private:

    // std::unique_ptr<TTree> tSource;
    // std::unique_ptr<TTree> tTrack;

    // std::unique_ptr<TH1D> eLossHist[6];
    // std::unique_ptr<TH1D> eDep;
    // std::unique_ptr<TH2D> clusterMap;
    // std::unique_ptr<TH2D> angleMap;
    // std::unique_ptr<TH2D> doubleClusterMap;
    // double clusterDensity;
public:
    TGeantAnalyser() = delete;
    TGeantAnalyser(TString inputFile);
    TGeantAnalyser(TFile* inputFile);
    ~TGeantAnalyser();
    // void refineData();
    // void saveEnergyLossDistribution(std::string_view output_file_Name);
    // void saveTotalDepositEnergyInALPIDE(std::string_view output_file_Name);
    // void saveClustermap(std::string_view output_file_Name);
    // void saveAnglemap(std::string_view output_file_Name);
    // void saveDoubleClusterFrequencymap(std::string_view output_file_Name);
    // const double getClusterDensity() const;
};

#endif