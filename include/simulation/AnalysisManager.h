#ifndef __ANALYSISMANAGER__
#define __ANALYSISMANAGER__

#include "G4PhysicalConstants.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"

#include "DetectorConstruction.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

struct SourceEntry {
    Int_t pid;
    Double_t EKin;
    Double_t posX;
    Double_t posY;
    Double_t posZ;
    Double_t dirTheta;
    Double_t dirPhi;
    Double_t weight;
};

struct StepperEntry {
    Int_t pid;
    Double_t Ekin;
    Char_t preVolume;
    Char_t postVolume;
    Double_t preposX;
    Double_t preposY;
    Double_t preposZ;
    Double_t postposX;
    Double_t postposY;
    Double_t postposZ;
    Double_t dirTheta;
    Double_t dirPhi;
    Double_t TID;
    Double_t NIEL;
};

struct TrackEntry {
    Int_t    pid;
    Double_t  posX;
    Double_t  posY;
    Double_t  posZ;
    Int_t    nVol=6;
    Float_t  Eloss[10];
    Bool_t   Passed[10];
    Double_t dirTheta;
    Double_t dirPhi;
};

class AnalysisManager {
private:
    static AnalysisManager* fInstance;

    TFile* fFile;
    TString fFileName;
    TTree* fTreeSource;
    TTree* fTreeStepper;
    TTree* fTreeTrack;

    SourceEntry fSource;
    StepperEntry fStepper;
    TrackEntry fTrack;

    std::map<G4LogicalVolume*, Int_t> fScoringVolumes;
    G4LogicalVolume* fScoringStand;
    G4LogicalVolume* fScoringShield;
    G4LogicalVolume* fScoringALPIDECircuit;
    G4LogicalVolume* fScoringALPIDEEpitaxial;
    G4LogicalVolume* fScoringCarrierBoardLogical;
    
    std::map<Int_t, Float_t> WDeltaEnergy;
    std::map<Int_t, Bool_t>  WPassed;
    
    Bool_t bTreeSource = kTRUE;
    Bool_t bTreeStepper = kTRUE;
    Bool_t bTreeTrack = kTRUE;

    G4GenericMessenger* fMessenger;
    Bool_t fBooked = kFALSE;
    Bool_t fCmdDefined = kFALSE;
    Bool_t fRecordPrimaryOnly = kFALSE;

public:
    AnalysisManager();
    ~AnalysisManager();

    static AnalysisManager* Instance();

    void Write();
    void Book();
    void AutoSave();
    void Destroy();

    void FillStepper(const G4Step* step);
    void FillTrack(const G4Step* step);
    void FillSource(const G4Track* track);
    void EndOfTrack(const G4Track* track);

    void DefineCommand();
    void SetFileName(TString fName);
    void SetRecordSet(const Int_t number);
    void SetRecordPrimaryOnly(Bool_t primaryOnly = kTRUE);
    Bool_t GetRecordPrimaryOnly();
};

#endif