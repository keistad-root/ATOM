#include "TAnalyser.h"

TAnalyser::TAnalyser(TString inputFileTitle) : mInputFile(new TFile(inputFileTitle, "READ")) {
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

void TAnalyser::setConfFile(std::string_view configFileTitle) {
	mConfFile = new Configuration(configFileTitle);
	if ( mConfFile->getConfig("ExperimentSetting").has_value() ) {
		for ( const std::pair<std::string, std::string>& configSetting : mConfFile->getConfig("ExperimentSetting").value().getDictionary() ) {
			mConfigSetting.insert_or_assign(configSetting.first, stoi(configSetting.second));
		}
	}
}

void TAnalyser::setConfFile(Configuration* config) {
	mConfFile = config;
	if ( mConfFile->getConfig("ExperimentSetting").has_value() ) {
		for ( const std::pair<std::string, std::string>& configSetting : mConfFile->getConfig("ExperimentSetting").value().getDictionary() ) {
			mConfigSetting.insert_or_assign(configSetting.first, stoi(configSetting.second));
		}
	}
}

const Configuration* TAnalyser::getConfFile() const {
	return mConfFile;
}

const std::unordered_map<std::string, int> TAnalyser::getConfigSetting() const {
	return mConfigSetting;
}

TTree* TAnalyser::openTree(std::string treeName) {
	if ( mInputFile->Get(static_cast<TString>(treeName)) == nullptr ) {
		std::cerr << "TTree " << treeName << " isn't belong to TFile" << std::endl;
		return nullptr;
	} else if ( std::string(mInputFile->Get(static_cast<TString>(treeName))->ClassName()) != "TTree" ) {
		std::cerr << "" << treeName << " isn't TTree" << std::endl;
		return nullptr;
	} else {
		return (TTree*) mInputFile->Get(static_cast<TString>(treeName));
	}
}