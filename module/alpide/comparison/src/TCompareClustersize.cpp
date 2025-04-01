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
#include "TMath.h"

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
			mClusterSizeHistogram->Scale(stod(mConfig.find("RATIO")));
		}
	} else {
		std::string histName = histStr;
		mClusterSizeHistogram = setClusterSizeHistogram(histName);
		if ( mConfig.hasKey("RATIO") ) {
			mClusterSizeHistogram->Scale(stod(mConfig.find("RATIO")));
		}
	}
	gStyle->SetOptStat(0);
}

TClusterInfo::~TClusterInfo() { }

bool TClusterInfo::isInsideRegion(const std::vector<double>& roi, double roiTheta, double x, double y) {

	double cosTheta = std::cos(roiTheta);
	double sinTheta = std::sin(roiTheta);

	std::pair<double, double> p1 = {-roi[0] * cosTheta - roi[1] * sinTheta, -roi[0] * sinTheta + roi[1] * cosTheta};
	std::pair<double, double> p2 = {roi[0] * cosTheta - roi[1] * sinTheta, roi[0] * sinTheta + roi[1] * cosTheta};
	std::pair<double, double> p3 = {roi[0] * cosTheta + roi[1] * sinTheta, roi[0] * sinTheta - roi[1] * cosTheta};
	std::pair<double, double> p4 = {-roi[0] * cosTheta + roi[1] * sinTheta, -roi[0] * sinTheta - roi[1] * cosTheta};

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
	} else if ( cross1 < 0 && cross2 < 0 && cross3 < 0 && cross4 < 0 ) { return true; } else {
		return false;
	}
}

TH1D* TClusterInfo::setClusterSizeHistogram(std::string_view name) {
	static int iHist = 0;
	io::CSVReader<5> in(experimentInfoCSV);
	in.read_header(io::ignore_extra_column, "TAG", "MASKED_FILE", "CENTER", "ROI", "ROI_THETA");
	std::string tag, maskedFile, centerStr, roiStr, roiThetaStr;
	TH1D* hist = new TH1D(Form("hist_%d", iHist), "", 120, 0.5, 120.5);
	while ( in.read_row(tag, maskedFile, centerStr, roiStr, roiThetaStr) ) {
		if ( tag == name ) {
			std::vector<double> center = TPlotter::getDoubleSetFromString(centerStr);

			std::vector<double> roi = TPlotter::getDoubleSetFromString(roiStr);
			double roiTheta = stod(roiThetaStr) * TMath::Pi() / 180;
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
				if ( isInsideRegion(roi, roiTheta, x - center[0], y - center[1]) ) {
					hist->Fill(size);
				}
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
