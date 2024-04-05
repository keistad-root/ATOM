#include "AnalysisManager.h"

AnalysisManager* AnalysisManager::fInstance = nullptr;

AnalysisManager::AnalysisManager() {
    G4cout << "\033[1;35m" << "AnalysisManger" << "\033[0m" << " is armed" << G4endl;
    fInstance = this;
}

AnalysisManager::AnalysisManager(std::string name) {
    G4cout << "\033[1;35m" << "AnalysisManger" << "\033[0m" << " is armed" << G4endl;
    fInstance = this;
    openBook(name);
}

AnalysisManager::~AnalysisManager() {
    G4cout << "\033[1;35m" << "AnalysisManager" << "\033[0m" << " is terminated" << G4endl;
}

AnalysisManager* AnalysisManager::Instance() {
    if (fInstance == 0) {
        fInstance = new AnalysisManager();
    }
    return fInstance;
}

void AnalysisManager::openBook(std::string name) {
    mOutputFile = new TFile(static_cast<TString>(name),"RECREATE");
    setRunTree();
    setEventTree();
    setTrackTree();
    setStepTree();
}

void AnalysisManager::setRunTree() {
    mRunTree = new TTree("runTree","Tree for recording run informations");
    mRunTree->Branch("Run ID", &runTuple.runID, "runID/I");
    mRunTree->Branch("Number of Event", &runTuple.nEvents, "nEvent/I");
}

void AnalysisManager::setEventTree() {
    mEventTree = new TTree("eventTree", "Tree for recording event informations");
    mEventTree->Branch("Event Global ID", &eventTuple.eventGlobalID, "eventGlobalID/I");
    mEventTree->Branch("Event Local ID", &eventTuple.eventLocalID, "eventLocalID/I");
    mEventTree->Branch("Referenced Run ID", &eventTuple.runID, "runID/I");
    mEventTree->Branch("Number of Track", &eventTuple.nTracks, "nTrack/I");
}

void AnalysisManager::setTrackTree() {
    mTrackTree = new TTree("trackTree", "Tree for recording track informations");
    mTrackTree->Branch("Track Local ID", &trackTuple.trackLocalID, "trackLocalID/I");
    mTrackTree->Branch("Track Global ID", &trackTuple.trackGlobalID, "trackGlobalID/I");
    mTrackTree->Branch("Referenced Event Global ID", &trackTuple.eventGlobalID, "eventGlobalID/I");
    mTrackTree->Branch("Parent Local ID", &trackTuple.trackParentLocalID, "trackParentLocalID/I");
    mTrackTree->Branch("Number of Steps", &trackTuple.nSteps, "nSteps/I");
    mTrackTree->Branch("Particle Name", &trackTuple.particleName, "particleName");
    mTrackTree->Branch("Process Name", &trackTuple.processName, "processName");
    mTrackTree->Branch("Generating Volume Name", &trackTuple.volumeName, "volumeName");
    mTrackTree->Branch("Generating Time", &trackTuple.genTime, "genTime/D");
    mTrackTree->Branch("Generating Position X", &trackTuple.genPosition[0], "genPositionX/D");
    mTrackTree->Branch("Generating Position Y", &trackTuple.genPosition[1], "genPositionY/D");
    mTrackTree->Branch("Generating Position Z", &trackTuple.genPosition[2], "genPositionZ/D");
    mTrackTree->Branch("Generating Kinetic Energy", &trackTuple.genKineteicEnergy, "genKineteicEnergy/D");
    mTrackTree->Branch("Generating Momentum X", &trackTuple.genMomentum[0], "genMomentumX/D");
    mTrackTree->Branch("Generating Momentum Y", &trackTuple.genMomentum[1], "genMomentumY/D");
    mTrackTree->Branch("Generating Momentum Z", &trackTuple.genMomentum[2], "genMomentumZ/D");
}

void AnalysisManager::setStepTree() {
    mStepTree = new TTree("stepTree", "Tree for recording step information");
    mStepTree->Branch("Step Global ID", &stepTuple.stepGlobalID, "stepGlobalID/I");
    mStepTree->Branch("Referenced Track Global ID", &stepTuple.trackGlobalID, "trackGlobalID/I");
    mStepTree->Branch("Volume Name", &stepTuple.volumeName, "volumeName");
    mStepTree->Branch("Time", &stepTuple.time, "time/D");
    mStepTree->Branch("Position X", &stepTuple.position[0], "time/D");
    mStepTree->Branch("Position Y", &stepTuple.position[1], "time/D");
    mStepTree->Branch("Position Z", &stepTuple.position[2], "time/D");
    mStepTree->Branch("Kinetic Energy", &stepTuple.kineticEnergy, "time/D");
    mStepTree->Branch("Momentum X", &stepTuple.momentum[0], "time/D");
    mStepTree->Branch("Momentum Y", &stepTuple.momentum[1], "time/D");
    mStepTree->Branch("Momentum Z", &stepTuple.momentum[2], "time/D");
    mStepTree->Branch("Delta Energy", &stepTuple.deltaEnergy, "time/D");
    mStepTree->Branch("Total Deposit Energy", &stepTuple.totalDepositEnergy, "time/D");
    mStepTree->Branch("Non Ionizing Deposit Energy", &stepTuple.nonIonizingEnergyLoss, "time/D");
}

void AnalysisManager::RecordingRun(const G4Run* run) {
    runID = run->GetRunID();
    runTuple.runID = runID;
    runTuple.nEvents = run->GetNumberOfEvent();
    mRunTree->Fill();
    mRunTree->AutoSave();
}

void AnalysisManager::RecordingEvent(const G4Event* event) {
    eventTuple.eventGlobalID = eventID;
    eventTuple.eventLocalID = event->GetEventID();
    eventTuple.runID = runID;
    mEventTree->Fill();
    mEventTree->AutoSave();
    eventID++;
}

void AnalysisManager::RecordingTrackStart(const G4Track* track) {
    trackTuple.eventGlobalID = eventID;
    trackTuple.trackGlobalID = trackID;
    if (track->GetTrackID() != 1) {
        trackTuple.trackParentLocalID = track->GetParentID();
        trackTuple.processName = track->GetCreatorProcess()->GetProcessName();
    }
    trackTuple.particleName = track->GetParticleDefinition()->GetParticleName();
    trackTuple.volumeName = track->GetVolume()->GetName();
    trackTuple.genTime = track->GetGlobalTime();
    trackTuple.genPosition[0] = track->GetPosition()[0];
    trackTuple.genPosition[1] = track->GetPosition()[1];
    trackTuple.genPosition[2] = track->GetPosition()[2];
    trackTuple.genKineteicEnergy = track->GetKineticEnergy();
    trackTuple.genMomentum[0] = track->GetMomentum()[0];
    trackTuple.genMomentum[1] = track->GetMomentum()[1];
    trackTuple.genMomentum[2] = track->GetMomentum()[2];
    trackID++;
}

void AnalysisManager::RecordingTrackEnd(const G4Track* track) {
    trackTuple.nSteps = track->GetCurrentStepNumber();
    mTrackTree->Fill();
    mTrackTree->AutoSave();
    trackTuple.trackParentLocalID = 0;
    trackTuple.processName = "";
}

void AnalysisManager::RecordingStep(const G4Step* step) {
    stepTuple.stepGlobalID = stepID;
    stepTuple.trackGlobalID = trackID;
    stepTuple.position[0] = step->GetPostStepPoint()->GetPosition()[0];
    stepTuple.position[1] = step->GetPostStepPoint()->GetPosition()[1];
    stepTuple.position[2] = step->GetPostStepPoint()->GetPosition()[2];
    if ((abs(stepTuple.position[0]) < 149) && (abs(stepTuple.position[1]) < 149 && (abs(stepTuple.position[2]) < 74))) {
        stepTuple.volumeName = step->GetPostStepPoint()->GetPhysicalVolume()->GetName();
    } else {
        stepTuple.volumeName = "World";
    }
    stepTuple.time = step->GetPostStepPoint()->GetLocalTime();
    stepTuple.kineticEnergy = step->GetPostStepPoint()->GetKineticEnergy();
    stepTuple.momentum[0] = step->GetPostStepPoint()->GetMomentum()[0];
    stepTuple.momentum[1] = step->GetPostStepPoint()->GetMomentum()[1];
    stepTuple.momentum[2] = step->GetPostStepPoint()->GetMomentum()[2];
    stepTuple.totalDepositEnergy = step->GetTotalEnergyDeposit();
    stepTuple.nonIonizingEnergyLoss = step->GetNonIonizingEnergyDeposit();
    mStepTree->Fill();
    mStepTree->AutoSave();

    stepID++;
}

void AnalysisManager::closeBook() {
    mOutputFile->cd();
    mRunTree->Write();
    mEventTree->Write();
    mTrackTree->Write();
    mStepTree->Write();
    mOutputFile->Close();
}

