#include "TCompareClustersize.h"


TCompareClustersize::TCompareClustersize(const CppConfigFile* config) : TPlotter(config) {
	const CppConfigDictionary fileList = config->getConfig("FileList");
	for ( const std::string& file : fileList.getKeyList() ) {
		const std::string& path = fileList.find(file);
		mGraphFileSet.insert_or_assign(file, getClustersize(path));
	}


	mPlotDictionary = config->getConfig("Clustersize").getSubConfig("plots").getSubConfigSet();
}

TH1D* TCompareClustersize::getClustersize(const std::string& filePath) {
	static int i = 0;
	TH1D* clustersize = new TH1D(static_cast<TString>("graph" + std::to_string(i)), "", 120, 0, 120);
	i++;
	TFile* file = new TFile(static_cast<TString>(filePath), "READ");
	TTree* tree = static_cast<TTree*>(file->Get("cluster"));

	UInt_t size;
	tree->SetBranchAddress("Size", &size);

	Int_t nCluster = tree->GetEntries();

	for ( int iCluster = 0; iCluster < nCluster; iCluster++ ) {
		tree->GetEntry(iCluster);
		clustersize->Fill(size);
	}

	delete tree;
	delete file;
	return clustersize;
}

TH1D* TCompareClustersize::subtractClustersize(const std::string& graph1, const std::string& graph2) {
	TH1D* alphaGraph = mGraphFileSet.find(graph1)->second;
	TH1D* gammaGraph = mGraphFileSet.find(graph2)->second;

	TH1D* returnGraph = alphaGraph;
	returnGraph->Add(gammaGraph, -1);

	return returnGraph;
}


void TCompareClustersize::drawClustersize() {
	TCanvas* canvas = new TCanvas("canvas", "", 1500, 1000);
	for ( const CppConfigDictionary& plot : mPlotDictionary ) {
		TH1D* drawHist;
		if ( plot.find("hist").find('-') != std::string::npos ) {
			std::string graph1 = plot.find("hist").substr(0, plot.find("hist").find('-') - 1);
			std::string graph2 = plot.find("hist").substr(plot.find("hist").find('-') + 2);
			drawHist = subtractClustersize(graph1, graph2);
		} else {
			std::string graph1 = plot.find("hist");
			drawHist = mGraphFileSet.find(graph1)->second;
		}
		int sum = 0;
		for ( int i = 10; i < 21; i++ ) {
			sum += drawHist->GetBinContent(i);
		}
		if ( plot.hasKey("ratio") ) {
			drawHist->Scale(stod(plot.find("ratio")));
			std::cout << sum * stod(plot.find("ratio")) << "\t\t" << sqrt(sum) * stod(plot.find("ratio")) << std::endl;
		} else {
			std::cout << sum << "\t\t" << sqrt(sum) << std::endl;

		}
		savePlot(canvas, drawHist, plot);

	}

	saveCanvas(canvas, getOutputPath(), getMainConfig()->getConfig("Clustersize"));
}
