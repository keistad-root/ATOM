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
		readPrimaryTree();
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

		m2DHistograms["SRC_XY"]->Fill(mPrimaryTuple.position[0], mPrimaryTuple.position[1]);
		m1DHistograms["SRC_Z"]->Fill(mPrimaryTuple.position[2]);
		m1DHistograms["SRC_KE"]->Fill(mPrimaryTuple.kineticEnergy);
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

void TGeantAnalysis::readIncidentTree() {
	Int_t nEntries = mIncidentTree->GetEntries();

	std::vector<std::pair<Double_t, Double_t>> position;
	Int_t preTimeStamp = 0;

	std::array<Double_t, 4> totalDepositEnergy = {0., 0., 0., 0.};
	std::array<Double_t, 4> alphaDepositEnergy = {0., 0., 0., 0.};
	std::array<Double_t, 4> electronDepositEnergy = {0., 0., 0., 0.};
	std::array<Double_t, 4> gammaDepositEnergy = {0., 0., 0., 0.};

	ProgressBar* pbar = new ProgressBar(static_cast<int>(nEntries));
	Int_t nDouble = 0;
	for ( Int_t i = 0; i < nEntries; i++ ) {
		pbar->countUp();
		mIncidentTree->GetEntry(i);
		if ( isFromOutside() ) {
			if ( isRoi && (std::abs(mIncidentTuple.position[0]) > mRoi[0] || std::abs(mIncidentTuple.position[1]) > mRoi[1]) ) continue;
			Double_t momentum = TMath::Sqrt(mIncidentTuple.momentum[0] * mIncidentTuple.momentum[0] + mIncidentTuple.momentum[1] * mIncidentTuple.momentum[1] + mIncidentTuple.momentum[2] * mIncidentTuple.momentum[2]);
			Double_t theta = TMath::ACos(mIncidentTuple.momentum[2] / momentum) * 180. / TMath::Pi();
			Double_t multiple = 1.;
			if ( momentum > 0.0000001 ) {
				// Major Plots
				m2DHistograms["IN_XY"]->Fill(mIncidentTuple.position[0], mIncidentTuple.position[1]);

				multiple = mConfig.getConfig("IN_Z").hasKey("MULTIPLE") ? stod(mConfig.getConfig("IN_Z").find("MULTIPLE")) : 1.;
				m1DHistograms["IN_Z"]->Fill(mIncidentTuple.position[2] * multiple);

				m1DHistograms["IN_ANG"]->Fill(180 - theta);

				multiple = mConfig.getConfig("IN_KE").hasKey("MULTIPLE") ? stod(mConfig.getConfig("IN_KE").find("MULTIPLE")) : 1.;
				m1DHistograms["IN_KE"]->Fill(mIncidentTuple.kineticEnergy * multiple);

				m1DHistograms["IN_PA"]->Fill(mIncidentTuple.particleID);

				m1DHistograms["IN_VOL"]->Fill(mIncidentTuple.initialVolumeID);

				if ( totalDepositEnergy[1] > eV ) {
					multiple = mConfig.getConfig("IN_DE_MET").hasKey("MULTIPLE") ? stod(mConfig.getConfig("IN_DE_MET").find("MULTIPLE")) : 1.;
					m1DHistograms["IN_DE_MET"]->Fill(totalDepositEnergy[1] * multiple);

					multiple = mConfig.getConfig("IN_DE_MET_N_IN_ANG").hasKey("MULTIPLE") ? stod(mConfig.getConfig("IN_DE_MET_N_IN_ANG").find("MULTIPLE")) : 1.;
					m2DHistograms["IN_DE_MET_N_IN_ANG"]->Fill(180 - theta, totalDepositEnergy[1] * multiple);

				}
				totalDepositEnergy[1] = 0.;

				if ( totalDepositEnergy[2] > eV ) {
					multiple = mConfig.getConfig("IN_DE_EPI").hasKey("MULTIPLE") ? stod(mConfig.getConfig("IN_DE_EPI").find("MULTIPLE")) : 1.;
					m1DHistograms["IN_DE_EPI"]->Fill(totalDepositEnergy[2] * multiple);

					multiple = mConfig.getConfig("IN_DE_EPI_N_IN_ANG").hasKey("MULTIPLE") ? stod(mConfig.getConfig("IN_DE_EPI_N_IN_ANG").find("MULTIPLE")) : 1.;
					m2DHistograms["IN_DE_EPI_N_IN_ANG"]->Fill(180 - theta, totalDepositEnergy[2] * multiple);
				}

				totalDepositEnergy[2] = 0.;

				if ( totalDepositEnergy[3] > eV ) {
					multiple = mConfig.getConfig("IN_DE_SUB").hasKey("MULTIPLE") ? stod(mConfig.getConfig("IN_DE_SUB").find("MULTIPLE")) : 1.;
					m1DHistograms["IN_DE_SUB"]->Fill(totalDepositEnergy[3] * multiple);

					multiple = mConfig.getConfig("IN_DE_SUB_N_IN_ANG").hasKey("MULTIPLE") ? stod(mConfig.getConfig("IN_DE_SUB_N_IN_ANG").find("MULTIPLE")) : 1.;
					m2DHistograms["IN_DE_SUB_N_IN_ANG"]->Fill(180 - theta, totalDepositEnergy[3] * multiple);
				}
				totalDepositEnergy[3] = 0.;

				if ( totalDepositEnergy[0] > eV ) {
					multiple = mConfig.getConfig("IN_DE_TOT").hasKey("MULTIPLE") ? stod(mConfig.getConfig("IN_DE_TOT").find("MULTIPLE")) : 1.;
					m1DHistograms["IN_DE_TOT"]->Fill(totalDepositEnergy[0] * multiple);

					multiple = mConfig.getConfig("IN_DE_TOT_N_IN_ANG").hasKey("MULTIPLE") ? stod(mConfig.getConfig("IN_DE_TOT_N_IN_ANG").find("MULTIPLE")) : 1.;
					m2DHistograms["IN_DE_TOT_N_IN_ANG"]->Fill(180 - theta, totalDepositEnergy[0] * multiple);
				}
				totalDepositEnergy[0] = 0.;

				if ( mIncidentTuple.finalVolumeID == VOLUME::ALPIDESubstrate || mIncidentTuple.finalVolumeID == VOLUME::ALPIDEEpitaxial || mIncidentTuple.finalVolumeID == VOLUME::ALPIDEMetal ) {
					m2DHistograms["IN_ANG_N_FIN_Z"]->Fill(180 - theta, mIncidentTuple.position[2]);
				}
				if ( mIncidentTuple.particleID == PARTICLE::alpha ) {
					m2DHistograms["AL_IN_XY"]->Fill(mIncidentTuple.position[0], mIncidentTuple.position[1]);

					multiple = mConfig.getConfig("AL_IN_Z").hasKey("MULTIPLE") ? stod(mConfig.getConfig("AL_IN_Z").find("MULTIPLE")) : 1.;
					m1DHistograms["AL_IN_Z"]->Fill(mIncidentTuple.position[2] * multiple);

					m1DHistograms["AL_IN_ANG"]->Fill(180 - theta);

					multiple = mConfig.getConfig("AL_IN_KE").hasKey("MULTIPLE") ? stod(mConfig.getConfig("AL_IN_KE").find("MULTIPLE")) : 1.;
					m1DHistograms["AL_IN_KE"]->Fill(mIncidentTuple.kineticEnergy * multiple);

					m1DHistograms["AL_IN_VOL"]->Fill(mIncidentTuple.initialVolumeID);

					if ( alphaDepositEnergy[1] > eV ) {
						multiple = mConfig.getConfig("AL_IN_DE_MET").hasKey("MULTIPLE") ? stod(mConfig.getConfig("AL_IN_DE_MET").find("MULTIPLE")) : 1.;
						m1DHistograms["AL_IN_DE_MET"]->Fill(alphaDepositEnergy[1] * multiple);

						multiple = mConfig.getConfig("AL_IN_DE_MET_N_IN_ANG").hasKey("MULTIPLE") ? stod(mConfig.getConfig("AL_IN_DE_MET_N_IN_ANG").find("MULTIPLE")) : 1.;
						m2DHistograms["AL_IN_DE_MET_N_IN_ANG"]->Fill(180 - theta, alphaDepositEnergy[1] * multiple);
					}
					alphaDepositEnergy[1] = 0.;

					if ( alphaDepositEnergy[2] > eV ) {
						multiple = stod(mConfig.getConfig("AL_IN_DE_EPI").find("MULTIPLE"));
						m1DHistograms["AL_IN_DE_EPI"]->Fill(alphaDepositEnergy[2] * multiple);

						multiple = stod(mConfig.getConfig("AL_IN_DE_EPI_N_IN_ANG").find("MULTIPLE"));
						m2DHistograms["AL_IN_DE_EPI_N_IN_ANG"]->Fill(180 - theta, alphaDepositEnergy[2] * multiple);
					}
					alphaDepositEnergy[2] = 0.;

					if ( alphaDepositEnergy[3] > eV ) {
						multiple = mConfig.getConfig("AL_IN_DE_SUB").hasKey("MULTIPLE") ? stod(mConfig.getConfig("AL_IN_DE_SUB").find("MULTIPLE")) : 1.;
						m1DHistograms["AL_IN_DE_SUB"]->Fill(alphaDepositEnergy[3] * multiple);

						multiple = mConfig.getConfig("AL_IN_DE_SUB_N_IN_ANG").hasKey("MULTIPLE") ? stod(mConfig.getConfig("AL_IN_DE_SUB_N_IN_ANG").find("MULTIPLE")) : 1.;
						m2DHistograms["AL_IN_DE_SUB_N_IN_ANG"]->Fill(180 - theta, alphaDepositEnergy[3] * multiple);
					}
					alphaDepositEnergy[3] = 0.;
					if ( alphaDepositEnergy[0] > eV ) {
						multiple = mConfig.getConfig("AL_IN_DE_TOT").hasKey("MULTIPLE") ? stod(mConfig.getConfig("AL_IN_DE_TOT").find("MULTIPLE")) : 1.;
						m1DHistograms["AL_IN_DE_TOT"]->Fill(alphaDepositEnergy[0] * multiple);

						multiple = mConfig.getConfig("AL_IN_DE_TOT_N_IN_ANG").hasKey("MULTIPLE") ? stod(mConfig.getConfig("AL_IN_DE_TOT_N_IN_ANG").find("MULTIPLE")) : 1.;
						m2DHistograms["AL_IN_DE_TOT_N_IN_ANG"]->Fill(180 - theta, alphaDepositEnergy[0] * multiple);
					}
				}
				if ( mIncidentTuple.particleID == PARTICLE::electron ) {
					m2DHistograms["EL_IN_XY"]->Fill(mIncidentTuple.position[0], mIncidentTuple.position[1]);

					multiple = mConfig.getConfig("EL_IN_Z").hasKey("MULTIPLE") ? stod(mConfig.getConfig("EL_IN_Z").find("MULTIPLE")) : 1.;
					m1DHistograms["EL_IN_Z"]->Fill(mIncidentTuple.position[2] * multiple);

					m1DHistograms["EL_IN_ANG"]->Fill(180 - theta);

					multiple = mConfig.getConfig("EL_IN_KE").hasKey("MULTIPLE") ? stod(mConfig.getConfig("EL_IN_KE").find("MULTIPLE")) : 1.;
					m1DHistograms["EL_IN_KE"]->Fill(mIncidentTuple.kineticEnergy * multiple);

					m1DHistograms["EL_INI_VOL"]->Fill(mIncidentTuple.initialVolumeID);

					if ( electronDepositEnergy[1] > eV ) {
						multiple = mConfig.getConfig("EL_IN_DE_MET").hasKey("MULTIPLE") ? stod(mConfig.getConfig("EL_IN_DE_MET").find("MULTIPLE")) : 1.;
						m1DHistograms["EL_IN_DE_MET"]->Fill(electronDepositEnergy[1] * multiple);

						multiple = mConfig.getConfig("EL_IN_DE_MET_N_IN_ANG").hasKey("MULTIPLE") ? stod(mConfig.getConfig("EL_IN_DE_MET_N_IN_ANG").find("MULTIPLE")) : 1.;
						m2DHistograms["EL_IN_DE_MET_N_IN_ANG"]->Fill(180 - theta, electronDepositEnergy[1] * multiple);
					}
					electronDepositEnergy[1] = 0.;

					if ( electronDepositEnergy[2] > eV ) {
						multiple = mConfig.getConfig("EL_IN_DE_EPI").hasKey("MULTIPLE") ? stod(mConfig.getConfig("EL_IN_DE_EPI").find("MULTIPLE")) : 1.;
						m1DHistograms["EL_IN_DE_EPI"]->Fill(electronDepositEnergy[2] * multiple);

						multiple = mConfig.getConfig("EL_IN_DE_EPI_N_IN_ANG").hasKey("MULTIPLE") ? stod(mConfig.getConfig("EL_IN_DE_EPI_N_IN_ANG").find("MULTIPLE")) : 1.;
						m2DHistograms["EL_IN_DE_EPI_N_IN_ANG"]->Fill(180 - theta, electronDepositEnergy[2] * multiple);
					}
					electronDepositEnergy[2] = 0.;

					if ( electronDepositEnergy[3] > eV ) {
						multiple = mConfig.getConfig("EL_IN_DE_SUB").hasKey("MULTIPLE") ? stod(mConfig.getConfig("EL_IN_DE_SUB").find("MULTIPLE")) : 1.;
						m1DHistograms["EL_IN_DE_SUB"]->Fill(electronDepositEnergy[3] * multiple);

						multiple = mConfig.getConfig("EL_IN_DE_SUB_N_IN_ANG").hasKey("MULTIPLE") ? stod(mConfig.getConfig("EL_IN_DE_SUB_N_IN_ANG").find("MULTIPLE")) : 1.;
						m2DHistograms["EL_IN_DE_SUB_N_IN_ANG"]->Fill(180 - theta, electronDepositEnergy[3] * multiple);
					}
					electronDepositEnergy[3] = 0.;

					if ( electronDepositEnergy[0] > eV ) {
						multiple = mConfig.getConfig("EL_IN_DE_TOT").hasKey("MULTIPLE") ? stod(mConfig.getConfig("EL_IN_DE_TOT").find("MULTIPLE")) : 1.;
						m1DHistograms["EL_IN_DE_TOT"]->Fill(electronDepositEnergy[0] * multiple);

						multiple = mConfig.getConfig("EL_IN_DE_TOT_N_IN_ANG").hasKey("MULTIPLE") ? stod(mConfig.getConfig("EL_IN_DE_TOT_N_IN_ANG").find("MULTIPLE")) : 1.;
						m2DHistograms["EL_IN_DE_TOT_N_IN_ANG"]->Fill(180 - theta, electronDepositEnergy[0] * multiple);
					}
					electronDepositEnergy[0] = 0.;
				}
				if ( mIncidentTuple.particleID == PARTICLE::gamma1 ) {

					m2DHistograms["GA_IN_XY"]->Fill(mIncidentTuple.position[0], mIncidentTuple.position[1]);

					multiple = mConfig.getConfig("GA_IN_Z").hasKey("MULTIPLE") ? stod(mConfig.getConfig("GA_IN_Z").find("MULTIPLE")) : 1.;
					m1DHistograms["GA_IN_Z"]->Fill(mIncidentTuple.position[2] * multiple);

					m1DHistograms["GA_IN_ANG"]->Fill(180 - theta);

					m1DHistograms["GA_IN_KE"]->Fill(mIncidentTuple.kineticEnergy * multiple);

					m1DHistograms["GA_IN_GT"]->Fill(mIncidentTuple.globalTime);

					m1DHistograms["GA_INI_LT"]->Fill(mIncidentTuple.localTime);

					m1DHistograms["GA_INI_VOL"]->Fill(mIncidentTuple.initialVolumeID);

					Double_t gammaDepositEnergyTotal = 0.;
					if ( mIncidentTuple.particleID == PARTICLE::gamma1 && gammaDepositEnergy[0] > eV ) {
						if ( m1DHistograms.count("GammaDepositEnergyMetal") ) {
							Double_t multiple = mConfig.getConfig("GammaDepositEnergyMetal").hasKey("MULTIPLE") ? stod(mConfig.getConfig("GammaDepositEnergyMetal").find("MULTIPLE")) : 1.;
							m1DHistograms["GammaDepositEnergyMetal"]->Fill(gammaDepositEnergy[0] * multiple);
						}
						if ( m2DHistograms.count("GammaCorrelationDepositEnergyMetalAndIncidentAngle") ) {
							Double_t multiple = mConfig.getConfig("GammaCorrelationDepositEnergyMetalAndIncidentAngle").hasKey("MULTIPLE") ? stod(mConfig.getConfig("GammaCorrelationDepositEnergyMetalAndIncidentAngle").find("MULTIPLE")) : 1.;
							m2DHistograms["GammaCorrelationDepositEnergyMetalAndIncidentAngle"]->Fill(180 - theta, gammaDepositEnergy[0] * multiple);
						}
						gammaDepositEnergyTotal += gammaDepositEnergy[0];
					}
					gammaDepositEnergy[0] = 0.;
					if ( mIncidentTuple.particleID == PARTICLE::gamma1 && gammaDepositEnergy[1] > eV ) {
						if ( m1DHistograms.count("GammaDepositEnergyEpitaxial") ) {
							Double_t multiple = mConfig.getConfig("GammaDepositEnergyEpitaxial").hasKey("MULTIPLE") ? stod(mConfig.getConfig("GammaDepositEnergyEpitaxial").find("MULTIPLE")) : 1.;
							m1DHistograms["GammaDepositEnergyEpitaxial"]->Fill(gammaDepositEnergy[1] * multiple);
						}
						if ( m2DHistograms.count("GammaCorrelationDepositEnergyEpitaxialAndIncidentAngle") ) {
							Double_t multiple = mConfig.getConfig("GammaCorrelationDepositEnergyEpitaxialAndIncidentAngle").hasKey("MULTIPLE") ? stod(mConfig.getConfig("GammaCorrelationDepositEnergyEpitaxialAndIncidentAngle").find("MULTIPLE")) : 1.;
							m2DHistograms["GammaCorrelationDepositEnergyEpitaxialAndIncidentAngle"]->Fill(180 - theta, gammaDepositEnergy[1] * multiple);
						}
						gammaDepositEnergyTotal += gammaDepositEnergy[1];
					}
					gammaDepositEnergy[1] = 0.;
					if ( mIncidentTuple.particleID == PARTICLE::gamma1 && gammaDepositEnergy[2] > eV ) {
						if ( m1DHistograms.count("GammaDepositEnergySubstrate") ) {
							Double_t multiple = mConfig.getConfig("GammaDepositEnergySubstrate").hasKey("MULTIPLE") ? stod(mConfig.getConfig("GammaDepositEnergySubstrate").find("MULTIPLE")) : 1.;
							m1DHistograms["GammaDepositEnergySubstrate"]->Fill(gammaDepositEnergy[2] * multiple);
						}
						if ( m2DHistograms.count("GammaCorrelationDepositEnergySubstrateAndIncidentAngle") ) {
							Double_t multiple = mConfig.getConfig("GammaCorrelationDepositEnergySubstrateAndIncidentAngle").hasKey("MULTIPLE") ? stod(mConfig.getConfig("GammaCorrelationDepositEnergySubstrateAndIncidentAngle").find("MULTIPLE")) : 1.;
							m2DHistograms["GammaCorrelationDepositEnergySubstrateAndIncidentAngle"]->Fill(180 - theta, gammaDepositEnergy[2] * multiple);
						}
						gammaDepositEnergyTotal += gammaDepositEnergy[2];
					}
					gammaDepositEnergy[2] = 0.;
					if ( mIncidentTuple.particleID == PARTICLE::gamma1 && gammaDepositEnergyTotal > eV ) {
						if ( m1DHistograms.count("GammaDepositEnergyTotal") ) {
							Double_t multiple = mConfig.getConfig("GammaDepositEnergyTotal").hasKey("MULTIPLE") ? stod(mConfig.getConfig("GammaDepositEnergyTotal").find("MULTIPLE")) : 1.;
							m1DHistograms["GammaDepositEnergyTotal"]->Fill(gammaDepositEnergyTotal * multiple);
						}
						if ( m2DHistograms.count("GammaCorrelationDepositEnergyTotalAndIncidentAngle") ) {
							Double_t multiple = mConfig.getConfig("GammaCorrelationDepositEnergyTotalAndIncidentAngle").hasKey("MULTIPLE") ? stod(mConfig.getConfig("GammaCorrelationDepositEnergyTotalAndIncidentAngle").find("MULTIPLE")) : 1.;
							m2DHistograms["GammaCorrelationDepositEnergyTotalAndIncidentAngle"]->Fill(180 - theta, gammaDepositEnergyTotal * multiple);
						}
					}
				}
				// Minor Plots
				if ( m1DHistograms.count("IncidentResidualX") ) {
					double n = (mIncidentTuple.position[0] * 1000 - 14.62) / 29.24 + 1;
					double residual = mIncidentTuple.position[0] * 1000 - (14.62 + (std::round(n) - 1) * 29.24);
					m1DHistograms["IncidentResidualX"]->Fill(residual);
				}
				if ( m1DHistograms.count("IncidentResidualY") ) {
					double n = (mIncidentTuple.position[1] * 1000 - 13.44) / 26.88 + 1;
					double residual = mIncidentTuple.position[1] * 1000 - (13.44 + (std::round(n) - 1) * 26.88);
					m1DHistograms["IncidentResidualY"]->Fill(residual);
				}
				if ( m1DHistograms.count("IncidentXLine") ) {
					if ( TMath::Abs(mIncidentTuple.position[1]) < 0.028 ) {
						m1DHistograms["IncidentXLine"]->Fill(mIncidentTuple.position[0]);
					}
				}
				if ( m1DHistograms.count("IncidentYLine") ) {
					if ( TMath::Abs(mIncidentTuple.position[0]) < 0.028 ) {
						m1DHistograms["IncidentYLine"]->Fill(mIncidentTuple.position[1]);
					}
				}
				if ( m1DHistograms.count("IncidentGlobalTime") ) {
					Double_t multiple = mConfig.getConfig("IncidentGlobalTime").hasKey("MULTIPLE") ? stod(mConfig.getConfig("IncidentGlobalTime").find("MULTIPLE")) : 1.;
					m1DHistograms["IncidentGlobalTime"]->Fill(mIncidentTuple.globalTime * multiple);
				}
				if ( m1DHistograms.count("IncidentLocalTime") ) {
					Double_t multiple = mConfig.getConfig("IncidentLocalTime").hasKey("MULTIPLE") ? stod(mConfig.getConfig("IncidentLocalTime").find("MULTIPLE")) : 1.;
					m1DHistograms["IncidentLocalTime"]->Fill(mIncidentTuple.localTime * multiple);
				}
				if ( mIncidentTuple.particleID == PARTICLE::alpha && m1DHistograms.count("AlphaIncidentResidualX") ) {
					double n = (mIncidentTuple.position[0] * 1000 - 14.62) / 29.24 + 1;
					double residual = mIncidentTuple.position[0] * 1000 - (14.62 + (std::round(n) - 1) * 29.24);
					m1DHistograms["AlphaIncidentResidualX"]->Fill(residual);
				}
				if ( mIncidentTuple.particleID == PARTICLE::alpha && m1DHistograms.count("AlphaIncidentResidualY") ) {
					double n = (mIncidentTuple.position[1] * 1000 - 13.44) / 26.88 + 1;
					double residual = mIncidentTuple.position[1] * 1000 - (13.44 + (std::round(n) - 1) * 26.88);
					m1DHistograms["AlphaIncidentResidualY"]->Fill(residual);
				}
				if ( mIncidentTuple.particleID == PARTICLE::alpha && m1DHistograms.count("AlphaIncidentGlobalTime") ) {
					Double_t multiple = mConfig.getConfig("AlphaIncidentGlobalTime").hasKey("MULTIPLE") ? stod(mConfig.getConfig("AlphaIncidentGlobalTime").find("MULTIPLE")) : 1.;
					m1DHistograms["AlphaIncidentGlobalTime"]->Fill(mIncidentTuple.globalTime * multiple);
				}
				if ( mIncidentTuple.particleID == PARTICLE::alpha && m1DHistograms.count("AlphaIncidentLocalTime") ) {
					Double_t multiple = mConfig.getConfig("AlphaIncidentLocalTime").hasKey("MULTIPLE") ? stod(mConfig.getConfig("AlphaIncidentLocalTime").find("MULTIPLE")) : 1.;
					m1DHistograms["AlphaIncidentLocalTime"]->Fill(mIncidentTuple.localTime * multiple);
				}
				if ( mIncidentTuple.particleID == PARTICLE::electron && m1DHistograms.count("ElectronIncidentResidualX") ) {
					double n = (mIncidentTuple.position[0] * 1000 - 14.62) / 29.24 + 1;
					double residual = mIncidentTuple.position[0] * 1000 - (14.62 + (std::round(n) - 1) * 29.24);
					m1DHistograms["ElectronIncidentResidualX"]->Fill(residual);
				}
				if ( mIncidentTuple.particleID == PARTICLE::electron && m1DHistograms.count("ElectronIncidentResidualY") ) {
					double n = (mIncidentTuple.position[1] * 1000 - 13.44) / 26.88 + 1;
					double residual = mIncidentTuple.position[1] * 1000 - (13.44 + (std::round(n) - 1) * 26.88);
					m1DHistograms["ElectronIncidentResidualY"]->Fill(residual);
				}
				if ( mIncidentTuple.particleID == PARTICLE::electron && m1DHistograms.count("ElectronIncidentGlobalTime") ) {
					Double_t multiple = mConfig.getConfig("ElectronIncidentGlobalTime").hasKey("MULTIPLE") ? stod(mConfig.getConfig("ElectronIncidentGlobalTime").find("MULTIPLE")) : 1.;
					m1DHistograms["ElectronIncidentGlobalTime"]->Fill(mIncidentTuple.globalTime * multiple);
				}
				if ( mIncidentTuple.particleID == PARTICLE::electron && m1DHistograms.count("ElectronIncidentLocalTime") ) {
					Double_t multiple = mConfig.getConfig("ElectronIncidentLocalTime").hasKey("MULTIPLE") ? stod(mConfig.getConfig("ElectronIncidentLocalTime").find("MULTIPLE")) : 1.;
					m1DHistograms["ElectronIncidentLocalTime"]->Fill(mIncidentTuple.localTime * multiple);
				}
				if ( mIncidentTuple.particleID == PARTICLE::gamma1 && m1DHistograms.count("GammaIncidentResidualX") ) {
					double n = (mIncidentTuple.position[0] * 1000 - 14.62) / 29.24 + 1;
					double residual = mIncidentTuple.position[0] * 1000 - (14.62 + (std::round(n) - 1) * 29.24);
					m1DHistograms["GammaIncidentResidualX"]->Fill(residual);
				}
				if ( mIncidentTuple.particleID == PARTICLE::gamma1 && m1DHistograms.count("GammaIncidentResidualY") ) {
					double n = (mIncidentTuple.position[1] * 1000 - 13.44) / 26.88 + 1;
					double residual = mIncidentTuple.position[1] * 1000 - (13.44 + (std::round(n) - 1) * 26.88);
					m1DHistograms["GammaIncidentResidualY"]->Fill(residual);
				}
				if ( mIncidentTuple.particleID == PARTICLE::gamma1 && m1DHistograms.count("GammaIncidentGlobalTime") ) {
					Double_t multiple = mConfig.getConfig("GammaIncidentGlobalTime").hasKey("MULTIPLE") ? stod(mConfig.getConfig("GammaIncidentGlobalTime").find("MULTIPLE")) : 1.;
					m1DHistograms["GammaIncidentGlobalTime"]->Fill(mIncidentTuple.globalTime * multiple);
				}
				if ( mIncidentTuple.particleID == PARTICLE::gamma1 && m1DHistograms.count("GammaIncidentLocalTime") ) {
					Double_t multiple = mConfig.getConfig("GammaIncidentLocalTime").hasKey("MULTIPLE") ? stod(mConfig.getConfig("GammaIncidentLocalTime").find("MULTIPLE")) : 1.;
					m1DHistograms["GammaIncidentLocalTime"]->Fill(mIncidentTuple.localTime * multiple);
				}
			}
			totalDepositEnergy[0] = 0.;
			totalDepositEnergy[1] = 0.;
			totalDepositEnergy[2] = 0.;
			totalDepositEnergy[3] = 0.;
			alphaDepositEnergy[0] = 0.;
			alphaDepositEnergy[1] = 0.;
			alphaDepositEnergy[2] = 0.;
			alphaDepositEnergy[3] = 0.;
			electronDepositEnergy[0] = 0.;
			electronDepositEnergy[1] = 0.;
			electronDepositEnergy[2] = 0.;
			electronDepositEnergy[3] = 0.;
			gammaDepositEnergy[0] = 0.;
			gammaDepositEnergy[1] = 0.;
			gammaDepositEnergy[2] = 0.;
			gammaDepositEnergy[3] = 0.;
			if ( mIncidentTuple.depositEnergy[0] > eV ) {
				totalDepositEnergy[0] += mIncidentTuple.depositEnergy[0];
				totalDepositEnergy[1] += mIncidentTuple.depositEnergy[0];
				if ( mIncidentTuple.particleID == PARTICLE::alpha ) {
					alphaDepositEnergy[0] += mIncidentTuple.depositEnergy[0];
					alphaDepositEnergy[1] += mIncidentTuple.depositEnergy[0];
				}
				if ( mIncidentTuple.particleID == PARTICLE::electron ) {
					electronDepositEnergy[0] += mIncidentTuple.depositEnergy[0];
					electronDepositEnergy[1] += mIncidentTuple.depositEnergy[0];
				}
				if ( mIncidentTuple.particleID == PARTICLE::gamma1 ) {
					gammaDepositEnergy[0] += mIncidentTuple.depositEnergy[0];
					gammaDepositEnergy[1] += mIncidentTuple.depositEnergy[0];
				}
			}
			if ( mIncidentTuple.depositEnergy[1] > eV ) {
				totalDepositEnergy[0] += mIncidentTuple.depositEnergy[1];
				totalDepositEnergy[2] += mIncidentTuple.depositEnergy[1];
				if ( mIncidentTuple.particleID == PARTICLE::alpha ) {
					alphaDepositEnergy[0] += mIncidentTuple.depositEnergy[1];
					alphaDepositEnergy[2] += mIncidentTuple.depositEnergy[1];
				}
				if ( mIncidentTuple.particleID == PARTICLE::electron ) {
					electronDepositEnergy[0] += mIncidentTuple.depositEnergy[1];
					electronDepositEnergy[2] += mIncidentTuple.depositEnergy[1];
				}
				if ( mIncidentTuple.particleID == PARTICLE::gamma1 ) {
					gammaDepositEnergy[0] += mIncidentTuple.depositEnergy[1];
					gammaDepositEnergy[2] += mIncidentTuple.depositEnergy[1];
				}
			}
			if ( mIncidentTuple.depositEnergy[2] > eV ) {
				totalDepositEnergy[0] += mIncidentTuple.depositEnergy[2];
				totalDepositEnergy[3] += mIncidentTuple.depositEnergy[2];
				if ( mIncidentTuple.particleID == PARTICLE::alpha ) {
					alphaDepositEnergy[0] += mIncidentTuple.depositEnergy[2];
					alphaDepositEnergy[3] += mIncidentTuple.depositEnergy[2];
				}
				if ( mIncidentTuple.particleID == PARTICLE::electron ) {
					electronDepositEnergy[0] += mIncidentTuple.depositEnergy[2];
					electronDepositEnergy[3] += mIncidentTuple.depositEnergy[2];
				}
				if ( mIncidentTuple.particleID == PARTICLE::gamma1 ) {
					gammaDepositEnergy[0] += mIncidentTuple.depositEnergy[2];
					gammaDepositEnergy[3] += mIncidentTuple.depositEnergy[2];
				}
			}
			if ( mIncidentTuple.particleID == PARTICLE::alpha ) {
				position.push_back({mIncidentTuple.position[0], mIncidentTuple.position[1]});
			}
			if ( preTimeStamp != mIncidentTuple.eventID / 107 ) {
				preTimeStamp = mIncidentTuple.eventID / 107;
				nDouble += getNDouble(position);
				position.clear();
				position.shrink_to_fit();
			}
		} else if ( isFromALPIDE() ) {
			if ( mIncidentTuple.depositEnergy[0] > eV ) {
				totalDepositEnergy[0] += mIncidentTuple.depositEnergy[0];
				totalDepositEnergy[1] += mIncidentTuple.depositEnergy[0];
				if ( mIncidentTuple.particleID == PARTICLE::alpha ) {
					alphaDepositEnergy[0] += mIncidentTuple.depositEnergy[0];
					alphaDepositEnergy[1] += mIncidentTuple.depositEnergy[0];
				}
				if ( mIncidentTuple.particleID == PARTICLE::electron ) {
					electronDepositEnergy[0] += mIncidentTuple.depositEnergy[0];
					electronDepositEnergy[1] += mIncidentTuple.depositEnergy[0];
				}
				if ( mIncidentTuple.particleID == PARTICLE::gamma1 ) {
					gammaDepositEnergy[0] += mIncidentTuple.depositEnergy[0];
					gammaDepositEnergy[1] += mIncidentTuple.depositEnergy[0];
				}
			}
			if ( mIncidentTuple.depositEnergy[1] > eV ) {
				totalDepositEnergy[0] += mIncidentTuple.depositEnergy[1];
				totalDepositEnergy[2] += mIncidentTuple.depositEnergy[1];
				if ( mIncidentTuple.particleID == PARTICLE::alpha ) {
					alphaDepositEnergy[0] += mIncidentTuple.depositEnergy[1];
					alphaDepositEnergy[2] += mIncidentTuple.depositEnergy[1];
				}
				if ( mIncidentTuple.particleID == PARTICLE::electron ) {
					electronDepositEnergy[0] += mIncidentTuple.depositEnergy[1];
					electronDepositEnergy[2] += mIncidentTuple.depositEnergy[1];
				}
				if ( mIncidentTuple.particleID == PARTICLE::gamma1 ) {
					gammaDepositEnergy[0] += mIncidentTuple.depositEnergy[1];
					gammaDepositEnergy[2] += mIncidentTuple.depositEnergy[1];
				}
			}
			if ( mIncidentTuple.depositEnergy[2] > eV ) {
				totalDepositEnergy[0] += mIncidentTuple.depositEnergy[2];
				totalDepositEnergy[3] += mIncidentTuple.depositEnergy[2];
				if ( mIncidentTuple.particleID == PARTICLE::alpha ) {
					alphaDepositEnergy[0] += mIncidentTuple.depositEnergy[2];
					alphaDepositEnergy[3] += mIncidentTuple.depositEnergy[2];
				}
				if ( mIncidentTuple.particleID == PARTICLE::electron ) {
					electronDepositEnergy[0] += mIncidentTuple.depositEnergy[2];
					electronDepositEnergy[3] += mIncidentTuple.depositEnergy[2];
				}
				if ( mIncidentTuple.particleID == PARTICLE::gamma1 ) {
					gammaDepositEnergy[0] += mIncidentTuple.depositEnergy[2];
					gammaDepositEnergy[3] += mIncidentTuple.depositEnergy[2];
				}
			}
			if ( m1DHistograms.count("SE_PA_IN_ALPIDE") ) {
				m1DHistograms["SE_PA_IN_ALPIDE"]->Fill(mIncidentTuple.particleID);
			}
			if ( m1DHistograms.count("SE_DIS_BEG_END_IN_ALPIDE") ) {
				if ( mIncidentTuple.finalVolumeID < 4 ) {
					Double_t distance = TMath::Sqrt(TMath::Power(mIncidentTuple.initialPosition[0] - mIncidentTuple.finalPosition[0], 2) + TMath::Power(mIncidentTuple.initialPosition[1] - mIncidentTuple.finalPosition[1], 2) + TMath::Power(mIncidentTuple.initialPosition[2] - mIncidentTuple.finalPosition[2], 2));
					Double_t multiple = mConfig.getConfig("SE_DIS_BEG_END_IN_ALPIDE").hasKey("MULTIPLE") ? stod(mConfig.getConfig("SE_DIS_BEG_END_IN_ALPIDE").find("MULTIPLE")) : 1.;
					m1DHistograms["SE_DIS_BEG_END_IN_ALPIDE"]->Fill(distance * multiple);
				}
			}
			if ( m1DHistograms.count("SE_INI_VOL_IN_ALPIDE") ) {
				m1DHistograms["SE_INI_VOL_IN_ALPIDE"]->Fill(mIncidentTuple.finalVolumeID);
			}
			if ( m1DHistograms.count("SE_FIN_VOL_IN_ALPIDE") ) {
				m1DHistograms["SE_FIN_VOL_IN_ALPIDE"]->Fill(mIncidentTuple.finalVolumeID);
			}
		}
	}
	delete pbar;
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