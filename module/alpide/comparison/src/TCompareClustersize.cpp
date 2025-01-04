#include "TCompareClustersize.h"


TCompareClustersize::TCompareClustersize(const CppConfigFile& config) : TPlotter(), mConfig(config) {
	const CppConfigDictionary fileList = mConfig.getConfig("FileList");
	for ( const std::string& file : fileList.getKeyList() ) {
		const std::string& path = fileList.find(file);
		mGraphFileSet.insert_or_assign(file, getClustersize(path));
	}
	mPlotDictionary = mConfig.getConfig("Clustersize").getSubConfig("plots").getSubConfigSet();
	mRegionDictionary = mConfig.getConfig("Regions");
	mOutputPath = mConfig.getConfig("File").find("output_directory");
	setGraphSet();
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

void TCompareClustersize::setGraphSet() {
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
		mGraphSet.push_back({plot, drawHist});
		if ( plot.hasKey("ratio") ) {
			drawHist->Scale(stod(plot.find("ratio")));
		}
	}
}

void TCompareClustersize::drawClustersize() {
	TCanvas* canvas = new TCanvas("canvas", "", 1500, 1000);
	TLegend* legend;
	addLegend(canvas, legend, mConfig.getConfig("Clustersize"));
	for ( const auto& plot : mGraphSet ) {
		setAttribute(plot.second, plot.first);
		drawPlot(canvas, plot.second, "SAME HISTE");
		legend->AddEntry(plot.second, static_cast<TString>(plot.first.find("legend")));
		for ( int i = 3; i < 33; i++ ) {
			double y_total = plot.second->GetBinContent(i) + plot.second->GetBinContent(i + 1) + plot.second->GetBinContent(i + 2);
			// double y_total = plot.second->GetBinContent(i - 1) + plot.second->GetBinContent(i) + plot.second->GetBinContent(i + 1) + plot.second->GetBinContent(i + 2) + plot.second->GetBinContent(i + 3);
			double y_value = plot.second->GetBinContent(i + 1);
			std::cout.precision(3);
			std::cout << y_value / y_total << "\t";
		}
		std::cout << std::endl;
	}
	setCanvasAttribute(canvas, mConfig.getConfig("Clustersize"));
	saveLegend(canvas, legend);
	saveCanvas(canvas, mOutputPath, mConfig.getConfig("Clustersize"));
}

void TCompareClustersize::drawRegion() {
	std::vector<CppConfigDictionary> regionConfig = mRegionDictionary.getSubConfig("region").getSubConfigSet();
	int nRegion = regionConfig.size();

	std::vector<std::pair<int, int>> regionList;
	std::vector<TGraphErrors*> entryGraph;

	for ( int i = 0; i < nRegion; i++ ) {
		entryGraph.push_back(new TGraphErrors());
		std::string region = regionConfig[i].find("range");
		std::vector<int> range = getIntegerSetFromString(region);

		regionList.push_back(std::make_pair(range[0], range[1]));
	}

	for ( const auto& graph : mGraphSet ) {
		TH1D* drawHist = graph.second;
		std::vector<double> entrySet;
		double area = graph.first.hasKey("area") ? stod(graph.first.find("area")) : 1;
		double areaError = graph.first.hasKey("area_error") ? stod(graph.first.find("area_error")) : 0;
		double ratio = graph.first.hasKey("ratio") ? stod(graph.first.find("ratio")) : 1;
		for ( int i = 0; i < nRegion; i++ ) {
			double entry = 0;
			for ( int j = regionList[i].first; j < regionList[i].second + 1; j++ ) {
				entry += drawHist->GetBinContent(j + 1);
			}
			entryGraph[i]->SetPoint(entryGraph[i]->GetN(), area, entry);
			entryGraph[i]->SetPointError(entryGraph[i]->GetN() - 1, areaError, std::sqrt(entry / ratio) * ratio);
			entrySet.push_back(entry);
			std::cout << "Region " << regionList[i].first << "," << regionList[i].second << " : " << entry << std::endl;
		}
	}

	TMultiGraph* mg = new TMultiGraph();
	TCanvas* canvas = new TCanvas("region", "", 1500, 1000);
	TLegend* legend;
	addLegend(canvas, legend, mRegionDictionary);
	for ( int i = 0; i < nRegion; i++ ) {
		setAttribute(entryGraph[i], regionConfig[i]);
		legend->AddEntry(entryGraph[i], static_cast<TString>(regionConfig[i].find("legend")));
		mg->Add(entryGraph[i]);
	}

	drawPlot(canvas, mg, "AP");
	setCanvasAttribute(canvas, mRegionDictionary);
	saveLegend(canvas, legend);
	saveCanvas(canvas, mOutputPath, mRegionDictionary);
}