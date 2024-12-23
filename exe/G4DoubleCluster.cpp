#include<csv.h>

#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TH1.h"
#include "TCanvas.h"

#include "cppargs.h"
#include "CppConfigFile.h"

CppConfigFile setEnvironment(const std::string& iTag) {
	CppConfigFile config("/home/ychoi/ATOM/config/g4simulation/g4comparison.config");

	io::CSVReader<2> mainCSV("/home/ychoi/ATOM/config/g4simulation/g4comparison.csv");
	std::string tag, fileListStr;
	while ( mainCSV.read_row(tag, fileListStr) ) {
		if ( tag == iTag ) {
			config.modifyConfig("FileList").
				break;
		}
	}



	return config;
}
Int_t getDoubleClusterNum(std::vector<std::pair<Double_t, Double_t>> position) {
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

void fillDistance(TH1* histogram, std::vector<std::pair<Double_t, Double_t>> position) {
	for ( Int_t i = 0; i < position.size(); i++ ) {
		for ( Int_t j = i + 1; j < position.size(); j++ ) {
			Double_t distance = TMath::Sqrt(TMath::Power(position[i].first - position[j].first, 2) + TMath::Power(position[i].second - position[j].second, 2));
			histogram->Fill(distance);
		}
	}
}

int main(int argc, char** argv) {
	CppConfigFile config;

	std::filesystem::path inputPath = config.getConfig("File").find("input_file");

	TFile* file = new TFile(static_cast<TString>(inputPath), "READ");
	TTree* tree = static_cast<TTree*>(file->Get("incidentTree"));

	Int_t preTimeStamp = -1;
	Int_t eventID;
	Int_t particleID;
	const Int_t activity = 4300;
	const Double_t time = 0.025;
	const Int_t eventSize = activity * time;
	Double_t x, y;
	tree->SetBranchAddress("eventID", &eventID);
	tree->SetBranchAddress("particleID", &particleID);
	tree->SetBranchAddress("x", &x);
	tree->SetBranchAddress("y", &y);

	TH1* distanceHistogram = new TH1D("distanceHistogram", "Distance; Distance [mm]; Counts", 200, 0, 2);

	std::vector<std::pair<Double_t, Double_t>> position;

	Int_t nDoubleCluster = 0;
	Int_t nCluster = 0;
	for ( Int_t i = 0; i < tree->GetEntries(); i++ ) {
		tree->GetEntry(i);
		if ( preTimeStamp != eventID / eventSize ) {
			preTimeStamp = eventID / eventSize;
			nDoubleCluster += getDoubleClusterNum(position);
			fillDistance(distanceHistogram, position);
			nCluster += position.size();
			position.clear();
		}
		if ( particleID == 1 ) {
			position.push_back({x, y});
		}
	}

	TCanvas* canvas = new TCanvas("distanceCanvas", "", 2000, 1000);
	distanceHistogram->Draw();
	canvas->SaveAs("distanceHistogram.png");

	std::cout << "The number of double clusters are " << nDoubleCluster << " in " << nCluster << " clusters" << std::endl;
	return 0;
}