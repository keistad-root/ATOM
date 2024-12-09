#include "TAnalysisManager.h"

TAnalysisManager* TAnalysisManager::mInstance = nullptr;

TAnalysisManager::TAnalysisManager() {
	mInstance = this;
}

TAnalysisManager::~TAnalysisManager() { }

TAnalysisManager* TAnalysisManager::Instance() {
	if ( mInstance == 0 ) {
		mInstance = new TAnalysisManager();
	}
	return mInstance;
}

void TAnalysisManager::open(const G4String& name) {
	mFile = new TFile(name, "RECREATE");
	mInitTree = new TTree("initTree", "Tree for recording initial informations");

	mInitTree->Branch("Particle Name", &initTuple.particleName, "InitParticleName[]/C");
	mInitTree->Branch("X", &initTuple.x, "InitX/D");
	mInitTree->Branch("Y", &initTuple.y, "InitY/D");
	mInitTree->Branch("Z", &initTuple.z, "InitZ/D");
	mInitTree->Branch("PX", &initTuple.px, "InitPX/D");
	mInitTree->Branch("PY", &initTuple.py, "InitPY/D");
	mInitTree->Branch("PZ", &initTuple.pz, "InitPZ/D");

	mALPIDETree = new TTree("alpideTree", "Tree for recording ALPIDE informations");
	mALPIDETree->Branch("DepositEnergyMetal", &alpideTuple.depositEnergyMetal, "DepositEnergyMetal/D");
	mALPIDETree->Branch("DepositEnergyEpitaxial", &alpideTuple.depositEnergyEpitaxial, "DepositEnergyEpitaxial/D");
	mALPIDETree->Branch("DepositEnergySubstrate", &alpideTuple.depositEnergySubstrate, "DepositEnergySubstrate/D");

	mIncidnetTree = new TTree("incidentTree", "Tree for recording incident informations");
	mIncidnetTree->Branch("Particle ID", &incidentTuple.particleID, "IncidentParticleID/I");
	mIncidnetTree->Branch("X", &incidentTuple.x, "IncidentX/D");
	mIncidnetTree->Branch("Y", &incidentTuple.y, "IncidentY/D");
	mIncidnetTree->Branch("Z", &incidentTuple.z, "IncidentZ/D");
	mIncidnetTree->Branch("PX", &incidentTuple.px, "IncidentPX/D");
	mIncidnetTree->Branch("PY", &incidentTuple.py, "IncidentPY/D");
	mIncidnetTree->Branch("PZ", &incidentTuple.pz, "IncidentPZ/D");
}

void TAnalysisManager::recordInit(const G4Event* event) {
	initTuple.particleName = event->GetPrimaryVertex(0)->GetPrimary(0)->GetParticleDefinition()->GetParticleName();
	initTuple.x = event->GetPrimaryVertex(0)->GetX0();
	initTuple.y = event->GetPrimaryVertex(0)->GetY0();
	initTuple.z = event->GetPrimaryVertex(0)->GetZ0();
	initTuple.px = event->GetPrimaryVertex(0)->GetPrimary(0)->GetPx();
	initTuple.py = event->GetPrimaryVertex(0)->GetPrimary(0)->GetPy();
	initTuple.pz = event->GetPrimaryVertex(0)->GetPrimary(0)->GetPz();

	mInitTree->Fill();
	mInitTree->AutoSave();
}

void TAnalysisManager::recordALPIDE(G4bool metal, G4bool epitaxial, G4bool substrate, G4double metalDepositEnergy, G4double epitaxialDepositEnergy, G4double substrateDepositEnergy) {
	if ( metal ) {
		alpideTuple.nParticleMetal++;
		alpideTuple.depositEnergyMetal = metalDepositEnergy;
	}
	if ( epitaxial ) {
		alpideTuple.nParticleEpitaxial++;
		alpideTuple.depositEnergyEpitaxial = epitaxialDepositEnergy;
	}
	if ( substrate ) {
		alpideTuple.nParticleSubstrate++;
		alpideTuple.depositEnergySubstrate = substrateDepositEnergy;
	}
	mALPIDETree->Fill();
	mALPIDETree->AutoSave();
}

void TAnalysisManager::recordIncident(const G4Step* step) {

	TString particleName = step->GetTrack()->GetParticleDefinition()->GetParticleName();
	if ( particleName == "alpha" ) {
		incidentTuple.particleID = 1;
	} else if ( particleName == "e-" ) {
		incidentTuple.particleID = 2;
	} else {
		incidentTuple.particleID = 0;
		std::cout << particleName << " " << step->GetTrack()->GetVertexPosition() << std::endl;
	}
	incidentTuple.x = step->GetPreStepPoint()->GetPosition().x() / mm;
	incidentTuple.y = step->GetPreStepPoint()->GetPosition().y() / mm;
	incidentTuple.z = step->GetPreStepPoint()->GetPosition().z() / mm;
	incidentTuple.px = step->GetPreStepPoint()->GetMomentum().x() / MeV;
	incidentTuple.py = step->GetPreStepPoint()->GetMomentum().y() / MeV;
	incidentTuple.pz = step->GetPreStepPoint()->GetMomentum().z() / MeV;

	mIncidnetTree->Fill();
	mIncidnetTree->AutoSave();
}

void TAnalysisManager::close() {
	mFile->cd();
	mInitTree->Write();
	mALPIDETree->Write();
	mIncidnetTree->Write();
	mFile->Close();
}