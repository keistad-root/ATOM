#include "TGeantAnalysis.h"

#include "TH1D.h"
#include "TH2D.h"
#include<cmath>

TGeantAnalysis::TGeantAnalysis(const CppConfigFile& config) : mConfig(config) {
	isPrimary = mConfig.getConfig("FILE").hasKey("INPUT_PRIMARY_FILE");
	isIncident = mConfig.getConfig("FILE").hasKey("INPUT_INCIDENT_FILE");
	isSecondary = mConfig.getConfig("FILE").hasKey("INPUT_SECONDARY_FILE");
	if ( isPrimary ) {
		TString primaryFilePath = mConfig.getConfig("FILE").find("INPUT_PRIMARY_FILE");
		mPrimaryFile = new TFile(primaryFilePath, "READ");
		mPrimaryTree = static_cast<TTree*>(mPrimaryFile->Get("PrimaryAnalysis"));
		readPrimaryFile();
	}
	if ( isIncident ) {
		TString incidentFilePath = mConfig.getConfig("FILE").find("INPUT_INCIDENT_FILE");
		mIncidentFile = new TFile(incidentFilePath, "READ");
		mIncidentTree = static_cast<TTree*>(mIncidentFile->Get("IncidentAnalysis"));
		readIncidentFile();
	}
	if ( isSecondary ) {
		TString secondaryFilePath = mConfig.getConfig("FILE").find("INPUT_SECONDARY_FILE");
		mSecondaryFile = new TFile(secondaryFilePath, "READ");
		mSecondaryTree = static_cast<TTree*>(mSecondaryFile->Get("SecondaryAnalysis"));
		readSecondaryFile();
	}
}

TGeantAnalysis::~TGeantAnalysis() { }

void TGeantAnalysis::readPrimaryFile() {
	mPrimaryTree->SetBranchAddress("eventID", &mPrimaryTuple.eventID);
	mPrimaryTree->SetBranchAddress("x", &mPrimaryTuple.position[0]);
	mPrimaryTree->SetBranchAddress("y", &mPrimaryTuple.position[1]);
	mPrimaryTree->SetBranchAddress("z", &mPrimaryTuple.position[2]);
	mPrimaryTree->SetBranchAddress("px", &mPrimaryTuple.momentum[0]);
	mPrimaryTree->SetBranchAddress("py", &mPrimaryTuple.momentum[1]);
	mPrimaryTree->SetBranchAddress("pz", &mPrimaryTuple.momentum[2]);
	mPrimaryTree->SetBranchAddress("kineticEnergy", &mPrimaryTuple.kineticEnergy);
}

void TGeantAnalysis::readIncidentFile() {
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

void TGeantAnalysis::readSecondaryFile() {
	mSecondaryTree->SetBranchAddress("eventID", &mSecondaryTuple.eventID);
	mSecondaryTree->SetBranchAddress("trackID", &mSecondaryTuple.trackID);
	mSecondaryTree->SetBranchAddress("parentID", &mSecondaryTuple.parentID);
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

void TGeantAnalysis::setHistograms() {
	std::vector<CppConfigDictionary> configList = mConfig.getConfigList();
	for ( const CppConfigDictionary& config : configList ) {
		std::string key = config.hasKey("NAME") ? config.find("NAME") : "";
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

void TGeantAnalysis::readTree() {
	if ( isPrimary ) {
		// readPrimaryTree();
	}
	if ( isIncident ) {
		readIncidentTree();
	}
	if ( isSecondary ) {
		readSecondaryTree();
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
			position.shrink_to_fit();
		}
		mIncidentSet.push_back(mIncidentTuple);
		// }
	}

	// mEntry[0] = m1DHistograms["ElectronDepositEnergyMetal"]->GetEffectiveEntries();
	// mEntry[1] = m1DHistograms["AlphaDepositEnergyMetal"]->GetEffectiveEntries();
	// mEntry[2] = m1DHistograms["AlphaDepositEnergyEpitaxial"]->GetEffectiveEntries();
	mEntry[3] = nDouble;
}

int TGeantAnalysis::getNDouble(std::vector<std::pair<Double_t, Double_t>> position) {
	Int_t num = 0;
	Int_t nPosition = position.size();
	for ( Int_t i = 0; i < nPosition; i++ ) {
		for ( Int_t j = i + 1; j < nPosition; j++ ) {
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
	for ( auto& [key, hist] : m1DHistograms ) {
		Double_t momentum = TMath::Sqrt(mPrimaryTuple.momentum[0] * mPrimaryTuple.momentum[0] + mPrimaryTuple.momentum[1] * mPrimaryTuple.momentum[1] + mPrimaryTuple.momentum[2] * mPrimaryTuple.momentum[2]);
		Double_t theta = TMath::ACos(mPrimaryTuple.momentum[2] / momentum) * 180. / TMath::Pi();
		Double_t phi = TMath::ATan2(mPrimaryTuple.momentum[1], mPrimaryTuple.momentum[0]) * 180. / TMath::Pi();
		if ( key == "SourceZ" ) {
			Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
			hist->Fill(mPrimaryTuple.position[2] / multiple);
		}
		if ( key == "SourceKineticEnergy" ) {
			Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
			hist->Fill(mPrimaryTuple.kineticEnergy / multiple);
		}
		if ( key == "SourceTheta" ) {
			hist->Fill(180 - theta);
		}
		if ( key == "SourcePhi" ) {
			hist->Fill(phi);
		}
	}
	for ( auto& [key, hist] : m2DHistograms ) {
		if ( key == "SourceXY" ) {
			Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
			hist->Fill(mPrimaryTuple.position[0] / multiple, mPrimaryTuple.position[1] / multiple);
		}
	}
}

void TGeantAnalysis::fillIncidentHistograms() {
	Double_t incidentMomentum = TMath::Sqrt(mIncidentTuple.momentum[0] * mIncidentTuple.momentum[0] + mIncidentTuple.momentum[1] * mIncidentTuple.momentum[1] + mIncidentTuple.momentum[2] * mIncidentTuple.momentum[2]);
	Double_t incidentTheta = TMath::ACos(mIncidentTuple.momentum[2] / incidentMomentum) * 180. / TMath::Pi();
	Double_t depositEnergy = 0.;
	bool isDeposit = false;
	if ( mIncidentTuple.depositEnergy[0] > .000001 ) {
		depositEnergy += mIncidentTuple.depositEnergy[0];
		isDeposit = true;
	}
	if ( mIncidentTuple.depositEnergy[1] > .000001 ) {
		depositEnergy += mIncidentTuple.depositEnergy[1];
		isDeposit = true;
	}
	if ( mIncidentTuple.depositEnergy[2] > .000001 ) {
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
			Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
			hist->Fill(mIncidentTuple.position[2] / multiple);
		}
		if ( key == "IncidentAngle" ) {
			hist->Fill(180 - incidentTheta);
		}
		if ( key == "IncidentKineticEnergy" ) {
			Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
			hist->Fill(mIncidentTuple.kineticEnergy / multiple);
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
					Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
					hist->Fill(depositEnergy / multiple);
				}
			}
		}
		if ( key == "DepositEnergyMetal" ) {
			if ( mIncidentTuple.depositEnergy[0] > .000001 ) {
				Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
				hist->Fill(mIncidentTuple.depositEnergy[0] / multiple);
			}
		}
		if ( key == "DepositEnergyEpitaxial" ) {
			if ( mIncidentTuple.depositEnergy[1] > .000001 ) {
				Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
				hist->Fill(mIncidentTuple.depositEnergy[1] / multiple);
			}
		}
		if ( key == "DepositEnergySubstrate" ) {
			if ( mIncidentTuple.depositEnergy[2] > .000001 ) {
				Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
				hist->Fill(mIncidentTuple.depositEnergy[2] / multiple);
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
				Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
				hist->Fill(mIncidentTuple.kineticEnergy / multiple);
			}
			if ( key == "AlphaIncidentGlobalTime" ) {
				hist->Fill(mIncidentTuple.globalTime);
			}
			if ( key == "AlphaIncidentLocalTime" ) {
				hist->Fill(mIncidentTuple.localTime);
			}
			if ( key == "AlphaDepositEnergyTotal" ) {
				if ( isDeposit ) {
					Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
					hist->Fill(depositEnergy / multiple);
				}
			}
			if ( key == "AlphaDepositEnergyMetal" ) {
				if ( mIncidentTuple.depositEnergy[0] > .000001 ) {
					Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
					hist->Fill(mIncidentTuple.depositEnergy[0] / multiple);
				}
			}
			if ( key == "AlphaDepositEnergyEpitaxial" ) {
				if ( mIncidentTuple.depositEnergy[1] > .000001 ) {
					Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
					hist->Fill(mIncidentTuple.depositEnergy[1] / multiple);
				}
			}
			if ( key == "AlphaDepositEnergySubstrate" ) {
				if ( mIncidentTuple.depositEnergy[2] > .000001 ) {
					Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
					hist->Fill(mIncidentTuple.depositEnergy[2] / multiple);
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
			Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
			hist->Fill(mIncidentTuple.position[0] / multiple, mIncidentTuple.position[1] / multiple);
		}
		if ( key == "CorrelationDepositEnergyTotalAndIncidentAngle" ) {
			if ( isDeposit ) {
				Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
				hist->Fill(180 - incidentTheta, depositEnergy / multiple);
			}
		}
		if ( key == "CorrelationDepositEnergyMetalAndIncidentAngle" ) {
			if ( mIncidentTuple.depositEnergy[0] > .000001 ) {
				Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
				hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[0] / multiple);
			}
		}
		if ( key == "CorrelationDepositEnergyEpitaxialAndIncidentAngle" ) {
			if ( mIncidentTuple.depositEnergy[1] > .000001 ) {
				Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
				hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[1] / multiple);
			}
		}
		if ( key == "CorrelationDepositEnergySubstrateAndIncidentAngle" ) {
			if ( mIncidentTuple.depositEnergy[2] > .000001 ) {
				Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
				hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[2] / multiple);
			}
		}
		if ( key == "CorrelationIncidentAngleAndStopPosition" ) {
			if ( mIncidentTuple.finalVolumeID == VOLUME::ALPIDEMetal || mIncidentTuple.finalVolumeID == VOLUME::ALPIDEEpitaxial || mIncidentTuple.finalVolumeID == VOLUME::ALPIDESubstrate ) {
				Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
				hist->Fill(180 - incidentTheta, -mIncidentTuple.finalPosition[2] / multiple);
			}
		}
		if ( mIncidentTuple.particleID == PARTICLE::alpha ) {
			if ( key == "AlphaIncidentXY" ) {
				Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
				hist->Fill(mIncidentTuple.position[0] / multiple, mIncidentTuple.position[1] / multiple);
			}
			if ( key == "AlphaCorrelationDepositEnergyTotalAndIncidentAngle" ) {
				if ( isDeposit ) {
					Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
					hist->Fill(180 - incidentTheta, depositEnergy / multiple);
				}
			}
			if ( key == "AlphaCorrelationDepositEnergyMetalAndIncidentAngle" ) {
				if ( mIncidentTuple.depositEnergy[0] > .000001 ) {
					Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
					hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[0] / multiple);
				}
			}
			if ( key == "AlphaCorrelationDepositEnergyEpitaxialAndIncidentAngle" ) {
				if ( mIncidentTuple.depositEnergy[1] > .000001 ) {
					Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
					hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[1] / multiple);
				}
			}
			if ( key == "AlphaCorrelationDepositEnergySubstrateAndIncidentAngle" ) {
				if ( mIncidentTuple.depositEnergy[2] > .000001 ) {
					Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
					hist->Fill(180 - incidentTheta, mIncidentTuple.depositEnergy[2] / multiple);
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
	TIncidentAnalysisTuple incidentTuple;
	static int nIncident = 0;
	for ( int iIncident = nIncident; iIncident < mIncidentSet.size(); iIncident++ ) {
		if ( mIncidentSet[iIncident].eventID == mSecondaryTuple.eventID && mIncidentSet[iIncident].trackID == mSecondaryTuple.parentID ) {
			incidentTuple = mIncidentSet[iIncident];
			if ( incidentTuple.particleID == PARTICLE::unknown ) std::cout << "Unknown particle" << std::endl;
			nIncident = iIncident;
			break;
		}
	}

	for ( const auto& [key, hist] : m1DHistograms ) {
		Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
		if ( key == "SecondaryParticleInALPIDE" ) {
			hist->Fill(mSecondaryTuple.particleID);
		}
		if ( key == "SecondaryMeanFreePathInALPIDE" ) {
			Double_t meanFreePath = TMath::Sqrt(TMath::Power(mSecondaryTuple.finalPosition[0] - mSecondaryTuple.initialPosition[0], 2) + TMath::Power(mSecondaryTuple.finalPosition[1] - mSecondaryTuple.initialPosition[1], 2) + TMath::Power(mSecondaryTuple.finalPosition[2] - mSecondaryTuple.initialPosition[2], 2));
			hist->Fill(meanFreePath / multiple);
		}
		if ( key == "SecondaryMotherParticleInALPIDE" ) {
			hist->Fill(incidentTuple.particleID);
		}
		if ( key == "SecondaryInALPIDEVolume" ) {
			hist->Fill(mSecondaryTuple.initialVolumeID);
		}
		if ( key == "SecondaryInALPIDEDecayVolume" ) {
			hist->Fill(mSecondaryTuple.finalVolumeID);
		}
		if ( mSecondaryTuple.particleID == PARTICLE::electron ) {
			if ( key == "SecondaryElectronInitialZ" ) {
				hist->Fill(-mSecondaryTuple.initialPosition[2] / multiple);
			}
		}
	}

	for ( const auto& [key, hist] : m2DHistograms ) {
		if ( key == "MotherDoughterCorrelationInALPIDE" ) {
			hist->Fill(incidentTuple.particleID, mSecondaryTuple.particleID);
		}
	}
}
void TGeantAnalysis::saveFile() {
	TString outputFileName = std::string(mConfig.getConfig("FILE").find("OUTPUT_FILE"));
	TFile outputFile(outputFileName, "RECREATE");
	for ( const auto& [key, hist] : m1DHistograms ) {
		hist->Write(key.data());
	}
	for ( const auto& [key, hist] : m2DHistograms ) {
		hist->Write(key.data());
	}
	outputFile.Close();
}