#include "TCompareClustersize.h"

#include<filesystem>
#include<unordered_map>

#include "TH1.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "TPlotter.h"
#include "CppConfigFile.h"
#include "config.h"
#include "TStyle.h"

#include<csv.h>

const std::string experimentInfoCSV = std::string(SOURCE_DIR) + "/build/config/EXPERIMENT_INFORMATION.csv";

TClusterInfo::TClusterInfo(std::string_view tag, const CppConfigDictionary& config) : mTag(tag), mConfig(config) {
	std::string histStr = mConfig.find("HIST");
	if ( histStr.find('-') != std::string::npos ) {
		std::string histName1 = histStr.substr(0, histStr.find('-') - 1);
		mClusterSizeHistogram = setClusterSizeHistogram(histName1);
		std::string histName2 = histStr.substr(histStr.find('-') + 2);
		TH1D* hist2 = setClusterSizeHistogram(histName2);
		mClusterSizeHistogram->Add(hist2, -1);
		if ( mConfig.hasKey("RATIO") ) {
			std::cout << "RATIO: " << mConfig.find("RATIO") << std::endl;
			mClusterSizeHistogram->Scale(stod(mConfig.find("RATIO")));
		}
	} else {
		std::string histName = histStr;
		mClusterSizeHistogram = setClusterSizeHistogram(histName);
		if ( mConfig.hasKey("RATIO") ) {
			std::cout << "RATIO: " << mConfig.find("RATIO") << std::endl;
			mClusterSizeHistogram->Scale(stod(mConfig.find("RATIO")));
		}
	}
	gStyle->SetOptStat(0);
}

TClusterInfo::~TClusterInfo() { }

TH1D* TClusterInfo::setClusterSizeHistogram(std::string_view name) {
	static int iHist = 0;
	io::CSVReader<3> in(experimentInfoCSV);
	in.read_header(io::ignore_extra_column, "TAG", "MASKED_FILE", "CENTER");
	std::string tag, maskedFile, centerStr;
	TH1D* hist = new TH1D(Form("hist_%d", iHist), "", 120, 0.5, 120.5);
	while ( in.read_row(tag, maskedFile, centerStr) ) {
		if ( tag == name ) {
			std::vector<double> center = TPlotter::getDoubleSetFromString(centerStr);
			TFile* file = new TFile(static_cast<TString>(maskedFile), "READ");
			TTree* tree = static_cast<TTree*>(file->Get("cluster"));
			Double_t x, y;
			UInt_t size;
			tree->SetBranchAddress("X", &x);
			tree->SetBranchAddress("Y", &y);
			tree->SetBranchAddress("Size", &size);

			Int_t nCluster = tree->GetEntries();
			for ( int iCluster = 0; iCluster < nCluster; iCluster++ ) {
				tree->GetEntry(iCluster);
				// if ( center[0] - 150 < x && x < center[0] + 150 && center[1] - 10 < y && y < center[1] + 10 ) {
				hist->Fill(size);
				// }
			}
			delete tree;
			delete file;
		}
	}
	iHist++;
	return hist;
}

TCompareClustersize::TCompareClustersize(const CppConfigFile& config) : mConfig(config) {
	mOutputPath = mConfig.getConfig("FILE").find("OUTPUT_DIRECTORY");
	for ( auto& plotConfig : mConfig.getConfig("PLOT_SET").getSubConfigSet() ) {
		TClusterInfo temp(plotConfig.getConfigName(), plotConfig);
		mClusterInfo.push_back(temp);
	}
}

void TCompareClustersize::drawClustersize() {
	TCanvas* canvas = TPlotter::initCanvas(mConfig.getConfig("CLUSTERSIZE"));
	TLegend* legend = TPlotter::initLegend(mConfig.getConfig("CLUSTERSIZE"));
	for ( auto& plot : mClusterInfo ) {
		TPlotter::drawPlot(canvas, plot.getClusterSizeHistogram(), plot.getConfig());
		legend->AddEntry(plot.getClusterSizeHistogram(), static_cast<TString>(plot.getConfig().find("LEGEND")));

		double entry = 0.;
		double stdev = 0.;
		entry = plot.getClusterSizeHistogram()->GetBinContent(1);
		stdev = plot.getClusterSizeHistogram()->GetBinError(1);
		std::cout << entry << ", " << stdev << ", ";

		entry = 0.;
		stdev = 0.;
		for ( int i = 4; i < 33; i++ ) {
			entry += plot.getClusterSizeHistogram()->GetBinContent(i);
			stdev += std::pow(plot.getClusterSizeHistogram()->GetBinError(i), 2);
		}
		std::cout << entry << ", " << std::sqrt(stdev) << ", ";

		entry = 0.;
		stdev = 0.;
		for ( int i = 33; i < 120; i++ ) {
			entry += plot.getClusterSizeHistogram()->GetBinContent(i);
			stdev += std::pow(plot.getClusterSizeHistogram()->GetBinError(i), 2);
		}
		std::cout << entry << ", " << std::sqrt(stdev) << std::endl;
	}

	TPlotter::saveLegend(canvas, legend);
	TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("CLUSTERSIZE"));
}
