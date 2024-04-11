#include "TClusterAnalyser.h"

/**
 * @brief Construct a new TClusterAnalyser::TClusterAnalyser object
 * @details It takes ROOT file as parameter and send it to the mother class, TAnalyser.
 * The tree which name is hit is taken from the mother class and stored it in data member `mTree`.
 * The branches stored in `mTree` is pointed by `openRootFile()` function and the data are pushed back to `mEvents` variable.
 * @param file
 * @warning The type of input file is pointer of TFile object.
 * @bug none
 * @todo none
 * @see openRootFile(), storedRawEvent() are called in this function
 */
TClusterAnalyser::TClusterAnalyser(TFile* file) : TAnalyser(file) {
	std::cout << "TClusterAnalyser object is armed." << std::endl;
	mTree = openTree("hit"); // Store `hit` tree to data member.
	openRootFile(); // Set Branche Addresses
	storeRawEvents(); // Get event datas and stored
}

/**
 * @brief Construct a new TClusterAnalyser::TClusterAnalyser object
 * @details This is created for controlling cluster analysis stream by config file.
 * The ROOT file and config file is sent to the mother class.
 * According to the config file list, frame of plots are generated and store to `mPlots` variable.
 * @param conf
 * @warning The config file should be written proper way. If it is wrong the unknown errors will occur.
 * @bug Yet
 * @todo Add more plot frame if needed.
 * @see
 */
TClusterAnalyser::TClusterAnalyser(Configuration* conf) : TAnalyser(conf->getConfig("File").value().find("input_file")) {
	setConfFile(conf); // Storing config file to mother class.
	if ( getConfFile()->getConfig("Hitmap").has_value() ) { // Hitmap
		TH2D* hitmap = new TH2D("hitmap", "Hitmap", 1024, 0, 1024, 512, 0, 512);
		mPlots.insert_or_assign("hitmap", hitmap);
		if ( getConfFile()->getConfig("Masking").has_value() ) { // Masked hitmap
			TH2D* maskedHitmap = new TH2D("maskedHitmap", "Masked Hitmap", 1024, 0, 1024, 512, 0, 512);
			mPlots.insert_or_assign("maskedHitmap", maskedHitmap);
		}
	}
	if ( getConfFile()->getConfig("Clustermap").has_value() ) { // Clustermap
		TH2D* clustermap = new TH2D("clustermap", "Clustermap", 1024, 0, 1024, 512, 0, 512);
		mPlots.insert_or_assign("clustermap", clustermap);
		if ( getConfFile()->getConfig("Masking").has_value() ) { // Masked clustermap
			TH2D* maskedClustermap = new TH2D("maskedClustermap", "Masked Clustermap", 1024, 0, 1024, 512, 0, 512);
			mPlots.insert_or_assign("maskedClustermap", maskedClustermap);
		}
	}
	if ( getConfFile()->getConfig("Clustersize").has_value() ) { // Clustersize
		TH1D* clustersize = new TH1D("clustersize", "Cluster Size", 80, .5, 80.5);
		mPlots.insert_or_assign("clustersize", clustersize);
		if ( getConfFile()->getConfig("Masking").has_value() ) { // Masked clustersize
			TH1D* maskedClustersize = new TH1D("maskedClustersize", "Masked Cluster Size", 80, .5, 80.5);
			mPlots.insert_or_assign("maskedClustersize", maskedClustersize);
		}
	}
}

/**
 * @brief Destroy the TClusterAnalyser::TClusterAnalyser object
 * @details mEvents, mMaskedEvents, mClusters, mMaskedClusters data members are deleted.
 * If additional data members are allocated dynamically, it should be deleted here.
 * @warning The objects allocated dynamically should be deleted.
 * @bug
 * @todo Find missing data member should be deleted.
 */
TClusterAnalyser::~TClusterAnalyser() {
	for ( auto& event : mEvents ) {
		delete event;
		event = nullptr;
	}
	for ( auto& event : mMaskedEvents ) {
		delete event;
		event = nullptr;
	}
	for ( auto& event : mClusters ) {
		delete event;
		event = nullptr;
	}
	for ( auto& event : mMaskedClusters ) {
		delete event;
		event = nullptr;
	}
	std::cout << "TClusterAnalyser object is terminated." << std::endl;
}

/**
 * @brief open ROOT File
 * @details The root file and its trees are stored in mother class, TAnalyser.
 * In this function, the indicator of data sets about ALPIDE using MOSAIC board is pointed.
 * The data sturcture has a tree that name is "hit" and contains 4 branches, "ChipID", "TimeStamp", "X" and "Y" which types are Int_t.
 * @return void
 * @warning Check input file have proper extension and data structure.
 * @todo none
 */
void TClusterAnalyser::openRootFile() {
	mTree = openTree("hit");
	mTree->SetBranchAddress("ChipID", &mInput.chipid);
	mTree->SetBranchAddress("TimeStamp", &mInput.timeStamp);
	mTree->SetBranchAddress("X", &mInput.x);
	mTree->SetBranchAddress("Y", &mInput.y);
}

/**
 * @brief Extract events data.
 * @details Extract events data from ROOT file and store it to the member `mEvents`.
 * Data which have same event number and time stamp are considered as including same event.
 * @return void
 * @warning
 * @todo
 */
void TClusterAnalyser::storeRawEvents() {
	UInt_t preTime = 0; // Storing previous time for classifying the fired pixels are in same events.

	TALPIDEEvent* tempALPIDE = new TALPIDEEvent(); // Temporary event to store pixels which event numbers are same.
	// Save event number and time stamp of first data.
	tempALPIDE->setEvent(0);
	tempALPIDE->setTime((long int) 0);
	// Stack data of all events by for loop
	for ( int entry = 0; entry < mTree->GetEntries(); entry++ ) {
		mTree->GetEntry(entry); // Set a order of entry for `mTree` give values to pointed variable.
		if ( mInput.timeStamp != preTime ) { // In case that the current entry isn't belong to same event with previous entry
			// For recording a previous event.
			tempALPIDE->removeDuplication(); // Sometimes some data are recorded several times in event. Remove them.
			tempALPIDE->sortPixel(); // Sorting pixels in ascending order of y and x. Y is first condition in this case. It is necessary to clusterize data effectively.
			mEvents.push_back(tempALPIDE); // Add temparory event pointer to `mEvents` data member.
			tempALPIDE = nullptr; // clear the address of temporary. DO NOT USE DELETE. It will remove storing data in `mEvents`.

			// For making temporary event to recorde a current event.
			tempALPIDE = new TALPIDEEvent();
			tempALPIDE->setEvent(mInput.timeStamp); // Store event number of current event
			tempALPIDE->setTime(mInput.timeStamp); // Store time stamp of current event
			tempALPIDE->pushData({mInput.x, mInput.y}); // Store first pixel recording on current event.
			preTime = mInput.timeStamp; // Reset previous time
		} else { // In case that the current entry is belong to same event with previous entry
			tempALPIDE->pushData({mInput.x, mInput.y}); // Store pixel recording on current temporary event.
		}
	}
}

/**
 * @brief Making plots from raw and masked data and clustering
 * @details The plots for hitmap, clustermap, clustersize are filled here.
 * The plots stored in mPlots data member are filled.
 * The clusterizing process is performed here if necessary.
 * The valid plots are hitmap, clustermap, clustersize, masked hitmap, masked clustermap, masked clustersize and shape infos.
 * @return void
 * @warning
 * @todo Add more plots if necessary.
 */
void TClusterAnalyser::refineData() {
	// Fill hitmap from `mEvents` data member
	if ( mPlots.count("hitmap") ) {
		TH2D* hitmap = static_cast<TH2D*>(mPlots.find("hitmap")->second); // Point to `hitmap` plots in `mPlots` member.
		for ( const TALPIDEEvent* event : mEvents ) {
			for ( const std::pair<int, int>& pixel : event->getData() ) {
				hitmap->Fill(pixel.first, pixel.second); // Fill pixel to hitmap
			}
		}
		hitmap = nullptr; // Point out the pointer to prevent mistakes.
	}
	// Fill clustermap or clustersize from `mClusters` data member calculated by TClusterization object.
	if ( mPlots.count("clustermap") || mPlots.count("clustersize") ) {
		// The raw data should be clusterized for ploting clustermap or clustersize distribution
		TClusterization cluster(mEvents);
		cluster.clusterize();
		mClusters = cluster.getClusters(); // Store clusterizing results to `mClusters` data member
		// Fill clustermap from 'mClusters` data member.
		if ( mPlots.count("clustermap") ) {
			TH2D* clustermap = static_cast<TH2D*>(mPlots.find("clustermap")->second); // Point to `clustermap` plots in `mPlots` member.
			for ( const TCluster* cluster : mClusters ) {
				clustermap->Fill(cluster->getCenter().first, cluster->getCenter().second); // Fill cluster center coordinate to clustermap
			}
			clustermap = nullptr; // Point out the pointer to prevent mistakes.
		}
		// Fill clustersize from 'mClusters` data member.
		if ( mPlots.count("clustersize") ) {
			TH1D* clustersize = static_cast<TH1D*>(mPlots.find("clustersize")->second); // Point to `clustersize` plots in `mPlots` member.
			for ( const TCluster* cluster : mClusters ) {
				clustersize->Fill(cluster->getSize()); // Fill clustersize to clustersize distribution
			}
			clustersize = nullptr; // Point out the pointer to prevent mistakes.
		}
	}
	// Fill masked hitmap from `mMaskedEvents` data member
	if ( mPlots.count("maskedHitmap") || mPlots.count("maskedClustermap") || mPlots.count("maskedClustersize") ) {
		setMask(stoi(getConfFile()->getConfig("Masking").value().find("cut"))); // Set masking options. The cut is invoked from config file.
		masking(); // Do masking. Copy unmasked data from `mEvents` to `mMaskedEvents` variable
		if ( mPlots.count("maskedHitmap") ) {
			TH2D* maskedHitmap = static_cast<TH2D*>(mPlots.find("maskedHitmap")->second);  // Point to `maskedJitmap` plots in `mPlots` member.
			for ( const TALPIDEEvent* event : mMaskedEvents ) {
				for ( const std::pair<int, int>& pixel : event->getData() ) {
					maskedHitmap->Fill(pixel.first, pixel.second); // Fill pixel to masked hitmap
				}
			}
			maskedHitmap = nullptr; // Point out the pointer to prevent mistakes.
		}
		// Fill masked clustermap or masked clustersize from `mMaskedClusters` data member calculated by TClusterization object.
		if ( mPlots.count("maskedClustermap") || mPlots.count("maskedClustersize") ) {
			// The masked data should be clusterized for ploting masked clustermap or masked clustersize distribution
			TClusterization maskedCluster(mMaskedEvents);
			maskedCluster.clusterize();
			mMaskedClusters = maskedCluster.getClusters(); // Store clusterizing results to `mMaskedClusters` data member
			// Fill masked clustermap from 'mMaskedClusters` data member.
			if ( mPlots.count("maskedClustermap") ) {
				TH2D* maskedClustermap = static_cast<TH2D*>(mPlots.find("maskedClustermap")->second); // Point to `maskedClustermap` plots in `mPlots` member.
				for ( const TCluster* maskedCluster : mMaskedClusters ) {
					maskedClustermap->Fill(maskedCluster->getCenter().first, maskedCluster->getCenter().second); // Fill cluster center coordinate of masked clsuter to masked clustermap
				}
				maskedClustermap = nullptr; // Point out the pointer to prevent mistakes.
			}
			// Fill masked clustersize from 'mMaskedClusters` data member.
			if ( mPlots.count("maskedClustersize") ) {
				TH1D* maskedClustersize = static_cast<TH1D*>(mPlots.find("maskedClustersize")->second); // Point to `maskedClustersize` plots in `mPlots` member.
				for ( const TCluster* maskedCluster : mMaskedClusters ) {
					maskedClustersize->Fill(maskedCluster->getSize()); // Fill clustersize to clustersize distribution
				}
				maskedClustersize = nullptr; // Point out the pointer to prevent mistakes.
			}
		}
	}
	// Cluster shape doesn't generate or fill plots here. But using `TClusterN` class the analysis is performed there.
	if ( getConfFile()->getConfig("Shape").has_value() ) {
		for ( int clusterSize = stoi(getConfFile()->getConfig("Shape").value().find("min_cut")); clusterSize < stoi(getConfFile()->getConfig("Shape").value().find("max_cut")); clusterSize++ ) { // For looking shape, cut is important because cluster size 1 or 2 omit meanless results.
			if ( static_cast<TH1D*>(mPlots.find("maskedClustersize")->second)->GetBinContent(clusterSize) ) {
				TClusterN* clusterN = new TClusterN(mMaskedClusters, clusterSize); // define clusterN object
				clusterN->calShapeSpecies(); // classify the cluster shape. It is same if they have reverse or rotatinal reverse symmetry.
				mClusterN.push_back(clusterN); // add clusterN object to mClusterN data member because of making plots to other function.
			}
		}
	}
}

/**
 * @brief Setting Masking option
 * @details Setting whether to mask.
 * If this function is called, then `mIsMask` member is converted to true.
 *
 * @param hot Cut value for masking
 * @return void
 * @warning This function should be called for calling masking function
 * @todo Determine if summarizing it or not
 */
void TClusterAnalyser::setMask(int hot) {
	mIsMask = true;
	mMaskOver = hot;
}

/**
 * @brief Set masking
 *
 */
void TClusterAnalyser::masking() {
	TH2D* hitmap;

	if ( mPlots.count("hitmap") ) {
		hitmap = static_cast<TH2D*>(mPlots.find("hitmap")->second);
	} else {
		hitmap = new TH2D("hitmap", "Hitmap", 1024, 0, 1024, 512, 0, 512);
		for ( const TALPIDEEvent* event : mEvents ) {
			for ( const std::pair<int, int>& pixel : event->getData() ) {
				hitmap->Fill(pixel.first, pixel.second);
			}
		}
		hitmap = nullptr;
	}
	for ( int x = 0; x < 1024; x++ ) {
		for ( int y = 0; y < 512; y++ ) {
			if ( hitmap->GetBinContent(x, y) > mMaskOver )
				mHotPixels.push_back({x - 1, y - 1});
		}
	}
	for ( const TALPIDEEvent* mEvent : mEvents ) {
		TALPIDEEvent* tempEvent = new TALPIDEEvent();
		tempEvent->setEvent(mEvent->getEvent());
		tempEvent->setTime(mEvent->getTime());
		for ( const std::pair<int, int>& pixel : mEvent->getData() ) {
			if ( !isHot(pixel) ) {
				tempEvent->pushData(std::move(pixel));
			}
		}
		mMaskedEvents.push_back(tempEvent);
		tempEvent = nullptr;
	}

}

void TClusterAnalyser::saveShapes(const std::filesystem::path& savePath, TFile* saveRootFile) {
	TDirectory* mainDir = saveRootFile->mkdir("shapes");
	mainDir->cd();
	std::filesystem::create_directory(savePath / "shapes");
	for ( const TClusterN* clusterN : mClusterN ) {
		int clusterSize = clusterN->getClusterSize();
		std::vector<std::pair<TCluster*, int>> clustersWithShape = clusterN->getClustersWithShape();
		std::vector<TImage*> shapeImages;

		std::filesystem::create_directory(savePath / "shapes" / static_cast<std::string>(Form("ClusterShapes(size=%d)", clusterSize)));
		TDirectory* dir = mainDir->mkdir(Form("ClusterShapes(size=%d)", clusterSize));
		dir->cd();
		int iShape = 0;
		for ( const std::pair<TCluster*, int>& clusterWithShape : clustersWithShape ) {
			TMatrix2D<int> matrix = clusterWithShape.first->getShape();
			int nRow = matrix.getNRow();
			int nColumn = matrix.getNColumn();
			TH2I* shapeMap = new TH2I(Form("shapeMap%d", iShape), "", nRow + 2, 0, nRow + 2, nColumn + 2, 0, nColumn + 2);
			for ( int iRow = 0; iRow < nRow; iRow++ ) {
				for ( int iColumn = 0; iColumn < nColumn; iColumn++ ) {
					if ( matrix.getElement(iRow, iColumn) == 1 ) {
						shapeMap->Fill(iRow + 1, iColumn + 1);
					}
				}
			}
			TCanvas* shapeCan = new TCanvas(Form("can%d_%d", iShape, clusterSize), Form("Cluster Shape with cluster size %d(#: %d)", clusterSize, clusterWithShape.second), 100 * (nRow + 2), 100 * (nColumn + 2));
			shapeCan->SetMargin(0, 0, 0, 0);
			shapeCan->SetGrid();
			shapeMap->GetXaxis()->SetNdivisions(nRow + 2, 0, 0, true);
			shapeMap->GetYaxis()->SetNdivisions(nColumn + 2, 0, 0, true);
			shapeMap->GetZaxis()->SetNdivisions(0, 0, 0, true);
			shapeMap->SetStats(0);
			shapeMap->Draw("COLZ L");
			TImage* shapeImage = TImage::Create();
			shapeImage->FromPad(shapeCan);
			shapeImages.push_back(shapeImage);
			shapeMap->SetTitle(Form("Cluster Shape with cluster size %d(#: %d)", clusterSize, clusterWithShape.second));
			shapeCan->SaveAs(static_cast<TString>(savePath / "shapes" / static_cast<std::string>(Form("ClusterShapes(size=%d)", clusterSize)) / static_cast<std::string>(Form("%dth Cluster Shape(size = %d).png", iShape, clusterSize))));
			shapeCan->Write();
			iShape++;
		}
		mShapeImages.push_back(shapeImages);
		iShape = 0;
		dir->Write();
		mainDir->cd();
		dir = nullptr;
	}
}

void TClusterAnalyser::saveShapeInformation(const std::filesystem::path& savePath, TFile* saveRootFile) {
	TDirectory* mainDir = saveRootFile->mkdir("shapesInfo");
	mainDir->cd();
	int iClusterSize = 0;
	for ( const TClusterN* clusterN : mClusterN ) {
		int clusterSize = clusterN->getClusterSize();
		std::vector<std::pair<TCluster*, int>> clustersWithShape = clusterN->getClustersWithShape();

		TGraph* rowGraph = new TGraph();
		TGraph* columnGraph = new TGraph();
		TGraph* rowColumnGraph = new TGraph();
		TGraph* stdevXGraph = new TGraph();
		TGraph* stdevYGraph = new TGraph();
		TGraph* stdevGraph = new TGraph();
		TLegend* dotLegend = new TLegend(0.67, 0.57, 0.98, 0.75);
		dotLegend->SetNColumns(3);
		dotLegend->SetTextSize(.02);
		TLegend* graphLegend = new TLegend(0.67, 0.77, 0.98, 0.93);
		graphLegend->AddEntry(rowGraph, "Row", "p");
		graphLegend->AddEntry(columnGraph, "Column", "p");
		graphLegend->AddEntry(rowColumnGraph, "Row + Column", "p");
		graphLegend->AddEntry(stdevXGraph, "Stdev Row", "p");
		graphLegend->AddEntry(stdevYGraph, "Stdev Column", "p");
		graphLegend->AddEntry(stdevGraph, "RMS of distance", "p");

		int iShape = 0;
		for ( const std::pair<TCluster*, int>& clusterWithShape : clustersWithShape ) {
			TMatrix2D<int> matrix = clusterWithShape.first->getShape();
			int nRow = matrix.getNRow();
			int nColumn = matrix.getNColumn();
			rowGraph->SetPoint(iShape, iShape, nRow);
			columnGraph->SetPoint(iShape, iShape + (1. / 16), nColumn);
			rowColumnGraph->SetPoint(iShape, iShape, nRow + nColumn);
			stdevXGraph->SetPoint(iShape, iShape, clusterWithShape.first->getStdevInAxis().first);
			stdevYGraph->SetPoint(iShape, iShape, clusterWithShape.first->getStdevInAxis().second);
			stdevGraph->SetPoint(iShape, iShape, clusterWithShape.first->getStdev());
			dotLegend->AddEntry(rowGraph, Form("%dth Entry: %d", iShape, clusterWithShape.second), "");
			iShape++;
		}

		TCanvas* shapeRowCan = new TCanvas(Form("shapeRow%d", clusterSize), "The number of rows in same cluster shape", 1500, 950);
		rowColumnGraph->GetXaxis()->SetLimits(0, iShape);
		rowColumnGraph->GetXaxis()->SetNdivisions(iShape, 0, 0, true);
		rowColumnGraph->SetTitle(Form("Cluster Shape Informations (Cluster Size: %d)", clusterSize));
		rowColumnGraph->GetXaxis()->SetTitle("Shape Number");
		rowColumnGraph->GetYaxis()->SetTitle("\\mathrm{Length}(1 = 28\\mathrm{\\mu m})");
		rowColumnGraph->GetYaxis()->SetLabelOffset(0.005);
		rowColumnGraph->GetYaxis()->SetTitleOffset(0.7);
		rowColumnGraph->SetMinimum(0);
		rowColumnGraph->SetMarkerStyle(20);
		rowColumnGraph->SetMarkerColor(kRed);
		rowColumnGraph->SetMarkerSize(1);
		rowColumnGraph->Draw("AP");
		rowGraph->SetMarkerStyle(20);
		rowGraph->SetMarkerColor(kMagenta);
		rowGraph->SetMarkerSize(1);
		rowGraph->Draw("P");
		columnGraph->SetMarkerStyle(20);
		columnGraph->SetMarkerColor(kOrange);
		columnGraph->SetMarkerSize(1);
		columnGraph->Draw("P");
		stdevXGraph->SetMarkerStyle(20);
		stdevXGraph->SetMarkerColor(kGreen);
		stdevXGraph->SetMarkerSize(1);
		stdevXGraph->Draw("P");
		stdevYGraph->SetMarkerStyle(20);
		stdevYGraph->SetMarkerColor(kCyan);
		stdevYGraph->SetMarkerSize(1);
		stdevYGraph->Draw("P");
		stdevGraph->SetMarkerStyle(20);
		stdevGraph->SetMarkerColor(kBlue);
		stdevGraph->SetMarkerSize(1);
		stdevGraph->Draw("P");
		dotLegend->Draw();
		graphLegend->Draw();
		for ( int i = 0; i < clustersWithShape.size(); i++ ) {
			double scaleFactor = std::max(mShapeImages[iClusterSize][i]->GetWidth(), mShapeImages[iClusterSize][i]->GetHeight());
			TPad* pad = new TPad(Form("pad%d", i),
								 "",
								 0.05 + 0.1 * static_cast<int>(i % 10) - 0.05 * (mShapeImages[iClusterSize][i]->GetWidth() / scaleFactor),
								 0.4 - 0.16 * static_cast<int>(i / 10) - 0.08 * (mShapeImages[iClusterSize][i]->GetHeight() / scaleFactor),
								 0.05 + 0.1 * static_cast<int>(i % 10) + 0.05 * (mShapeImages[iClusterSize][i]->GetWidth() / scaleFactor),
								 0.4 - 0.16 * static_cast<int>(i / 10) + 0.08 * (mShapeImages[iClusterSize][i]->GetHeight() / scaleFactor));
			mShapeImages[iClusterSize][i]->Scale((mShapeImages[iClusterSize][i]->GetWidth() / scaleFactor) * 150, (mShapeImages[iClusterSize][i]->GetHeight() / scaleFactor) * 140);
			pad->Draw();
			pad->cd();
			pad->SetMargin(0, 0, 0, 0);
			mShapeImages[iClusterSize][i]->Draw();
			TText* t = new TText(.5, .5, Form("%d", i));
			t->SetTextSize(.4);
			t->SetTextAlign(22);
			t->SetTextColor(kGray + 2);
			t->Draw();
			shapeRowCan->cd();
		}
		shapeRowCan->SetMargin(0.06, 0.35, 0.57, 0.07);
		shapeRowCan->SaveAs(static_cast<TString>(savePath / "shapes" / Form("shape_info_%d.png", clusterSize)));
		shapeRowCan->Write();
		iShape = 0;
		iClusterSize++;
	}
}

void TClusterAnalyser::savePlots() {
	std::filesystem::path savePath(getConfFile()->getConfig("File").value().find("output_path"));
	TFile* saveRootFile = new TFile(static_cast<TString>(savePath / "ExperimentPlots.root"), "RECREATE");
	gSystem->RedirectOutput("/dev/null");
	if ( mPlots.count("hitmap") ) {
		saveHitmap(savePath, saveRootFile);
	}

	if ( mPlots.count("clustermap") ) {
		saveClustermap(savePath, saveRootFile);
	}

	if ( mPlots.count("clustersize") ) {
		saveClustersize(savePath, saveRootFile);
	}

	if ( mPlots.count("maskedHitmap") ) {
		saveMaskedHitmap(savePath, saveRootFile);
	}
	if ( mPlots.count("maskedClustermap") ) {
		saveMaskedClustermap(savePath, saveRootFile);
	}
	if ( mPlots.count("maskedClustersize") ) {
		saveMaskedClustersize(savePath, saveRootFile);
	}

	if ( getConfFile()->getConfig("Shape").has_value() ) {
		saveShapes(savePath, saveRootFile);
		saveShapeInformation(savePath, saveRootFile);

	}
	gSystem->RedirectOutput(0);
	saveRootFile->Write();
	saveRootFile->Close();
}

void TClusterAnalyser::saveHitmap(const std::filesystem::path& savePath, TFile* saveRootFile) {
	std::filesystem::create_directory(savePath / "hitmap");
	TDirectory* hitmapDir = saveRootFile->mkdir("hitmap");
	hitmapDir->cd();
	TCanvas* hitmapCan = new TCanvas("hitmap", "Hitmap; rows; columns", 1000, 500);
	TH2D* hitmap = static_cast<TH2D*>(mPlots.find("hitmap")->second->Clone());

	hitmap->Draw("COLZ");
	hitmapCan->SaveAs(static_cast<TString>(savePath / "hitmap" / "hitmap_colz.png"));
	hitmapCan->Write("hitmapCOLZ");

	hitmap->Draw("LEGO0");
	hitmapCan->SaveAs(static_cast<TString>(savePath / "hitmap" / "hitmap_lego.png"));
	hitmapCan->Write("hitmapLEGO");

	hitmapDir->Write();
	saveRootFile->cd();
}

void TClusterAnalyser::saveClustermap(const std::filesystem::path& savePath, TFile* saveRootFile) {
	std::filesystem::create_directory(savePath / "clustermap");
	TDirectory* clustermapDir = saveRootFile->mkdir("clustermap");
	clustermapDir->cd();
	TCanvas* clustermapCan = new TCanvas("clustermap", "Clustermap; rows; columns", 1000, 500);
	TH2D* clustermap = static_cast<TH2D*>(mPlots.find("clustermap")->second);

	clustermap->Draw("COLZ");
	clustermapCan->SaveAs(static_cast<TString>(savePath / "clustermap" / "clustermap_colz.png"));
	clustermapCan->Write("clustermapCOLZ");

	clustermap->Draw("LEGO0");
	clustermapCan->SaveAs(static_cast<TString>(savePath / "clustermap" / "clustermap_LEGO.png"));
	clustermapCan->Write("clustermapLEGO");

	clustermapDir->Write();
	saveRootFile->cd();
}
void TClusterAnalyser::saveClustersize(const std::filesystem::path& savePath, TFile* saveRootFile) {
	std::filesystem::create_directory(savePath / "clustersize");
	TDirectory* clustersizeDir = saveRootFile->mkdir("clustersize");
	clustersizeDir->cd();
	TCanvas* clustersizeCan = new TCanvas("clustersize", "Cluster Size Distribution; Cluster Size; Entry", 500, 500);
	TH1D* clustersize = static_cast<TH1D*>(mPlots.find("clustersize")->second);

	clustersize->Draw();
	clustersizeCan->SaveAs(static_cast<TString>(savePath / "clustersize" / "clustersize.png"));
	clustersizeCan->Write("clustersize");

	clustersizeCan->SetLogy();
	clustersizeCan->SaveAs(static_cast<TString>(savePath / "clustersize" / "clustersize_log.png"));
	clustersizeCan->Write("clustersizeLOG");

	clustersizeDir->Write();
	saveRootFile->cd();
}
void TClusterAnalyser::saveMaskedHitmap(const std::filesystem::path& savePath, TFile* saveRootFile) {
	std::filesystem::create_directory(savePath / "maskedHitmap");
	TDirectory* maskedHitmapDir = saveRootFile->mkdir("maskedHitmap");
	maskedHitmapDir->cd();
	TCanvas* maskedHitmapCan = new TCanvas("maskedHitmap", "Masked Hitmap; rows; columns", 1000, 500);
	TH2D* maskedHitmap = static_cast<TH2D*>(mPlots.find("maskedHitmap")->second);

	maskedHitmap->Draw("COLZ");
	maskedHitmapCan->SaveAs(static_cast<TString>(savePath / "maskedHitmap" / "masked_hitmap_colz.png"));
	maskedHitmapCan->Write("maskedHitmapCOLZ");

	maskedHitmap->Draw("LEGO0");
	maskedHitmapCan->SaveAs(static_cast<TString>(savePath / "maskedHitmap" / "masked_hitmap_lego.png"));
	maskedHitmapCan->Write("maskedHitmapLEGO");

	maskedHitmapDir->Write();
	saveRootFile->cd();
}
void TClusterAnalyser::saveMaskedClustermap(const std::filesystem::path& savePath, TFile* saveRootFile) {
	std::filesystem::create_directory(savePath / "maskedClustermap");
	TDirectory* maskedClustermapDir = saveRootFile->mkdir("maskedClustermap");
	maskedClustermapDir->cd();
	TCanvas* maskedClustermapCan = new TCanvas("maskedClustermap", "Masked Clustermap; rows; columns", 1000, 500);
	TH2D* maskedClustermap = static_cast<TH2D*>(mPlots.find("maskedClustermap")->second);

	maskedClustermap->Draw("COLZ");
	maskedClustermapCan->SaveAs(static_cast<TString>(savePath / "maskedClustermap" / "masked_clustermap_colz.png"));
	maskedClustermapCan->Write("maskedClustermapCOLZ");

	maskedClustermap->Draw("LEGO0");
	maskedClustermapCan->SaveAs(static_cast<TString>(savePath / "maskedClustermap" / "masked_clustermap_LEGO.png"));
	maskedClustermapCan->Write("maskedClustermapLEGO");

	maskedClustermapDir->Write();
	saveRootFile->cd();
}
void TClusterAnalyser::saveMaskedClustersize(const std::filesystem::path& savePath, TFile* saveRootFile) {
	std::filesystem::create_directory(savePath / "maskedClustersize");
	TDirectory* maskedClustersizeDir = saveRootFile->mkdir("maskedClustersize");
	maskedClustersizeDir->cd();
	TCanvas* maskedClustersizeCan = new TCanvas("maskedClustersize", "Masked Cluster Size Distribution; Cluster Size; Entry", 500, 500);
	TH1D* maskedClustersize = static_cast<TH1D*>(mPlots.find("maskedClustersize")->second);

	maskedClustersize->Draw();
	maskedClustersizeCan->SaveAs(static_cast<TString>(savePath / "maskedClustersize" / "masked_clustersize.png"));
	maskedClustersizeCan->Write("maskedClustersize");

	maskedClustersizeCan->SetLogy();
	maskedClustersizeCan->SaveAs(static_cast<TString>(savePath / "maskedClustersize" / "masked_clustersize_log.png"));
	maskedClustersizeCan->Write("maskedClustersizeLOG");

	maskedClustersizeDir->Write();
	saveRootFile->cd();
}
void TClusterAnalyser::saveMaskingFile(std::string title) {
	std::ofstream maskFile(title);
	for ( const std::pair<int, int>& hotPixel : mHotPixels ) {
		maskFile << hotPixel.first << " " << hotPixel.second << std::endl;
	}
	maskFile.close();
}

bool TClusterAnalyser::isHot(const std::pair<int, int>& pixel) {
	if ( find(mHotPixels.begin(), mHotPixels.end(), pixel) == mHotPixels.end() ) {
		return false;
	} else {
		return true;
	}
}