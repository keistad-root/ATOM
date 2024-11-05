#include "TThresholdAnalyser.h"

TThresholdAnalyser::TThresholdAnalyser(const CppConfigFile& config) {
	std::clog << "The threshold analyser is armed" << std::endl;
	mConfig = config;
	CppConfigDictionary fileConfig = mConfig.getConfig("File");
	std::filesystem::path datPath = fileConfig.find("dat_file");
	std::filesystem::path cfgPath = fileConfig.find("cfg_file");

	mDataPath = fileConfig.find("output_file");
	std::filesystem::create_directories(mDataPath.parent_path());

	chipID = TObjString(static_cast<TString>(fileConfig.find("chip_id")));

	mFile = std::ifstream(datPath);
	mCfgFile = std::ifstream(cfgPath);

	mThresholdFile = new TFile(static_cast<TString>(mDataPath), "RECREATE");
	chipID.Write();
	setConfig();
}


TThresholdAnalyser::~TThresholdAnalyser() {
	mThresholdFile->Close();
	std::cout << "The threshold analyser is terminated" << std::endl;
}

void TThresholdAnalyser::getThreshold() {
	std::string line;
	std::array<Int_t, 50> dacs;
	std::streampos originalPos = mFile.tellg();

	int numLines = 0;
	char ch;
	while ( mFile.get(ch) ) {
		if ( ch == '\n' ) {
			++numLines;
		}
	}
	mFile.clear();
	mFile.seekg(originalPos);
	ProgressBar bar(numLines + 1);

	while ( mFile ) {
		bar.printProgress();
		getline(mFile, line);
		std::stringstream values(line);
		int maskStage, pixelInRegion, iDac, dac;
		values >> maskStage >> pixelInRegion >> iDac >> dac;
		dacs[iDac] = dac;
		if ( iDac == 49 ) {
			int x = 2 * maskStage + (pixelInRegion % 2);
			int y = floor(pixelInRegion / 2);
			TThreshold* temp(new TThreshold(x, y, dacs));
			mThresholds.push_back(temp);
		}
	}
}

void TThresholdAnalyser::setConfig() {
	std::string line;
	while ( mCfgFile ) {
		std::getline(mCfgFile, line);
		if ( line != "" ) {
			std::stringstream values(line);
			std::string key, value;
			values >> key >> value;
			mSetting.insert_or_assign(key, stoi(value));
		}
	}
	TTree* configTree = new TTree("config", "Configuration");
	TString configName;
	Int_t configValue;
	configTree->Branch("name", &configName);
	configTree->Branch("value", &configValue);

	for ( const auto& setting : mSetting ) {
		configName = setting.first;
		configValue = setting.second;
		configTree->Fill();
	}
	mThresholdFile->cd();
	configTree->Write();
}

void TThresholdAnalyser::saveThresholdData() {
	TTree* thresholdTree = new TTree("threshold", "Threshold Data");
	Int_t x, y;
	Double_t threshold, noise, qualityFactor;

	thresholdTree->Branch("x", &x);
	thresholdTree->Branch("y", &y);
	thresholdTree->Branch("threshold", &threshold);
	thresholdTree->Branch("noise", &noise);
	thresholdTree->Branch("q_factor", &qualityFactor);

	for ( const TThreshold* thr : mThresholds ) {
		if ( thr->getCondition() == ThrCondition::good ) {
			x = thr->getX();
			y = thr->getY();
			threshold = thr->getThreshold();
			noise = thr->getError();
			qualityFactor = thr->getQualityFactor();
			thresholdTree->Fill();
			// if ( qualityFactor > 10 ) {
			// 	thr->savePlot(mDataPath.parent_path());
			// }
		}
	}
	mThresholdFile->cd();
	thresholdTree->Write();
}