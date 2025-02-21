#include "TCompareClustersize.h"
#include<filesystem>
#include<unordered_map>

#include "TH1.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"

#include "TLegend.h"

#include "TPlotter.h"
#include "CppConfigFile.h"
#include "config.h"
#include<csv.h>

const std::string experimentInfoCSV = std::string(SOURCE_DIR) + "/build/config/EXPERIMENT_INFORMATION.csv";

TClusterInfo::TClusterInfo(std::string_view tag, const CppConfigDictionary& config) : mTag(tag), mConfig(config) {
	std::string histStr = mConfig.find("hist");
	if ( histStr.find('-') != std::string::npos ) {
		std::string histName1 = histStr.substr(0, histStr.find('-') - 1);
		mClusterSizeHistogram = setClusterSizeHistogram(histName1);
		std::string histName2 = histStr.substr(histStr.find('-') + 2);
		TH1D* hist2 = setClusterSizeHistogram(histName2);
		mClusterSizeHistogram->Add(hist2, -1);
		if ( mConfig.hasKey("ratio") ) {
			mClusterSizeHistogram->Scale(stod(mConfig.find("ratio")));
		}
	} else {
		std::string histName = histStr;
		mClusterSizeHistogram = setClusterSizeHistogram(histName);
		if ( mConfig.hasKey("ratio") ) {
			mClusterSizeHistogram->Scale(stod(mConfig.find("ratio")));
		}
	}
}

TClusterInfo::~TClusterInfo() { }

TH1D* TClusterInfo::setClusterSizeHistogram(std::string_view name) {
	io::CSVReader<2> in(experimentInfoCSV);
	in.read_header(io::ignore_extra_column, "TAG", "MASKED_FILE");
	std::string tag, maskedFile;
	TH1D* hist = new TH1D(static_cast<TString>(name), "", 120, 0.5, 120.5);
	while ( in.read_row(tag, maskedFile) ) {
		if ( tag == name ) {
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
				hist->Fill(size);
			}
			delete tree;
			delete file;
		}
	}
	return hist;
}

TCompareClustersize::TCompareClustersize(const CppConfigFile& config) : TPlotter(), mConfig(config) {
	mOutputPath = mConfig.getConfig("FILE").find("output_directory");
	std::filesystem::create_directories(mOutputPath);
	for ( auto& plotConfig : mConfig.getConfig("PLOT_SET").getSubConfigSet() ) {
		TClusterInfo temp(plotConfig.getConfigName(), plotConfig);
		mClusterInfo.push_back(temp);
	}
}

void TCompareClustersize::drawClustersize() {
	TCanvas* canvas;
	TLegend* legend;
	initCanvas(canvas, mConfig.getConfig("Clustersize"));
	initLegend(legend, mConfig.getConfig("Clustersize"));
	for ( auto& plot : mClusterInfo ) {
		setAttribute(plot.getClusterSizeHistogram(), plot.getConfig());
		drawPlot(canvas, plot.getClusterSizeHistogram(), plot.getConfig(), "SAME HISTE");
		legend->AddEntry(plot.getClusterSizeHistogram(), static_cast<TString>(plot.getConfig().find("legend")));
	}
	saveLegend(canvas, legend);
	saveCanvas(canvas, mOutputPath, mConfig.getConfig("Clustersize"));
}
