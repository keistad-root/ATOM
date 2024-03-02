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

void AnalysisManager::Destroy() {
    G4cout << "AnalysisManager Closing file." << G4endl;
    if (fFile) {
        fFile->Delete("*");
    }
    G4cout << "AnalysisManager End of closing." << G4endl;
}

AnalysisManager* AnalysisManager::Instance() {
    if (fInstance == 0) {
        fInstance = new AnalysisManager();
    }
    return fInstance;
}

void AnalysisManager::DefineCommand() {
    fMessenger = new G4GenericMessenger(this, "/Analysis/","Analysis control");

    // auto& SetFileNameCmd = fMessenger->DeclareMethod("SetFileName", &AnalysisManager::SetFileName,"SetFileName");
}

void AnalysisManager::Write(){
    // G4cout<<"AVSAnalysisManager::Write - Writing File to " << fFile->GetName() <<G4endl;
    // TString fst = fFile->IsOpen() ? "True" : "False";
    // G4cout<<"File is opened? : "<< fst << G4endl;
    // G4cout<<"In Option : "<< fFile->GetOption() << G4endl;
    // fFile->Write(0, TObject::kOverwrite, 0);
    // G4cout<<"AVSAnalysisManager::Write - File writed in " << fFile->GetName() <<G4endl;
}