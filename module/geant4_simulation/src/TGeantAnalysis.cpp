#include "TGeantAnalysis.h"

#include "TH1D.h"
#include "TH2D.h"
#include<cmath>

TGeantAnalysis::TGeantAnalysis(const CppConfigFile& config) : mConfig(config) {
	if ( mConfig.getConfig("FILE").hasKey("INPUT_PRIMARY_FILE") ) {
		TString primaryFilePath = mConfig.getConfig("FILE").find("INPUT_PRIMARY_FILE");
		mPrimaryFile = new TFile(primaryFilePath, "READ");
		mPrimaryTree = static_cast<TTree*>(mPrimaryFile->Get("PrimaryAnalysis"));
	}
	if ( mConfig.getConfig("FILE").hasKey("INPUT_INCIDENT_FILE") ) {
		TString incidentFilePath = mConfig.getConfig("FILE").find("INPUT_INCIDENT_FILE");
		mIncidentFile = new TFile(incidentFilePath, "READ");
		mIncidentTree = static_cast<TTree*>(mIncidentFile->Get("IncidentAnalysis"));
	}

	TString secondaryFilePath = mConfig.getConfig("FILE").find("INPUT_SECONDARY_FILE");
	mSecondaryFile = new TFile(secondaryFilePath, "READ");
	mSecondaryTree = static_cast<TTree*>(mSecondaryFile->Get("SecondaryAnalysis"));
}

TGeantAnalysis::~TGeantAnalysis() { }

void TGeantAnalysis::readIncidentFile(std::filesystem::path inputFilePath) {
	TString inputFileName = std::string(inputFilePath);
	mIncidentFile = new TFile(inputFileName, "READ");
	mIncidentTree = static_cast<TTree*>(mIncidentFile->Get("IncidentAnalysis"));

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

void TGeantAnalysis::readPrimaryFile(std::filesystem::path inputFilePath) {
	TString inputFileName = std::string(inputFilePath);
	mPrimaryFile = new TFile(inputFileName, "READ");
	mPrimaryTree = static_cast<TTree*>(mPrimaryFile->Get("PrimaryAnalysis"));

	mPrimaryTree->SetBranchAddress("eventID", &mPrimaryTuple.eventID);
	mPrimaryTree->SetBranchAddress("x", &mPrimaryTuple.position[0]);
	mPrimaryTree->SetBranchAddress("y", &mPrimaryTuple.position[1]);
	mPrimaryTree->SetBranchAddress("z", &mPrimaryTuple.position[2]);
	mPrimaryTree->SetBranchAddress("px", &mPrimaryTuple.momentum[0]);
	mPrimaryTree->SetBranchAddress("py", &mPrimaryTuple.momentum[1]);
	mPrimaryTree->SetBranchAddress("pz", &mPrimaryTuple.momentum[2]);
	mPrimaryTree->SetBranchAddress("kineticEnergy", &mPrimaryTuple.kineticEnergy);
}

void TGeantAnalysis::readSecondaryFile(std::filesystem::path inputFilePath) {
	TString inputFileName = std::string(inputFilePath);
	mSecondaryFile = new TFile(inputFileName, "READ");
	mSecondaryTree = static_cast<TTree*>(mSecondaryFile->Get("SecondaryAnalysis"));

	mSecondaryTree->SetBranchAddress("eventID", &mSecondaryTuple.eventID);
	mSecondaryTree->SetBranchAddress("trackID", &mSecondaryTuple.trackID);
	mSecondaryTree->SetBranchAddress("particleID", &mSecondaryTuple.particleID);
	mSecondaryTree->SetBranchAddress("initialVolumeID", &mSecondaryTuple.initialVolumeID);
	mSecondaryTree->SetBranchAddress("initX", &mSecondaryTuple.initialPosition[0]);
	mSecondaryTree->SetBranchAddress("initY", &mSecondaryTuple.initialPosition[1]);
	mSecondaryTree->SetBranchAddress("initZ", &mSecondaryTuple.initialPosition[2]);
	mSecondaryTree->SetBranchAddress("initPX", &mSecondaryTuple.initialMomentum[0]);
	mSecondaryTree->SetBranchAddress("initPY", &mSecondaryTuple.initialMomentum[1]);
	mSecondaryTree->SetBranchAddress("initPZ", &mSecondaryTuple.initialMomentum[2]);
	mSecondaryTree->SetBranchAddress("initKineticEnergy", &mSecondaryTuple.initialKineticEnergy);
	mSecondaryTree->SetBranchAddress("finalVolumeID", &mSecondaryTuple.finalVolumeID);
	mSecondaryTree->SetBranchAddress("finalX", &mSecondaryTuple.finalPosition[0]);
	mSecondaryTree->SetBranchAddress("finalY", &mSecondaryTuple.finalPosition[1]);
	mSecondaryTree->SetBranchAddress("finalZ", &mSecondaryTuple.finalPosition[2]);
	mSecondaryTree->SetBranchAddress("finalPX", &mSecondaryTuple.finalMomentum[0]);
	mSecondaryTree->SetBranchAddress("finalPY", &mSecondaryTuple.finalMomentum[1]);
	mSecondaryTree->SetBranchAddress("finalPZ", &mSecondaryTuple.finalMomentum[2]);
	mSecondaryTree->SetBranchAddress("finalKineticEnergy", &mSecondaryTuple.finalKineticEnergy);
}

void TGeantAnalysis::setHistograms(const std::vector<CppConfigDictionary>& configList) {
	for ( const CppConfigDictionary& config : configList ) {
		std::string_view key = config.getConfigName();
		if ( config.hasKey("type") && config.find("type") == "1H" ) {
			TH1D* hist = TPlotter::init1DHist(config);
			m1DHistograms.insert_or_assign(key, hist);
		}
		if ( config.hasKey("type") && config.find("type") == "2H" ) {
			TH2D* hist = TPlotter::init2DHist(config);
			m2DHistograms.insert_or_assign(key, hist);
		}
	}
}

void TGeantAnalysis::readPrimaryTree() {
	Int_t nEntries = mPrimaryTree->GetEntries();
	ProgressBar progressBar(static_cast<int>(nEntries));
	for ( Int_t i = 0; i < nEntries; i++ ) {
		progressBar.printProgress();
		mPrimaryTree->GetEntry(i);
		fillPrimaryHistograms();
	}
}

void TGeantAnalysis::readSecondaryTree() {
	Int_t nEntries = mSecondaryTree->GetEntries();
	ProgressBar progressBar(static_cast<int>(nEntries));
	for ( Int_t i = 0; i < nEntries; i++ ) {
		progressBar.printProgress();
		mSecondaryTree->GetEntry(i);
		fillSecondaryHistograms();
	}
}

void TGeantAnalysis::readIncidentTree() {
	Int_t nEntries = mIncidentTree->GetEntries();

	std::vector<std::pair<Double_t, Double_t>> position;
	Int_t preTimeStamp = 0;
	ProgressBar progressBar(static_cast<int>(nEntries));
	Int_t nDouble = 0;
	for ( Int_t i = 0; i < nEntries; i++ ) {
		progressBar.printProgress();
		mIncidentTree->GetEntry(i);
		// if ( std::abs(mIncidentTuple.position[0]) < 4.35 && std::abs(mIncidentTuple.position[1]) < 0.27 ) { // 300 pixel * 20 pixel
		fillIncidentHistograms();
		if ( mIncidentTuple.particleID == PARTICLE::alpha ) {
			position.push_back({mIncidentTuple.position[0], mIncidentTuple.position[1]});
		}
		if ( preTimeStamp != mIncidentTuple.eventID / 107 ) {
			preTimeStamp = mIncidentTuple.eventID / 107;
			nDouble += getNDouble(position);
			position.clear();
		}
		// }
	}

	mEntry[0] = m1DHistograms["ElectronDepositEnergyMetal"]->GetEffectiveEntries();
	mEntry[1] = m1DHistograms["AlphaDepositEnergyMetal"]->GetEffectiveEntries();
	mEntry[2] = m1DHistograms["AlphaDepositEnergyEpitaxial"]->GetEffectiveEntries();
	mEntry[3] = nDouble;
}

int TGeantAnalysis::getNDouble(std::vector<std::pair<Double_t, Double_t>> position) {
	Int_t num = 0;
	Int_t nParticipant = 0;
	for ( Int_t i = 0; i < position.size(); i++ ) {
		for ( Int_t j = i + 1; j < position.size(); j++ ) {
			Double_t distance = TMath::Sqrt(TMath::Power(position[i].first - position[j].first, 2) + TMath::Power(position[i].second - position[j].second, 2));
			m1DHistograms["DistanceBetweenIncidentAlpha"]->Fill(distance * 1000);
			if ( distance > 0.048 && distance < 0.154 ) {
				num++;
			}
		}
	}
	return num;
}

void TGeantAnalysis::fillPrimaryHistograms() {
	for ( const auto& [key, hist] : m1DHistograms ) {
		if ( key == "SourceZ" ) {
			hist->Fill(mPrimaryTuple.position[2]);
		}
		if ( key == "SourceTheta" ) {
			Double_t momentum = TMath::Sqrt(mPrimaryTuple.momentum[0] * mPrimaryTuple.momentum[0] + mPrimaryTuple.momentum[1] * mPrimaryTuple.momentum[1] + mPrimaryTuple.momentum[2] * mPrimaryTuple.momentum[2]);
			Double_t theta = TMath::ACos(mPrimaryTuple.momentum[2] / momentum) * 180. / TMath::Pi();
			hist->Fill(180 - theta);
		}
		if ( key == "SourcePhi" ) {
			Double_t phi = TMath::ATan2(mPrimaryTuple.momentum[1], mPrimaryTuple.momentum[0]) * 180. / TMath::Pi();
			hist->Fill(phi);
		}
		if ( key == "SourceKineticEnergy" ) {
			hist->Fill(mPrimaryTuple.kineticEnergy);
		}
	}
	for ( const auto& [key, hist] : m2DHistograms ) {
		if ( key == "SourceXY" ) {
			hist->Fill(mPrimaryTuple.position[0], mPrimaryTuple.position[1]);
		}
	}
}

void TGeantAnalysis::fillIncidentHistograms() {
	Double_t incidentMomentum = TMath::Sqrt(mIncidentTuple.momentum[0] * mIncidentTuple.momentum[0] + mIncidentTuple.momentum[1] * mIncidentTuple.momentum[1] + mIncidentTuple.momentum[2] * mIncidentTuple.momentum[2]);
	Double_t incidentTheta = TMath::ACos(mIncidentTuple.momentum[2] / incidentMomentum) * 180. / TMath::Pi();
	if ( incidentTheta < 90 ) {
		std::cout << mIncidentTuple.initialVolumeID << " " << mIncidentTuple.initialPosition[2] << " " << mIncidentTuple.position[2] << " " << mIncidentTuple.finalPosition[2] << " " << incidentTheta << std::endl;
	}
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
		if ( key == "IncidentResidualY" ) {
			double n = (mIncidentTuple.position[1] * 1000 - 13.44) / 26.88 + 1;
			double residual = mIncidentTuple.position[1] * 1000 - (13.44 + (std::round(n) - 1) * 26.88);
			hist->Fill(residual);
		}
		if ( key == "IncidentResidualX" ) {
			double n = (mIncidentTuple.position[0] * 1000 - 14.62) / 29.24 + 1;
			double residual = mIncidentTuple.position[0] * 1000 - (14.62 + (std::round(n) - 1) * 29.24);
			hist->Fill(residual);
		}
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
			hist->Fill(mIncidentTuple.position[2] * 1000);
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
		if ( key == "IncidentVolume" ) {
			hist->Fill(mIncidentTuple.initialVolumeID);
		}
		if ( key == "DepositEnergyTotal" ) {
			if ( isDeposit ) {
				if ( depositEnergy > .000001 ) {
					hist->Fill(depositEnergy * 1000);
				}
			}
		}
		if ( key == "DepositEnergyMetal" ) {
			if ( mIncidentTuple.depositEnergy[0] > .000001 ) {
				hist->Fill(mIncidentTuple.depositEnergy[0] * 1000);
			}
		}
		if ( key == "DepositEnergyEpitaxial" ) {
			if ( mIncidentTuple.depositEnergy[1] > .000001 ) {
				hist->Fill(mIncidentTuple.depositEnergy[1] * 1000);
			}
		}
		if ( key == "DepositEnergySubstrate" ) {
			if ( mIncidentTuple.depositEnergy[2] > .000001 ) {
				hist->Fill(mIncidentTuple.depositEnergy[2] * 1000);
			}
		}
		if ( key == "IncidentGlobalTime" ) {
			hist->Fill(mIncidentTuple.globalTime);
		}
		if ( key == "IncidentLocalTime" ) {
			hist->Fill(mIncidentTuple.localTime);
		}
		if ( mIncidentTuple.particleID == PARTICLE::alpha ) {
			if ( key == "AlphaIncidentResidualY" ) {
				double n = (mIncidentTuple.position[1] * 1000 - 13.44) / 26.88 + 1;
				double residual = mIncidentTuple.position[1] * 1000 - (13.44 + (std::round(n) - 1) * 26.88);
				hist->Fill(residual);
			}
			if ( key == "AlphaIncidentResidualX" ) {
				double n = (mIncidentTuple.position[0] * 1000 - 14.62) / 29.24 + 1;
				double residual = mIncidentTuple.position[0] * 1000 - (14.62 + (std::round(n) - 1) * 29.24);
				hist->Fill(residual);
			}
			if ( key == "AlphaIncidentVolume" ) {
				hist->Fill(mIncidentTuple.initialVolumeID);
			}
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
			if ( key == "ElectronIncidentResidualY" ) {
				double n = (mIncidentTuple.position[1] * 1000 - 13.44) / 26.88 + 1;
				double residual = mIncidentTuple.position[1] * 1000 - (13.44 + (std::round(n) - 1) * 26.88);
				hist->Fill(residual);
			}
			if ( key == "ElectronIncidentResidualX" ) {
				double n = (mIncidentTuple.position[0] * 1000 - 14.62) / 29.24 + 1;
				double residual = mIncidentTuple.position[0] * 1000 - (14.62 + (std::round(n) - 1) * 29.24);
				hist->Fill(residual);
			}
			if ( key == "ElectronIncidentVolume" ) {
				hist->Fill(mIncidentTuple.initialVolumeID);
			}
			if ( key == "ElectronFinalVolume" ) {
				hist->Fill(mIncidentTuple.finalVolumeID);
			}
			if ( key == "ElectronIncidentAngle" ) {
				hist->Fill(180 - incidentTheta);
			}
			if ( key == "ElectronIncidentKineticEnergy" ) {
				hist->Fill(mIncidentTuple.kineticEnergy * 1000.);
			}
			if ( key == "ElectronIncidentGlobalTime" ) {
				hist->Fill(mIncidentTuple.globalTime);
			}
			if ( key == "ElectronIncidentLocalTime" ) {
				hist->Fill(mIncidentTuple.localTime);
			}
			if ( key == "ElectronDepositEnergyTotal" ) {
				if ( isDeposit ) {
					hist->Fill(depositEnergy * 1000);
				}
			}
			if ( key == "ElectronDepositEnergyMetal" ) {
				if ( mIncidentTuple.depositEnergy[0] > -.5 ) {
					hist->Fill(mIncidentTuple.depositEnergy[0] * 1000);
				}
			}
			if ( key == "ElectronDepositEnergyEpitaxial" ) {
				if ( mIncidentTuple.depositEnergy[1] > -.5 ) {
					hist->Fill(mIncidentTuple.depositEnergy[1] * 1000);
				}
			}
			if ( key == "ElectronDepositEnergySubstrate" ) {
				if ( mIncidentTuple.depositEnergy[2] > -.5 ) {
					hist->Fill(mIncidentTuple.depositEnergy[2] * 1000);
				}
			}
		}
		if ( mIncidentTuple.particleID == PARTICLE::gamma1 ) {
			if ( key == "GammaIncidentZ" ) {
				hist->Fill(mIncidentTuple.position[2] * 1000);
			}
			if ( key == "GammaIncidentAngle" ) {
				hist->Fill(180 - incidentTheta);
			}
			if ( key == "GammaIncidentKineticEnergy" ) {
				hist->Fill(mIncidentTuple.kineticEnergy * 1000);
			}
			if ( key == "GammaIncidentVolume" ) {
				hist->Fill(mIncidentTuple.initialVolumeID);
			}
			if ( key == "GammaDepositEnergyTotal" ) {
				if ( isDeposit && depositEnergy > .000001 ) {
					hist->Fill(depositEnergy * 1000000);
				}
			}
			if ( key == "GammaDepositEnergyMetal" ) {
				if ( mIncidentTuple.depositEnergy[0] > .000001 ) {
					hist->Fill(mIncidentTuple.depositEnergy[0] * 1000000);
				}
			}
			if ( key == "GammaDepositEnergyEpitaxial" ) {
				if ( mIncidentTuple.depositEnergy[1] > .000001 ) {
					hist->Fill(mIncidentTuple.depositEnergy[1] * 1000000);
				}
			}
			if ( key == "GammaDepositEnergySubstrate" ) {
				if ( mIncidentTuple.depositEnergy[2] > .000001 ) {
					hist->Fill(mIncidentTuple.depositEnergy[2] * 1000000);
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
				if ( depositEnergy > .000001 ) {
					hist->Fill(180 - incidentTheta, depositEnergy * 1000);
				}
			}
		}
		if ( key == "CorrelationDepositEnergyMetalAndIncidentAngle" ) {
			if ( mIncidentTuple.depositEnergy[0] > .000001 ) {
				hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[0] * 1000);
			}
		}
		if ( key == "CorrelationDepositEnergyEpitaxialAndIncidentAngle" ) {
			if ( mIncidentTuple.depositEnergy[1] > .000001 ) {
				hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[1] * 1000);
			}
		}
		if ( key == "CorrelationDepositEnergySubstrateAndIncidentAngle" ) {
			if ( mIncidentTuple.depositEnergy[2] > .000001 ) {
				hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[2] * 1000);
			}
		}
		if ( key == "CorrelationIncidentAngleAndStopPosition" ) {
			if ( mIncidentTuple.finalVolumeID == VOLUME::ALPIDEMetal || mIncidentTuple.finalVolumeID == VOLUME::ALPIDEEpitaxial || mIncidentTuple.finalVolumeID == VOLUME::ALPIDESubstrate ) {
				hist->Fill(180 - incidentTheta, -mIncidentTuple.finalPosition[2] * 1000);
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
			if ( key == "ElctronIncidentXYWithElectrode" ) {
				if ( TMath::Abs(mIncidentTuple.position[0]) < 5 * 0.028 && TMath::Abs(mIncidentTuple.position[1]) < 5 * 0.028 ) {
					hist->Fill(mIncidentTuple.position[0], mIncidentTuple.position[1]);
				}
			}
			if ( key == "ElectronCorrelationDepositEnergyTotalAndIncidentAngle" ) {
				if ( isDeposit ) {
					hist->Fill(180 - incidentTheta, depositEnergy * 1000);
				}
			}
			if ( key == "ElectronCorrelationDepositEnergyMetalAndIncidentAngle" ) {
				if ( mIncidentTuple.depositEnergy[0] > -.5 ) {
					hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[0] * 1000);
				}
			}
			if ( key == "ElectronCorrelationDepositEnergyEpitaxialAndIncidentAngle" ) {
				if ( mIncidentTuple.depositEnergy[1] > -.5 ) {
					hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[1] * 1000);
				}
			}
			if ( key == "ElectronCorrelationDepositEnergySubstrateAndIncidentAngle" ) {
				if ( mIncidentTuple.depositEnergy[2] > -.5 ) {
					hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[2] * 1000);
				}
			}
		}
		if ( mIncidentTuple.particleID == PARTICLE::gamma1 ) {
			if ( key == "GammaIncidentXY" ) {
				hist->Fill(mIncidentTuple.position[0], mIncidentTuple.position[1]);
			}
			if ( key == "GammaCorrelationDepositEnergyTotalAndIncidentAngle" ) {
				if ( isDeposit ) {
					if ( depositEnergy > .000001 ) {
						hist->Fill(180 - incidentTheta, depositEnergy * 1000000);
					}
				}
			}
			if ( key == "GammaCorrelationDepositEnergyMetalAndIncidentAngle" ) {
				if ( mIncidentTuple.depositEnergy[0] > .000001 ) {
					hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[0] * 1000000);
				}
			}
			if ( key == "GammaCorrelationDepositEnergyEpitaxialAndIncidentAngle" ) {
				if ( mIncidentTuple.depositEnergy[1] > .000001 ) {
					hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[1] * 1000000);
				}
			}
			if ( key == "GammaCorrelationDepositEnergySubstrateAndIncidentAngle" ) {
				if ( mIncidentTuple.depositEnergy[2] > .000001 ) {
					hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[2] * 1000000);
				}
			}
		}
	}
}

void TGeantAnalysis::fillSecondaryHistograms() {
	if ( mSecondaryTuple.initialVolumeID == VOLUME::ALPIDEMetal || mSecondaryTuple.initialVolumeID == VOLUME::ALPIDEEpitaxial || mSecondaryTuple.initialVolumeID == VOLUME::ALPIDESubstrate ) {
		for ( const auto& [key, hist] : m1DHistograms ) {
			if ( key == "SecondaryParticleInALPIDE" ) {
				hist->Fill(mSecondaryTuple.particleID);
			}
			if ( key == "SecondaryInALPIDEVolume" ) {
				hist->Fill(mSecondaryTuple.initialVolumeID);
			}
			if ( key == "SecondaryInALPIDEDecayVolume" ) {
				hist->Fill(mSecondaryTuple.finalVolumeID);
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