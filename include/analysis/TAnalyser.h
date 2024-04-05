#ifndef __TANALYSER__
#define __TANALYSER__

#include "Headers.h"
#include "TKey.h"

class TAnalyser {
private:
    TFile* mInputFile;
public:
    TAnalyser() = delete;
    TAnalyser(TString inputFileTitle);
    TAnalyser(TFile& inputFile);
    TAnalyser(TFile* inputFile);
    ~TAnalyser();

    void setInputFile(TString inputFileTitle);
    void setInputFile(TFile* inputFile);
    void setInputFile(TFile& inputFile);
    
    TTree* openTree(std::string treeName);
};

#endif