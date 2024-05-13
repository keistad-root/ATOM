#define __TANALYSER_HEADERS__

#include "TAnalyser.h"

TAnalyser::TAnalyser(TFile* inputFile, std::unordered_map<std::string, TExperimentData*> expData) : mInputFile(inputFile), mExpData(expData), fBits(kNotDeleted) {
	std::filesystem::path inputPath = mInputFile->GetName();
	std::clog << "TAnalyser object for \033[1;32m" << inputPath.stem() << "\033[0m is armed" << std::endl;
	mTree = openTree("hit");
	mTree->SetBranchAddress("ChipID", &mInput.chipid);
	mTree->SetBranchAddress("TimeStamp", &mInput.timeStamp);
	mTree->SetBranchAddress("X", &mInput.x);
	mTree->SetBranchAddress("Y", &mInput.y);
	gErrorIgnoreLevel = kWarning;
}

TAnalyser::TAnalyser(const TAnalyser& copy) : mInputFile(copy.mInputFile), mTree(copy.mTree), mInput(copy.mInput), mOutputFile(copy.mOutputFile), mIsOutputGraph(copy.mIsOutputGraph), mExpSettingLegend(copy.mExpSettingLegend), mHitmaps(copy.mHitmaps), mDirectorySet(copy.mDirectorySet), mExpData(copy.mExpData), mDivideData(copy.mDivideData), fBits(kNotDeleted) {
	std::clog << "Copy TAnalyser object is armed" << std::endl;
}

TAnalyser::~TAnalyser() {
	if ( mOutputFile != nullptr && !mOutputFile->IsDestructed() ) {
		mOutputFile->Close();
		mOutputFile = nullptr;
	}
	// for ( const auto& pair : mHitmaps ) {
	// 	if ( !pair.second->IsDestructed() ) {
	// 		delete pair.second;
	// 	}
	// }
	// for ( const auto& pair : mExpData ) {
	// 	if ( !pair.second->IsDestructed() ) {
	// 		delete pair.second;
	// 	}
	// }
	// if ( mInputFile != nullptr && !mInputFile->IsDestructed() ) {
	// 	mInputFile->Close();
	// 	delete mInputFile;
	// 	mInputFile = nullptr;
	// }
	// if ( mTree != nullptr && !mTree->IsDestructed() ) {
	// 	delete mTree;
	// 	mTree = nullptr;
	// }
	// if ( mExpSettingLegend != nullptr && !mExpSettingLegend->IsDestructed() ) {
	// 	delete mExpSettingLegend;
	// 	mExpSettingLegend = nullptr;
	// }
	std::clog << "TAnalyser object is terminated" << std::endl;
}

TTree* TAnalyser::openTree(std::string treeName) {
	if ( mInputFile->Get(static_cast<TString>(treeName)) == nullptr ) {
		std::cerr << "TTree " << treeName << " isn't belong to TFile" << std::endl;
		return nullptr;
	} else if ( std::string(mInputFile->Get(static_cast<TString>(treeName))->ClassName()) != "TTree" ) {
		std::cerr << "" << treeName << " isn't TTree" << std::endl;
		return nullptr;
	} else {
		return (TTree*) mInputFile->Get(static_cast<TString>(treeName));
	}
}

void TAnalyser::openOutputGraphFile(std::string_view fileName) {
	mOutputFile = new TFile(static_cast<TString>(fileName), "RECREATE");
	mIsOutputGraph = true;
}

void TAnalyser::openDirectory(std::string_view typeName) {
	mOutputFile->cd();
	TDirectory* directory = mOutputFile->mkdir(static_cast<TString>(typeName));
	mDirectorySet.insert_or_assign(std::string(typeName), directory);
}

void TAnalyser::storeEvents() {
	std::clog << "Storing Events..." << std::endl;
	std::vector<TALPIDEEvent*> tempEvents;
	UInt_t preTime = 0;
	tempEvents.push_back(new TALPIDEEvent());
	tempEvents.back()->setEvent(0);
	tempEvents.back()->setTime(static_cast<long int>(0));
	ProgressBar* pbar = new ProgressBar(mTree->GetEntries());
	for ( int entry = 0; entry < mTree->GetEntries(); entry++ ) {
		pbar->printProgress();
		mTree->GetEntry(entry);
		if ( mInput.timeStamp == preTime ) {
			tempEvents.back()->pushData({mInput.x, mInput.y});
		} else {
			tempEvents.back()->removeDuplication();
			tempEvents.back()->sortPixel();

			tempEvents.push_back(new TALPIDEEvent());
			tempEvents.back()->setEvent(mInput.timeStamp);
			tempEvents.back()->setTime(mInput.timeStamp);
			tempEvents.back()->pushData({mInput.x, mInput.y});
			preTime = mInput.timeStamp;
		}
	}
	tempEvents.back()->removeDuplication();
	tempEvents.back()->sortPixel();

	delete pbar;
	std::clog << "The " << tempEvents.size() << " of events is extracted from " << mTree->GetEntries() << " stamps." << std::endl;
	mExpData.find("Basic")->second->setEvents(std::move(tempEvents));
}

void TAnalyser::doMasking(int mMaskOver) {
	TMatrix2D<int> matrix(1024, 512);
	for ( int x = 0; x < 1024; x++ ) {
		for ( int y = 0; y < 512; y++ ) {
			if ( mHitmaps.find("Basic")->second->GetBinContent(x, y) > mMaskOver ) {
				matrix.setElement(x - 1, y - 1, 1);
			}
		}
	}

	std::vector<TALPIDEEvent*> tempEvents;
	std::vector<TALPIDEEvent*> noiseEvents;

	for ( const TALPIDEEvent* event : mExpData.find("Basic")->second->getEvents() ) {
		tempEvents.push_back(new TALPIDEEvent());
		tempEvents.back()->setEvent(event->getEvent());
		tempEvents.back()->setTime(event->getTime());
		noiseEvents.push_back(new TALPIDEEvent());
		noiseEvents.back()->setEvent(event->getEvent());
		noiseEvents.back()->setTime(event->getTime());
		for ( const std::pair<int, int>& pixel : event->getData() ) {
			if ( matrix.getElement(pixel.first, pixel.second) != 1 ) {
				tempEvents.back()->pushData(std::move(pixel));
			} else {
				noiseEvents.back()->pushData(std::move(pixel));
			}
		}
	}

	TExperimentData* maskedData = new TExperimentData();
	maskedData->setEvents(std::move(tempEvents));
	mExpData.insert_or_assign("Masked", maskedData);

	TExperimentData* noisePixelData = new TExperimentData();
	noisePixelData->setEvents(std::move(noiseEvents));
	mExpData.insert_or_assign("NoisePixel", noisePixelData);
}

void TAnalyser::setExpSettingLegend(CppConfigDictionary settingConfig) {
	mExpSettingLegend = new TPaveText(.78, .1, .981, .65, "NDC");

	mExpSettingLegend->AddText(static_cast<TString>("VRESETP=" + settingConfig.find("VRESETP") + ", VRESETD=" + settingConfig.find("VRESETD")));
	mExpSettingLegend->AddText(static_cast<TString>("IRESET=" + settingConfig.find("IRESET")));
	mExpSettingLegend->AddText(static_cast<TString>("VPULSEH=" + settingConfig.find("VPULSEH") + ", VPULSEL=" + settingConfig.find("VPULSEL")));
	mExpSettingLegend->AddText(static_cast<TString>("VCASP=" + settingConfig.find("VCASP")));
	mExpSettingLegend->AddText(static_cast<TString>("IBIAS=" + settingConfig.find("IBIAS") + ", IDB=" + settingConfig.find("IDB")));
	mExpSettingLegend->AddText(static_cast<TString>("VCASN=" + settingConfig.find("VCASN") + ", VCASN2=" + settingConfig.find("VCASN2")));
	mExpSettingLegend->AddText(static_cast<TString>("ITHR=" + settingConfig.find("ITHR")));
	mExpSettingLegend->AddText(static_cast<TString>("VCLIP=" + settingConfig.find("VCLIP")));
	mExpSettingLegend->AddText(static_cast<TString>("NTRIG=" + settingConfig.find("NTRIG")));
	mExpSettingLegend->AddText(static_cast<TString>("PULSEDURATION=" + settingConfig.find("PULSEDURATION")));
	mExpSettingLegend->AddText(static_cast<TString>("PULSEDELAY=" + settingConfig.find("PULSEDELAY")));
	mExpSettingLegend->AddText(static_cast<TString>("STROBEDURATION=" + settingConfig.find("STROBEDURATION")));
	mExpSettingLegend->AddText(static_cast<TString>("STROBEDELAYCHIP=" + settingConfig.find("STROBEDELAYCHIP")));
	mExpSettingLegend->SetTextAlign(11);
	mExpSettingLegend->SetTextFont(42);
}

TExperimentData* TAnalyser::getAnEventSet(std::string_view typeName) const {
	if ( mExpData.count(std::string(typeName)) ) {
		return mExpData.find(std::string(typeName))->second;
	} else {
		return new TExperimentData();
	}
}

TH2D* TAnalyser::getHitPlot(const CppConfigDictionary& config, const std::vector<TALPIDEEvent*>& events) {
	// Static variable for numbering
	static int iHMap = 0;
	// Allocate a 2d histogram.
	std::string plotTitle = "";
	if ( config.hasKey("title") ) {
		plotTitle += config.find("title");
	}
	if ( config.hasKey("x_title") ) {
		plotTitle += "; " + config.find("x_title");
	} else {
		plotTitle += "; ";
	}
	if ( config.hasKey("y_title") ) {
		plotTitle += "; " + config.find("y_title");
	} else {
		plotTitle += "; ";
	}

	TH2D* map = new TH2D(Form("hmap%d", iHMap), static_cast<TString>(plotTitle), 1024, 0, 1024, 512, 0, 512);
	// Fill data
	for ( const TALPIDEEvent* event : events ) {
		for ( const std::pair<int, int>& pixel : event->getData() ) {
			map->Fill(pixel.first, pixel.second);
		}
	}
	// Canvas setting
	TCanvas* canvas = new TCanvas(Form("hmapCan%d", iHMap), "", 2500, 1000);
	canvas->SetMargin(.07, .35, .12, .08);
	map->GetXaxis()->SetTitleOffset(1.4);
	map->GetXaxis()->SetLabelOffset(0.003);
	// Find directory for saving clustermap. If it doesn't exist, then make the directory with mother directories.
	std::filesystem::path filePath(config.find("output_path"));
	filePath /= config.find("subdirectory");
	std::filesystem::create_directories(filePath);
	// Draw plot with options
	if ( config.hasKey("options") ) {
		for ( const std::string& optionName : config.getSubConfig("options").getValueList() ) {
			map->Draw(static_cast<TString>(optionName));
			mExpSettingLegend->Draw("SAME");
			std::filesystem::path file = filePath / (config.find("filename") + "_" + optionName);
			if ( config.hasKey("extension") ) {
				file.replace_extension(config.find("extension"));
			} else {
				file.replace_extension("png");
			}
			canvas->SaveAs(static_cast<TString>(file));
		}
	} else {
		map->Draw();
		mExpSettingLegend->Draw("SAME");
		canvas->SaveAs(static_cast<TString>(filePath));
	}
	// Delete canvas;
	delete canvas;
	iHMap++;
	return map;
}

void TAnalyser::saveHitmap(std::string typeName, const CppConfigDictionary& config) {
	std::clog << "Generating \033[1;32m" << typeName << " Hitmap\033[1;0m..." << std::endl;
	if ( !mHitmaps.count(typeName) ) {
		mHitmaps.insert_or_assign(typeName, getHitPlot(config, mExpData.find(typeName)->second->getEvents()));
		if ( mIsOutputGraph ) {
			mDirectorySet.find(std::string(typeName))->second->cd();
			mHitmaps.find(std::string(typeName))->second->Write("hitmap");
			mOutputFile->cd();
		}
	} else {
		getHitPlot(config, mExpData.find(typeName)->second->getEvents());
	}
}

void TAnalyser::doDivideBySize(std::string_view typeName) {
	TClusterDivideData* clusterDivideData = new TClusterDivideData(mExpData.find(std::string(typeName))->second->getClusters());
	mDivideData.insert_or_assign(std::string(typeName), clusterDivideData);
}
