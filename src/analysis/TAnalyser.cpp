#include "TAnalyser.h"

TAnalyser::TAnalyser(TString inputFileTitle) : mInputFile(new TFile(inputFileTitle,"READ")) {
    std::cout << "TAnalyser class is armed" << std::endl;
}

TAnalyser::TAnalyser(TFile* inputFile) {
    std::cout << "TAnalyser class is armed" << std::endl;
    mInputFile = inputFile;
}

TAnalyser::TAnalyser(TFile& inputFile) {
    std::cout << "TAnalyser class is armed" << std::endl;
    mInputFile = &inputFile;
}

TAnalyser::~TAnalyser() {
    std::cout << "TAnalyser class is terminated" << std::endl;
    mInputFile->Close();
}

void TAnalyser::setInputFile(TString inputFileTitle) {
    mInputFile->Open(inputFileTitle, "READ");
}

void TAnalyser::setInputFile(TFile* inputFile) {
    mInputFile->Copy(*inputFile);

}

void TAnalyser::setInputFile(TFile& inputFile) {
    mInputFile->Copy(inputFile);
}

TTree* TAnalyser::openTree(std::string treeName) {
    if (mInputFile->Get(static_cast<TString>(treeName)) == nullptr) {
        std::cerr << "TTree " << treeName << " isn't belong to TFile" << std::endl;
        return nullptr;
    } else if (std::string(mInputFile->Get(static_cast<TString>(treeName))->ClassName()) != "TTree" ) {
        std::cerr << "" << treeName << " isn't TTree" << std::endl;
        return nullptr;
    } else {
        return (TTree*) mInputFile->Get(static_cast<TString>(treeName));
    }
}