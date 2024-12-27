#include "TGeantAnalysis.h"


TGeantAnalysis::TGeantAnalysis() : TPlotter() { }

TGeantAnalysis::~TGeantAnalysis() { }

void TGeantAnalysis::readInputFile(std::filesystem::path inputFilePath) {
	TString inputFileName = std::string(inputFilePath);
	mInputFile = std::make_unique<TFile>(inputFileName, "READ");
	mIncidentTree.reset(static_cast<TTree*>(mInputFile->Get("incidentTree")));

	mIncidentTree->SetBranchAddress("eventID", &mIncidentTuple.eventID);
	mIncidentTree->SetBranchAddress("trackID", &mIncidentTuple.trackID);
	mIncidentTree->SetBranchAddress("particleID", &mIncidentTuple.particleID);
	mIncidentTree->SetBranchAddress("initialVolumeID", &mIncidentTuple.initialVolumeID);
	mIncidentTree->SetBranchAddress("initX", &mIncidentTuple.initialPosition[0]);
	mIncidentTree->SetBranchAddress("initY", &mIncidentTuple.initialPosition[1]);
	mIncidentTree->SetBranchAddress("initZ", &mIncidentTuple.initialPosition[2]);
	mIncidentTree->SetBranchAddress("initPX", &mIncidentTuple.initialMomentum[0]);
	mIncidentTree->SetBranchAddress("initPY", &mIncidentTuple.initialMomentum[1]);
	mIncidentTree->SetBranchAddress("initPZ", &mIncidentTuple.initialMomentum[2]);
	mIncidentTree->SetBranchAddress("initKineticEnergy", &mIncidentTuple.initialKineticEnergy);
	mIncidentTree->SetBranchAddress("depositEnergyMetal", &mIncidentTuple.depositEnergy[0]);
	mIncidentTree->SetBranchAddress("depositEnergyEpitaxial", &mIncidentTuple.depositEnergy[1]);
	mIncidentTree->SetBranchAddress("depositEnergySubstrate", &mIncidentTuple.depositEnergy[2]);
	mIncidentTree->SetBranchAddress("incidentX", &mIncidentTuple.position[0]);
	mIncidentTree->SetBranchAddress("incidentY", &mIncidentTuple.position[1]);
	mIncidentTree->SetBranchAddress("incidentZ", &mIncidentTuple.position[2]);
	mIncidentTree->SetBranchAddress("incidentPX", &mIncidentTuple.momentum[0]);
	mIncidentTree->SetBranchAddress("incidentPY", &mIncidentTuple.momentum[1]);
	mIncidentTree->SetBranchAddress("incidentPZ", &mIncidentTuple.momentum[2]);
	mIncidentTree->SetBranchAddress("incidentKineticEnergy", &mIncidentTuple.kineticEnergy);
	mIncidentTree->SetBranchAddress("incidentGlobalTime", &mIncidentTuple.globalTime);
	mIncidentTree->SetBranchAddress("incidentLocalTime", &mIncidentTuple.localTime);
	mIncidentTree->SetBranchAddress("finalVolumeID", &mIncidentTuple.finalVolumeID);
	mIncidentTree->SetBranchAddress("finalX", &mIncidentTuple.finalPosition[0]);
	mIncidentTree->SetBranchAddress("finalY", &mIncidentTuple.finalPosition[1]);
	mIncidentTree->SetBranchAddress("finalZ", &mIncidentTuple.finalPosition[2]);
	mIncidentTree->SetBranchAddress("finalPX", &mIncidentTuple.finalMomentum[0]);
	mIncidentTree->SetBranchAddress("finalPY", &mIncidentTuple.finalMomentum[1]);
	mIncidentTree->SetBranchAddress("finalPZ", &mIncidentTuple.finalMomentum[2]);
	mIncidentTree->SetBranchAddress("finalKineticEnergy", &mIncidentTuple.finalKineticEnergy);
}

void TGeantAnalysis::setHistograms(const std::vector<CppConfigDictionary>& configList) {
	for ( const CppConfigDictionary& config : configList ) {
		std::string_view key = config.getConfigName();
		if ( config.hasKey("type") && config.find("type") == "1H" ) {
			std::unique_ptr<TH1D> hist = std::make_unique<TH1D>();
			initHist(hist, config);
			m1DHistograms.insert_or_assign(key, std::move(hist));
		}
		if ( config.hasKey("type") && config.find("type") == "2H" ) {
			std::unique_ptr<TH2D> hist = std::make_unique<TH2D>();
			initHist(hist, config);
			m2DHistograms.insert_or_assign(key, std::move(hist));
		}
	}
}

void TGeantAnalysis::readTree() {
	Int_t nEntries = mIncidentTree->GetEntries();

	std::vector<std::pair<Double_t, Double_t>> position;
	Int_t preTimeStamp = 0;
	ProgressBar progressBar(static_cast<int>(nEntries));
	Int_t nDouble = 0;
	for ( Int_t i = 0; i < nEntries; i++ ) {
		progressBar.printProgress();
		mIncidentTree->GetEntry(i);
		fillHistograms();

		if ( mIncidentTuple.particleID == PARTICLE::alpha && mIncidentTuple.depositEnergy[1] > 1.17 ) {
			position.push_back({mIncidentTuple.position[0], mIncidentTuple.position[1]});
		}
		if ( preTimeStamp != mIncidentTuple.eventID / 107 ) {
			preTimeStamp = mIncidentTuple.eventID / 107;
			nDouble += getNDouble(position);
			position.clear();
		}
	}
	std::cout << nDouble << std::endl;
}

int TGeantAnalysis::getNDouble(std::vector<std::pair<Double_t, Double_t>> position) {
	Int_t num = 0;
	for ( Int_t i = 0; i < position.size(); i++ ) {
		for ( Int_t j = i + 1; j < position.size(); j++ ) {
			Double_t distance = TMath::Sqrt(TMath::Power(position[i].first - position[j].first, 2) + TMath::Power(position[i].second - position[j].second, 2));
			if ( distance > 0.048 && distance < 0.154 ) {
				num++;
			}
		}
	}
	return num;
}


void TGeantAnalysis::fillHistograms() {
	Double_t incidentMomentum = TMath::Sqrt(mIncidentTuple.momentum[0] * mIncidentTuple.momentum[0] + mIncidentTuple.momentum[1] * mIncidentTuple.momentum[1] + mIncidentTuple.momentum[2] * mIncidentTuple.momentum[2]);
	Double_t incidentTheta = TMath::ACos(mIncidentTuple.momentum[2] / incidentMomentum) * 180. / TMath::Pi();
	Double_t depositEnergy = 0.;
	bool isDeposit = false;
	if ( mIncidentTuple.depositEnergy[0] > -.5 ) {
		depositEnergy += mIncidentTuple.depositEnergy[0];
		isDeposit = true;
	}
	if ( mIncidentTuple.depositEnergy[1] > -.5 ) {
		depositEnergy += mIncidentTuple.depositEnergy[1];
		isDeposit = true;
	}
	if ( mIncidentTuple.depositEnergy[2] > -.5 ) {
		depositEnergy += mIncidentTuple.depositEnergy[2];
		isDeposit = true;
	}

	for ( const auto& [key, hist] : m1DHistograms ) {
		if ( key == "IncidentXLine" ) {
			if ( TMath::Abs(mIncidentTuple.position[1]) < 0.028 ) {
				hist->Fill(mIncidentTuple.position[0]);
			}
		}
		if ( key == "IncidentYLine" ) {
			if ( TMath::Abs(mIncidentTuple.position[0]) < 0.028 ) {
				hist->Fill(mIncidentTuple.position[1]);
			}
		}
		if ( key == "IncidentZ" ) {
			hist->Fill(mIncidentTuple.position[2]);
		}
		if ( key == "IncidentAngle" ) {
			hist->Fill(180 - incidentTheta);
		}
		if ( key == "IncidentKineticEnergy" ) {
			hist->Fill(mIncidentTuple.kineticEnergy);
		}
		if ( key == "IncidentParticle" ) {
			hist->Fill(mIncidentTuple.particleID);
		}
		if ( key == "DepositEnergyTotal" ) {
			if ( isDeposit ) {
				hist->Fill(depositEnergy);
			}
		}
		if ( key == "DepositEnergyMetal" ) {
			if ( mIncidentTuple.depositEnergy[0] > -.5 ) {
				hist->Fill(mIncidentTuple.depositEnergy[0]);
			}
		}
		if ( key == "DepositEnergyEpitaxial" ) {
			if ( mIncidentTuple.depositEnergy[1] > -.5 ) {
				hist->Fill(mIncidentTuple.depositEnergy[1]);
			}
		}
		if ( key == "DepositEnergySubstrate" ) {
			if ( mIncidentTuple.depositEnergy[2] > -.5 ) {
				hist->Fill(mIncidentTuple.depositEnergy[2]);
			}
		}
		if ( key == "IncidentGlobalTime" ) {
			hist->Fill(mIncidentTuple.globalTime);
		}
		if ( key == "IncidentLocalTime" ) {
			hist->Fill(mIncidentTuple.localTime);
		}
		if ( mIncidentTuple.particleID == PARTICLE::alpha ) {
			if ( key == "AlphaIncidentAngle" ) {
				hist->Fill(180 - incidentTheta);
			}
			if ( key == "AlphaIncidentKineticEnergy" ) {
				hist->Fill(mIncidentTuple.kineticEnergy);
			}
			if ( key == "AlphaIncidentGlobalTime" ) {
				hist->Fill(mIncidentTuple.globalTime);
			}
			if ( key == "AlphaIncidentLocalTime" ) {
				hist->Fill(mIncidentTuple.localTime);
			}
			if ( key == "AlphaDepositEnergyTotal" ) {
				if ( isDeposit ) {
					hist->Fill(depositEnergy);
				}
			}
			if ( key == "AlphaDepositEnergyMetal" ) {
				if ( mIncidentTuple.depositEnergy[0] > -.5 ) {
					hist->Fill(mIncidentTuple.depositEnergy[0]);
				}
			}
			if ( key == "AlphaDepositEnergyEpitaxial" ) {
				if ( mIncidentTuple.depositEnergy[1] > -.5 ) {
					hist->Fill(mIncidentTuple.depositEnergy[1]);
				}
			}
			if ( key == "AlphaDepositEnergySubstrate" ) {
				if ( mIncidentTuple.depositEnergy[2] > -.5 ) {
					hist->Fill(mIncidentTuple.depositEnergy[2]);
				}
			}
		}
		if ( mIncidentTuple.particleID == PARTICLE::electron ) {
			if ( key == "ElectronIncidentAngle" ) {
				hist->Fill(180 - incidentTheta);
			}
			if ( key == "ElectronIncidentKineticEnergy" ) {
				hist->Fill(mIncidentTuple.kineticEnergy);
			}
			if ( key == "ElectronIncidentGlobalTime" ) {
				hist->Fill(mIncidentTuple.globalTime);
			}
			if ( key == "ElectronIncidentLocalTime" ) {
				hist->Fill(mIncidentTuple.localTime);
			}
			if ( key == "ElectronDepositEnergyTotal" ) {
				if ( isDeposit ) {
					hist->Fill(depositEnergy);
				}
			}
			if ( key == "ElectronDepositEnergyMetal" ) {
				if ( mIncidentTuple.depositEnergy[0] > -.5 ) {
					hist->Fill(mIncidentTuple.depositEnergy[0]);
				}
			}
			if ( key == "ElectronDepositEnergyEpitaxial" ) {
				if ( mIncidentTuple.depositEnergy[1] > -.5 ) {
					hist->Fill(mIncidentTuple.depositEnergy[1]);
				}
			}
			if ( key == "ElectronDepositEnergySubstrate" ) {
				if ( mIncidentTuple.depositEnergy[2] > -.5 ) {
					hist->Fill(mIncidentTuple.depositEnergy[2]);
				}
			}
		}
	}
	for ( const auto& [key, hist] : m2DHistograms ) {
		if ( key == "IncidentXY" ) {
			hist->Fill(mIncidentTuple.position[0], mIncidentTuple.position[1]);
		}
		if ( key == "CorrelationDepositEnergyTotalAndIncidentAngle" ) {
			if ( isDeposit ) {
				hist->Fill(180 - incidentTheta, depositEnergy);
			}
		}
		if ( key == "CorrelationDepositEnergyMetalAndIncidentAngle" ) {
			if ( mIncidentTuple.depositEnergy[0] > -.5 ) {
				hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[0]);
			}
		}
		if ( key == "CorrelationDepositEnergyEpitaxialAndIncidentAngle" ) {
			if ( mIncidentTuple.depositEnergy[1] > -.5 ) {
				hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[1]);
			}
		}
		if ( key == "CorrelationDepositEnergySubstrateAndIncidentAngle" ) {
			if ( mIncidentTuple.depositEnergy[2] > -.5 ) {
				hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[2]);
			}
		}
		if ( mIncidentTuple.particleID == PARTICLE::alpha ) {
			if ( key == "AlphaIncidentXY" ) {
				hist->Fill(mIncidentTuple.position[0], mIncidentTuple.position[1]);
			}
			if ( key == "AlphaCorrelationDepositEnergyTotalAndIncidentAngle" ) {
				if ( isDeposit ) {
					hist->Fill(180 - incidentTheta, depositEnergy);
				}
			}
			if ( key == "AlphaCorrelationDepositEnergyMetalAndIncidentAngle" ) {
				if ( mIncidentTuple.depositEnergy[0] > -.5 ) {
					hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[0]);
				}
			}
			if ( key == "AlphaCorrelationDepositEnergyEpitaxialAndIncidentAngle" ) {
				if ( mIncidentTuple.depositEnergy[1] > -.5 ) {
					hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[1]);
				}
			}
			if ( key == "AlphaCorrelationDepositEnergySubstrateAndIncidentAngle" ) {
				if ( mIncidentTuple.depositEnergy[2] > -.5 ) {
					hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[2]);
				}
			}
		}
		if ( mIncidentTuple.particleID == PARTICLE::electron ) {
			if ( key == "ElectronIncidentXY" ) {
				hist->Fill(mIncidentTuple.position[0], mIncidentTuple.position[1]);
			}
			if ( key == "ElectronCorrelationDepositEnergyTotalAndIncidentAngle" ) {
				if ( isDeposit ) {
					hist->Fill(180 - incidentTheta, depositEnergy);
				}
			}
			if ( key == "ElectronCorrelationDepositEnergyMetalAndIncidentAngle" ) {
				if ( mIncidentTuple.depositEnergy[0] > -.5 ) {
					hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[0]);
				}
			}
			if ( key == "ElectronCorrelationDepositEnergyEpitaxialAndIncidentAngle" ) {
				if ( mIncidentTuple.depositEnergy[1] > -.5 ) {
					hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[1]);
				}
			}
			if ( key == "ElectronCorrelationDepositEnergySubstrateAndIncidentAngle" ) {
				if ( mIncidentTuple.depositEnergy[2] > -.5 ) {
					hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[2]);
				}
			}
		}
	}
}

void TGeantAnalysis::saveFile(std::filesystem::path outputFilePath) {
	TString outputFileName = std::string(outputFilePath);
	TFile outputFile(outputFileName, "RECREATE");
	for ( const auto& [key, hist] : m1DHistograms ) {
		hist->Write(key.data());
	}
	for ( const auto& [key, hist] : m2DHistograms ) {
		hist->Write(key.data());
	}
	outputFile.Close();
}

