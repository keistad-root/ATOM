#define __TMERGE_HEADER__
#include "TMerge.h"

TMerge::TMerge(std::string_view outputFileName, const std::vector<std::string>& inputFileNames) : mOutputFileName(outputFileName) {
	mInputFileNames.assign(inputFileNames.begin(), inputFileNames.end());
}

TMergeExperimentROOT::TMergeExperimentROOT(std::string_view outputFileName, const std::vector<std::string>& inputFileNames) : TMerge(outputFileName, inputFileNames) {
	mOutputFile = new TFile(static_cast<TString>(outputFileName), "RECREATE");
}

void TMergeExperimentROOT::mergeFile() {
	try {
		TTree* outputTree = new TTree("hit", "hit");
		outputTree->Branch("ChipID", &mOutputValue.chipid, "id/b");
		outputTree->Branch("TimeStamp", &mOutputValue.timeStamp, "t/i");
		outputTree->Branch("X", &mOutputValue.x, "x/s");
		outputTree->Branch("Y", &mOutputValue.y, "y/s");

		int preTime = 0;
		for ( std::string_view inputFileName : mInputFileNames ) {
			TFile* inputFile = new TFile(static_cast<TString>(inputFileName), "READ");
			if ( !inputFile->IsOpen() ) {
				throw MergeFileOpen(inputFileName);
			} else if ( inputFile->Get("hit") == nullptr ) {
				throw MergeTreeOpen(inputFileName);
			}
			std::clog << inputFileName << std::endl;
			TTree* inputTree = static_cast<TTree*>(inputFile->Get("hit"));
			inputTree->SetBranchAddress("ChipID", &mInputValue.chipid);
			inputTree->SetBranchAddress("TimeStamp", &mInputValue.timeStamp);
			inputTree->SetBranchAddress("X", &mInputValue.x);
			inputTree->SetBranchAddress("Y", &mInputValue.y);
			ProgressBar* pbar = new ProgressBar(inputTree->GetEntries());
			for ( int entry = 0; entry < inputTree->GetEntries(); entry++ ) {
				pbar->printProgress();
				inputTree->GetEntry(entry);
				mOutputValue.chipid = mInputValue.chipid;
				mOutputValue.timeStamp = mInputValue.timeStamp + preTime;
				mOutputValue.x = mInputValue.x;
				mOutputValue.y = mInputValue.y;
				outputTree->Fill();
			}
			delete pbar;
			preTime = mInputValue.timeStamp + 1;
			inputFile->Close();
		}
		mOutputFile->Write();
	} catch ( MergeFileOpen error ) {
		std::cerr << error.what() << std::endl;
		exit(1);
	} catch ( MergeTreeOpen error ) {
		std::cerr << error.what() << std::endl;
		exit(1);
	}
}

TMergeExperimentROOT::~TMergeExperimentROOT() {
	mOutputFile->Close();
}