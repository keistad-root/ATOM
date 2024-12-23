#include "TGeantAnalysis.h"

TGeantAnalysis::TGeantAnalysis(const CppConfigFile& config) : TPlotter(&config), mConfig(config) {
	std::string inputFileName = config.getConfig("File").find("input_file");
	mOutputPath = mConfig.getConfig("File").find("output_directory");
	mFile = new TFile(static_cast<TString>(inputFileName), "READ");
	if ( mConfig.getConfig("File").hasKey("incident_file") ) {
		std::string incidentFileName = mConfig.getConfig("File").find("incident_file");
		mIncidentFile = new TFile(static_cast<TString>(incidentFileName), "RECREATE");
	}
	std::filesystem::create_directories(mOutputPath);

	mConfigList = mConfig.getConfigurableNameList();
}

TGeantAnalysis::~TGeantAnalysis() {
	mFile->Close();
}

void TGeantAnalysis::readTrack() {
	trackTuple track;
	TTree* trackTree = static_cast<TTree*>(mFile->Get("trackTree"));
	trackTree->SetBranchAddress("eventID", &track.eventID);
	trackTree->SetBranchAddress("trackID", &track.trackID);
	trackTree->SetBranchAddress("parentID", &track.parentID);
	trackTree->SetBranchAddress("particleID", &track.particleID);
	trackTree->SetBranchAddress("initX", &track.initX);
	trackTree->SetBranchAddress("initY", &track.initY);
	trackTree->SetBranchAddress("initZ", &track.initZ);
	trackTree->SetBranchAddress("initPX", &track.initPX);
	trackTree->SetBranchAddress("initPY", &track.initPY);
	trackTree->SetBranchAddress("initPZ", &track.initPZ);
	trackTree->SetBranchAddress("initKineticEnergy", &track.initKineticEnergy);
	trackTree->SetBranchAddress("initVolumeID", &track.initVolumeID);
	trackTree->SetBranchAddress("finalX", &track.finalX);
	trackTree->SetBranchAddress("finalY", &track.finalY);
	trackTree->SetBranchAddress("finalZ", &track.finalZ);
	trackTree->SetBranchAddress("finalPX", &track.finalPX);
	trackTree->SetBranchAddress("finalPY", &track.finalPY);
	trackTree->SetBranchAddress("finalPZ", &track.finalPZ);
	trackTree->SetBranchAddress("finalKineticEnergy", &track.finalKineticEnergy);
	trackTree->SetBranchAddress("finalVolumeID", &track.finalVolumeID);

	incidentTuple incident;
	TTree* incidentTree = static_cast<TTree*>(mFile->Get("incidentTree"));
	incidentTree->SetBranchAddress("eventID", &incident.eventID);
	incidentTree->SetBranchAddress("trackID", &incident.trackID);
	incidentTree->SetBranchAddress("depositEnergyMetal", &incident.depositEnergy[0]);
	incidentTree->SetBranchAddress("depositEnergyEpitaxial", &incident.depositEnergy[1]);
	incidentTree->SetBranchAddress("depositEnergySubstrate", &incident.depositEnergy[2]);
	incidentTree->SetBranchAddress("x", &incident.position[0]);
	incidentTree->SetBranchAddress("y", &incident.position[1]);
	incidentTree->SetBranchAddress("z", &incident.position[2]);
	incidentTree->SetBranchAddress("px", &incident.momentum[0]);
	incidentTree->SetBranchAddress("py", &incident.momentum[1]);
	incidentTree->SetBranchAddress("pz", &incident.momentum[2]);
	incidentTree->SetBranchAddress("kineticEnergy", &incident.kineticEnergy);
	incidentTree->SetBranchAddress("globalTime", &incident.globalTime);
	incidentTree->SetBranchAddress("localTime", &incident.localTime);

	TTree* outputTree = new TTree("incidentTree", "incidentTree");
	incidentTuple outputIncident;
	outputTree->Branch("eventID", &outputIncident.eventID);
	outputTree->Branch("trackID", &outputIncident.trackID);
	outputTree->Branch("particleID", &outputIncident.particleID);
	outputTree->Branch("initVolumeID", &outputIncident.initVolumeID);
	outputTree->Branch("depositEnergyMetal", &outputIncident.depositEnergy[0]);
	outputTree->Branch("depositEnergyEpitaxial", &outputIncident.depositEnergy[1]);
	outputTree->Branch("depositEnergySubstrate", &outputIncident.depositEnergy[2]);
	outputTree->Branch("x", &outputIncident.position[0]);
	outputTree->Branch("y", &outputIncident.position[1]);
	outputTree->Branch("z", &outputIncident.position[2]);
	outputTree->Branch("px", &outputIncident.momentum[0]);
	outputTree->Branch("py", &outputIncident.momentum[1]);
	outputTree->Branch("pz", &outputIncident.momentum[2]);
	outputTree->Branch("kineticEnergy", &outputIncident.kineticEnergy);
	outputTree->Branch("globalTime", &outputIncident.globalTime);
	outputTree->Branch("localTime", &outputIncident.localTime);

	Int_t nEntries = trackTree->GetEntries();
	mProgressBar = new ProgressBar(static_cast<int>(nEntries));
	Int_t iIncident = 0;
	for ( Int_t i = 0; i < nEntries; i++ ) {
		mProgressBar->printProgress();
		trackTree->GetEntry(i);
		getTrackInformations(track);

		incidentTree->GetEntry(iIncident);
		if ( track.eventID == incident.eventID && track.trackID == incident.trackID ) {
			incident.particleID = track.particleID;
			incident.initVolumeID = track.initVolumeID;
			getIncidentInformations(incident);
			outputIncident = incident;
			outputTree->Fill();
			iIncident++;
		}
	}
	delete mProgressBar;

	outputTree->Write();
	mFile->Close();
	mIncidentFile->Close();

}

void TGeantAnalysis::getTrackInformations(const trackTuple& track) {
	if ( track.parentID == 0 ) {
		if ( mConfig.hasConfig("SourceXY") ) {
			TH2* hist = static_cast<TH2D*>(mHistograms.find("SourceXY")->second);
			hist->Fill(track.initX, track.initY);
		}
		if ( mConfig.hasConfig("SourceZ") ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("SourceZ")->second);
			hist->Fill(track.initZ);
		}
		if ( mConfig.hasConfig("SourceKineticEnergy") ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("SourceKineticEnergy")->second);
			hist->Fill(track.initKineticEnergy);
		}
		if ( mConfig.hasConfig("SourceTheta") ) {
			Double_t momentum = TMath::Sqrt(track.initPX * track.initPX + track.initPY * track.initPY + track.initPZ * track.initPZ);
			Double_t theta = TMath::ACos(track.initPZ / momentum) * 180. / TMath::Pi();
			TH1* hist = static_cast<TH1D*>(mHistograms.find("SourceTheta")->second);
			hist->Fill(theta);
		}
		if ( mConfig.hasConfig("SourcePhi") ) {
			Double_t phi = TMath::ATan2(track.initPY, track.initPX) * 180. / TMath::Pi();
			TH1* hist = static_cast<TH1D*>(mHistograms.find("SourcePhi")->second);
			hist->Fill(phi);
		}
		if ( mConfig.hasConfig("SourceVolume") ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("SourceVolume")->second);
			hist->Fill(track.initVolumeID);
		}
	} else {
		if ( mConfig.hasConfig("SecondaryVolume") ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("SecondaryVolume")->second);
			hist->Fill(track.finalVolumeID);
		}
		if ( mConfig.hasConfig("SecondaryParticle") ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("SecondaryParticle")->second);
			hist->Fill(track.particleID);
		}
		if ( mConfig.hasConfig("SecondaryVolumeVSParticle") ) {
			TH2* hist = static_cast<TH2D*>(mHistograms.find("SecondaryVolumeVSParticle")->second);
			hist->Fill(track.finalVolumeID, track.particleID);
		}
	}
}

void TGeantAnalysis::getIncidentInformations(const incidentTuple& incident) {
	Double_t incidentMomentum = TMath::Sqrt(incident.momentum[0] * incident.momentum[0] + incident.momentum[1] * incident.momentum[1] + incident.momentum[2] * incident.momentum[2]);
	Double_t incidentTheta = TMath::ACos(incident.momentum[2] / incidentMomentum) * 180. / TMath::Pi();
	Double_t incidentPhi = TMath::ATan2(incident.momentum[1], incident.momentum[0]) * 180. / TMath::Pi();

	Double_t depositEnergy = 0.;
	bool isDeposit = false;
	if ( incident.depositEnergy[0] > -.5 ) {
		depositEnergy += incident.depositEnergy[0];
		isDeposit = true;
	}
	if ( incident.depositEnergy[1] > -.5 ) {
		depositEnergy += incident.depositEnergy[1];
		isDeposit = true;
	}
	if ( incident.depositEnergy[2] > -.5 ) {
		depositEnergy += incident.depositEnergy[2];
		isDeposit = true;
	}

	if ( mConfig.hasConfig("IncidentZ") ) {
		TH1* hist = static_cast<TH1D*>(mHistograms.find("IncidentZ")->second);
		hist->Fill(incident.position[2]);
	}
	if ( mConfig.hasConfig("IncidentXY") ) {
		TH2* hist = static_cast<TH2D*>(mHistograms.find("IncidentXY")->second);
		hist->Fill(incident.position[0], incident.position[1]);
	}

	if ( mConfig.hasConfig("IncidentXLine") ) {
		if ( std::abs(incident.position[1]) < 7.5 / 512. ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("IncidentXLine")->second);
			hist->Fill(incident.position[0]);
		}
	}
	if ( mConfig.hasConfig("IncidentYLine") ) {
		if ( std::abs(incident.position[0]) < 7.5 / 512. ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("IncidentYLine")->second);
			hist->Fill(incident.position[1]);
		}
	}
	if ( mConfig.hasConfig("IncidentAngle") ) {
		TH1* hist = static_cast<TH1D*>(mHistograms.find("IncidentAngle")->second);
		hist->Fill(180 - incidentTheta);
	}
	if ( mConfig.hasConfig("IncidentKineticEnergy") ) {
		TH1* hist = static_cast<TH1D*>(mHistograms.find("IncidentKineticEnergy")->second);
		hist->Fill(incident.kineticEnergy);
	}
	if ( mConfig.hasConfig("IncidentLocalTime") ) {
		TH1* hist = static_cast<TH1D*>(mHistograms.find("IncidentLocalTime")->second);
		hist->Fill(incident.localTime);
	}
	if ( mConfig.hasConfig("IncidentGlobalTime") ) {
		TH1* hist = static_cast<TH1D*>(mHistograms.find("IncidentGlobalTime")->second);
		hist->Fill(incident.globalTime);
	}
	if ( mConfig.hasConfig("IncidentParticle") ) {
		TH1* hist = static_cast<TH1D*>(mHistograms.find("IncidentParticle")->second);
		hist->Fill(incident.particleID);
	}
	if ( mConfig.hasConfig("IncidentVolume") ) {
		TH1* hist = static_cast<TH1D*>(mHistograms.find("IncidentVolume")->second);
		hist->Fill(incident.initVolumeID);
	}
	if ( mConfig.hasConfig("DepositEnergyTotal") ) {
		if ( isDeposit ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("DepositEnergyTotal")->second);
			hist->Fill(depositEnergy);
		}
	}
	if ( mConfig.hasConfig("DepositEnergyMetal") ) {
		if ( incident.depositEnergy[0] > -.5 ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("DepositEnergyMetal")->second);
			hist->Fill(incident.depositEnergy[0]);
		}
	}
	if ( mConfig.hasConfig("DepositEnergyEpitaxial") ) {
		if ( incident.depositEnergy[1] > -.5 ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("DepositEnergyEpitaxial")->second);
			hist->Fill(incident.depositEnergy[1]);
		}
	}
	if ( mConfig.hasConfig("DepositEnergySubstrate") ) {
		if ( incident.depositEnergy[2] > -.5 ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("DepositEnergySubstrate")->second);
			hist->Fill(incident.depositEnergy[2]);
		}
	}
	if ( mConfig.hasConfig("CorrelationDepositEnergyTotalAndIncidentAngle") ) {
		if ( isDeposit ) {
			TH2* hist = static_cast<TH2D*>(mHistograms.find("CorrelationDepositEnergyTotalAndIncidentAngle")->second);
			hist->Fill(180 - incidentTheta, depositEnergy);
		}
	}
	if ( mConfig.hasConfig("CorrelationDepositEnergyMetalAndIncidentAngle") ) {
		if ( incident.depositEnergy[0] > -.5 ) {
			TH2* hist = static_cast<TH2D*>(mHistograms.find("CorrelationDepositEnergyMetalAndIncidentAngle")->second);
			hist->Fill(180 - incidentTheta, incident.depositEnergy[0]);
		}
	}
	if ( mConfig.hasConfig("CorrelationDepositEnergyEpitaxialAndIncidentAngle") ) {
		if ( incident.depositEnergy[1] > -.5 ) {
			TH2* hist = static_cast<TH2D*>(mHistograms.find("CorrelationDepositEnergyEpitaxialAndIncidentAngle")->second);
			hist->Fill(180 - incidentTheta, incident.depositEnergy[1]);
		}
	}
	if ( mConfig.hasConfig("CorrelationDepositEnergySubstrateAndIncidentAngle") ) {
		if ( incident.depositEnergy[2] > -.5 ) {
			TH2* hist = static_cast<TH2D*>(mHistograms.find("CorrelationDepositEnergySubstrateAndIncidentAngle")->second);
			hist->Fill(180 - incidentTheta, incident.depositEnergy[2]);
		}
	}

	if ( incident.particleID == PARTICLE::alpha ) {
		if ( mConfig.hasConfig("AlphaDepositEnergyTotal") ) {
			if ( isDeposit ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("AlphaDepositEnergyTotal")->second);
				hist->Fill(depositEnergy);
			}
		}
		if ( mConfig.hasConfig("AlphaDepositEnergyMetal") ) {
			if ( incident.depositEnergy[0] > -.5 ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("AlphaDepositEnergyMetal")->second);
				hist->Fill(incident.depositEnergy[0]);
			}
		}
		if ( mConfig.hasConfig("AlphaDepositEnergyEpitaxial") ) {
			if ( incident.depositEnergy[1] > -.5 ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("AlphaDepositEnergyEpitaxial")->second);
				hist->Fill(incident.depositEnergy[1]);
			}
		}
		if ( mConfig.hasConfig("AlphaDepositEnergySubstrate") ) {
			if ( incident.depositEnergy[2] > -.5 ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("AlphaDepositEnergySubstrate")->second);
				hist->Fill(incident.depositEnergy[2]);
			}
		}
		if ( mConfig.hasConfig("AlphaIncidentXY") ) {
			TH2* hist = static_cast<TH2D*>(mHistograms.find("AlphaIncidentXY")->second);
			hist->Fill(incident.position[0], incident.position[1]);
		}
		if ( mConfig.hasConfig("AlphaIncidentAngle") ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("AlphaIncidentAngle")->second);
			hist->Fill(180 - incidentTheta);
		}
		if ( mConfig.hasConfig("AlphaIncidentVolume") ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("AlphaIncidentVolume")->second);
			hist->Fill(incident.initVolumeID);
		}
		if ( mConfig.hasConfig("AlphaIncidentLocalTime") ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("AlphaIncidentLocalTime")->second);
			hist->Fill(incident.localTime);
		}
		if ( mConfig.hasConfig("AlphaIncidentGlobalTime") ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("AlphaIncidentGlobalTime")->second);
			hist->Fill(incident.globalTime);
		}
		if ( mConfig.hasConfig("AlphaCorrelationDepositEnergyTotalAndIncidentAngle") ) {
			if ( isDeposit ) {
				TH2* hist = static_cast<TH2D*>(mHistograms.find("AlphaCorrelationDepositEnergyTotalAndIncidentAngle")->second);
				hist->Fill(180 - incidentTheta, depositEnergy);
			}
		}
		if ( mConfig.hasConfig("AlphaCorrelationDepositEnergyMetalAndIncidentAngle") ) {
			if ( incident.depositEnergy[0] > -.5 ) {
				TH2* hist = static_cast<TH2D*>(mHistograms.find("AlphaCorrelationDepositEnergyMetalAndIncidentAngle")->second);
				hist->Fill(180 - incidentTheta, incident.depositEnergy[0]);
			}
		}
		if ( mConfig.hasConfig("AlphaCorrelationDepositEnergyEpitaxialAndIncidentAngle") ) {
			if ( incident.depositEnergy[1] > -.5 ) {
				TH2* hist = static_cast<TH2D*>(mHistograms.find("AlphaCorrelationDepositEnergyEpitaxialAndIncidentAngle")->second);
				hist->Fill(180 - incidentTheta, incident.depositEnergy[1]);
			}
		}
		if ( mConfig.hasConfig("AlphaCorrelationDepositEnergySubstrateAndIncidentAngle") ) {
			if ( incident.depositEnergy[2] > -.5 ) {
				TH2* hist = static_cast<TH2D*>(mHistograms.find("AlphaCorrelationDepositEnergySubstrateAndIncidentAngle")->second);
				hist->Fill(180 - incidentTheta, incident.depositEnergy[2]);
			}
		}
	}

	if ( incident.particleID == PARTICLE::electron ) {
		if ( mConfig.hasConfig("ElectronDepositEnergyTotal") ) {
			if ( isDeposit ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("ElectronDepositEnergyTotal")->second);
				hist->Fill(depositEnergy);
			}
		}
		if ( mConfig.hasConfig("ElectronDepositEnergyMetal") ) {
			if ( incident.depositEnergy[0] > -.5 ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("ElectronDepositEnergyMetal")->second);
				hist->Fill(incident.depositEnergy[0]);
			}
		}
		if ( mConfig.hasConfig("ElectronDepositEnergyEpitaxial") ) {
			if ( incident.depositEnergy[1] > -.5 ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("ElectronDepositEnergyEpitaxial")->second);
				hist->Fill(incident.depositEnergy[1]);
			}
		}
		if ( mConfig.hasConfig("ElectronDepositEnergySubstrate") ) {
			if ( incident.depositEnergy[2] > -.5 ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("ElectronDepositEnergySubstrate")->second);
				hist->Fill(incident.depositEnergy[2]);
			}
		}
		if ( mConfig.hasConfig("ElectronIncidentXY") ) {
			TH2* hist = static_cast<TH2D*>(mHistograms.find("ElectronIncidentXY")->second);
			hist->Fill(incident.position[0], incident.position[1]);
		}
		if ( mConfig.hasConfig("ElectronIncidentAngle") ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("ElectronIncidentAngle")->second);
			hist->Fill(180 - incidentTheta);
		}
		if ( mConfig.hasConfig("ElectronIncidentVolume") ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("ElectronIncidentVolume")->second);
			hist->Fill(incident.initVolumeID);
		}
		if ( mConfig.hasConfig("ElectronIncidentLocalTime") ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("ElectronIncidentLocalTime")->second);
			hist->Fill(incident.localTime);
		}
		if ( mConfig.hasConfig("ElectronIncidentGlobalTime") ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("ElectronIncidentGlobalTime")->second);
			hist->Fill(incident.globalTime);
		}
		if ( mConfig.hasConfig("ElectronCorrelationDepositEnergyTotalAndIncidentAngle") ) {
			if ( isDeposit ) {
				TH2* hist = static_cast<TH2D*>(mHistograms.find("ElectronCorrelationDepositEnergyTotalAndIncidentAngle")->second);
				hist->Fill(180 - incidentTheta, depositEnergy);
			}
		}
		if ( mConfig.hasConfig("ElectronCorrelationDepositEnergyMetalAndIncidentAngle") ) {
			if ( incident.depositEnergy[0] > -.5 ) {
				TH2* hist = static_cast<TH2D*>(mHistograms.find("ElectronCorrelationDepositEnergyMetalAndIncidentAngle")->second);
				hist->Fill(180 - incidentTheta, incident.depositEnergy[0]);
			}
		}
		if ( mConfig.hasConfig("ElectronCorrelationDepositEnergyEpitaxialAndIncidentAngle") ) {
			if ( incident.depositEnergy[1] > -.5 ) {
				TH2* hist = static_cast<TH2D*>(mHistograms.find("ElectronCorrelationDepositEnergyEpitaxialAndIncidentAngle")->second);
				hist->Fill(180 - incidentTheta, incident.depositEnergy[1]);
			}
		}
		if ( mConfig.hasConfig("ElectronCorrelationDepositEnergySubstrateAndIncidentAngle") ) {
			if ( incident.depositEnergy[2] > -.5 ) {
				TH2* hist = static_cast<TH2D*>(mHistograms.find("ElectronCorrelationDepositEnergySubstrateAndIncidentAngle")->second);
				hist->Fill(180 - incidentTheta, incident.depositEnergy[2]);
			}
		}
	}
}











void TGeantAnalysis::getTrackInformations() {
	for ( const trackTuple& track : mTrackTupleSet ) {
		if ( track.parentID == 0 ) {
			if ( mConfig.hasConfig("SourceXY") ) {
				TH2* hist = static_cast<TH2D*>(mHistograms.find("SourceXY")->second);
				hist->Fill(track.initX, track.initY);
			}
			if ( mConfig.hasConfig("SourceZ") ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("SourceZ")->second);
				hist->Fill(track.initZ);
			}
			if ( mConfig.hasConfig("SourceKineticEnergy") ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("SourceKineticEnergy")->second);
				hist->Fill(track.initKineticEnergy);
			}
			if ( mConfig.hasConfig("SourceTheta") ) {
				Double_t momentum = TMath::Sqrt(track.initPX * track.initPX + track.initPY * track.initPY + track.initPZ * track.initPZ);
				Double_t theta = TMath::ACos(track.initPZ / momentum) * 180. / TMath::Pi();
				TH1* hist = static_cast<TH1D*>(mHistograms.find("SourceTheta")->second);
				hist->Fill(theta);
			}
			if ( mConfig.hasConfig("SourcePhi") ) {
				Double_t phi = TMath::ATan2(track.initPY, track.initPX) * 180. / TMath::Pi();
				TH1* hist = static_cast<TH1D*>(mHistograms.find("SourcePhi")->second);
				hist->Fill(phi);
			}
			if ( mConfig.hasConfig("SourceVolume") ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("SourceVolume")->second);
				hist->Fill(track.initVolumeID);
			}
		} else {
			if ( mConfig.hasConfig("SecondaryVolume") ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("SecondaryVolume")->second);
				hist->Fill(track.finalVolumeID);
			}
			if ( mConfig.hasConfig("SecondaryParticle") ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("SecondaryParticle")->second);
				hist->Fill(track.particleID);
			}
		}
	}
}

void TGeantAnalysis::getIncidentInformations() {
	for ( const incidentTuple& incident : mIncidentTupleSet ) {
		Double_t incidentMomentum = TMath::Sqrt(incident.momentum[0] * incident.momentum[0] + incident.momentum[1] * incident.momentum[1] + incident.momentum[2] * incident.momentum[2]);
		Double_t incidentTheta = TMath::ACos(incident.momentum[2] / incidentMomentum) * 180. / TMath::Pi();
		Double_t incidentPhi = TMath::ATan2(incident.momentum[1], incident.momentum[0]) * 180. / TMath::Pi();

		if ( mConfig.hasConfig("IncidentZ") ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("IncidentZ")->second);
			hist->Fill(incident.position[2]);
		}
		if ( mConfig.hasConfig("IncidentXY") ) {
			TH2* hist = static_cast<TH2D*>(mHistograms.find("IncidentXY")->second);
			hist->Fill(incident.position[0], incident.position[1]);
		}
		if ( mConfig.hasConfig("IncidentAngle") ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("IncidentAngle")->second);
			hist->Fill(180 - incidentTheta);
		}
		if ( mConfig.hasConfig("IncidentKineticEnergy") ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("IncidentKineticEnergy")->second);
			hist->Fill(incident.kineticEnergy);
		}
		if ( mConfig.hasConfig("IncidentLocalTime") ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("IncidentLocalTime")->second);
			hist->Fill(incident.localTime);
		}
		if ( mConfig.hasConfig("IncidentGlobalTime") ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("IncidentGlobalTime")->second);
			hist->Fill(incident.globalTime);
		}
		if ( mConfig.hasConfig("IncidentParticle") ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("IncidentParticle")->second);
			hist->Fill(incident.particleID);
		}
		if ( mConfig.hasConfig("IncidentVolume") ) {
			TH1* hist = static_cast<TH1D*>(mHistograms.find("IncidentVolume")->second);
			hist->Fill(incident.initVolumeID);
		}
		if ( mConfig.hasConfig("DepositEnergyMetal") ) {
			if ( incident.depositEnergy[0] > -.5 ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("DepositEnergyMetal")->second);
				hist->Fill(incident.depositEnergy[0]);
			}
		}
		if ( mConfig.hasConfig("DepositEnergyEpitaxial") ) {
			if ( incident.depositEnergy[1] > -.5 ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("DepositEnergyEpitaxial")->second);
				hist->Fill(incident.depositEnergy[1]);
			}
		}
		if ( mConfig.hasConfig("DepositEnergySubstrate") ) {
			if ( incident.depositEnergy[2] > -.5 ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("DepositEnergySubstrate")->second);
				hist->Fill(incident.depositEnergy[2]);
			}
		}
		if ( mConfig.hasConfig("CorrelationDepositEnergyMetalAndIncidentAngle") ) {
			if ( incident.depositEnergy[0] > -.5 ) {
				TH2* hist = static_cast<TH2D*>(mHistograms.find("CorrelationDepositEnergyMetalAndIncidentAngle")->second);
				hist->Fill(180 - incidentTheta, incident.depositEnergy[0]);
			}
		}
		if ( mConfig.hasConfig("CorrelationDepositEnergyEpitaxialAndIncidentAngle") ) {
			if ( incident.depositEnergy[1] > -.5 ) {
				TH2* hist = static_cast<TH2D*>(mHistograms.find("CorrelationDepositEnergyEpitaxialAndIncidentAngle")->second);
				hist->Fill(180 - incidentTheta, incident.depositEnergy[1]);
			}
		}
		if ( mConfig.hasConfig("CorrelationDepositEnergySubstrateAndIncidentAngle") ) {
			if ( incident.depositEnergy[2] > -.5 ) {
				TH2* hist = static_cast<TH2D*>(mHistograms.find("CorrelationDepositEnergySubstrateAndIncidentAngle")->second);
				hist->Fill(180 - incidentTheta, incident.depositEnergy[2]);
			}
		}

		if ( incident.particleID == PARTICLE::alpha ) {
			if ( mConfig.hasConfig("AlphaDepositEnergyMetal") ) {
				if ( incident.depositEnergy[0] > -.5 ) {
					TH1* hist = static_cast<TH1D*>(mHistograms.find("AlphaDepositEnergyMetal")->second);
					hist->Fill(incident.depositEnergy[0]);
				}
			}
			if ( mConfig.hasConfig("AlphaDepositEnergyEpitaxial") ) {
				if ( incident.depositEnergy[1] > -.5 ) {
					TH1* hist = static_cast<TH1D*>(mHistograms.find("AlphaDepositEnergyEpitaxial")->second);
					hist->Fill(incident.depositEnergy[1]);
				}
			}
			if ( mConfig.hasConfig("AlphaDepositEnergySubstrate") ) {
				if ( incident.depositEnergy[2] > -.5 ) {
					TH1* hist = static_cast<TH1D*>(mHistograms.find("AlphaDepositEnergySubstrate")->second);
					hist->Fill(incident.depositEnergy[2]);
				}
			}
			if ( mConfig.hasConfig("AlphaIncidentAngle") ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("AlphaIncidentAngle")->second);
				hist->Fill(180 - incidentTheta);
			}
			if ( mConfig.hasConfig("AlphaIncidentLocalTime") ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("AlphaIncidentLocalTime")->second);
				hist->Fill(incident.localTime);
			}
			if ( mConfig.hasConfig("AlphaIncidentGlobalTime") ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("AlphaIncidentGlobalTime")->second);
				hist->Fill(incident.globalTime);
			}
			if ( mConfig.hasConfig("AlphaCorrelationDepositEnergyMetalAndIncidentAngle") ) {
				if ( incident.depositEnergy[0] > -.5 ) {
					TH2* hist = static_cast<TH2D*>(mHistograms.find("AlphaCorrelationDepositEnergyMetalAndIncidentAngle")->second);
					hist->Fill(180 - incidentTheta, incident.depositEnergy[0]);
				}
			}
			if ( mConfig.hasConfig("AlphaCorrelationDepositEnergyEpitaxialAndIncidentAngle") ) {
				if ( incident.depositEnergy[1] > -.5 ) {
					TH2* hist = static_cast<TH2D*>(mHistograms.find("AlphaCorrelationDepositEnergyEpitaxialAndIncidentAngle")->second);
					hist->Fill(180 - incidentTheta, incident.depositEnergy[1]);
				}
			}
			if ( mConfig.hasConfig("AlphaCorrelationDepositEnergySubstrateAndIncidentAngle") ) {
				if ( incident.depositEnergy[2] > -.5 ) {
					TH2* hist = static_cast<TH2D*>(mHistograms.find("AlphaCorrelationDepositEnergySubstrateAndIncidentAngle")->second);
					hist->Fill(180 - incidentTheta, incident.depositEnergy[2]);
				}
			}
		}

		if ( incident.particleID == PARTICLE::electron ) {
			if ( mConfig.hasConfig("ElectronDepositEnergyMetal") ) {
				if ( incident.depositEnergy[0] > -.5 ) {
					TH1* hist = static_cast<TH1D*>(mHistograms.find("ElectronDepositEnergyMetal")->second);
					hist->Fill(incident.depositEnergy[0]);
				}
			}
			if ( mConfig.hasConfig("ElectronDepositEnergyEpitaxial") ) {
				if ( incident.depositEnergy[1] > -.5 ) {
					TH1* hist = static_cast<TH1D*>(mHistograms.find("ElectronDepositEnergyEpitaxial")->second);
					hist->Fill(incident.depositEnergy[1]);
				}
			}
			if ( mConfig.hasConfig("ElectronDepositEnergySubstrate") ) {
				if ( incident.depositEnergy[2] > -.5 ) {
					TH1* hist = static_cast<TH1D*>(mHistograms.find("ElectronDepositEnergySubstrate")->second);
					hist->Fill(incident.depositEnergy[2]);
				}
			}
			if ( mConfig.hasConfig("ElectronIncidentAngle") ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("ElectronIncidentAngle")->second);
				hist->Fill(180 - incidentTheta);
			}
			if ( mConfig.hasConfig("ElectronIncidentLocalTime") ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("ElectronIncidentLocalTime")->second);
				hist->Fill(incident.localTime);
			}
			if ( mConfig.hasConfig("ElectronIncidentGlobalTime") ) {
				TH1* hist = static_cast<TH1D*>(mHistograms.find("ElectronIncidentGlobalTime")->second);
				hist->Fill(incident.globalTime);
			}
			if ( mConfig.hasConfig("ElectronCorrelationDepositEnergyMetalAndIncidentAngle") ) {
				TH2* hist = static_cast<TH2D*>(mHistograms.find("ElectronCorrelationDepositEnergyMetalAndIncidentAngle")->second);
				hist->Fill(180 - incidentTheta, incident.depositEnergy[0]);
			}
			if ( mConfig.hasConfig("ElectronCorrelationDepositEnergyEpitaxialAndIncidentAngle") ) {
				TH2* hist = static_cast<TH2D*>(mHistograms.find("ElectronCorrelationDepositEnergyEpitaxialAndIncidentAngle")->second);
				hist->Fill(180 - incidentTheta, incident.depositEnergy[1]);
			}
			if ( mConfig.hasConfig("ElectronCorrelationDepositEnergySubstrateAndIncidentAngle") ) {
				TH2* hist = static_cast<TH2D*>(mHistograms.find("ElectronCorrelationDepositEnergySubstrateAndIncidentAngle")->second);
				hist->Fill(180 - incidentTheta, incident.depositEnergy[2]);
			}
		}
	}
}

void TGeantAnalysis::setHistograms() {
	for ( const std::string& key : mConfigList ) {
		CppConfigDictionary config = mConfig.getConfig(key);
		if ( config.hasKey("type") && config.find("type") == "1H" ) {
			TH1D* hist = new TH1D();
			initHist(hist, config);
			mHistograms.insert_or_assign(key, hist);
		}
		if ( config.hasKey("type") && config.find("type") == "2H" ) {
			TH2D* hist = new TH2D();
			initHist(hist, config);
			mHistograms.insert_or_assign(key, hist);
		}
	}
}

void TGeantAnalysis::saveHistograms() {
	for ( const std::string& key : mConfigList ) {
		CppConfigDictionary config = mConfig.getConfig(key);
		if ( config.hasKey("type") && config.find("type") == "1H" ) {
			TCanvas* canvas = new TCanvas();
			if ( (key == "IncidentParticle") || (key == "SecondaryParticle") ) {
				for ( int i = 0; i < static_cast<TH1D*>(mHistograms.find(key)->second)->GetNbinsX(); i++ ) {
					static_cast<TH1D*>(mHistograms.find(key)->second)->GetXaxis()->SetBinLabel(i + 1, mParticleList[i].c_str());
				}
			}
			if ( (key == "SourceVolume") || (key == "SecondaryVolume") || (key == "IncidentVolume") ) {
				for ( int i = 0; i < static_cast<TH1D*>(mHistograms.find(key)->second)->GetNbinsX(); i++ ) {
					static_cast<TH1D*>(mHistograms.find(key)->second)->GetXaxis()->SetBinLabel(i + 1, mVolumeList[i].c_str());
				}
			}
			savePlot(canvas, static_cast<TH1D*>(mHistograms.find(key)->second), config);
			saveCanvas(canvas, mOutputPath, config);
		}
		if ( config.hasKey("type") && config.find("type") == "2H" ) {
			TCanvas* canvas = new TCanvas();
			if ( key == "SecondaryVolumeVSParticle" ) {
				for ( int i = 0; i < static_cast<TH2D*>(mHistograms.find(key)->second)->GetNbinsX(); i++ ) {
					static_cast<TH2D*>(mHistograms.find(key)->second)->GetXaxis()->SetBinLabel(i + 1, mVolumeList[i].c_str());
				}
				for ( int i = 0; i < static_cast<TH2D*>(mHistograms.find(key)->second)->GetNbinsY(); i++ ) {
					static_cast<TH2D*>(mHistograms.find(key)->second)->GetYaxis()->SetBinLabel(i + 1, mParticleList[i].c_str());
				}
			}
			savePlot(canvas, static_cast<TH2D*>(mHistograms.find(key)->second), config);
			saveCanvas(canvas, mOutputPath, config);
		}
	}
}

// void TGeantAnalysis::getIncidentHist() {
// 	for ( const incidentTuple& incident : mIncidentTupleSet ) {
// 		if ( mConfig.hasConfig("IncidentZ") ) {
// 			mIncidentZHist->Fill(incident.position[2]);
// 		}
// 		if ( mConfig.hasConfig("IncidentXY") ) {
// 			mIncidentXYHist->Fill(incident.position[0], incident.position[1]);
// 		}
// 		if ( mConfig.hasConfig("IncidentAngle") ) {
// 			Double_t momentum = TMath::Sqrt(incident.momentum[0] * incident.momentum[0] + incident.momentum[1] * incident.momentum[1] + incident.momentum[2] * incident.momentum[2]);
// 			Double_t theta = TMath::ACos(incident.momentum[2] / momentum) * 180. / TMath::Pi();
// 			mIncidentAngleHist->Fill(180 - theta);
// 		}
// 		if ( mConfig.hasConfig("IncidentKineticEnergy") ) {
// 			mIncidentKineticEnergyHist->Fill(incident.kineticEnergy);
// 		}
// 		if ( mConfig.hasConfig("IncidentLocalTime") ) {
// 			mIncidentLocalTimeHist->Fill(incident.localTime);
// 		}
// 		if ( mConfig.hasConfig("IncidentGlobalTime") ) {
// 			mIncidentGlobalTimeHist->Fill(incident.globalTime);
// 		}
// 		if ( mConfig.hasConfig("IncidentParticle") ) {
// 			mIncidentParticleHist->Fill(incident.particleID);
// 		}
// 		if ( mConfig.hasConfig("IncidentVolumeID") ) {
// 			mIncidentVolumeIDHist->Fill(incident.initVolumeID);
// 		}
// 		if ( mConfig.hasConfig("DepositEnergyMetal") ) {
// 			if ( incident.depositEnergy[0] > -.5 ) {
// 				mDepositEnergyMetalHist->Fill(incident.depositEnergy[0]);
// 			}
// 		}
// 		if ( mConfig.hasConfig("DepositEnergyEpitaxial") ) {
// 			if ( incident.depositEnergy[1] > -.5 ) {
// 				mDepositEnergyEpitaxialHist->Fill(incident.depositEnergy[1]);
// 			}
// 		}
// 		if ( mConfig.hasConfig("DepositEnergySubstrate") ) {
// 			if ( incident.depositEnergy[2] > -.5 ) {
// 				mDepositEnergySubstrateHist->Fill(incident.depositEnergy[2]);
// 			}
// 		}
// 	}
// }


// void TGeantAnalysis::getSecondaryHist() {
// 	for ( const std::vector<trackTuple>& trackSet : mTrackTupleSet ) {
// 		for ( const trackTuple& track : trackSet ) {
// 			if ( track.parentID != 0 ) {
// 				if ( mConfig.hasConfig("SecondaryVolumeID") ) {
// 					mSecondaryVolumeIDHist->Fill(track.finalVolumeID);
// 				}
// 				if ( mConfig.hasConfig("SecondaryParticleID") ) {
// 					mSecondaryParticleIDHist->Fill(track.particleID);
// 				}
// 			}
// 		}
// 	}
// }

// void TGeantAnalysis::saveSecondaryHist() {
// 	if ( mIsSecondaryVolumeID ) {
// 		TCanvas* secondaryVolumeIDCanvas = new TCanvas();
// 		savePlot(secondaryVolumeIDCanvas, mSecondaryVolumeIDHist, mConfig.getConfig("SecondaryVolumeID"));
// 		saveCanvas(secondaryVolumeIDCanvas, mOutputPath, mConfig.getConfig("SecondaryVolumeID"));
// 	}
// 	if ( mIsSecondaryParticleID ) {
// 		TCanvas* secondaryParticleIDCanvas = new TCanvas();
// 		for ( int i = 0; i < mSecondaryParticleIDHist->GetNbinsX(); i++ ) {
// 			mSecondaryParticleIDHist->GetXaxis()->SetBinLabel(i + 1, mParticleList[i].c_str());
// 		}
// 		savePlot(secondaryParticleIDCanvas, mSecondaryParticleIDHist, mConfig.getConfig("SecondaryParticleID"));
// 		saveCanvas(secondaryParticleIDCanvas, mOutputPath, mConfig.getConfig("SecondaryParticleID"));
// 	}
// }


// void TGeantAnalysis::getSourceXY(const CppConfigDictionary& config) {
// 	TH2* sourceXY = new TH2I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(sourceXY, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.parentID == 0 ) {
// 			sourceXY->Fill(track.initX, track.initY);
// 		}
// 	}
// 	savePlot(canvas, sourceXY, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getSourceZ(const CppConfigDictionary& config) {
// 	TH1* sourceZ = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(sourceZ, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.parentID == 0 ) {
// 			sourceZ->Fill(track.initZ);
// 		}
// 	}
// 	savePlot(canvas, sourceZ, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getSourceKineticEnergy(const CppConfigDictionary& config) {
// 	TH1* sourceKineticEnergy = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(sourceKineticEnergy, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.parentID == 0 ) {
// 			sourceKineticEnergy->Fill(track.initKineticEnergy);
// 		}
// 	}
// 	savePlot(canvas, sourceKineticEnergy, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getSourceTheta(const CppConfigDictionary& config) {
// 	TH1* sourceTheta = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(sourceTheta, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.parentID == 0 ) {
// 			Double_t momentum = TMath::Sqrt(track.initPX * track.initPX + track.initPY * track.initPY + track.initPZ * track.initPZ);
// 			Double_t theta = TMath::ACos(track.initPZ / momentum) * 180. / TMath::Pi();
// 			sourceTheta->Fill(theta);
// 		}
// 	}
// 	savePlot(canvas, sourceTheta, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getSourcePhi(const CppConfigDictionary& config) {
// 	TH1* sourcePhi = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(sourcePhi, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.parentID == 0 ) {
// 			Double_t phi = TMath::ATan2(track.initPY, track.initPX) * 180. / TMath::Pi();
// 			sourcePhi->Fill(phi);
// 		}
// 	}
// 	savePlot(canvas, sourcePhi, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getSourceVolume(const CppConfigDictionary& config) {
// 	TH1* sourceVolume = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(sourceVolume, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.parentID == 0 ) {
// 			sourceVolume->Fill(track.initVolumeID);
// 		}
// 	}
// 	savePlot(canvas, sourceVolume, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getIncidentXY(const CppConfigDictionary& config) {
// 	TH2* incidentXY = new TH2I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(incidentXY, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.initVolumeID != VOLUME::ALPIDEMetal && track.initVolumeID != VOLUME::ALPIDEEpitaxial && track.initVolumeID != VOLUME::ALPIDESubstrate ) {
// 			if ( isRealIncident(track) ) {
// 				incidentXY->Fill(track.incidentX, track.incidentY);
// 			}
// 		}
// 	}
// 	savePlot(canvas, incidentXY, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getIncidentZ(const CppConfigDictionary& config) {
// 	TH1* incidentZ = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(incidentZ, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.initVolumeID != VOLUME::ALPIDEMetal && track.initVolumeID != VOLUME::ALPIDEEpitaxial && track.initVolumeID != VOLUME::ALPIDESubstrate ) {
// 			if ( isRealIncident(track) ) {
// 				incidentZ->Fill(track.incidentZ);
// 			}
// 		}
// 	}
// 	savePlot(canvas, incidentZ, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getIncidentAngle(const CppConfigDictionary& config) {
// 	TH1* incidentAngle = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(incidentAngle, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.initVolumeID != VOLUME::ALPIDEMetal && track.initVolumeID != VOLUME::ALPIDEEpitaxial && track.initVolumeID != VOLUME::ALPIDESubstrate ) {
// 			if ( isRealIncident(track) ) {
// 				Double_t momentum = TMath::Sqrt(track.incidentPX * track.incidentPX + track.incidentPY * track.incidentPY + track.incidentPZ * track.incidentPZ);
// 				Double_t theta = TMath::ACos(track.incidentPZ / momentum) * 180. / TMath::Pi();
// 				incidentAngle->Fill(180 - theta);
// 			}
// 		}
// 	}
// 	savePlot(canvas, incidentAngle, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getIncidentKineticEnergy(const CppConfigDictionary& config) {
// 	TH1* incidentKineticEnergy = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(incidentKineticEnergy, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.initVolumeID != VOLUME::ALPIDEMetal && track.initVolumeID != VOLUME::ALPIDEEpitaxial && track.initVolumeID != VOLUME::ALPIDESubstrate ) {
// 			if ( isRealIncident(track) ) {
// 				incidentKineticEnergy->Fill(track.incidentEnergy);
// 			}
// 		}
// 	}
// 	savePlot(canvas, incidentKineticEnergy, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getIncidentLocalTime(const CppConfigDictionary& config) {
// 	TH1* incidentLocalTime = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(incidentLocalTime, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.initVolumeID != VOLUME::ALPIDEMetal && track.initVolumeID != VOLUME::ALPIDEEpitaxial && track.initVolumeID != VOLUME::ALPIDESubstrate ) {
// 			if ( isRealIncident(track) ) {
// 				incidentLocalTime->Fill(track.incidentLocalTime);
// 			}
// 		}
// 	}
// 	savePlot(canvas, incidentLocalTime, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getIncidentGlobalTime(const CppConfigDictionary& config) {
// 	TH1* incidentGlobalTime = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(incidentGlobalTime, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.initVolumeID != VOLUME::ALPIDEMetal && track.initVolumeID != VOLUME::ALPIDEEpitaxial && track.initVolumeID != VOLUME::ALPIDESubstrate ) {
// 			if ( isRealIncident(track) ) {
// 				incidentGlobalTime->Fill(track.incidentGlobalTime);
// 			}
// 		}
// 	}
// 	savePlot(canvas, incidentGlobalTime, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getTotalDepositedEnergy(const CppConfigDictionary& config) {
// 	TH1* totalDepositEnergy = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(totalDepositEnergy, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.initVolumeID != VOLUME::ALPIDEMetal && track.initVolumeID != VOLUME::ALPIDEEpitaxial && track.initVolumeID != VOLUME::ALPIDESubstrate ) {
// 			if ( isRealIncident(track) ) {
// 				Double_t totalDeposit = 0.;
// 				totalDeposit += track.depositEnergyMetal;
// 				totalDeposit += track.depositEnergyEpitaxial;
// 				totalDeposit += track.depositEnergySubstrate;
// 				totalDepositEnergy->Fill(totalDeposit);
// 			}
// 		}
// 	}
// 	savePlot(canvas, totalDepositEnergy, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getDepositedEnergyMetal(const CppConfigDictionary& config) {
// 	TH1* depositEnergyMetal = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(depositEnergyMetal, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.initVolumeID != VOLUME::ALPIDEMetal && track.initVolumeID != VOLUME::ALPIDEEpitaxial && track.initVolumeID != VOLUME::ALPIDESubstrate ) {
// 			if ( isRealIncident(track) ) {
// 				if ( track.depositEnergyMetal > 0.000001 ) {
// 					depositEnergyMetal->Fill(track.depositEnergyMetal);
// 				}
// 			}
// 		}
// 	}
// 	savePlot(canvas, depositEnergyMetal, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getDepositedEnergyEpitaxial(const CppConfigDictionary& config) {
// 	TH1* depositEnergyEpitaxial = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(depositEnergyEpitaxial, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.initVolumeID != VOLUME::ALPIDEMetal && track.initVolumeID != VOLUME::ALPIDEEpitaxial && track.initVolumeID != VOLUME::ALPIDESubstrate ) {
// 			if ( isRealIncident(track) ) {
// 				if ( track.depositEnergyEpitaxial > 0.000001 ) {
// 					depositEnergyEpitaxial->Fill(track.depositEnergyEpitaxial);
// 				}
// 			}
// 		}
// 	}
// 	savePlot(canvas, depositEnergyEpitaxial, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getDepositedEnergySubstrate(const CppConfigDictionary& config) {
// 	TH1* depositEnergySubstrate = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(depositEnergySubstrate, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.initVolumeID != VOLUME::ALPIDEMetal && track.initVolumeID != VOLUME::ALPIDEEpitaxial && track.initVolumeID != VOLUME::ALPIDESubstrate ) {
// 			if ( isRealIncident(track) ) {
// 				if ( track.depositEnergySubstrate > 0.000001 ) {
// 					depositEnergySubstrate->Fill(track.depositEnergySubstrate);
// 				}
// 			}
// 		}
// 	}
// 	savePlot(canvas, depositEnergySubstrate, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getParticleType(const CppConfigDictionary& config) {
// 	TH1* particleType = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(particleType, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.initVolumeID != VOLUME::ALPIDEMetal && track.initVolumeID != VOLUME::ALPIDEEpitaxial && track.initVolumeID != VOLUME::ALPIDESubstrate ) {
// 			particleType->Fill(track.particleID);
// 		}
// 	}

// 	for ( int i = 0; i < particleType->GetNbinsX(); i++ ) {
// 		particleType->GetXaxis()->SetBinLabel(i + 1, mParticleList[i].c_str());
// 	}

// 	savePlot(canvas, particleType, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getIncidentParticleType(const CppConfigDictionary& config) {
// 	TH1* incidentParticleType = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(incidentParticleType, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.initVolumeID != VOLUME::ALPIDEMetal && track.initVolumeID != VOLUME::ALPIDEEpitaxial && track.initVolumeID != VOLUME::ALPIDESubstrate ) {
// 			if ( isRealIncident(track) ) {
// 				incidentParticleType->Fill(track.particleID);
// 			}
// 		}
// 	}

// 	for ( int i = 0; i < incidentParticleType->GetNbinsX(); i++ ) {
// 		incidentParticleType->GetXaxis()->SetBinLabel(i + 1, mParticleList[i].c_str());
// 	}

// 	savePlot(canvas, incidentParticleType, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getAlphaDepositedEnergyMetal(const CppConfigDictionary& config) {
// 	TH1* alphaDepositEnergyMetal = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(alphaDepositEnergyMetal, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.initVolumeID != VOLUME::ALPIDEMetal && track.initVolumeID != VOLUME::ALPIDEEpitaxial && track.initVolumeID != VOLUME::ALPIDESubstrate ) {
// 			if ( isRealIncident(track) && track.particleID == PARTICLE::alpha ) {
// 				if ( track.depositEnergyMetal > 0.000001 ) {
// 					alphaDepositEnergyMetal->Fill(track.depositEnergyMetal);
// 				}
// 			}
// 		}
// 	}
// 	savePlot(canvas, alphaDepositEnergyMetal, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getAlphaDepositedEnergyEpitaxial(const CppConfigDictionary& config) {
// 	TH1* alphaDepositEnergyEpitaxial = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(alphaDepositEnergyEpitaxial, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.initVolumeID != VOLUME::ALPIDEMetal && track.initVolumeID != VOLUME::ALPIDEEpitaxial && track.initVolumeID != VOLUME::ALPIDESubstrate ) {
// 			if ( isRealIncident(track) && track.particleID == PARTICLE::alpha ) {
// 				if ( track.depositEnergyEpitaxial > 0.000001 ) {
// 					alphaDepositEnergyEpitaxial->Fill(track.depositEnergyEpitaxial);
// 				}
// 			}
// 		}
// 	}
// 	savePlot(canvas, alphaDepositEnergyEpitaxial, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getAlphaDepositedEnergySubstrate(const CppConfigDictionary& config) {
// 	TH1* alphaDepositEnergySubstrate = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(alphaDepositEnergySubstrate, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.initVolumeID != VOLUME::ALPIDEMetal && track.initVolumeID != VOLUME::ALPIDEEpitaxial && track.initVolumeID != VOLUME::ALPIDESubstrate ) {
// 			if ( isRealIncident(track) && track.particleID == PARTICLE::alpha ) {
// 				if ( track.depositEnergySubstrate > 0.000001 ) {
// 					alphaDepositEnergySubstrate->Fill(track.depositEnergySubstrate);
// 				}
// 			}
// 		}
// 	}
// 	savePlot(canvas, alphaDepositEnergySubstrate, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getAlphaIncidentAngle(const CppConfigDictionary& config) {
// 	TH1* alphaIncidentAngle = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(alphaIncidentAngle, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( isRealIncident(track) && track.particleID == PARTICLE::alpha ) {
// 			Double_t momentum = TMath::Sqrt(track.incidentPX * track.incidentPX + track.incidentPY * track.incidentPY + track.incidentPZ * track.incidentPZ);
// 			Double_t theta = TMath::ACos(track.incidentPZ / momentum) * 180. / TMath::Pi();
// 			alphaIncidentAngle->Fill(180 - theta);
// 		}
// 	}

// 	savePlot(canvas, alphaIncidentAngle, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getAlphaGlobalTime(const CppConfigDictionary& config) {
// 	TH1* alphaGlobalTime = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(alphaGlobalTime, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( isRealIncident(track) && track.particleID == PARTICLE::alpha ) {
// 			alphaGlobalTime->Fill(track.incidentGlobalTime);
// 		}
// 	}

// 	savePlot(canvas, alphaGlobalTime, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getAlphaLocalTime(const CppConfigDictionary& config) {
// 	TH1* alphaLocalTime = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(alphaLocalTime, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( isRealIncident(track) && track.particleID == PARTICLE::alpha ) {
// 			alphaLocalTime->Fill(track.incidentLocalTime);
// 		}
// 	}

// 	savePlot(canvas, alphaLocalTime, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getElectronDepositedEnergyMetal(const CppConfigDictionary& config) {
// 	TH1* electronDepositEnergyMetal = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(electronDepositEnergyMetal, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.initVolumeID != VOLUME::ALPIDEMetal && track.initVolumeID != VOLUME::ALPIDEEpitaxial && track.initVolumeID != VOLUME::ALPIDESubstrate ) {
// 			if ( isRealIncident(track) && track.particleID == PARTICLE::electron ) {
// 				if ( track.depositEnergyMetal > 0.000001 ) {
// 					electronDepositEnergyMetal->Fill(track.depositEnergyMetal);
// 				}
// 			}
// 		}
// 	}
// 	savePlot(canvas, electronDepositEnergyMetal, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getElectronDepositedEnergyEpitaxial(const CppConfigDictionary& config) {
// 	TH1* electronDepositEnergyEpitaxial = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(electronDepositEnergyEpitaxial, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.initVolumeID != VOLUME::ALPIDEMetal && track.initVolumeID != VOLUME::ALPIDEEpitaxial && track.initVolumeID != VOLUME::ALPIDESubstrate ) {
// 			if ( isRealIncident(track) && track.particleID == PARTICLE::electron ) {
// 				if ( track.depositEnergyEpitaxial > 0.000001 ) {
// 					electronDepositEnergyEpitaxial->Fill(track.depositEnergyEpitaxial);
// 				}
// 			}
// 		}
// 	}
// 	savePlot(canvas, electronDepositEnergyEpitaxial, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getElectronDepositedEnergySubstrate(const CppConfigDictionary& config) {
// 	TH1* electronDepositEnergySubstrate = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(electronDepositEnergySubstrate, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.initVolumeID != VOLUME::ALPIDEMetal && track.initVolumeID != VOLUME::ALPIDEEpitaxial && track.initVolumeID != VOLUME::ALPIDESubstrate ) {
// 			if ( isRealIncident(track) && track.particleID == PARTICLE::electron ) {
// 				if ( track.depositEnergySubstrate > 0.000001 ) {
// 					electronDepositEnergySubstrate->Fill(track.depositEnergySubstrate);
// 				}
// 			}
// 		}
// 	}
// 	savePlot(canvas, electronDepositEnergySubstrate, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getElectronIncidentAngle(const CppConfigDictionary& config) {
// 	TH1* electronIncidentAngle = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(electronIncidentAngle, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( isRealIncident(track) && track.particleID == PARTICLE::electron ) {
// 			if ( track.depositEnergyMetal > 0.000001 ) {
// 				Double_t momentum = TMath::Sqrt(track.incidentPX * track.incidentPX + track.incidentPY * track.incidentPY + track.incidentPZ * track.incidentPZ);
// 				Double_t theta = TMath::ACos(track.incidentPZ / momentum) * 180. / TMath::Pi();
// 				electronIncidentAngle->Fill(180 - theta);
// 			}
// 		}
// 	}

// 	savePlot(canvas, electronIncidentAngle, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getElectronGlobalTime(const CppConfigDictionary& config) {
// 	TH1* electronGlobalTime = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(electronGlobalTime, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( isRealIncident(track) && track.particleID == PARTICLE::electron ) {
// 			electronGlobalTime->Fill(track.incidentGlobalTime);
// 		}
// 	}

// 	savePlot(canvas, electronGlobalTime, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getElectronLocalTime(const CppConfigDictionary& config) {
// 	TH1* electronLocalTime = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(electronLocalTime, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( isRealIncident(track) && track.particleID == PARTICLE::electron ) {
// 			electronLocalTime->Fill(track.incidentLocalTime);
// 		}
// 	}

// 	savePlot(canvas, electronLocalTime, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getCorrelationDepositEnergyMetalAndIncidentAngle(const CppConfigDictionary& config) {
// 	TH2* correlationDepositEnergyMetalAndIncidentAngle = new TH2I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(correlationDepositEnergyMetalAndIncidentAngle, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( isRealIncident(track) ) {
// 			if ( track.depositEnergyMetal > 0.000001 ) {
// 				Double_t momentum = TMath::Sqrt(track.incidentPX * track.incidentPX + track.incidentPY * track.incidentPY + track.incidentPZ * track.incidentPZ);
// 				Double_t theta = TMath::ACos(track.incidentPZ / momentum) * 180. / TMath::Pi();
// 				correlationDepositEnergyMetalAndIncidentAngle->Fill(180 - theta, track.depositEnergyMetal);
// 			}
// 		}
// 	}

// 	savePlot(canvas, correlationDepositEnergyMetalAndIncidentAngle, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getCorrelationDepositEnergyEpitaxialAndIncidentAngle(const CppConfigDictionary& config) {
// 	TH2* correlationDepositEnergyEpitaxialAndIncidentAngle = new TH2I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(correlationDepositEnergyEpitaxialAndIncidentAngle, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( isRealIncident(track) ) {
// 			if ( track.depositEnergyEpitaxial > 0.000001 ) {
// 				Double_t momentum = TMath::Sqrt(track.incidentPX * track.incidentPX + track.incidentPY * track.incidentPY + track.incidentPZ * track.incidentPZ);
// 				Double_t theta = TMath::ACos(track.incidentPZ / momentum) * 180. / TMath::Pi();
// 				correlationDepositEnergyEpitaxialAndIncidentAngle->Fill(180 - theta, track.depositEnergyEpitaxial);
// 			}
// 		}
// 	}

// 	savePlot(canvas, correlationDepositEnergyEpitaxialAndIncidentAngle, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getCorrelationDepositEnergySubstrateAndIncidentAngle(const CppConfigDictionary& config) {
// 	TH2* correlationDepositEnergySubstrateAndIncidentAngle = new TH2I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(correlationDepositEnergySubstrateAndIncidentAngle, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( isRealIncident(track) ) {
// 			if ( track.depositEnergySubstrate > 0.000001 ) {
// 				Double_t momentum = TMath::Sqrt(track.incidentPX * track.incidentPX + track.incidentPY * track.incidentPY + track.incidentPZ * track.incidentPZ);
// 				Double_t theta = TMath::ACos(track.incidentPZ / momentum) * 180. / TMath::Pi();
// 				correlationDepositEnergySubstrateAndIncidentAngle->Fill(180 - theta, track.depositEnergySubstrate);
// 			}
// 		}
// 	}

// 	savePlot(canvas, correlationDepositEnergySubstrateAndIncidentAngle, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getCorrelationAlphaDepositEnergyAndIncidentAngle(const CppConfigDictionary& config) {
// 	TH2* correlationAlphaDepositEnergyAndIncidentAngle = new TH2I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(correlationAlphaDepositEnergyAndIncidentAngle, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( isRealIncident(track) && track.particleID == PARTICLE::alpha ) {
// 			Double_t momentum = TMath::Sqrt(track.incidentPX * track.incidentPX + track.incidentPY * track.incidentPY + track.incidentPZ * track.incidentPZ);
// 			Double_t theta = TMath::ACos(track.incidentPZ / momentum) * 180. / TMath::Pi();
// 			correlationAlphaDepositEnergyAndIncidentAngle->Fill(180 - theta, track.depositEnergyMetal + track.depositEnergyEpitaxial + track.depositEnergySubstrate);
// 		}
// 	}

// 	savePlot(canvas, correlationAlphaDepositEnergyAndIncidentAngle, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getCorrelationAlphaDepositEnergyMetalAndIncidentAngle(const CppConfigDictionary& config) {
// 	TH2* correlationAlphaDepositEnergyMetalAndIncidentAngle = new TH2I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(correlationAlphaDepositEnergyMetalAndIncidentAngle, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( isRealIncident(track) && track.particleID == PARTICLE::alpha ) {
// 			if ( track.depositEnergyMetal > 0.000001 ) {
// 				Double_t momentum = TMath::Sqrt(track.incidentPX * track.incidentPX + track.incidentPY * track.incidentPY + track.incidentPZ * track.incidentPZ);
// 				Double_t theta = TMath::ACos(track.incidentPZ / momentum) * 180. / TMath::Pi();
// 				correlationAlphaDepositEnergyMetalAndIncidentAngle->Fill(180 - theta, track.depositEnergyMetal);
// 			}
// 		}
// 	}

// 	savePlot(canvas, correlationAlphaDepositEnergyMetalAndIncidentAngle, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getCorrelationAlphaDepositEnergyEpitaxialAndIncidentAngle(const CppConfigDictionary& config) {
// 	TH2* correlationAlphaDepositEnergyEpitaxialAndIncidentAngle = new TH2I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(correlationAlphaDepositEnergyEpitaxialAndIncidentAngle, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( isRealIncident(track) && track.particleID == PARTICLE::alpha ) {
// 			if ( track.depositEnergyEpitaxial > 0.000001 ) {
// 				Double_t momentum = TMath::Sqrt(track.incidentPX * track.incidentPX + track.incidentPY * track.incidentPY + track.incidentPZ * track.incidentPZ);
// 				Double_t theta = TMath::ACos(track.incidentPZ / momentum) * 180. / TMath::Pi();
// 				correlationAlphaDepositEnergyEpitaxialAndIncidentAngle->Fill(180 - theta, track.depositEnergyEpitaxial);
// 			}
// 		}
// 	}

// 	savePlot(canvas, correlationAlphaDepositEnergyEpitaxialAndIncidentAngle, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getCorrelationAlphaDepositEnergySubstrateAndIncidentAngle(const CppConfigDictionary& config) {
// 	TH2* correlationAlphaDepositEnergySubstrateAndIncidentAngle = new TH2I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(correlationAlphaDepositEnergySubstrateAndIncidentAngle, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( isRealIncident(track) && track.particleID == PARTICLE::alpha ) {
// 			if ( track.depositEnergySubstrate > 0.000001 ) {
// 				Double_t momentum = TMath::Sqrt(track.incidentPX * track.incidentPX + track.incidentPY * track.incidentPY + track.incidentPZ * track.incidentPZ);
// 				Double_t theta = TMath::ACos(track.incidentPZ / momentum) * 180. / TMath::Pi();
// 				correlationAlphaDepositEnergySubstrateAndIncidentAngle->Fill(180 - theta, track.depositEnergySubstrate);
// 			}
// 		}
// 	}

// 	savePlot(canvas, correlationAlphaDepositEnergySubstrateAndIncidentAngle, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getCorrelationElectronDepositEnergyMetalAndIncidentAngle(const CppConfigDictionary& config) {
// 	TH2* correlationElectronDepositEnergyMetalAndIncidentAngle = new TH2I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(correlationElectronDepositEnergyMetalAndIncidentAngle, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( isRealIncident(track) && track.particleID == PARTICLE::electron ) {
// 			if ( track.depositEnergyMetal > 0.000001 ) {
// 				Double_t momentum = TMath::Sqrt(track.incidentPX * track.incidentPX + track.incidentPY * track.incidentPY + track.incidentPZ * track.incidentPZ);
// 				Double_t theta = TMath::ACos(track.incidentPZ / momentum) * 180. / TMath::Pi();
// 				correlationElectronDepositEnergyMetalAndIncidentAngle->Fill(180 - theta, track.depositEnergyMetal);
// 			}
// 		}
// 	}

// 	savePlot(canvas, correlationElectronDepositEnergyMetalAndIncidentAngle, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getCorrelationElectronDepositEnergyEpitaxialAndIncidentAngle(const CppConfigDictionary& config) {
// 	TH2* correlationElectronDepositEnergyEpitaxialAndIncidentAngle = new TH2I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(correlationElectronDepositEnergyEpitaxialAndIncidentAngle, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( isRealIncident(track) && track.particleID == PARTICLE::electron ) {
// 			if ( track.depositEnergyEpitaxial > 0.000001 ) {
// 				Double_t momentum = TMath::Sqrt(track.incidentPX * track.incidentPX + track.incidentPY * track.incidentPY + track.incidentPZ * track.incidentPZ);
// 				Double_t theta = TMath::ACos(track.incidentPZ / momentum) * 180. / TMath::Pi();
// 				correlationElectronDepositEnergyEpitaxialAndIncidentAngle->Fill(180 - theta, track.depositEnergyEpitaxial);
// 			}
// 		}
// 	}

// 	savePlot(canvas, correlationElectronDepositEnergyEpitaxialAndIncidentAngle, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getCorrelationElectronDepositEnergySubstrateAndIncidentAngle(const CppConfigDictionary& config) {
// 	TH2* correlationElectronDepositEnergySubstrateAndIncidentAngle = new TH2I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(correlationElectronDepositEnergySubstrateAndIncidentAngle, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( isRealIncident(track) && track.particleID == PARTICLE::electron ) {
// 			if ( track.depositEnergySubstrate > 0.000001 ) {
// 				Double_t momentum = TMath::Sqrt(track.incidentPX * track.incidentPX + track.incidentPY * track.incidentPY + track.incidentPZ * track.incidentPZ);
// 				Double_t theta = TMath::ACos(track.incidentPZ / momentum) * 180. / TMath::Pi();
// 				correlationElectronDepositEnergySubstrateAndIncidentAngle->Fill(180 - theta, track.depositEnergySubstrate);
// 			}
// 		}
// 	}

// 	savePlot(canvas, correlationElectronDepositEnergySubstrateAndIncidentAngle, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getSecondaryVolumeID(const CppConfigDictionary& config) {
// 	TH1* secondaryVolumeID = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(secondaryVolumeID, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.parentID != 0 ) {
// 			secondaryVolumeID->Fill(track.initVolumeID);
// 		}
// 	}

// 	savePlot(canvas, secondaryVolumeID, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getIncidentSecondaryVolumeID(const CppConfigDictionary& config) {
// 	TH1* incidentSecondaryVolumeID = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(incidentSecondaryVolumeID, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.parentID != 0 ) {
// 			if ( isRealIncident(track) ) {
// 				incidentSecondaryVolumeID->Fill(track.initVolumeID);
// 			}
// 		}
// 	}

// 	savePlot(canvas, incidentSecondaryVolumeID, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// void TGeantAnalysis::getSecondaryZ(const CppConfigDictionary& config) {
// 	TH1* secondaryZ = new TH1I();
// 	TCanvas* canvas = new TCanvas();

// 	initHist(secondaryZ, config);

// 	for ( const trackTuple& track : mTrackTupleSet ) {
// 		if ( track.parentID != 0 ) {
// 			secondaryZ->Fill(track.initZ);
// 		}
// 	}

// 	savePlot(canvas, secondaryZ, config);
// 	saveCanvas(canvas, mOutputPath, config);
// }

// bool TGeantAnalysis::isRealIncident(const trackTuple& track) {
// 	if ( (track.initVolumeID != VOLUME::ALPIDEMetal) && (track.initVolumeID != VOLUME::ALPIDEEpitaxial) && (track.initVolumeID != VOLUME::ALPIDESubstrate) && track.isIncident ) {
// 		return true;
// 	}
// 	return false;
// }