#ifndef __TANALYSISMANAGER__
#define __TANALYSISMANAGER__

#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

#include "TFile.h"
#include "TTree.h"

struct InitTuple {
	TString particleName;
	Double_t x, y, z;
	Double_t px, py, pz;
};

struct ALPIDETuple {
	Int_t nParticleMetal, nParticleEpitaxial, nParticleSubstrate;
	Double_t depositEnergyMetal, depositEnergyEpitaxial, depositEnergySubstrate;
};

struct IncidentTuple {
	Int_t particleID;
	Double_t x, y, z;
	Double_t px, py, pz;
};

class TAnalysisManager {
public:
	TAnalysisManager();
	~TAnalysisManager();

private:
	static TAnalysisManager* mInstance;

	TFile* mFile = nullptr;
	TTree* mInitTree = nullptr;
	TTree* mALPIDETree = nullptr;
	TTree* mIncidnetTree = nullptr;

	InitTuple initTuple;
	ALPIDETuple alpideTuple;
	IncidentTuple incidentTuple;

public:
	static TAnalysisManager* Instance();

	void open(const G4String& name);
	void close();

	void recordInit(const G4Event* event);
	void recordALPIDE(G4bool metal, G4bool epitaxial, G4bool substrate, G4double metalDepositEnergy, G4double epitaxialDepositEnergy, G4double substrateDepositEnergy);
	void recordIncident(const G4Step* step);
};

#endif