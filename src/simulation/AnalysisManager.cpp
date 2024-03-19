#include "AnalysisManager.h"

AnalysisManager* AnalysisManager::fInstance = nullptr;

AnalysisManager::AnalysisManager() {
    fInstance = this;
    if (!fCmdDefined) DefineCommand();
}

AnalysisManager::~AnalysisManager() {
    G4cout << "AnalysisManager Deleting..." << G4endl;
    Destroy();
}

AnalysisManager* AnalysisManager::Instance() {
    if (fInstance == 0) {
        fInstance = new AnalysisManager();
    }
    return fInstance;
}

void AnalysisManager::Write(){
    G4cout<<"AnalysisManager::Write - Writing File to " << fFile->GetName() <<G4endl;
    TString fst = fFile->IsOpen() ? "True" : "False";
    G4cout<<"File is opened? : "<< fst << G4endl;
    G4cout<<"In Option : "<< fFile->GetOption() << G4endl;
    fFile->Write(0, TObject::kOverwrite, 0);
    G4cout<<"AnalysisManager::Write - File writed in " << fFile->GetName() <<G4endl;
}

void AnalysisManager::Book() {
    if (fBooked) return;

    fFile = new TFile(fFileName.Data(), "RECREATE");
    fTreeSource = new TTree("tSource", "Source");
    fTreeSource->Branch("pid", &fSource.pid, "pid/I");
    fTreeSource->Branch("EKin", &fSource.EKin, "EKin/I");
    fTreeSource->Branch("posX", &fSource.posX, "posX/D");
    fTreeSource->Branch("posY", &fSource.posY, "posY/D");
    fTreeSource->Branch("posZ", &fSource.posZ, "posZ/D");
    fTreeSource->Branch("dirTheta", &fSource.dirTheta, "dirTheta/D");
    fTreeSource->Branch("dirPhi", &fSource.dirPhi, "dirPhi/D");
    fTreeSource->Branch("weight", &fSource.weight, "weight/D");

    fTreeStepper = new TTree("tStepper", "Stepper");
    fTreeStepper->Branch("pid", & fStepper.pid, "pid/I");
    fTreeStepper->Branch("Ekin", & fStepper.Ekin, "Ekin/D");
    fTreeStepper->Branch("preVolume", & fStepper.preVolume, "preVolume/B");
    fTreeStepper->Branch("postVolume", & fStepper.postVolume, "postVolume/B");
    fTreeStepper->Branch("preposX", & fStepper.preposX, "preposX/D");
    fTreeStepper->Branch("preposY", & fStepper.preposY, "preposY/D");
    fTreeStepper->Branch("preposZ", & fStepper.preposZ, "preposZ/D");
    fTreeStepper->Branch("postposX", & fStepper.postposX, "postposX/D");
    fTreeStepper->Branch("postposY", & fStepper.postposY, "postposY/D");
    fTreeStepper->Branch("postposZ", & fStepper.postposZ, "postposZ/D");
    fTreeStepper->Branch("dirTheta", & fStepper.dirTheta, "dirTheta/D");
    fTreeStepper->Branch("dirPhi", & fStepper.dirPhi, "dirPhi/D");
    fTreeStepper->Branch("TID", & fStepper.TID, "TID/D");
    fTreeStepper->Branch("NIEL", & fStepper.NIEL, "NIEL/D");

    fTreeTrack = new TTree("tTrack", "TrackCollector");
    fTreeTrack->Branch("pid", & fTrack.pid, "pid/I");
    fTreeTrack->Branch("posX", & fTrack.posX, "posX/D");
    fTreeTrack->Branch("posY", & fTrack.posY, "posY/D");
    fTreeTrack->Branch("posZ", & fTrack.posZ, "posZ/D");
    fTreeTrack->Branch("nVol", & fTrack.nVol, "nVol/I");
    fTreeTrack->Branch("Eloss", &fTrack.Eloss, "Eloss[nVol]/F");
    fTreeTrack->Branch("Passed", &fTrack.Passed, "Passed[nVol]/O");
    fTreeTrack->Branch("dirTheta", & fTrack.dirTheta, "dirTheta/D");
    fTreeTrack->Branch("dirPhi", & fTrack.dirPhi, "dirPhi/D");

    const DetectorConstruction* detectorConstruction = static_cast<const DetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    if (!fScoringALPIDECircuit) {
        fScoringALPIDECircuit = detectorConstruction->GetScoringALPIDECircuit();
    }
    if (!fScoringALPIDEEpitaxial) {
        fScoringALPIDEEpitaxial = detectorConstruction->GetScoringALPIDEEpitaxial();
    }
    if (!fScoringCarrierBoardLogical) {
        fScoringCarrierBoardLogical = detectorConstruction->GetScoringCarrierBoard();
    }
    if (!fScoringStand && detectorConstruction->GetScoringStand()) {
        fScoringStand = detectorConstruction->GetScoringStand();
    }
    if (!fScoringShield && detectorConstruction->GetScoringShield()) {
        fScoringShield = detectorConstruction->GetScoringShield();
    }

    G4cout << "Volume Mapping" << G4endl;
    G4cout << "1\t" << fScoringStand << G4endl;
    G4cout << "2\t" << fScoringShield << G4endl;
    G4cout << "3\t" << fScoringALPIDECircuit << G4endl;
    G4cout << "4\t" << fScoringALPIDEEpitaxial << G4endl;
    G4cout << "5\t" << fScoringCarrierBoardLogical << G4endl;
    fScoringVolumes[fScoringStand] = 1;
    fScoringVolumes[fScoringShield] = 2;
    fScoringVolumes[fScoringALPIDECircuit] = 3;
    fScoringVolumes[fScoringALPIDEEpitaxial] = 4;
    fScoringVolumes[fScoringCarrierBoardLogical] = 5;

    G4cout << "Scoring Mapping" << G4endl;
    for (auto it = fScoringVolumes.begin(); it != fScoringVolumes.end(); it++) {
        G4cout << it->first << " :: " << it->second << G4endl;
    }

    fBooked = true;
}

void AnalysisManager::AutoSave() {
    fTreeSource->AutoSave();
    fTreeStepper->AutoSave();
    fTreeTrack->AutoSave();
}

void AnalysisManager::Destroy() {
    G4cout << "AnalysisManager Closing file." << G4endl;
    if (fFile) {
        fFile->Delete("*");
    }
    G4cout << "AnalysisManager End of closing." << G4endl;
}

void AnalysisManager::FillStepper(const G4Step* step) {
    if (!bTreeStepper) return;
    if (step->GetTrack()->GetParentID() != 0 && GetRecordPrimaryOnly()) return;

    G4Track* track = step->GetTrack();
    fStepper.pid = track->GetDynamicParticle()->GetPDGcode();
    fStepper.Ekin = step->GetPreStepPoint()->GetKineticEnergy();

    G4ThreeVector preStep = step->GetPreStepPoint()->GetPosition();
    fStepper.preposX = preStep.x()/CLHEP::mm;
    fStepper.preposY = preStep.y()/CLHEP::mm;
    fStepper.preposZ = preStep.z()/CLHEP::mm;

    G4ThreeVector direction = track->GetMomentumDirection();
    fStepper.dirTheta = direction.theta();
    fStepper.dirPhi = direction.phi();
    fStepper.TID = step->GetTotalEnergyDeposit();
    fStepper.NIEL = step->GetNonIonizingEnergyDeposit();

    G4LogicalVolume* preVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    G4LogicalVolume* postVolume = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    fStepper.preVolume = fScoringVolumes[preVolume];
    fStepper.postVolume = fScoringVolumes[postVolume];

    fTreeStepper->Fill();
}

void AnalysisManager::FillTrack(const G4Step* step) {
    if (!bTreeTrack) return;
    if (step->GetTrack()->GetParentID() != 0 && GetRecordPrimaryOnly()) return;
    fTrack.pid = step->GetTrack()->GetDynamicParticle()->GetPDGcode();

    G4LogicalVolume* lv1 = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    Int_t lv1n = fScoringVolumes[lv1];

    G4double edep = (step->GetPreStepPoint()->GetKineticEnergy()-step->GetPostStepPoint()->GetKineticEnergy())/MeV;
    WDeltaEnergy[lv1n] += edep;

    if (!WPassed[lv1n]) WPassed[lv1n] = kTRUE;

    G4ThreeVector pos = step->GetPostStepPoint()->GetPosition();
    fTrack.posX = pos.getX();
    fTrack.posY = pos.getY();
    fTrack.posZ = pos.getZ();

    G4ThreeVector direction = step->GetTrack()->GetMomentumDirection();
    fSource.dirTheta = direction.theta();
    fSource.dirPhi = direction.phi();
}

void AnalysisManager::FillSource(const G4Track* track) {
    if (!bTreeSource) return;
    if (track->GetParentID() != 0 && GetRecordPrimaryOnly()) return;

    fSource.pid = track->GetDynamicParticle()->GetPDGcode();
    fSource.EKin = track->GetKineticEnergy();

    G4ThreeVector vertex = track->GetPosition();
    fSource.posX = vertex.x();
    fSource.posY = vertex.y();
    fSource.posZ = vertex.z();

    G4ThreeVector direction = track->GetMomentumDirection();
    fSource.dirTheta = direction.theta();
    fSource.dirPhi = direction.phi();

    if (fSource.dirPhi < 0.) fSource.dirPhi += twopi;

    fSource.weight = track->GetWeight();
    fTreeSource->Fill();
}

void AnalysisManager::EndOfTrack(const G4Track* track) {
    if (track->GetParentID() != 0 && GetRecordPrimaryOnly()) return;

    G4cout << "TrackPID = " << fTrack.pid << G4endl;

    G4cout << "Track Eloss\t";
    for (auto it = WDeltaEnergy.begin(); it != WDeltaEnergy.end(); ++it) {
        fTrack.Eloss[it->first] = it->second;
        G4cout << "LOSSAT" << it->first << "::" << it->second << "\t";
    }
    G4cout << G4endl;

    for (auto it = WPassed.begin(); it != WPassed.end(); ++it) {
        fTrack.Passed[it->first] = it->second;
        if (it->second) G4cout << "PASSED" << it->first << "\t";
    }
    G4cout << G4endl;

    if (bTreeTrack) {
        fTreeTrack->Fill();
    }
    WDeltaEnergy.clear();
    WPassed.clear();
    for (Int_t i = 0; i < fTrack.nVol; i++) {
        fTrack.Eloss[i] = 0;
        fTrack.Passed[i] = kFALSE;
    }
}

void AnalysisManager::DefineCommand() {
    fMessenger = new G4GenericMessenger(this, "/Analysis/","Analysis control");

    auto& SetFileNameCmd = fMessenger->DeclareMethod("SetFileName", &AnalysisManager::SetFileName,"SetFileName");
    SetFileNameCmd.SetParameterName("fName", true);
    SetFileNameCmd.SetDefaultValue("result.root");

    auto& SetRecordPrimaryCmd = fMessenger->DeclareMethod("RecordPrimaryOnly", & AnalysisManager::SetRecordPrimaryOnly, "RecordPrimaryOnly");
    SetRecordPrimaryCmd.SetParameterName("PrimaryOnly", true);
    SetRecordPrimaryCmd.SetDefaultValue("true");

    auto& SetRecordSetCmd = fMessenger->DeclareMethod("SetRecordSet", &AnalysisManager::SetRecordSet, "Option for RecordingSet, [Track, Stepper, Source]");
    SetRecordSetCmd.SetParameterName("RecordSet", true);
    SetRecordSetCmd.SetDefaultValue("111");
}

void AnalysisManager::SetFileName(TString fName) {
    if (fBooked) {
        G4cout << "AnalysisManager::SetFileName - Already the output rootfile is written before setting filename. (The Filename is changed to " << fName << " from " << fFileName << G4endl;
    }
    fFileName = fName;
}

void AnalysisManager::SetRecordSet(const Int_t number) {
    Int_t tempNum = number;
    if (tempNum % 10 == 1) {
        bTreeSource = true;
    } else {
        bTreeSource = false;
    }
    tempNum = tempNum / 10;
    if (tempNum % 10 == 1) {
        bTreeStepper = true;
    } else {
        bTreeStepper = false;
    }
    tempNum = tempNum / 10;
    if (tempNum % 10 == 1) {
        bTreeTrack = true;
    } else {
        bTreeTrack = false;
    }
}

void AnalysisManager::SetRecordPrimaryOnly(Bool_t primaryOnly) {
    fRecordPrimaryOnly = primaryOnly;
}

Bool_t AnalysisManager::GetRecordPrimaryOnly() {
    return fRecordPrimaryOnly;
}