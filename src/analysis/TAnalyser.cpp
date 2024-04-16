#include "TAnalyser.h"

TAnalyser::TAnalyser(TFile* inputFile, TExperimentData* expData) : mInputFile(inputFile), mExpData(expData) {
	std::cout << "TAnalyser class is armed" << std::endl;

	mTree = openTree("hit");
	mTree->SetBranchAddress("ChipID", &mInput.chipid);
	mTree->SetBranchAddress("TimeStamp", &mInput.timeStamp);
	mTree->SetBranchAddress("X", &mInput.x);
	mTree->SetBranchAddress("Y", &mInput.y);
}

TAnalyser::TAnalyser(const TAnalyser& copy) : mInputFile(copy.mInputFile), mTree(copy.mTree), mInput(copy.mInput), mHitmap(copy.mHitmap), mExpData(copy.mExpData), mSavePath(copy.mSavePath), mExpSettingLegend(copy.mExpSettingLegend) { }

TAnalyser::~TAnalyser() {
	std::cout << "TAnalyser class is terminated" << std::endl;
	mInputFile->Close();
}

void TAnalyser::storeEvents() {
	std::clog << "Storing Events..." << std::endl;
	std::vector<TALPIDEEvent*> tempEvents;
	UInt_t preTime = 0;
	tempEvents.push_back(new TALPIDEEvent());
	tempEvents.back()->setEvent(0);
	tempEvents.back()->setTime(static_cast<long int>(0));
	for ( int entry = 0; entry < mTree->GetEntries(); entry++ ) {
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
	mExpData->setEvents(std::move(tempEvents)); // Need Check
}

void TAnalyser::saveHitmap() {
	std::clog << "Generating Hitmap..." << std::endl;
	mHitmap = new TH2D("hitmap", "Hitmap; Rows; Columns", 1024, 0, 1024, 512, 0, 512);
	for ( const TALPIDEEvent* event : mExpData->getEvents() ) {
		for ( const std::pair<int, int>& pixel : event->getData() ) {
			mHitmap->Fill(pixel.first, pixel.second); // Fill pixel to hitmap
		}
	}
	TCanvas* canvas = new TCanvas("hitmap", "Hitmap", 2500, 1000);
	canvas->SetMargin(.07, .35, .12, .08);
	mHitmap->GetXaxis()->SetTitleOffset(1.4);
	mHitmap->GetXaxis()->SetLabelOffset(0.003);

	std::filesystem::create_directories(mSavePath / "hitmap");
	mHitmap->Draw("COLZ");
	mExpSettingLegend->Draw("SAME");
	canvas->SaveAs(static_cast<TString>(mSavePath / "hitmap" / "hitmap_colz.png"));
	mHitmap->Draw("LEGO0");
	mExpSettingLegend->Draw("SAME");
	canvas->SaveAs(static_cast<TString>(mSavePath / "hitmap" / "hitmap_lego.png"));
}

void TAnalyser::doMasking(int mMaskOver) {
	TMatrix2D<int> matrix(1024, 512);
	for ( int x = 0; x < 1024; x++ ) {
		for ( int y = 0; y < 512; y++ ) {
			if ( mHitmap->GetBinContent(x, y) > mMaskOver ) {
				matrix.setElement(x - 1, y - 1, 1);
			}
		}
	}

	std::vector<TALPIDEEvent*> tempEvents;
	for ( const TALPIDEEvent* event : mExpData->getEvents() ) {
		tempEvents.push_back(new TALPIDEEvent());
		tempEvents.back()->setEvent(event->getEvent());
		tempEvents.back()->setTime(event->getTime());
		for ( const std::pair<int, int>& pixel : event->getData() ) {
			if ( matrix.getElement(pixel.first, pixel.second) != 1 ) {
				tempEvents.back()->pushData(std::move(pixel));
			}
		}
	}
	mExpData->setMaskedEvents(std::move(tempEvents));
}

void TAnalyser::saveMaskedHitmap() {
	std::clog << "Generating Masked Hitmap..." << std::endl;
	TH2D* map = new TH2D("maskedHitmap", "Masked Hitmap; Rows; Columns", 1024, 0, 1024, 512, 0, 512);
	for ( const TALPIDEEvent* event : mExpData->getMaskedEvents() ) {
		for ( const std::pair<int, int>& pixel : event->getData() ) {
			map->Fill(pixel.first, pixel.second); // Fill pixel to hitmap
		}
	}
	TCanvas* canvas = new TCanvas("maskedHitmap", "Masked Hitmap", 2500, 1000);
	canvas->SetMargin(.07, .35, .12, .08);
	map->GetXaxis()->SetTitleOffset(1.4);
	map->GetXaxis()->SetLabelOffset(0.003);

	std::filesystem::create_directories(mSavePath / "masked_hitmap");
	map->Draw("COLZ");
	mExpSettingLegend->Draw("SAME");
	canvas->SaveAs(static_cast<TString>(mSavePath / "masked_hitmap" / "masked_hitmap_colz.png"));
	map->Draw("LEGO0");
	mExpSettingLegend->Draw("SAME");
	canvas->SaveAs(static_cast<TString>(mSavePath / "masked_hitmap" / "masked_hitmap_lego.png"));
}

void TAnalyser::setSavePath(const std::filesystem::path& savePath) {
	mSavePath = savePath;
}

void TAnalyser::setExpSettingLegend(Configurable settingConfig) {
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