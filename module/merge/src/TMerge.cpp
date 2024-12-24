#define __TMERGE_HEADER__
#include "TMerge.h"

#include<iostream>
#include<filesystem>
#include<memory>

#include "TFile.h"
#include "TTree.h"

#include "cpptqdm.h"

TMerge::TMerge(std::string_view outputFileName, const std::vector<std::string>& inputFileNames) : mOutputFileName(outputFileName) {
	mInputFileNameSet.assign(inputFileNames.begin(), inputFileNames.end());
}

TMerge::~TMerge() {
	std::clog << "Total " << mInputFileNameSet.size() << " files are merged as " << mOutputFileName << "." << std::endl;
}

void TMerge::checkFile() {
	std::vector<std::string_view> unreadableFiles;
	bool isReadableAll = true;
	for ( const auto& inputFileName : mInputFileNameSet ) {
		if ( !std::filesystem::exists(inputFileName) || !std::filesystem::is_regular_file(inputFileName) ) {
			unreadableFiles.push_back(inputFileName);
			isReadableAll = false;
			continue;
		}
		std::filesystem::file_status status = std::filesystem::status(inputFileName);
		auto permissions = status.permissions();
		if ( (permissions & std::filesystem::perms::owner_read) == std::filesystem::perms::none &&
			(permissions & std::filesystem::perms::group_read) == std::filesystem::perms::none &&
			(permissions & std::filesystem::perms::others_read) == std::filesystem::perms::none ) {
			unreadableFiles.push_back(inputFileName);
			isReadableAll = false;
		}
	}
	if ( !isReadableAll ) {
		std::cerr << "Cannot read the following files:" << std::endl;
		for ( const auto& unreadableFile : unreadableFiles ) {
			std::cerr << "\t" << unreadableFile << std::endl;
		}
		exit(1);
	}
}

void TMerge::mergeFileALPIDEROOT() {
	auto outputFile = std::make_unique<TFile>(static_cast<TString>(mOutputFileName), "RECREATE");

	UChar_t outputChipid;
	UInt_t outputTimeStamp;
	UShort_t outputX, outputY;

	auto outputTree = std::make_unique<TTree>("hit", "hit");
	outputTree->Branch("ChipID", &outputChipid, "id/b");
	outputTree->Branch("TimeStamp", &outputTimeStamp, "t/i");
	outputTree->Branch("X", &outputX, "x/s");
	outputTree->Branch("Y", &outputY, "y/s");

	UInt_t preTimeStamp;

	ProgressBar pbar(mInputFileNameSet.size());
	for ( std::string_view inputFileName : mInputFileNameSet ) {
		pbar.printProgress();
		auto inputFile = std::make_unique<TFile>(static_cast<TString>(inputFileName), "READ");
		auto inputTree = std::unique_ptr<TTree>(static_cast<TTree*>(inputFile->Get("hit")));

		UChar_t inputChipid;
		UInt_t inputTimeStamp;
		UShort_t inputX, inputY;

		inputTree->SetBranchAddress("ChipID", &inputChipid);
		inputTree->SetBranchAddress("TimeStamp", &inputTimeStamp);
		inputTree->SetBranchAddress("X", &inputX);
		inputTree->SetBranchAddress("Y", &inputY);
		for ( int entry = 0; entry < inputTree->GetEntries(); entry++ ) {
			inputTree->GetEntry(entry);
			outputChipid = inputChipid;
			outputTimeStamp = inputTimeStamp + preTimeStamp + 1;
			outputX = inputX;
			outputY = inputY;
			outputTree->Fill();
		}
		preTimeStamp = inputTimeStamp;
	}

	outputFile->cd();
	outputTree->Write();
}