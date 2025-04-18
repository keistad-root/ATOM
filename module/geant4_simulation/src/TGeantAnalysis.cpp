#include "TGeantAnalysis.h"

#include "TH1D.h"
#include "TH2D.h"
#include<cmath>

const Double_t eV = 0.000001;

TGeantAnalysis::TGeantAnalysis(const CppConfigFile& config) : mConfig(config) {
	isPrimary = mConfig.getConfig("FILE").hasKey("INPUT_PRIMARY_FILE");
	isIncident = mConfig.getConfig("FILE").hasKey("INPUT_INCIDENT_FILE");
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
	if ( mConfig.getConfig("FILE").hasKey("ROI") ) {
		isRoi = true;
		std::vector<double> temp = TPlotter::getDoubleSetFromString(mConfig.getConfig("FILE").find("ROI"));
		mRoi = {temp[0], temp[1]};
	}
	mRoiTheta = mConfig.getConfig("FILE").hasKey("ROI_THETA") ? stod(mConfig.getConfig("FILE").find("ROI_THETA")) * TMath::Pi() / 180 : 0;
}

TGeantAnalysis::~TGeantAnalysis() { }

void TGeantAnalysis::readPrimaryFile() {
	mPrimaryTree->SetBranchAddress("eventID", &mPrimaryTuple.eventID);
	mPrimaryTree->SetBranchAddress("position", mPrimaryTuple.position);
	mPrimaryTree->SetBranchAddress("momentum", mPrimaryTuple.momentum);
	mPrimaryTree->SetBranchAddress("kineticEnergy", &mPrimaryTuple.kineticEnergy);
}

void TGeantAnalysis::readIncidentFile() {
	mIncidentTree->SetBranchAddress("eventID", &mIncidentTuple.eventID);
	mIncidentTree->SetBranchAddress("trackID", &mIncidentTuple.trackID);
	mIncidentTree->SetBranchAddress("particleID", &mIncidentTuple.particleID);
	mIncidentTree->SetBranchAddress("initialVolumeID", &mIncidentTuple.initialVolumeID);
	mIncidentTree->SetBranchAddress("initPosition", mIncidentTuple.initialPosition);
	mIncidentTree->SetBranchAddress("initMomentum", mIncidentTuple.initialMomentum);
	mIncidentTree->SetBranchAddress("initKineticEnergy", &mIncidentTuple.initialKineticEnergy);
	mIncidentTree->SetBranchAddress("depositEnergy", mIncidentTuple.depositEnergy);
	mIncidentTree->SetBranchAddress("incidentPosition", mIncidentTuple.position);
	mIncidentTree->SetBranchAddress("incidentMomentum", mIncidentTuple.momentum);
	mIncidentTree->SetBranchAddress("incidentKineticEnergy", &mIncidentTuple.kineticEnergy);
	mIncidentTree->SetBranchAddress("incidentGlobalTime", &mIncidentTuple.globalTime);
	mIncidentTree->SetBranchAddress("incidentLocalTime", &mIncidentTuple.localTime);
	mIncidentTree->SetBranchAddress("finalVolumeID", &mIncidentTuple.finalVolumeID);
	mIncidentTree->SetBranchAddress("finalPosition", mIncidentTuple.finalPosition);
	mIncidentTree->SetBranchAddress("finalMomentum", mIncidentTuple.finalMomentum);
	mIncidentTree->SetBranchAddress("finalKineticEnergy", &mIncidentTuple.finalKineticEnergy);
}

void TGeantAnalysis::setHistograms() {
	std::vector<CppConfigDictionary> configList = mConfig.getConfigList();
	for ( const CppConfigDictionary& config : configList ) {
		std::string key = std::string(config.getConfigName());
		if ( config.hasKey("TYPE") && config.find("TYPE") == "1H" ) {
			TH1D* hist = TPlotter::init1DHist(config);
			m1DHistograms.insert_or_assign(key, hist);
		}
		if ( config.hasKey("TYPE") && config.find("TYPE") == "2H" ) {
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
}

void TGeantAnalysis::readPrimaryTree() {
	Int_t nEntries = mPrimaryTree->GetEntries();
	ProgressBar* pbar = new ProgressBar(static_cast<int>(nEntries));
	for ( Int_t i = 0; i < nEntries; i++ ) {
		pbar->countUp();
		mPrimaryTree->GetEntry(i);

		Double_t momentum = TMath::Sqrt(mPrimaryTuple.momentum[0] * mPrimaryTuple.momentum[0] + mPrimaryTuple.momentum[1] * mPrimaryTuple.momentum[1] + mPrimaryTuple.momentum[2] * mPrimaryTuple.momentum[2]);
		Double_t theta = TMath::ACos(mPrimaryTuple.momentum[2] / momentum) * 180. / TMath::Pi();
		Double_t phi = TMath::ATan2(mPrimaryTuple.momentum[1], mPrimaryTuple.momentum[0]) * 180. / TMath::Pi();

		Double_t multiple = 1.;

		m2DHistograms["SRC_XY"]->Fill(mPrimaryTuple.position[0], mPrimaryTuple.position[1]);
		m1DHistograms["SRC_Z"]->Fill(mPrimaryTuple.position[2]);
		multiple = mConfig.getConfig("SRC_KE").hasKey("MULTIPLE") ? stod(mConfig.getConfig("SRC_KE").find("MULTIPLE")) : 1.;
		m1DHistograms["SRC_KE"]->Fill(mPrimaryTuple.kineticEnergy * multiple);
		m1DHistograms["SRC_THETA"]->Fill(180 - theta);
		m1DHistograms["SRC_PHI"]->Fill(phi);
	}
	delete pbar;
}

bool TGeantAnalysis::isFromOutside() {
	if ( mIncidentTuple.initialVolumeID == VOLUME::Collimator || mIncidentTuple.initialVolumeID == VOLUME::Screen || mIncidentTuple.initialVolumeID == VOLUME::World ) {
		return true;
	}
	return false;
}

bool TGeantAnalysis::isFromALPIDE() {
	if ( mIncidentTuple.initialVolumeID == VOLUME::ALPIDESubstrate || mIncidentTuple.initialVolumeID == VOLUME::ALPIDEEpitaxial || mIncidentTuple.initialVolumeID == VOLUME::ALPIDEMetal ) {
		return true;
	}
	return false;
}

void TGeantAnalysis::fill1DHistograms(std::string_view name, double value) {
	if ( m1DHistograms.count(std::string(name)) ) {
		Double_t multiple = mConfig.getConfig(std::string(name)).hasKey("MULTIPLE") ? stod(mConfig.getConfig(std::string(name)).find("MULTIPLE")) : 1.;
		m1DHistograms[std::string(name)]->Fill(value * multiple);
	}
}

void TGeantAnalysis::fill2DHistograms(std::string_view name, double x, double y) {
	if ( m2DHistograms.count(std::string(name)) ) {
		Double_t multiple = mConfig.getConfig(std::string(name)).hasKey("MULTIPLE") ? stod(mConfig.getConfig(std::string(name)).find("MULTIPLE")) : 1.;
		m2DHistograms[std::string(name)]->Fill(x * multiple, y * multiple);
	}
}

bool TGeantAnalysis::isInsideRegion(double x, double y) {
	double cosTheta = std::cos(mRoiTheta);
	double sinTheta = std::sin(mRoiTheta);

	std::pair<double, double> p1 = {-mRoi[0] * cosTheta - mRoi[1] * sinTheta, -mRoi[0] * sinTheta + mRoi[1] * cosTheta};
	std::pair<double, double> p2 = {mRoi[0] * cosTheta - mRoi[1] * sinTheta, mRoi[0] * sinTheta + mRoi[1] * cosTheta};
	std::pair<double, double> p3 = {mRoi[0] * cosTheta + mRoi[1] * sinTheta, mRoi[0] * sinTheta - mRoi[1] * cosTheta};
	std::pair<double, double> p4 = {-mRoi[0] * cosTheta + mRoi[1] * sinTheta, -mRoi[0] * sinTheta - mRoi[1] * cosTheta};

	std::pair<double, double> v1 = {p2.first - p1.first, p2.second - p1.second};
	std::pair<double, double> v2 = {p3.first - p2.first, p3.second - p2.second};
	std::pair<double, double> v3 = {p4.first - p3.first, p4.second - p3.second};
	std::pair<double, double> v4 = {p1.first - p4.first, p1.second - p4.second};

	std::pair<double, double> vp1 = {x - p1.first, y - p1.second};
	std::pair<double, double> vp2 = {x - p2.first, y - p2.second};
	std::pair<double, double> vp3 = {x - p3.first, y - p3.second};
	std::pair<double, double> vp4 = {x - p4.first, y - p4.second};

	double cross1 = v1.first * vp1.second - v1.second * vp1.first;
	double cross2 = v2.first * vp2.second - v2.second * vp2.first;
	double cross3 = v3.first * vp3.second - v3.second * vp3.first;
	double cross4 = v4.first * vp4.second - v4.second * vp4.first;

	if ( cross1 > 0 && cross2 > 0 && cross3 > 0 && cross4 > 0 ) {
		return true;
	} else if ( cross1 < 0 && cross2 < 0 && cross3 < 0 && cross4 < 0 ) {
		return true;
	} else {
		return false;
	}
}

void TGeantAnalysis::readIncidentTree() {
	Int_t nEntries = mIncidentTree->GetEntries();

	std::vector<std::pair<Double_t, Double_t>> position;
	Int_t preTimeStamp = 0;

	TEventInformation eventInfo;
	ProgressBar* pbar = new ProgressBar(static_cast<int>(nEntries));
	Int_t nDouble = 0;
	for ( Int_t i = 0; i < nEntries; i++ ) {
		pbar->countUp();
		mIncidentTree->GetEntry(i);
		if ( isFromOutside() ) {
			if ( !isRoi || isInsideRegion(eventInfo.incidentPosition[0], eventInfo.incidentPosition[1]) ) {
				if ( eventInfo.incidentMomentum > eV ) {
					position.push_back({eventInfo.incidentPosition[0], eventInfo.incidentPosition[1]});
					fill2DHistograms("IN_XY", eventInfo.incidentPosition[0], eventInfo.incidentPosition[1]);
					fill1DHistograms("IN_Z", eventInfo.incidentPosition[2]);
					fill1DHistograms("IN_ANG", eventInfo.incidentAngle);
					fill1DHistograms("IN_KE", eventInfo.incidentKineticEnergy);
					fill1DHistograms("IN_PA", eventInfo.particleID);
					fill1DHistograms("IN_VOL", eventInfo.initialVolumeID);
					if ( eventInfo.totalDepositEnergy[0] > eV ) {
						fill1DHistograms("IN_DE_TOT", eventInfo.totalDepositEnergy[0]);
						fill2DHistograms("IN_DE_TOT_N_IN_ANG", eventInfo.incidentAngle, eventInfo.totalDepositEnergy[0]);
					}
					if ( eventInfo.totalDepositEnergy[1] > eV ) {
						fill1DHistograms("IN_DE_MET", eventInfo.totalDepositEnergy[1]);
						fill2DHistograms("IN_DE_MET_N_IN_ANG", eventInfo.incidentAngle, eventInfo.totalDepositEnergy[1]);
					}
					if ( eventInfo.totalDepositEnergy[2] > eV ) {
						fill1DHistograms("IN_DE_EPI", eventInfo.totalDepositEnergy[2]);
						fill2DHistograms("IN_DE_EPI_N_IN_ANG", eventInfo.incidentAngle, eventInfo.totalDepositEnergy[2]);
					}
					if ( eventInfo.totalDepositEnergy[3] > eV ) {
						fill1DHistograms("IN_DE_SUB", eventInfo.totalDepositEnergy[3]);
						fill2DHistograms("IN_DE_SUB_N_IN_ANG", eventInfo.incidentAngle, eventInfo.totalDepositEnergy[3]);
					}
					if ( eventInfo.particleID == PARTICLE::alpha ) {
						fill2DHistograms("AL_IN_XY", eventInfo.incidentPosition[0], eventInfo.incidentPosition[1]);
						fill1DHistograms("AL_IN_Z", eventInfo.incidentPosition[2]);
						fill1DHistograms("AL_IN_ANG", eventInfo.incidentAngle);
						fill1DHistograms("AL_IN_KE", eventInfo.incidentKineticEnergy);
						fill1DHistograms("AL_IN_VOL", eventInfo.initialVolumeID);
						if ( eventInfo.totalDepositEnergy[0] > eV ) {
							fill1DHistograms("AL_IN_DE_TOT", eventInfo.totalDepositEnergy[0]);
							fill2DHistograms("AL_IN_DE_TOT_N_IN_ANG", eventInfo.incidentAngle, eventInfo.totalDepositEnergy[0]);
						}
						if ( eventInfo.totalDepositEnergy[1] > eV ) {
							fill1DHistograms("AL_IN_DE_MET", eventInfo.totalDepositEnergy[1]);
							fill2DHistograms("AL_IN_DE_MET_N_IN_ANG", eventInfo.incidentAngle, eventInfo.totalDepositEnergy[1]);
						}
						if ( eventInfo.totalDepositEnergy[2] > eV ) {
							fill1DHistograms("AL_IN_DE_EPI", eventInfo.totalDepositEnergy[2]);
							fill2DHistograms("AL_IN_DE_EPI_N_IN_ANG", eventInfo.incidentAngle, eventInfo.totalDepositEnergy[2]);
						}
						if ( eventInfo.totalDepositEnergy[3] > eV ) {
							fill1DHistograms("AL_IN_DE_SUB", eventInfo.totalDepositEnergy[3]);
							fill2DHistograms("AL_IN_DE_SUB_N_IN_ANG", eventInfo.incidentAngle, eventInfo.totalDepositEnergy[3]);
						}
					}
					if ( eventInfo.particleID == PARTICLE::electron ) {
						fill2DHistograms("EL_IN_XY", eventInfo.incidentPosition[0], eventInfo.incidentPosition[1]);
						fill1DHistograms("EL_IN_Z", eventInfo.incidentPosition[2]);
						fill1DHistograms("EL_IN_ANG", eventInfo.incidentAngle);
						fill1DHistograms("EL_IN_KE", eventInfo.incidentKineticEnergy);
						fill1DHistograms("EL_IN_VOL", eventInfo.initialVolumeID);
						if ( eventInfo.totalDepositEnergy[0] > eV ) {
							fill1DHistograms("EL_IN_DE_TOT", eventInfo.totalDepositEnergy[0]);
							fill2DHistograms("EL_IN_DE_TOT_N_IN_ANG", eventInfo.incidentAngle, eventInfo.totalDepositEnergy[0]);
						}
						if ( eventInfo.totalDepositEnergy[1] > eV ) {
							fill1DHistograms("EL_IN_DE_MET", eventInfo.totalDepositEnergy[1]);
							fill2DHistograms("EL_IN_DE_MET_N_IN_ANG", eventInfo.incidentAngle, eventInfo.totalDepositEnergy[1]);
						}
						if ( eventInfo.totalDepositEnergy[2] > eV ) {
							fill1DHistograms("EL_IN_DE_EPI", eventInfo.totalDepositEnergy[2]);
							fill2DHistograms("EL_IN_DE_EPI_N_IN_ANG", eventInfo.incidentAngle, eventInfo.totalDepositEnergy[2]);
						}
						if ( eventInfo.totalDepositEnergy[3] > eV ) {
							fill1DHistograms("EL_IN_DE_SUB", eventInfo.totalDepositEnergy[3]);
							fill2DHistograms("EL_IN_DE_SUB_N_IN_ANG", eventInfo.incidentAngle, eventInfo.totalDepositEnergy[3]);
						}
					}
					if ( eventInfo.particleID == PARTICLE::gamma1 ) {
						fill2DHistograms("GA_IN_XY", eventInfo.incidentPosition[0], eventInfo.incidentPosition[1]);
						fill1DHistograms("GA_IN_Z", eventInfo.incidentPosition[2]);
						fill1DHistograms("GA_IN_ANG", eventInfo.incidentAngle);
						fill1DHistograms("GA_IN_KE", eventInfo.incidentKineticEnergy);
						fill1DHistograms("GA_IN_VOL", eventInfo.initialVolumeID);
						if ( eventInfo.totalDepositEnergy[0] > eV ) {
							fill1DHistograms("GA_IN_DE_TOT", eventInfo.totalDepositEnergy[0]);
							fill2DHistograms("GA_IN_DE_TOT_N_IN_ANG", eventInfo.incidentAngle, eventInfo.totalDepositEnergy[0]);
						}
						if ( eventInfo.totalDepositEnergy[1] > eV ) {
							fill1DHistograms("GA_IN_DE_MET", eventInfo.totalDepositEnergy[1]);
							fill2DHistograms("GA_IN_DE_MET_N_IN_ANG", eventInfo.incidentAngle, eventInfo.totalDepositEnergy[1]);
						}
						if ( eventInfo.totalDepositEnergy[2] > eV ) {
							fill1DHistograms("GA_IN_DE_EPI", eventInfo.totalDepositEnergy[2]);
							fill2DHistograms("GA_IN_DE_EPI_N_IN_ANG", eventInfo.incidentAngle, eventInfo.totalDepositEnergy[2]);
						}
						if ( eventInfo.totalDepositEnergy[3] > eV ) {
							fill1DHistograms("GA_IN_DE_SUB", eventInfo.totalDepositEnergy[3]);
							fill2DHistograms("GA_IN_DE_SUB_N_IN_ANG", eventInfo.incidentAngle, eventInfo.totalDepositEnergy[3]);
						}
					}
				}
			}


			eventInfo.eventID = mIncidentTuple.eventID;
			eventInfo.particleID = mIncidentTuple.particleID;
			eventInfo.initialVolumeID = mIncidentTuple.initialVolumeID;
			eventInfo.incidentPosition[0] = mIncidentTuple.position[0];
			eventInfo.incidentPosition[1] = mIncidentTuple.position[1];
			eventInfo.incidentPosition[2] = mIncidentTuple.position[2];
			eventInfo.incidentPosition[3] = mIncidentTuple.position[3];
			eventInfo.incidentMomentum = TMath::Sqrt(mIncidentTuple.momentum[0] * mIncidentTuple.momentum[0] + mIncidentTuple.momentum[1] * mIncidentTuple.momentum[1] + mIncidentTuple.momentum[2] * mIncidentTuple.momentum[2]);
			eventInfo.incidentKineticEnergy = mIncidentTuple.kineticEnergy;
			eventInfo.incidentAngle = 180 - TMath::ACos(mIncidentTuple.momentum[2] / eventInfo.incidentMomentum) * 180. / TMath::Pi();
			eventInfo.totalDepositEnergy[0] = mIncidentTuple.depositEnergy[0] + mIncidentTuple.depositEnergy[1] + mIncidentTuple.depositEnergy[2];
			eventInfo.totalDepositEnergy[1] = mIncidentTuple.depositEnergy[0];
			eventInfo.totalDepositEnergy[2] = mIncidentTuple.depositEnergy[1];
			eventInfo.totalDepositEnergy[3] = mIncidentTuple.depositEnergy[2];
		} else if ( isFromALPIDE() ) {
			eventInfo.nSecondaryParticle++;
			if ( mIncidentTuple.depositEnergy[0] > eV ) {
				eventInfo.totalDepositEnergy[0] += mIncidentTuple.depositEnergy[0];
				eventInfo.totalDepositEnergy[1] += mIncidentTuple.depositEnergy[0];
			}
			if ( mIncidentTuple.depositEnergy[1] > eV ) {
				eventInfo.totalDepositEnergy[0] += mIncidentTuple.depositEnergy[1];
				eventInfo.totalDepositEnergy[2] += mIncidentTuple.depositEnergy[1];
			}
			if ( mIncidentTuple.depositEnergy[2] > eV ) {
				eventInfo.totalDepositEnergy[0] += mIncidentTuple.depositEnergy[2];
				eventInfo.totalDepositEnergy[3] += mIncidentTuple.depositEnergy[2];
			}
			if ( m1DHistograms.count("SE_DIS_BEG_END_IN_ALPIDE") ) {
				if ( mIncidentTuple.finalVolumeID < 4 ) {
					Double_t distance = TMath::Sqrt(TMath::Power(mIncidentTuple.initialPosition[0] - mIncidentTuple.finalPosition[0], 2) + TMath::Power(mIncidentTuple.initialPosition[1] - mIncidentTuple.finalPosition[1], 2) + TMath::Power(mIncidentTuple.initialPosition[2] - mIncidentTuple.finalPosition[2], 2));
					fill1DHistograms("SE_DIS_BEG_END_IN_ALPIDE", distance);
				}
			}
		}
	}
	if ( !isRoi || isInsideRegion(eventInfo.incidentPosition[0], eventInfo.incidentPosition[1]) ) {
		if ( eventInfo.incidentMomentum > eV ) {
			fill2DHistograms("IN_XY", eventInfo.incidentPosition[0], eventInfo.incidentPosition[1]);
			fill1DHistograms("IN_Z", eventInfo.incidentPosition[2]);
			fill1DHistograms("IN_ANG", eventInfo.incidentAngle);
			fill1DHistograms("IN_KE", eventInfo.incidentKineticEnergy);
			fill1DHistograms("IN_PA", eventInfo.particleID);
			fill1DHistograms("IN_VOL", eventInfo.initialVolumeID);
			if ( eventInfo.totalDepositEnergy[0] > eV ) {
				fill1DHistograms("IN_DE_TOT", eventInfo.totalDepositEnergy[0]);
				fill2DHistograms("IN_DE_TOT_N_IN_ANG", eventInfo.incidentAngle, eventInfo.totalDepositEnergy[0]);
			}
			if ( eventInfo.totalDepositEnergy[1] > eV ) {
				fill1DHistograms("IN_DE_MET", eventInfo.totalDepositEnergy[1]);
				fill2DHistograms("IN_DE_MET_N_IN_ANG", eventInfo.incidentAngle, eventInfo.totalDepositEnergy[1]);
			}
			if ( eventInfo.totalDepositEnergy[2] > eV ) {
				fill1DHistograms("IN_DE_EPI", eventInfo.totalDepositEnergy[2]);
				fill2DHistograms("IN_DE_EPI_N_IN_ANG", eventInfo.incidentAngle, eventInfo.totalDepositEnergy[2]);
			}
			if ( eventInfo.totalDepositEnergy[3] > eV ) {
				fill1DHistograms("IN_DE_SUB", eventInfo.totalDepositEnergy[3]);
				fill2DHistograms("IN_DE_SUB_N_IN_ANG", eventInfo.incidentAngle, eventInfo.totalDepositEnergy[3]);
			}
		}
	}

	delete pbar;
	// mEntry[0] = m1DHistograms["ElectronDepositEnergyMetal"]->GetEffectiveEntries();
	// mEntry[1] = m1DHistograms["AlphaDepositEnergyMetal"]->GetEffectiveEntries();
	// mEntry[2] = m1DHistograms["AlphaDepositEnergyEpitaxial"]->GetEffectiveEntries();
	// mEntry[3] = nDouble;
}

int TGeantAnalysis::getNDouble(std::vector<std::pair<Double_t, Double_t>> position) {
	Int_t num = 0;
	Int_t nPosition = position.size();
	for ( Int_t i = 0; i < nPosition; i++ ) {
		for ( Int_t j = i + 1; j < nPosition; j++ ) {
			Double_t distance = TMath::Sqrt(TMath::Power(position[i].first - position[j].first, 2) + TMath::Power(position[i].second - position[j].second, 2));
			m1DHistograms["AL_IN_DIS"]->Fill(distance * 1000);
			if ( distance > 0.048 && distance < 0.154 ) {
				num++;
			}
		}
	}
	return num;
}


// 	for ( const auto& [key, hist] : m2DHistograms ) {
// 		Double_t multiple = mConfig.getConfig(key).hasKey("MULTIPLE") ? stod(mConfig.getConfig(key).find("MULTIPLE")) : 1.;
// 		if ( key == "MotherDoughterCorrelationInALPIDE" ) {
// 			if ( isFromPrimary ) {
// 				hist->Fill(incidentTuple.particleID, mSecondaryTuple.particleID);
// 			} else {
// 				hist->Fill(preSecondaryTuple.particleID, mSecondaryTuple.particleID);
// 			}
// 		}


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