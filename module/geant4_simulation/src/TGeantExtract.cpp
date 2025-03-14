#include "TGeantExtract.h"

#include "TVolumeID.h"

TGeantExtract::TGeantExtract(const CppConfigFile& config) {
	CppConfigDictionary fileConfig = config.getConfig("File");

	// if ( fileConfig.find("DIVIDED_NUM") != "1" ) {
	mIsDivide = true;
	mNumFile = std::stoi(fileConfig.find("DIVIDED_NUM"));
	// } else {
	// 	mIsDivide = false;
	// 	mNumFile = 1;
	// }

	std::filesystem::path inputFilePath = fileConfig.find("INPUT_FILE");
	mInputFileParentPath = inputFilePath.parent_path();
	mInputFileStem = inputFilePath.stem().string();
	mInputFileExtension = inputFilePath.extension().string();

	mPrimaryOutputFilePath = fileConfig.find("PRIMARY_OUTPUT_FILE");
	mIncidentOutputFilePath = fileConfig.find("INCIDENT_OUTPUT_FILE");
	// mSecondaryOutputFilePath = fileConfig.find("SECONDARY_OUTPUT_FILE");
}

TGeantExtract::~TGeantExtract() {
	delete mTrackTree;
}

void TGeantExtract::openInputFile() {
	static int iFile = 0;
	TString inputFilePath;
	if ( mIsDivide ) {
		inputFilePath = (mInputFileParentPath / (mInputFileStem + "_" + std::to_string(iFile))).replace_extension(mInputFileExtension);
		iFile++;
	} else {
		inputFilePath = (mInputFileParentPath / mInputFileStem).replace_extension(mInputFileExtension);
	}
	mInputFile.reset(new TFile(inputFilePath, "READ"));
	initTrackTree();
}


void TGeantExtract::openOutputFile() {
	mPrimaryAnalysisOutputFile = new TFile(mPrimaryOutputFilePath.c_str(), "RECREATE");
	if ( !mPrimaryAnalysisOutputFile->IsOpen() ) {
		std::cerr << "Error: Cannot open the primary analysis output file" << std::endl;
	}
	initPrimaryAnalysisTree();

	mIncidentAnalysisOutputFile = new TFile(mIncidentOutputFilePath.c_str(), "RECREATE");
	if ( !mIncidentAnalysisOutputFile->IsOpen() ) {
		std::cerr << "Error: Cannot open the incident analysis output file" << std::endl;
	}
	initIncidentAnalysisTree();

	// mSecondaryAnalysisOutputFile = new TFile(mSecondaryOutputFilePath.c_str(), "RECREATE");
	// if ( !mSecondaryAnalysisOutputFile->IsOpen() ) {
	// 	std::cerr << "Error: Cannot open the secondary analysis output file" << std::endl;
	// }
	// initSecondaryAnalysisTree();
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
	mPrimaryAnalysisOutputFile->Close();

	// Write and close incident analysis output file
	mIncidentAnalysisOutputFile->cd();
	mIncidentAnalysisTree->Write();
	mIncidentAnalysisOutputFile->Close();

	// Write and close secondary analysis output file
	// mSecondaryAnalysisOutputFile->cd();
	// mSecondaryAnalysisTree->Write();
	// mSecondaryAnalysisOutputFile->Close();
}

bool TGeantExtract::isIncident() {
	if ( mTrackTuple.isInALPIDE && (mTrackTuple.initVolumeID == VOLUME::World || mTrackTuple.initVolumeID == VOLUME::Collimator || mTrackTuple.initVolumeID == VOLUME::Screen) ) {
		return true;
	} else if ( mTrackTuple.initVolumeID == VOLUME::ALPIDEMetal || mTrackTuple.initVolumeID == VOLUME::ALPIDEEpitaxial || mTrackTuple.initVolumeID == VOLUME::ALPIDESubstrate ) {
		return true;
	} else {
		return false;
	}
}

// bool TGeantExtract::isProducedInALPIDE() {
// 	if ( mTrackTuple.initVolumeID == VOLUME::ALPIDEMetal || mTrackTuple.initVolumeID == VOLUME::ALPIDEEpitaxial || mTrackTuple.initVolumeID == VOLUME::ALPIDESubstrate ) {
// 		return true;
// 	} else {
// 		return false;
// 	}
// }

void TGeantExtract::extractFromAFile() {
	static int iFile = 0;
	openInputFile();
	Int_t nEntries = mTrackTree->GetEntries();
	Int_t iIncident = 0;
	std::clog << "Extracting the informations from the File (" << iFile + 1 << " / " << mNumFile << ")" << std::endl;
	ProgressBar progressBar(static_cast<int>(nEntries));
	for ( Int_t i = 0; i < nEntries; i++ ) {
		progressBar.printProgress();
		mTrackTree->GetEntry(i);
		if ( mTrackTuple.parentID == 0 ) {
			getPrimaryAnalysisInformation();
		}
		if ( isIncident() ) {
			getIncidentAnalysisInformation();
		}
		// if ( isProducedInALPIDE() ) {
		// 	getSecondaryAnalysisInformation();
		// }
	}
	iFile++;
}

void TGeantExtract::initTrackTree() {
	mTrackTree = static_cast<TTree*>(mInputFile->Get("trackTree"));

	mTrackTree->SetBranchAddress("eventID", &mTrackTuple.eventID);
	mTrackTree->SetBranchAddress("trackID", &mTrackTuple.trackID);
	mTrackTree->SetBranchAddress("parentID", &mTrackTuple.parentID);
	mTrackTree->SetBranchAddress("particleID", &mTrackTuple.particleID);
	mTrackTree->SetBranchAddress("initVolumeID", &mTrackTuple.initVolumeID);
	mTrackTree->SetBranchAddress("initPosition", mTrackTuple.initPosition);
	mTrackTree->SetBranchAddress("initMomentum", mTrackTuple.initMomentum);
	mTrackTree->SetBranchAddress("initKineticEnergy", &mTrackTuple.initKineticEnergy);
	mTrackTree->SetBranchAddress("incidentPosition", mTrackTuple.incidentPosition);
	mTrackTree->SetBranchAddress("incidentMomentum", mTrackTuple.incidentMomentum);
	mTrackTree->SetBranchAddress("incidentKineticEnergy", &mTrackTuple.incidentKineticEnergy);
	mTrackTree->SetBranchAddress("depositEnergy", mTrackTuple.depositEnergy);
	mTrackTree->SetBranchAddress("globalTime", &mTrackTuple.globalTime);
	mTrackTree->SetBranchAddress("localTime", &mTrackTuple.localTime);
	mTrackTree->SetBranchAddress("finalVolumeID", &mTrackTuple.finalVolumeID);
	mTrackTree->SetBranchAddress("finalPosition", mTrackTuple.finalPosition);
	mTrackTree->SetBranchAddress("finalMomentum", mTrackTuple.finalMomentum);
	mTrackTree->SetBranchAddress("finalKineticEnergy", &mTrackTuple.finalKineticEnergy);
	mTrackTree->SetBranchAddress("isInALPIDE", &mTrackTuple.isInALPIDE);
}

void TGeantExtract::initPrimaryAnalysisTree() {
	mPrimaryAnalysisTree = new TTree("PrimaryAnalysis", "Primary Analysis Tree");

	mPrimaryAnalysisTree->Branch("eventID", &mPrimaryAnalysisTuple.eventID);
	mPrimaryAnalysisTree->Branch("position", mPrimaryAnalysisTuple.position, "position[3]/D");
	mPrimaryAnalysisTree->Branch("momentum", mPrimaryAnalysisTuple.momentum, "momentum[3]/D");
	mPrimaryAnalysisTree->Branch("kineticEnergy", &mPrimaryAnalysisTuple.kineticEnergy);
}

void TGeantExtract::initIncidentAnalysisTree() {
	mIncidentAnalysisTree = new TTree("IncidentAnalysis", "Incident Analysis Tree");

	mIncidentAnalysisTree->Branch("eventID", &mIncidentAnalysisTuple.eventID);
	mIncidentAnalysisTree->Branch("trackID", &mIncidentAnalysisTuple.trackID);
	mIncidentAnalysisTree->Branch("particleID", &mIncidentAnalysisTuple.particleID);
	mIncidentAnalysisTree->Branch("initialVolumeID", &mIncidentAnalysisTuple.initialVolumeID);
	mIncidentAnalysisTree->Branch("initPosition", mIncidentAnalysisTuple.initialPosition, "initPosition[3]/D");
	mIncidentAnalysisTree->Branch("initMomentum", mIncidentAnalysisTuple.initialMomentum, "initMomentum[3]/D");
	mIncidentAnalysisTree->Branch("initKineticEnergy", &mIncidentAnalysisTuple.initialKineticEnergy);
	mIncidentAnalysisTree->Branch("depositEnergy", mIncidentAnalysisTuple.depositEnergy, "depositEnergy[3]/D");
	mIncidentAnalysisTree->Branch("incidentPosition", mIncidentAnalysisTuple.position, "incidentPosition[3]/D");
	mIncidentAnalysisTree->Branch("incidentMomentum", mIncidentAnalysisTuple.momentum, "incidentMomentum[3]/D");
	mIncidentAnalysisTree->Branch("incidentKineticEnergy", &mIncidentAnalysisTuple.kineticEnergy);
	mIncidentAnalysisTree->Branch("incidentGlobalTime", &mIncidentAnalysisTuple.globalTime);
	mIncidentAnalysisTree->Branch("incidentLocalTime", &mIncidentAnalysisTuple.localTime);
	mIncidentAnalysisTree->Branch("finalVolumeID", &mIncidentAnalysisTuple.finalVolumeID);
	mIncidentAnalysisTree->Branch("finalPosition", mIncidentAnalysisTuple.finalPosition, "finalPosition[3]/D");
	mIncidentAnalysisTree->Branch("finalMomentum", mIncidentAnalysisTuple.finalMomentum, "finalMomentum[3]/D");
	mIncidentAnalysisTree->Branch("finalKineticEnergy", &mIncidentAnalysisTuple.finalKineticEnergy);
}

// void TGeantExtract::initSecondaryAnalysisTree() {
// 	mSecondaryAnalysisTree = new TTree("SecondaryAnalysis", "Secondary Analysis Tree");

// 	mSecondaryAnalysisTree->Branch("eventID", &mSecondaryAnalysisTuple.eventID);
// 	mSecondaryAnalysisTree->Branch("trackID", &mSecondaryAnalysisTuple.trackID);
// 	mSecondaryAnalysisTree->Branch("parentID", &mSecondaryAnalysisTuple.parentID);
// 	mSecondaryAnalysisTree->Branch("particleID", &mSecondaryAnalysisTuple.particleID);
// 	mSecondaryAnalysisTree->Branch("initialVolumeID", &mSecondaryAnalysisTuple.initialVolumeID);
// 	mSecondaryAnalysisTree->Branch("initPosition", mSecondaryAnalysisTuple.initialPosition, "initPosition[3]/D");
// 	mSecondaryAnalysisTree->Branch("initMomentum", mSecondaryAnalysisTuple.initialMomentum, "initMomentum[3]/D");
// 	mSecondaryAnalysisTree->Branch("initKineticEnergy", &mSecondaryAnalysisTuple.initialKineticEnergy);
// 	mIncidentAnalysisTree->Branch("depositEnergy", mSecondaryAnalysisTuple.depositEnergy, "depositEnergy[3]/D");
// 	mSecondaryAnalysisTree->Branch("finalVolumeID", &mSecondaryAnalysisTuple.finalVolumeID);
// 	mSecondaryAnalysisTree->Branch("finalPosition", mSecondaryAnalysisTuple.finalPosition, "finalPosition[3]/D");
// 	mSecondaryAnalysisTree->Branch("finalMomentum", mSecondaryAnalysisTuple.finalMomentum, "finalMomentum[3]/D");
// 	mSecondaryAnalysisTree->Branch("finalKineticEnergy", &mSecondaryAnalysisTuple.finalKineticEnergy);
// }

void TGeantExtract::getPrimaryAnalysisInformation() {
	mPrimaryAnalysisTuple.eventID = mTrackTuple.eventID;
	mPrimaryAnalysisTuple.position[0] = mTrackTuple.initPosition[0];
	mPrimaryAnalysisTuple.position[1] = mTrackTuple.initPosition[1];
	mPrimaryAnalysisTuple.position[2] = mTrackTuple.initPosition[2];
	mPrimaryAnalysisTuple.momentum[0] = mTrackTuple.initMomentum[0];
	mPrimaryAnalysisTuple.momentum[1] = mTrackTuple.initMomentum[1];
	mPrimaryAnalysisTuple.momentum[2] = mTrackTuple.initMomentum[2];
	mPrimaryAnalysisTuple.kineticEnergy = mTrackTuple.initKineticEnergy;

	mPrimaryAnalysisTree->Fill();
}

void TGeantExtract::getIncidentAnalysisInformation() {
	mIncidentAnalysisTuple.eventID = mTrackTuple.eventID;
	mIncidentAnalysisTuple.trackID = mTrackTuple.trackID;
	mIncidentAnalysisTuple.particleID = mTrackTuple.particleID;
	mIncidentAnalysisTuple.initialVolumeID = mTrackTuple.initVolumeID;
	mIncidentAnalysisTuple.initialPosition[0] = mTrackTuple.initPosition[0];
	mIncidentAnalysisTuple.initialPosition[1] = mTrackTuple.initPosition[1];
	mIncidentAnalysisTuple.initialPosition[2] = mTrackTuple.initPosition[2];
	mIncidentAnalysisTuple.initialMomentum[0] = mTrackTuple.initMomentum[0];
	mIncidentAnalysisTuple.initialMomentum[1] = mTrackTuple.initMomentum[1];
	mIncidentAnalysisTuple.initialMomentum[2] = mTrackTuple.initMomentum[2];
	mIncidentAnalysisTuple.initialKineticEnergy = mTrackTuple.initKineticEnergy;
	mIncidentAnalysisTuple.depositEnergy[0] = mTrackTuple.depositEnergy[0];
	mIncidentAnalysisTuple.depositEnergy[1] = mTrackTuple.depositEnergy[1];
	mIncidentAnalysisTuple.depositEnergy[2] = mTrackTuple.depositEnergy[2];
	mIncidentAnalysisTuple.position[0] = mTrackTuple.incidentPosition[0];
	mIncidentAnalysisTuple.position[1] = mTrackTuple.incidentPosition[1];
	mIncidentAnalysisTuple.position[2] = mTrackTuple.incidentPosition[2];
	mIncidentAnalysisTuple.momentum[0] = mTrackTuple.incidentMomentum[0];
	mIncidentAnalysisTuple.momentum[1] = mTrackTuple.incidentMomentum[1];
	mIncidentAnalysisTuple.momentum[2] = mTrackTuple.incidentMomentum[2];
	mIncidentAnalysisTuple.kineticEnergy = mTrackTuple.incidentKineticEnergy;
	mIncidentAnalysisTuple.globalTime = mTrackTuple.globalTime;
	mIncidentAnalysisTuple.localTime = mTrackTuple.localTime;
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

// void TGeantExtract::getSecondaryAnalysisInformation() {
// 	mSecondaryAnalysisTuple.eventID = mTrackTuple.eventID;
// 	mSecondaryAnalysisTuple.trackID = mTrackTuple.trackID;
// 	mSecondaryAnalysisTuple.parentID = mTrackTuple.parentID;
// 	mSecondaryAnalysisTuple.particleID = mTrackTuple.particleID;
// 	mSecondaryAnalysisTuple.initialVolumeID = mTrackTuple.initVolumeID;
// 	mSecondaryAnalysisTuple.initialPosition[0] = mTrackTuple.initPosition[0];
// 	mSecondaryAnalysisTuple.initialPosition[1] = mTrackTuple.initPosition[1];
// 	mSecondaryAnalysisTuple.initialPosition[2] = mTrackTuple.initPosition[2];
// 	mSecondaryAnalysisTuple.initialMomentum[0] = mTrackTuple.initMomentum[0];
// 	mSecondaryAnalysisTuple.initialMomentum[1] = mTrackTuple.initMomentum[1];
// 	mSecondaryAnalysisTuple.initialMomentum[2] = mTrackTuple.initMomentum[2];
// 	mSecondaryAnalysisTuple.initialKineticEnergy = mTrackTuple.initKineticEnergy;
// 	mSecondaryAnalysisTuple.depositEnergy[0] = mTrackTuple.depositEnergy[0];
// 	mSecondaryAnalysisTuple.depositEnergy[1] = mTrackTuple.depositEnergy[1];
// 	mSecondaryAnalysisTuple.depositEnergy[2] = mTrackTuple.depositEnergy[2];
// 	mSecondaryAnalysisTuple.finalVolumeID = mTrackTuple.finalVolumeID;
// 	mSecondaryAnalysisTuple.finalPosition[0] = mTrackTuple.finalPosition[0];
// 	mSecondaryAnalysisTuple.finalPosition[1] = mTrackTuple.finalPosition[1];
// 	mSecondaryAnalysisTuple.finalPosition[2] = mTrackTuple.finalPosition[2];
// 	mSecondaryAnalysisTuple.finalMomentum[0] = mTrackTuple.finalMomentum[0];
// 	mSecondaryAnalysisTuple.finalMomentum[1] = mTrackTuple.finalMomentum[1];
// 	mSecondaryAnalysisTuple.finalMomentum[2] = mTrackTuple.finalMomentum[2];
// 	mSecondaryAnalysisTuple.finalKineticEnergy = mTrackTuple.finalKineticEnergy;

// 	mSecondaryAnalysisTree->Fill();
// }