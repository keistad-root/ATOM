#ifndef __ANALYSISMANAGER__
#define __ANALYSISMANAGER__

#include "TROOT.h"
#include "DetectorConstruction.h"

#include "TROOT.h"
#include "TFile.h"

class AnalysisManager {
private:
    static AnalysisManager* fInstance;
    Bool_t fCmdDefined = kFALSE;

    TFile* fFile;

    G4GenericMessenger* fMessenger;
public:
    AnalysisManager();
    ~AnalysisManager();

    static AnalysisManager* Instance();

    void Write();
};

#endif