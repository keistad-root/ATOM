#include "TGeantAnalysis.h"

TGeantAnalysis::TGeantAnalysis() {
	mFile = new TFile("output.root", "READ");
}

TGeantAnalysis::TGeantAnalysis(const CppConfigFile& config) {
	std::string inputFileName = config.getConfig("File").find("input_file");
	mOutputPath = config.getConfig("File").find("output_directory");
	mFile = new TFile(static_cast<TString>(inputFileName), "READ");

	std::filesystem::create_directories(mOutputPath);
	gStyle->SetOptStat(0);
}

TGeantAnalysis::~TGeantAnalysis() {
	mFile->Close();
}

void TGeantAnalysis::readInit() {
	mInitTree = (TTree*) mFile->Get("initTree");

	TString particleName;
	Double_t x, y, z;
	Double_t px, py, pz;

	mInitTree->SetBranchAddress("Particle Name", &particleName);
	mInitTree->SetBranchAddress("X", &x);
	mInitTree->SetBranchAddress("Y", &y);
	mInitTree->SetBranchAddress("Z", &z);
	mInitTree->SetBranchAddress("PX", &px);
	mInitTree->SetBranchAddress("PY", &py);
	mInitTree->SetBranchAddress("PZ", &pz);

	TH2D* xyHist = new TH2D("xyHist", "X-Y Position", 100, -5, 5, 100, -5, 5);
	TH1D* zHist = new TH1D("zHist", "Z Position", 100, 21, 23);

	TH1D* thetaHist = new TH1D("thetaHist", "Theta Angle", 180, 0, 180);
	TH1D* phiHist = new TH1D("phiHist", "Phi Angle", 360, -180, 180);

	for ( int i = 0; i < mInitTree->GetEntries(); i++ ) {
		mInitTree->GetEntry(i);

		xyHist->Fill(x, y);

		double momentum = sqrt(px * px + py * py + pz * pz);

		double theta = 180. * acos(pz / momentum) / TMath::Pi();
		double phi = 180. * atan2(py, px) / TMath::Pi();

		zHist->Fill(z);

		thetaHist->Fill(theta);
		phiHist->Fill(phi);
	}

	// gStyle->SetOptStat(0);
	TCanvas* positionCanvas = new TCanvas("positionCanvas", "", 2000, 1000);
	TPad* xyPad = new TPad("xyPad", "", .0, .0, 0.5, 1.0);
	TPad* zPad = new TPad("zPad", "", .5, .0, 1.0, 1.0);
	xyPad->Draw();
	zPad->Draw();

	xyPad->cd();
	xyHist->SetTitle("Source XY Position; X [mm]; Y [mm]");
	xyHist->Draw("colz");

	zPad->cd();
	zHist->SetTitle("Source Z Position; Z [mm]; Entries");
	zHist->Draw();

	positionCanvas->SaveAs(static_cast<TString>(mOutputPath / "source_position.png"));

	TCanvas* angleCanvas = new TCanvas("angleCanvas", "", 2000, 1000);
	TPad* thetaPad = new TPad("thetaPad", "", .0, .0, 0.5, 1.0);
	TPad* phiPad = new TPad("phiPad", "", .5, .0, 1.0, 1.0);
	thetaPad->Draw();
	phiPad->Draw();

	thetaPad->cd();
	thetaHist->SetTitle("#theta; #theta [degree]; Entries");
	thetaHist->Draw();

	phiPad->cd();
	phiHist->SetTitle("#phi; #phi [degree]; Entries");
	phiHist->Draw();

	angleCanvas->SaveAs(static_cast<TString>(mOutputPath / "source_angle.png"));
}

void TGeantAnalysis::readALPIDE() {
	TTree* alpideTree = (TTree*) mFile->Get("alpideTree");

	Double_t depositEnergyMetal, depositEnergyEpitaxial, depositEnergySubstrate;

	alpideTree->SetBranchAddress("DepositEnergyMetal", &depositEnergyMetal);
	alpideTree->SetBranchAddress("DepositEnergyEpitaxial", &depositEnergyEpitaxial);
	alpideTree->SetBranchAddress("DepositEnergySubstrate", &depositEnergySubstrate);

	TH1D* metalHist = new TH1D("metalHist", "Metal Deposit Energy", 1000, 0, 5.4);
	TH1D* epitaxialHist = new TH1D("epitaxialHist", "Epitaxial Deposit Energy", 1000, 0, 5.4);
	TH1D* substrateHist = new TH1D("substrateHist", "Substrate Deposit Energy", 1000, 0, 5.4);

	for ( int i = 0; i < alpideTree->GetEntries(); i++ ) {
		alpideTree->GetEntry(i);

		metalHist->Fill(depositEnergyMetal);
		epitaxialHist->Fill(depositEnergyEpitaxial);
		substrateHist->Fill(depositEnergySubstrate);
	}

	// gStyle->SetOptStat(0);

	TCanvas* depositCanvas = new TCanvas("depositCanvas", "", 3000, 1000);
	TPad* metalPad = new TPad("metalPad", "", .0, .0, 0.33, 1.0);
	TPad* epitaxialPad = new TPad("epitaxialPad", "", 0.33, .0, 0.66, 1.0);
	TPad* substratePad = new TPad("substratePad", "", 0.66, .0, 1.0, 1.0);

	metalPad->Draw();
	epitaxialPad->Draw();
	substratePad->Draw();

	metalPad->cd();
	metalHist->SetTitle("Metal Deposit Energy; Energy [MeV]; Entries");
	metalHist->SetBinContent(0, 0);
	metalHist->Draw();

	epitaxialPad->cd();
	epitaxialHist->SetTitle("Epitaxial Deposit Energy; Energy [MeV]; Entries");
	epitaxialHist->SetBinContent(0, 0);
	epitaxialHist->Draw();

	substratePad->cd();
	substrateHist->SetTitle("Substrate Deposit Energy; Energy [MeV]; Entries");
	substrateHist->SetBinContent(0, 0);
	substrateHist->Draw();

	depositCanvas->SaveAs(static_cast<TString>(mOutputPath / "deposit_energy.png"));
}

void TGeantAnalysis::readIncident() {
	TTree* incidentTree = (TTree*) mFile->Get("incidentTree");

	Int_t particleID;
	Double_t x, y, z;
	Double_t px, py, pz;

	incidentTree->SetBranchAddress("Particle ID", &particleID);
	incidentTree->SetBranchAddress("X", &x);
	incidentTree->SetBranchAddress("Y", &y);
	incidentTree->SetBranchAddress("Z", &z);
	incidentTree->SetBranchAddress("PX", &px);
	incidentTree->SetBranchAddress("PY", &py);
	incidentTree->SetBranchAddress("PZ", &pz);

	TH1I* particleIDHist = new TH1I("particleIDHist", "Particle ID", 3, 0, 3);
	TH2D* alpideHist = new TH2D("alpideHist", "Incident XY Position", 1024, -15, 15, 512, -7.5, 7.5);
	TH1D* incidentAngleHist = new TH1D("incidentAngleHist", "Incident Angle", 900, 0, 90);

	for ( int i = 0; i < incidentTree->GetEntries(); i++ ) {
		incidentTree->GetEntry(i);
		if ( particleID == 0 ) {
			std::cout << "Unknown Particle" << std::endl;
		}
		particleIDHist->Fill(particleID);

		double momentum = sqrt(px * px + py * py + pz * pz);

		double theta = 180. * acos(pz / momentum) / TMath::Pi();
		alpideHist->Fill(x, y);
		incidentAngleHist->Fill(180. - theta);
	}

	particleIDHist->GetXaxis()->SetBinLabel(1, "Unknown");
	particleIDHist->GetXaxis()->SetBinLabel(2, "#alpha");
	particleIDHist->GetXaxis()->SetBinLabel(3, "e^{-}");
	// gStyle->SetOptStat(0);
	TCanvas* positionCanvas = new TCanvas("incident_positionCanvas", "", 2000, 2000);
	TPad* xyPad = new TPad("incident_xyPad", "", .0, .5, 1., 1.);
	TPad* zPad = new TPad("incident_zPad", "", .0, .0, .5, .5);
	TPad* idPad = new TPad("incident_zPad", "", .5, .0, 1., .5);
	xyPad->Draw();
	zPad->Draw();
	idPad->Draw();

	xyPad->cd();
	alpideHist->SetTitle("Incident XY Position; X [mm]; Y [mm]");
	alpideHist->Draw("colz");

	zPad->cd();
	incidentAngleHist->SetTitle("Incident Angle; Incident Angle [Deg]; Entries");
	incidentAngleHist->Draw();

	idPad->cd();
	particleIDHist->SetTitle("Incident Particle; Particle Name; Entries");
	particleIDHist->GetXaxis()->SetLabelSize(0.075);
	particleIDHist->Draw();

	positionCanvas->SaveAs(static_cast<TString>(mOutputPath / "incident_position.png"));
}