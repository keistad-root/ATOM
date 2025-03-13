#include "TAnalysisManager.h"

TAnalysisManager* TAnalysisManager::mInstance = nullptr;

TAnalysisManager::TAnalysisManager() {
	mInstance = this;
}

TAnalysisManager::~TAnalysisManager() {
	close();
}

TAnalysisManager* TAnalysisManager::Instance() {
	if ( mInstance == 0 ) {
		mInstance = new TAnalysisManager();
	}
	return mInstance;
}

void TAnalysisManager::open(const G4String& name) {
	mFileName = name;
	std::filesystem::path path = (std::string) name;
	static int fileIndex = 0;
	G4String newPath = path.parent_path().string() + "/" + static_cast<std::string>(path.stem()) + "_" + std::to_string(fileIndex) + static_cast<std::string>(path.extension());
	fileIndex++;

	mFile = new TFile(newPath, "RECREATE");
	mTrackTree = new TTree("trackTree", "Track Information");

	mTrackTree->Branch("eventID", &mTrackTuple.eventID, "eventID/I");
	mTrackTree->Branch("trackID", &mTrackTuple.trackID, "trackID/I");
	mTrackTree->Branch("parentID", &mTrackTuple.parentID, "parentID/I");
	mTrackTree->Branch("particleID", &mTrackTuple.particleID, "particleID/I");
	mTrackTree->Branch("initVolumeID", &mTrackTuple.initVolumeID, "initVolumeID/I");
	mTrackTree->Branch("initPosition", mTrackTuple.initPosition, "initPosition[3]/D");
	mTrackTree->Branch("initMomentum", mTrackTuple.initMomentum, "initMomentum[3]/D");
	mTrackTree->Branch("initKineticEnergy", &mTrackTuple.initKineticEnergy, "initKineticEnergy/D");
	mTrackTree->Branch("incidentPosition", mTrackTuple.incidentPosition, "incidentPosition[3]/D");
	mTrackTree->Branch("incidentMomentum", mTrackTuple.incidentMomentum, "incidentMomentum[3]/D");
	mTrackTree->Branch("incidentKineticEnergy", &mTrackTuple.incidentKineticEnergy, "incidentKineticEnergy/D");
	mTrackTree->Branch("depositEnergy", mTrackTuple.depositEnergy, "depositEnergy[3]/D");
	mTrackTree->Branch("globalTime", &mTrackTuple.globalTime, "globalTime/D");
	mTrackTree->Branch("localTime", &mTrackTuple.localTime, "localTime/D");
	mTrackTree->Branch("finalVolumeID", &mTrackTuple.finalVolumeID, "finalVolumeID/I");
	mTrackTree->Branch("finalPosition", mTrackTuple.finalPosition, "finalPosition[3]/D");
	mTrackTree->Branch("finalMomentum", mTrackTuple.finalMomentum, "finalMomentum[3]/D");
	mTrackTree->Branch("finalKineticEnergy", &mTrackTuple.finalKineticEnergy, "finalKineticEnergy/D");
	mTrackTree->Branch("isInALPIDE", &isInALPIDE, "isInALPIDE/O");

	mParticleFile.open("/mnt/homes/ychoi/CLUSTER_SIZE/SIMULATION/unknown_particle.txt", std::ios::out);
}

Int_t TAnalysisManager::getParticleID(const G4String& particleID) {
	if ( particleID == "alpha" ) {
		return PARTICLE::alpha;
	} else if ( particleID == "e-" ) {
		return PARTICLE::electron;
	} else if ( particleID == "gamma" ) {
		return PARTICLE::gamma1;
	} else if ( particleID == "proton" ) {
		return PARTICLE::proton;
	} else if ( particleID == "neutron" ) {
		return PARTICLE::neutron;
	} else if ( particleID == "C12" ) {
		return PARTICLE::carbon12;
	} else if ( particleID == "C13" ) {
		return PARTICLE::carbon13;
	} else if ( particleID == "N14" ) {
		return PARTICLE::neutron;
	} else if ( particleID == "O16" ) {
		return PARTICLE::oxygen16;
	} else if ( particleID == "O18" ) {
		return PARTICLE::oxygen18;
	} else if ( particleID == "Si28" ) {
		return PARTICLE::silicon;
	} else {
		std::cout << "Unknown particle: " << particleID << std::endl;
		return PARTICLE::unknown;
	}
}

Int_t TAnalysisManager::getVolumeID(const G4LogicalVolume* volume) {
	if ( volume == mALPIDEMetalLogical ) {
		return 1;
	} else if ( volume == mALPIDEEpitaxialLogical ) {
		return 2;
	} else if ( volume == mALPIDESubstrateLogical ) {
		return 3;
	} else if ( volume == mCollimatorLogical ) {
		return 4;
	} else if ( volume == mScreenLogical ) {
		return 5;
	} else if ( volume == mWorldLogical ) {
		return 6;
	} else {
		return 0;
	}
}

void TAnalysisManager::close() {
	mFile->cd();
	mTrackTree->Write();
	mFile->Close();
}

void TAnalysisManager::doBeginOfRun(const G4Run* run) {
	mProgressBar = new ProgressBar(static_cast<int>(run->GetNumberOfEventToBeProcessed()));
}

void TAnalysisManager::doEndOfRun(const G4Run* run) {
	delete mProgressBar;
	mProgressBar = nullptr;

	std::cout << "Unknown particles: ";
	for ( const std::string& particle : mUnknownParticleList ) {
		std::cout << particle << " ";
	}
	std::cout << std::endl;
}

void TAnalysisManager::doBeginOfEvent(const G4Event* event) {
	mProgressBar->printProgress();
	mTrackTuple.eventID = event->GetEventID();
	static bool isFirstEvent = true;
	if ( event->GetEventID() % 2580000 == 0 ) {
		if ( !isFirstEvent ) {
			mFile->cd();
			mTrackTree->Write();
			mFile->Close();
			open(mFileName);
		} else {
			isFirstEvent = false;
			open(mFileName);
		}
	}
}

void TAnalysisManager::doEndOfEvent(const G4Event* event) { }

void TAnalysisManager::doPreTracking(const G4Track* track) {
	Int_t eventID = mTrackTuple.eventID;

	mTrackTuple.eventID = eventID;
	mTrackTuple.trackID = track->GetTrackID();
	mTrackTuple.parentID = track->GetParentID();
	mTrackTuple.particleID = getParticleID(track->GetDefinition()->GetParticleName());
	if ( mTrackTuple.particleID == 0 ) {
		if ( std::find(mUnknownParticleList.begin(), mUnknownParticleList.end(), track->GetDefinition()->GetParticleName()) == mUnknownParticleList.end() ) {
			mUnknownParticleList.push_back(track->GetDefinition()->GetParticleName());
		}
	}
	isInALPIDE = false;
	mTrackTuple.init();
}

void TAnalysisManager::doPostTracking(const G4Track* track) {
	const TDetectorConstruction* detectorConstruction = static_cast<const TDetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
	if ( mALPIDEMetalLogical == nullptr ) {
		mALPIDEMetalLogical = detectorConstruction->getALPIDEMetalLogical();
	}
	if ( mALPIDEEpitaxialLogical == nullptr ) {
		mALPIDEEpitaxialLogical = detectorConstruction->getALPIDEEpitaxialLogical();
	}
	if ( mALPIDESubstrateLogical == nullptr ) {
		mALPIDESubstrateLogical = detectorConstruction->getALPIDESubstrateLogical();
	}
	if ( mCollimatorLogical == nullptr ) {
		mCollimatorLogical = detectorConstruction->getCollimatorLogical();
	}
	if ( mScreenLogical == nullptr ) {
		mScreenLogical = detectorConstruction->getScreenLogical();
	}
	if ( mWorldLogical == nullptr ) {
		mWorldLogical = detectorConstruction->getWorldLogical();
	}

	G4ThreeVector vertexPosition = track->GetVertexPosition();
	mTrackTuple.initPosition[0] = vertexPosition.x();
	mTrackTuple.initPosition[1] = vertexPosition.y();
	mTrackTuple.initPosition[2] = vertexPosition.z();

	G4ThreeVector vertexMomentum = track->GetVertexMomentumDirection();
	mTrackTuple.initMomentum[0] = vertexMomentum.x();
	mTrackTuple.initMomentum[1] = vertexMomentum.y();
	mTrackTuple.initMomentum[2] = vertexMomentum.z();

	mTrackTuple.initKineticEnergy = track->GetVertexKineticEnergy();
	mTrackTuple.initVolumeID = getVolumeID(track->GetOriginTouchableHandle()->GetVolume()->GetLogicalVolume());

	G4ThreeVector finalPosition = track->GetPosition();
	mTrackTuple.finalPosition[0] = finalPosition.x();
	mTrackTuple.finalPosition[1] = finalPosition.y();
	mTrackTuple.finalPosition[2] = finalPosition.z();

	G4ThreeVector finalMomentum = track->GetMomentumDirection();
	mTrackTuple.finalMomentum[0] = finalMomentum.x();
	mTrackTuple.finalMomentum[1] = finalMomentum.y();
	mTrackTuple.finalMomentum[2] = finalMomentum.z();

	mTrackTuple.finalKineticEnergy = track->GetKineticEnergy();

	mTrackTuple.finalVolumeID = getVolumeID(track->GetVolume()->GetLogicalVolume());
	mTrackTuple.isInALPIDE = isInALPIDE;
	mTrackTree->Fill();
}

void TAnalysisManager::doStepPhase(const G4Step* step) {
	G4LogicalVolume* currentVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
	G4int volumeID = getVolumeID(currentVolume);

	const TDetectorConstruction* detectorConstruction = static_cast<const TDetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
	if ( mALPIDEMetalLogical == nullptr ) {
		mALPIDEMetalLogical = detectorConstruction->getALPIDEMetalLogical();
	}
	if ( mALPIDEEpitaxialLogical == nullptr ) {
		mALPIDEEpitaxialLogical = detectorConstruction->getALPIDEEpitaxialLogical();
	}
	if ( mALPIDESubstrateLogical == nullptr ) {
		mALPIDESubstrateLogical = detectorConstruction->getALPIDESubstrateLogical();
	}
	if ( mCollimatorLogical == nullptr ) {
		mCollimatorLogical = detectorConstruction->getCollimatorLogical();
	}
	if ( mScreenLogical == nullptr ) {
		mScreenLogical = detectorConstruction->getScreenLogical();
	}
	if ( mWorldLogical == nullptr ) {
		mWorldLogical = detectorConstruction->getWorldLogical();
	}

	if ( !isInALPIDE && currentVolume == mALPIDEMetalLogical && step->IsFirstStepInVolume() && getVolumeID(step->GetTrack()->GetOriginTouchableHandle()->GetVolume()->GetLogicalVolume()) > 3 ) {
		G4LogicalVolume* postVolume = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
		if ( postVolume != mWorldLogical && postVolume != mScreenLogical && postVolume != mCollimatorLogical ) {
			isInALPIDE = true;
			mTrackTuple.incidentPosition[0] = step->GetPreStepPoint()->GetPosition().x();
			mTrackTuple.incidentPosition[1] = step->GetPreStepPoint()->GetPosition().y();
			mTrackTuple.incidentPosition[2] = step->GetPreStepPoint()->GetPosition().z();
			mTrackTuple.incidentMomentum[0] = step->GetPreStepPoint()->GetMomentum().x();
			mTrackTuple.incidentMomentum[1] = step->GetPreStepPoint()->GetMomentum().y();
			mTrackTuple.incidentMomentum[2] = step->GetPreStepPoint()->GetMomentum().z();
			mTrackTuple.incidentKineticEnergy = step->GetPreStepPoint()->GetKineticEnergy();
			mTrackTuple.globalTime = step->GetPreStepPoint()->GetGlobalTime();
			mTrackTuple.localTime = step->GetPreStepPoint()->GetLocalTime();
		}
	}

	if ( currentVolume == mALPIDEMetalLogical ) {
		mTrackTuple.depositEnergy[0] += step->GetTotalEnergyDeposit();
		isInALPIDE = true;
	}
	if ( currentVolume == mALPIDEEpitaxialLogical ) {
		mTrackTuple.depositEnergy[1] += step->GetTotalEnergyDeposit();
		isInALPIDE = true;
	}
	if ( currentVolume == mALPIDESubstrateLogical ) {
		mTrackTuple.depositEnergy[2] += step->GetTotalEnergyDeposit();
		isInALPIDE = true;
	}
}

void TAnalysisManager::setFileName(const G4String& name) {
	mFileName = name;
}