#include "TDataPlotter.h"

TDataPlotter::TDataPlotter(const CppConfigFile* config) : TPlotter(config), mConfig(config) {
	std::filesystem::path inputPath = mConfig->getConfig("File").find("input_file");
	mInputFile = new TFile(static_cast<TString>(inputPath));

	mOutputPath = mConfig->getConfig("File").find("output_directory");
	std::filesystem::create_directories(mOutputPath);

	gStyle->SetOptStat(0);

	if ( mConfig->hasConfig("Hitmap") ) isHitmap = true;
	if ( mConfig->hasConfig("Clustermap") ) isClustermap = true;
	if ( mConfig->hasConfig("Clustersize") ) isClustersize = true;
}

TDataPlotter::~TDataPlotter() {

}

void TDataPlotter::InitPlot() {
	if ( isHitmap ) {
		mHitmap = new TH2D("hitmap", "Hitmap; Column; Row", ALPIDECOLUMN, 0, ALPIDECOLUMN, ALPIDEROW, 0, ALPIDEROW);
		setBins(mHitmap, mConfig->getConfig("Hitmap"));
	}
	if ( isClustermap ) {
		mClustermap = new TH2D("clustermap", "Clustermap; Column; Row", ALPIDECOLUMN, 0, ALPIDECOLUMN, ALPIDEROW, 0, ALPIDEROW);
		setBins(mClustermap, mConfig->getConfig("Clustermap"));
	}
	if ( isClustersize ) {
		mClustersize = new TH1D("clustersize", "Cluster size; Cluster size; Entry", 100, .5, 100.5);
		setBins(mClustersize, mConfig->getConfig("Clustersize"));
	}
}

void TDataPlotter::FillHitInfo() {
	UInt_t timeStamp;
	UShort_t x, y;
	TTree* hitTree = static_cast<TTree*>(mInputFile->Get("hit"));
	hitTree->SetBranchAddress("TimeStamp", &timeStamp);
	hitTree->SetBranchAddress("X", &x);
	hitTree->SetBranchAddress("Y", &y);

	Int_t nHit = hitTree->GetEntries();

	for ( int iHit = 0; iHit < nHit; iHit++ ) {
		hitTree->GetEntry(iHit);
		if ( isHitmap ) mHitmap->Fill(x, y);
	}

	for ( int x = 0; x < 1024; x++ ) {
		for ( int y = 0; y < 512; y++ ) {
			if ( mHitmap->GetBinContent(x, y) > 70 ) {
				mHitmap->SetBinContent(x, y, 0);
			}
		}
	}
}

void TDataPlotter::FillClusterInfo() {
	UInt_t timeStamp;
	Double_t x, y;
	UInt_t size;

	TTree* clusterTree = static_cast<TTree*>(mInputFile->Get("cluster"));
	clusterTree->SetBranchAddress("TimeStamp", &timeStamp);
	clusterTree->SetBranchAddress("X", &x);
	clusterTree->SetBranchAddress("Y", &y);
	clusterTree->SetBranchAddress("Size", &size);

	Int_t nCluster = clusterTree->GetEntries();

	for ( int iCluster = 0; iCluster < nCluster; iCluster++ ) {
		clusterTree->GetEntry(iCluster);
		if ( isClustermap ) mClustermap->Fill(x, y);
		if ( isClustersize ) mClustersize->Fill(size);
	}
}

void TDataPlotter::savePlots() {
	if ( isHitmap )	savePlot(mHitmap, "Hitmap");
	if ( isClustermap ) savePlot(mClustermap, "Clustermap");
	if ( isClustersize ) savePlot(mClustersize, "Clustersize");
}