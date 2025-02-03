#include "TGeantExtract.h"

TGeantExtract::TGeantExtract(const CppConfigFile& config) {
	CppConfigDictionary fileConfig = config.getConfig("File");

	if ( fileConfig.find("DIVIDED_NUM") != "1" ) {
		mIsDivide = true;
		mNumFile = std::stoi(fileConfig.find("DIVIDED_NUM"));
	} else {
		mIsDivide = false;
		mNumFile = 1;
	}

	std::filesystem::path inputFilePath = fileConfig.find("INPUT_FILE");
	mInputFileParentPath = inputFilePath.parent_path();
	mInputFileStem = inputFilePath.stem().string();
	mInputFileExtension = inputFilePath.extension().string();

	mPrimaryOutputFilePath = fileConfig.find("PRIMARY_OUTPUT_FILE");
	mIncidentOutputFilePath = fileConfig.find("INCIDENT_OUTPUT_FILE");
}

TGeantExtract::~TGeantExtract() { }

void TGeantExtract::openInputFile() {
	static int iFile = 0;
	TString inputFilePath;
	if ( mIsDivide ) {
		inputFilePath = (mInputFileParentPath / (mInputFileStem + "_" + std::to_string(iFile))).replace_extension(mInputFileExtension);
		iFile++;
	} else {
		inputFilePath = (mInputFileParentPath / mInputFileStem).replace_extension(mInputFileExtension);
	}
	mInputFile = std::make_unique<TFile>(inputFilePath, "READ");
	if ( !mInputFile->IsOpen() ) {
		std::cerr << "Error: Cannot open the input file" << std::endl;
	}
	initTrackTree();
	initIncidentTree();
	iFile++;
}


void TGeantExtract::openOutputFile() {
	mPrimaryAnalysisOutputFile = std::make_unique<TFile>(mPrimaryOutputFilePath.c_str(), "RECREATE");
	if ( !mPrimaryAnalysisOutputFile->IsOpen() ) {
		std::cerr << "Error: Cannot open the primary analysis output file" << std::endl;
	}
	initPrimaryAnalysisTree();

	mIncidentAnalysisOutputFile = std::make_unique<TFile>(mIncidentOutputFilePath.c_str(), "RECREATE");
	if ( !mIncidentAnalysisOutputFile->IsOpen() ) {
		std::cerr << "Error: Cannot open the incident analysis output file" << std::endl;
	}
	initIncidentAnalysisTree();
}

void TGeantExtract::extractTrack() {
	if ( mIsDivide ) {
		for ( int i = 0; i < mNumFile; i++ ) {
			extractFromAFile();
		}
	} else {
		extractFromAFile();
	}

	// Write and close primary analysis output file
	mPrimaryAnalysisOutputFile->cd();
	mPrimaryAnalysisTree->Write();
	mPrimaryAnalysisTree.reset(); // Reset the tree pointer
	mPrimaryAnalysisOutputFile->Close();

	// Write and close incident analysis output file
	mIncidentAnalysisOutputFile->cd();
	mIncidentAnalysisTree->Write();
	mIncidentAnalysisTree.reset(); // Reset the tree pointer
	mIncidentAnalysisOutputFile->Close();
}

void TGeantExtract::extractFromAFile() {
	static int j = 0;
	openInputFile();
	Int_t nEntries = mTrackTree->GetEntries();
	Int_t iIncident = 0;
	std::clog << "Extracting the informations from the File (" << j + 1 << " / " << mNumFile << ")" << std::endl;
	ProgressBar progressBar(static_cast<int>(nEntries));
	for ( Int_t i = 0; i < nEntries; i++ ) {
		progressBar.printProgress();
		mTrackTree->GetEntry(i);
		if ( mTrackTuple.parentID == 0 ) {
			getPrimaryAnalysisInformation();
		}
		mIncidentTree->GetEntry(iIncident);
		if ( mTrackTuple.eventID == mIncidentTuple.eventID && mTrackTuple.trackID == mIncidentTuple.trackID ) {
			getIncidentAnalysisInformation();
			iIncident++;
		}
	}
	j++;
}




















void TGeantExtract::initTrackTree() {
	mTrackTree.reset(static_cast<TTree*>(mInputFile->Get("trackTree")));

	mTrackTree->SetBranchAddress("eventID", &mTrackTuple.eventID);
	mTrackTree->SetBranchAddress("trackID", &mTrackTuple.trackID);
	mTrackTree->SetBranchAddress("parentID", &mTrackTuple.parentID);
	mTrackTree->SetBranchAddress("particleID", &mTrackTuple.particleID);
	mTrackTree->SetBranchAddress("initVolumeID", &mTrackTuple.initialVolumeID);
	mTrackTree->SetBranchAddress("initX", &mTrackTuple.initialPosition[0]);
	mTrackTree->SetBranchAddress("initY", &mTrackTuple.initialPosition[1]);
	mTrackTree->SetBranchAddress("initZ", &mTrackTuple.initialPosition[2]);
	mTrackTree->SetBranchAddress("initPX", &mTrackTuple.initialMomentum[0]);
	mTrackTree->SetBranchAddress("initPY", &mTrackTuple.initialMomentum[1]);
	mTrackTree->SetBranchAddress("initPZ", &mTrackTuple.initialMomentum[2]);
	mTrackTree->SetBranchAddress("initKineticEnergy", &mTrackTuple.initialKineticEnergy);
	mTrackTree->SetBranchAddress("finalVolumeID", &mTrackTuple.finalVolumeID);
	mTrackTree->SetBranchAddress("finalX", &mTrackTuple.finalPosition[0]);
	mTrackTree->SetBranchAddress("finalY", &mTrackTuple.finalPosition[1]);
	mTrackTree->SetBranchAddress("finalZ", &mTrackTuple.finalPosition[2]);
	mTrackTree->SetBranchAddress("finalPX", &mTrackTuple.finalMomentum[0]);
	mTrackTree->SetBranchAddress("finalPY", &mTrackTuple.finalMomentum[1]);
	mTrackTree->SetBranchAddress("finalPZ", &mTrackTuple.finalMomentum[2]);
	mTrackTree->SetBranchAddress("finalKineticEnergy", &mTrackTuple.finalKineticEnergy);
}

void TGeantExtract::initIncidentTree() {
	mIncidentTree.reset(static_cast<TTree*>(mInputFile->Get("incidentTree")));

	mIncidentTree->SetBranchAddress("eventID", &mIncidentTuple.eventID);
	mIncidentTree->SetBranchAddress("trackID", &mIncidentTuple.trackID);
	mIncidentTree->SetBranchAddress("depositEnergyMetal", &mIncidentTuple.depositEnergy[0]);
	mIncidentTree->SetBranchAddress("depositEnergyEpitaxial", &mIncidentTuple.depositEnergy[1]);
	mIncidentTree->SetBranchAddress("depositEnergySubstrate", &mIncidentTuple.depositEnergy[2]);
	mIncidentTree->SetBranchAddress("x", &mIncidentTuple.position[0]);
	mIncidentTree->SetBranchAddress("y", &mIncidentTuple.position[1]);
	mIncidentTree->SetBranchAddress("z", &mIncidentTuple.position[2]);
	mIncidentTree->SetBranchAddress("px", &mIncidentTuple.momentum[0]);
	mIncidentTree->SetBranchAddress("py", &mIncidentTuple.momentum[1]);
	mIncidentTree->SetBranchAddress("pz", &mIncidentTuple.momentum[2]);
	mIncidentTree->SetBranchAddress("kineticEnergy", &mIncidentTuple.kineticEnergy);
	mIncidentTree->SetBranchAddress("globalTime", &mIncidentTuple.globalTime);
	mIncidentTree->SetBranchAddress("localTime", &mIncidentTuple.localTime);
}

void TGeantExtract::initPrimaryAnalysisTree() {
	mPrimaryAnalysisTree = std::make_unique<TTree>("PrimaryAnalysis", "Primary Analysis Tree");

	mPrimaryAnalysisTree->Branch("eventID", &mPrimaryAnalysisTuple.eventID);
	mPrimaryAnalysisTree->Branch("x", &mPrimaryAnalysisTuple.position[0]);
	mPrimaryAnalysisTree->Branch("y", &mPrimaryAnalysisTuple.position[1]);
	mPrimaryAnalysisTree->Branch("z", &mPrimaryAnalysisTuple.position[2]);
	mPrimaryAnalysisTree->Branch("px", &mPrimaryAnalysisTuple.momentum[0]);
	mPrimaryAnalysisTree->Branch("py", &mPrimaryAnalysisTuple.momentum[1]);
	mPrimaryAnalysisTree->Branch("pz", &mPrimaryAnalysisTuple.momentum[2]);
	mPrimaryAnalysisTree->Branch("kineticEnergy", &mPrimaryAnalysisTuple.kineticEnergy);
}

void TGeantExtract::initIncidentAnalysisTree() {
	mIncidentAnalysisTree = std::make_unique<TTree>("IncidentAnalysis", "Incident Analysis Tree");

	mIncidentAnalysisTree->Branch("eventID", &mIncidentAnalysisTuple.eventID);
	mIncidentAnalysisTree->Branch("trackID", &mIncidentAnalysisTuple.trackID);
	mIncidentAnalysisTree->Branch("particleID", &mIncidentAnalysisTuple.particleID);
	mIncidentAnalysisTree->Branch("initialVolumeID", &mIncidentAnalysisTuple.initialVolumeID);
	mIncidentAnalysisTree->Branch("initX", &mIncidentAnalysisTuple.initialPosition[0]);
	mIncidentAnalysisTree->Branch("initY", &mIncidentAnalysisTuple.initialPosition[1]);
	mIncidentAnalysisTree->Branch("initZ", &mIncidentAnalysisTuple.initialPosition[2]);
	mIncidentAnalysisTree->Branch("initPX", &mIncidentAnalysisTuple.initialMomentum[0]);
	mIncidentAnalysisTree->Branch("initPY", &mIncidentAnalysisTuple.initialMomentum[1]);
	mIncidentAnalysisTree->Branch("initPZ", &mIncidentAnalysisTuple.initialMomentum[2]);
	mIncidentAnalysisTree->Branch("initKineticEnergy", &mIncidentAnalysisTuple.initialKineticEnergy);
	mIncidentAnalysisTree->Branch("depositEnergyMetal", &mIncidentAnalysisTuple.depositEnergy[0]);
	mIncidentAnalysisTree->Branch("depositEnergyEpitaxial", &mIncidentAnalysisTuple.depositEnergy[1]);
	mIncidentAnalysisTree->Branch("depositEnergySubstrate", &mIncidentAnalysisTuple.depositEnergy[2]);
	mIncidentAnalysisTree->Branch("incidentX", &mIncidentAnalysisTuple.position[0]);
	mIncidentAnalysisTree->Branch("incidentY", &mIncidentAnalysisTuple.position[1]);
	mIncidentAnalysisTree->Branch("incidentZ", &mIncidentAnalysisTuple.position[2]);
	mIncidentAnalysisTree->Branch("incidentPX", &mIncidentAnalysisTuple.momentum[0]);
	mIncidentAnalysisTree->Branch("incidentPY", &mIncidentAnalysisTuple.momentum[1]);
	mIncidentAnalysisTree->Branch("incidentPZ", &mIncidentAnalysisTuple.momentum[2]);
	mIncidentAnalysisTree->Branch("incidentKineticEnergy", &mIncidentAnalysisTuple.kineticEnergy);
	mIncidentAnalysisTree->Branch("incidentGlobalTime", &mIncidentAnalysisTuple.globalTime);
	mIncidentAnalysisTree->Branch("incidentLocalTime", &mIncidentAnalysisTuple.localTime);
	mIncidentAnalysisTree->Branch("finalVolumeID", &mIncidentAnalysisTuple.finalVolumeID);
	mIncidentAnalysisTree->Branch("finalX", &mIncidentAnalysisTuple.finalPosition[0]);
	mIncidentAnalysisTree->Branch("finalY", &mIncidentAnalysisTuple.finalPosition[1]);
	mIncidentAnalysisTree->Branch("finalZ", &mIncidentAnalysisTuple.finalPosition[2]);
	mIncidentAnalysisTree->Branch("finalPX", &mIncidentAnalysisTuple.finalMomentum[0]);
	mIncidentAnalysisTree->Branch("finalPY", &mIncidentAnalysisTuple.finalMomentum[1]);
	mIncidentAnalysisTree->Branch("finalPZ", &mIncidentAnalysisTuple.finalMomentum[2]);
	mIncidentAnalysisTree->Branch("finalKineticEnergy", &mIncidentAnalysisTuple.finalKineticEnergy);
}

void TGeantExtract::getPrimaryAnalysisInformation() {
	mPrimaryAnalysisTuple.eventID = mTrackTuple.eventID;
	mPrimaryAnalysisTuple.position[0] = mTrackTuple.initialPosition[0];
	mPrimaryAnalysisTuple.position[1] = mTrackTuple.initialPosition[1];
	mPrimaryAnalysisTuple.position[2] = mTrackTuple.initialPosition[2];
	mPrimaryAnalysisTuple.momentum[0] = mTrackTuple.initialMomentum[0];
	mPrimaryAnalysisTuple.momentum[1] = mTrackTuple.initialMomentum[1];
	mPrimaryAnalysisTuple.momentum[2] = mTrackTuple.initialMomentum[2];
	mPrimaryAnalysisTuple.kineticEnergy = mTrackTuple.initialKineticEnergy;

	mPrimaryAnalysisTree->Fill();
}

void TGeantExtract::getIncidentAnalysisInformation() {
	mIncidentAnalysisTuple.eventID = mIncidentTuple.eventID;
	mIncidentAnalysisTuple.trackID = mIncidentTuple.trackID;
	mIncidentAnalysisTuple.particleID = mTrackTuple.particleID;
	mIncidentAnalysisTuple.initialVolumeID = mTrackTuple.initialVolumeID;
	mIncidentAnalysisTuple.initialPosition[0] = mTrackTuple.initialPosition[0];
	mIncidentAnalysisTuple.initialPosition[1] = mTrackTuple.initialPosition[1];
	mIncidentAnalysisTuple.initialPosition[2] = mTrackTuple.initialPosition[2];
	mIncidentAnalysisTuple.initialMomentum[0] = mTrackTuple.initialMomentum[0];
	mIncidentAnalysisTuple.initialMomentum[1] = mTrackTuple.initialMomentum[1];
	mIncidentAnalysisTuple.initialMomentum[2] = mTrackTuple.initialMomentum[2];
	mIncidentAnalysisTuple.initialKineticEnergy = mTrackTuple.initialKineticEnergy;
	mIncidentAnalysisTuple.depositEnergy[0] = mIncidentTuple.depositEnergy[0];
	mIncidentAnalysisTuple.depositEnergy[1] = mIncidentTuple.depositEnergy[1];
	mIncidentAnalysisTuple.depositEnergy[2] = mIncidentTuple.depositEnergy[2];
	mIncidentAnalysisTuple.position[0] = mIncidentTuple.position[0];
	mIncidentAnalysisTuple.position[1] = mIncidentTuple.position[1];
	mIncidentAnalysisTuple.position[2] = mIncidentTuple.position[2];
	mIncidentAnalysisTuple.momentum[0] = mIncidentTuple.momentum[0];
	mIncidentAnalysisTuple.momentum[1] = mIncidentTuple.momentum[1];
	mIncidentAnalysisTuple.momentum[2] = mIncidentTuple.momentum[2];
	mIncidentAnalysisTuple.kineticEnergy = mIncidentTuple.kineticEnergy;
	mIncidentAnalysisTuple.globalTime = mIncidentTuple.globalTime;
	mIncidentAnalysisTuple.localTime = mIncidentTuple.localTime;
	mIncidentAnalysisTuple.finalVolumeID = mTrackTuple.finalVolumeID;
	mIncidentAnalysisTuple.finalPosition[0] = mTrackTuple.finalPosition[0];
	mIncidentAnalysisTuple.finalPosition[1] = mTrackTuple.finalPosition[1];
	mIncidentAnalysisTuple.finalPosition[2] = mTrackTuple.finalPosition[2];
	mIncidentAnalysisTuple.finalMomentum[0] = mTrackTuple.finalMomentum[0];
	mIncidentAnalysisTuple.finalMomentum[1] = mTrackTuple.finalMomentum[1];
	mIncidentAnalysisTuple.finalMomentum[2] = mTrackTuple.finalMomentum[2];
	mIncidentAnalysisTuple.finalKineticEnergy = mTrackTuple.finalKineticEnergy;

	mIncidentAnalysisTree->Fill();
}