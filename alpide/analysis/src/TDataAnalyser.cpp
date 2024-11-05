#include "TDataAnalyser.h"

TDataAnalyser::TDataAnalyser(CppConfigFile* config) : mConfig(config) {
	std::filesystem::path inputPath = mConfig->getConfig("File").find("input_file");

	std::clog << "TAnalyser object for \033[1;32m" << inputPath.stem() << "\033[0m is armed" << std::endl;
	mInputFile = new TFile(static_cast<TString>(inputPath));

	std::filesystem::path outputPath = mConfig->getConfig("File").find("output_file");
	mOutputFile = new TFile(static_cast<TString>(outputPath), "RECREATE");
}

TDataAnalyser::~TDataAnalyser() {
	mOutputFile->Close();
}

void TDataAnalyser::extractEvent() {
	std::clog << "Extracting Events..." << std::endl;

	UInt_t timeStamp;
	UShort_t x, y;

	TTree* inputTree = static_cast<TTree*>(mInputFile->Get("hit"));
	inputTree->SetBranchAddress("TimeStamp", &timeStamp);
	inputTree->SetBranchAddress("X", &x);
	inputTree->SetBranchAddress("Y", &y);

	Int_t nHit = inputTree->GetEntries();
	UInt_t preTime = 0;
	UInt_t preEvt = 0;

	mEventSet.push_back(new TALPIDEEvent());
	mEventSet.back()->setEvent(0);
	mEventSet.back()->setTime(static_cast<long int>(0));
	ProgressBar* pbar = new ProgressBar(nHit);
	for ( int iHit = 0; iHit < nHit; iHit++ ) {
		inputTree->GetEntry(iHit);
		if ( timeStamp == preTime ) {
			mEventSet.back()->pushData({x, y});
		} else {
			mEventSet.back()->removeDuplication();
			mEventSet.back()->sortPixel();
			mEventSet.push_back(new TALPIDEEvent());
			mEventSet.back()->setEvent(preEvt + 1);
			mEventSet.back()->setTime(timeStamp);
			preTime = timeStamp;
			preEvt++;
		}
		pbar->printProgress();
	}
	mEventSet.back()->removeDuplication();
	mEventSet.back()->sortPixel();

	mInputFile->Close();
}

void TDataAnalyser::excludeHotPixel() {
	for ( int X = 0; X < 1024; X++ ) {
		for ( int Y = 0; Y < 512; Y++ ) {
			mMaskingMap[X][Y] = false;
		}
	}
	std::filesystem::path maskingPath = mConfig->getConfig("File").find("mask_file");
	std::ifstream maskingFile(maskingPath);

	std::string maskingStr;
	int x, y;
	while ( getline(maskingFile, maskingStr) ) {
		std::stringstream str(maskingStr);
		str >> x >> y;
		mMaskingMap[x][y] = true;
	}
	for ( TALPIDEEvent* event : mEventSet ) {
		for ( const std::pair<int, int>& pixel : event->getData() ) {
			if ( mMaskingMap[pixel.first][pixel.second] ) {
				event->removePixel(pixel);
			}
		}
	}
	maskingFile.close();
}

void TDataAnalyser::extractHotPixel() {
	std::array<std::array<int, 1024>, 512> map = {0, };
	for ( TALPIDEEvent* event : mEventSet ) {
		for ( const std::pair<int, int>& pixel : event->getData() ) {
			map[pixel.second][pixel.first]++;
		}
	}

	std::filesystem::path hotPixelFilePath = mConfig->getConfig("File").find("hot_pixel_file");
	std::filesystem::create_directories(hotPixelFilePath.parent_path());
	std::ofstream hotPixelFile(hotPixelFilePath);

	int hitCut = stoi(mConfig->getConfig("Masking").find("hit_cut"));
	for ( int y = 0; y < 512; y++ ) {
		for ( int x = 0; x < 1024; x++ ) {
			if ( map[y][x] > hitCut ) {
				hotPixelFile << x << " " << y << std::endl;
			}
		}
	}
	hotPixelFile.close();
}

void TDataAnalyser::extractCluster() {
	TClusterization cluster(mEventSet);
	cluster.clusterize();
	mClusterSet = cluster.getClusters();
}

void TDataAnalyser::saveEvent() {
	TTree* hitTree = new TTree("hit", "hit");

	UInt_t timeStamp;
	UShort_t x, y;
	hitTree->Branch("TimeStamp", &timeStamp);
	hitTree->Branch("X", &x);
	hitTree->Branch("Y", &y);

	for ( const TALPIDEEvent* event : mEventSet ) {
		timeStamp = static_cast<UInt_t>(event->getTime());
		for ( const std::pair<int, int>& pixel : event->getData() ) {
			x = static_cast<UShort_t>(pixel.first);
			y = static_cast<UShort_t>(pixel.second);
			hitTree->Fill();
		}
	}
	mOutputFile->cd();
	hitTree->Write();
}

void TDataAnalyser::saveCluster() {
	TTree* clusterTree = new TTree("cluster", "cluster");

	UInt_t timeStamp;
	Double_t x;
	Double_t y;
	UInt_t size;

	clusterTree->Branch("TimeStamp", &timeStamp);
	clusterTree->Branch("X", &x);
	clusterTree->Branch("Y", &y);
	clusterTree->Branch("Size", &size);

	for ( const TCluster* cluster : mClusterSet ) {
		timeStamp = cluster->getTimeStamp();
		x = cluster->getCenter().first;
		y = cluster->getCenter().second;
		size = cluster->getSize();
		clusterTree->Fill();
	}

	mOutputFile->cd();
	clusterTree->Write();
}