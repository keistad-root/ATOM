#define __TTHRESHOLDANALYSER_HEADER__

#include "TThresholdAnalyser.h"

TThresholdAnalyser::TThresholdAnalyser() { }

TThresholdAnalyser::TThresholdAnalyser(std::ifstream& file) {
	openFile(file);
}

TThresholdAnalyser::~TThresholdAnalyser() {
	std::cout << "The threshold analyser is terminated" << std::endl;
}

void TThresholdAnalyser::openFile(std::ifstream& file) {
	std::string line;
	std::array<Int_t, 50> dacs;
	std::streampos originalPos = file.tellg();
	int numLines = 0;
	char ch;
	while ( file.get(ch) ) {
		if ( ch == '\n' ) {
			++numLines;
		}
	}
	file.clear();
	file.seekg(originalPos);
	ProgressBar bar(numLines + 1);
	while ( file ) {
		bar.printProgress();
		getline(file, line);
		std::stringstream values(line);
		int x, y, iDac, dac;
		values >> y >> x >> iDac >> dac;
		dacs[iDac] = dac;
		if ( iDac == 49 ) {
			TThreshold* temp(new TThreshold(x, y, dacs));
			mThresholds.push_back(temp);
		}
	}
}

void TThresholdAnalyser::refineData() {
	mThresholdDistribution = new TH1D("thrDist", "Threshold Distribution", 500, 0, 50);
	mErrorDistribution = new TH1D("errDist", "Error Distribution", 300, 0, 30);
	mThresholdmap = new TH2D("thrmap", "Threshold map", 1024, -0.5, 1023.5, 512, -0.5, 511.5);
	mChi2NdfDistribution = new TH1D("chi2", "Chi2 and NDoF", 200, 0, 200);
	int nHigh = 0;
	int nLow = 0;
	int nUnDefined = 0;
	for ( const TThreshold* threshold : mThresholds ) {
		if ( threshold->getCondition() == ThrCondition::good ) {
			mThresholdDistribution->Fill(threshold->getThreshold());
			mErrorDistribution->Fill(threshold->getError());
			mThresholdmap->SetBinContent(threshold->getX(), threshold->getY(), threshold->getThreshold());
			mChi2NdfDistribution->Fill(threshold->getQualityFactor());
		} else if ( threshold->getCondition() == ThrCondition::bad_too_high ) {
			nHigh++;
			mThresholdmap->SetBinContent(threshold->getX(), threshold->getY(), 50);
		} else if ( threshold->getCondition() == ThrCondition::bad_too_low ) {
			nLow++;
			mThresholdmap->SetBinContent(threshold->getX(), threshold->getY(), 0);
		} else if ( threshold->getCondition() == ThrCondition::bad_undefine ) {
			nUnDefined++;
		}
	}
	std::cout << mThresholdDistribution->GetMean() << std::endl;
	std::cout << mThresholdDistribution->GetStdDev() << std::endl;
	std::cout << mErrorDistribution->GetMean() << std::endl;
	std::cout << mErrorDistribution->GetStdDev() << std::endl;
	std::cout << mThresholdDistribution->GetEntries() << std::endl;
	std::cout << nLow << std::endl;
	std::cout << nHigh << std::endl;
	std::cout << nUnDefined << std::endl;
}

void TThresholdAnalyser::saveThresholdDistribution(std::string_view title) const {
	TCanvas* can = new TCanvas("thrDist", "Threshold Distribution; Threshold [$500 \times e^-$]; Entry", 1000, 500);
	mThresholdDistribution->Draw();
	can->SaveAs(static_cast<const TString>(title));
}

void TThresholdAnalyser::saveErrorDistribution(std::string_view title) const {
	TCanvas* can = new TCanvas("errDist", "Error Distribution;Threshold [$e^-$]; Entry", 1000, 500);
	mErrorDistribution->Draw();
	can->SaveAs(static_cast<const TString>(title));
}

void TThresholdAnalyser::saveThresholdmap(std::string_view title) const {
	TCanvas* can(new TCanvas("thrmap", "", 1000, 500));
	mThresholdmap->SetTitle("Threshold Distribution; rows; coloums");
	mThresholdmap->SetMinimum(0);
	mThresholdmap->SetMaximum(50);
	mThresholdmap->SetStats(0);
	mThresholdmap->Draw("COLZ0");
	can->SaveAs(static_cast<const TString>(title));
}

void TThresholdAnalyser::saveQualityDistribution(std::string_view title) const {
	TCanvas* can = new TCanvas("chi2", "Fit quality Distribution; Chi2/NDoF; entry", 1000, 500);
	mChi2NdfDistribution->Draw();
	can->SaveAs(static_cast<const TString>(title));
}