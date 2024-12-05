#define __TANALYSER_HEADERS__

#include "TAnalyser.h"

TAnalyser::TAnalyser(const CppConfigFile& configFile) : mConfigFile(configFile) {
	setConfig();

	std::filesystem::path inputPath = mFileConfig.find("input_file");

	mInputFile = new TFile(static_cast<TString>(inputPath));

	if ( mFileConfig.hasKey("output_graph") ) {
		std::filesystem::path graphPath = mFileConfig.find("output_graph");
		mGraphFile = new TFile(static_cast<TString>(graphPath), "RECREATE");
		isGraphFile = true;
	}

	mTree = static_cast<TTree*>(mInputFile->Get("hit"));
	mTree->SetBranchAddress("ChipID", &mInput.chipid);
	mTree->SetBranchAddress("TimeStamp", &mInput.timeStamp);
	mTree->SetBranchAddress("X", &mInput.x);
	mTree->SetBranchAddress("Y", &mInput.y);
}

/**
 * @brief Destroy the TAnalyser::TAnalyser object
 * @todo The desctructors are commented out. It should be set.
*/
TAnalyser::~TAnalyser() {
	std::clog << "TAnalyser object is terminated" << std::endl;
	mGraphFile->Close();
}

void TAnalyser::originMasking() {
	for ( const std::string& key : mMaskingConfig.getSubConfig("masked_pixel").getKeyList() ) {
		std::string maskedPixelStr = mMaskingConfig.getSubConfig("masked_pixel").find(key);
		int maskedX = stoi(maskedPixelStr.substr(0, maskedPixelStr.find(' ')));
		int maskedY = stoi(maskedPixelStr.substr(maskedPixelStr.find(' ') + 1));
		mMaskedPixelSet.push_back({maskedX, maskedY});
	}
}

void TAnalyser::setConfig() {
	mFileConfig = mConfigFile.getConfig("File");
	mMaskingConfig = mConfigFile.getConfig("Masking");
	mSettingConfig = mConfigFile.getConfig("ExperimentSetting");

	if ( mMaskingConfig.hasKey("time_stamp_cut") ) {
		isTimeStampCut = true;
		mTimeStampCut = stoi(mMaskingConfig.find("time_stamp_cut"));
	}

	if ( mMaskingConfig.hasKey("roi") ) {
		isROICut = true;
		setROI();
	} else {
		roiSet = {0, 1024, 0, 512};
	}

	if ( mMaskingConfig.hasKey("n_pix_per_timestamp_map") ) {
		isPixelPerTimeStampMap = true;
	}

	if ( mMaskingConfig.hasKey("pedestal_print") ) {
		isPedestalPrint = true;
	}

	if ( mMaskingConfig.hasKey("hitmap_cut") ) {
		isHitmapCut = true;
		mHitmapCut = stoi(mMaskingConfig.find("hitmap_cut"));
	}

	if ( mMaskingConfig.hasKey("high_cluster_cut") ) {
		isHighClusterCut = true;
		mHighClusterCut = stoi(mMaskingConfig.find("high_cluster_cut"));
	}


	mOutputPath = mFileConfig.hasKey("output_path") ? mFileConfig.find("output_path") : "";
}

void TAnalyser::setROI() {
	std::istringstream roiStr(mMaskingConfig.find("roi"));
	roiStr >> roiSet[0] >> roiSet[1] >> roiSet[2] >> roiSet[3];
	// while(std::getline(roiStr, ))
}


// /**
//  * @brief Open ROOT file to save results graph.
//  *
//  * @param fileName
// */
// void TAnalyser::openOutputGraphFile(std::string_view fileName) {
// 	mOutputFile = new TFile(static_cast<TString>(fileName), "RECREATE"); /** Open output file */
// 	mIsOutputGraph = true; /** Set control variable */
// }

/**
 * @brief Extract event from TTree.
 *
 * @param config
*/
void TAnalyser::storeEvents() {
	std::clog << "Extracting Events..." << std::endl;

	/** Store initial event to 'TALPIDEEvent' array. */
	mOriginEventSet.push_back(new TALPIDEEvent());
	mOriginEventSet.back()->setEvent(0);
	mOriginEventSet.back()->setTime(static_cast<long int>(0));

	// TH1I* timeStampHist = new TH1I("TSHist", "", 50001, 0, 50000);

	UInt_t preTime = 0; /** Variable for storing previous time. */
	UInt_t preEvt = 0;

	ProgressBar* pbar = new ProgressBar(mTree->GetEntries()); /** Open progress bar */
	for ( int entry = 0; entry < mTree->GetEntries(); entry++ ) {
		mTree->GetEntry(entry);
		if ( mInput.timeStamp == preTime ) {
			bool passed = true;
			if ( mInput.x < roiSet[0] || mInput.x > roiSet[1] || mInput.y < roiSet[2] || mInput.y > roiSet[3] ) {
				passed = false;
			} else {
				for ( const std::pair<int, int>& maskedPixel : mMaskedPixelSet ) {
					if ( (mInput.x == maskedPixel.first && mInput.y == maskedPixel.second) ) {
						passed = false;
						break;
					}
				}
			}
			if ( passed ) {
				mOriginEventSet.back()->pushData({mInput.x, mInput.y});
			}
		} else {
			mOriginEventSet.back()->removeDuplication();
			mOriginEventSet.back()->sortPixel();
			if ( isPixelPerTimeStampMap ) {
				nPixelPerFrame.push_back(mOriginEventSet.back()->getNData());
			}

			if ( isTimeStampCut && (mOriginEventSet.back()->getNData() < mTimeStampCut) ) {
				mTSCutEventSet.push_back(mTSCutEventSet.back());
			} else if ( isTimeStampCut && (mOriginEventSet.back()->getNData() >= mTimeStampCut) ) {
				mTSCutNoiseEventSet.push_back(mTSCutEventSet.back());
			}
			mOriginEventSet.push_back(new TALPIDEEvent());
			mOriginEventSet.back()->setEvent(preEvt + 1);
			mOriginEventSet.back()->setTime(mInput.timeStamp);
			bool passed = true;
			if ( mInput.x < roiSet[0] || mInput.x > roiSet[1] || mInput.y < roiSet[2] || mInput.y > roiSet[3] ) {
				passed = false;
			} else {
				for ( const std::pair<int, int>& maskedPixel : mMaskedPixelSet ) {
					if ( (mInput.x == maskedPixel.first && mInput.y == maskedPixel.second) ) {
						passed = false;
						break;
					}
				}
			}
			if ( passed ) {
				mOriginEventSet.back()->pushData({mInput.x, mInput.y});
			}
			preTime = mInput.timeStamp;
			preEvt++;
		}
		pbar->printProgress();
	}
	mOriginEventSet.back()->removeDuplication();
	mOriginEventSet.back()->sortPixel();

	if ( isPixelPerTimeStampMap ) {
		CppConfigDictionary pptsmConfig = mMaskingConfig.getSubConfig("n_pix_per_timestamp_map");
		// TH1IUser pptsm(pptsmConfig, {{"hist1", nPixelPerFrame}});
	}
}

void TAnalyser::doMasking() {
	TMatrix2D<int> matrix(1025, 513);
	int i = 0;
	std::ofstream noiseFile("Noise.txt");
	for ( int x = 0; x < 1025; x++ ) {
		for ( int y = 0; y < 513; y++ ) {
			if ( mOriginHitmap->GetBinContent(x, y) > mHitmapCut ) {
				matrix.setElement(x - 1, y - 1, 1);
				if ( isPedestalPrint ) {
					noiseFile << "pixel" << i << " = " << x - 1 << " " << y - 1 << std::endl;
					// std::cout << "pixel" << i << " = " << x - 1 << " " << y - 1 << std::endl;
				}
				i++;
			}
		}
	}
	noiseFile.close();
	for ( const TALPIDEEvent* event : mOriginEventSet ) {
		mHHCutEventSet.push_back(new TALPIDEEvent());
		mHHCutEventSet.back()->setEvent(event->getEvent());
		mHHCutEventSet.back()->setTime(event->getTime());
		mHHCutNoiseEventSet.push_back(new TALPIDEEvent());
		mHHCutNoiseEventSet.back()->setEvent(event->getEvent());
		mHHCutNoiseEventSet.back()->setTime(event->getTime());
		for ( const std::pair<int, int>& pixel : event->getData() ) {
			if ( matrix.getElement(pixel.first, pixel.second) != 1 ) {
				mHHCutEventSet.back()->pushData(std::move(pixel));
			} else {
				mHHCutNoiseEventSet.back()->pushData(std::move(pixel));
			}
		}
	}
}

void TAnalyser::setExpSettingLegend() {
	if ( isExpSetting ) {
		mExpSettingLegend = new TPaveText(.78, .1, .981, .65, "NDC");
		for ( const std::string& setting : mSettingConfig.getKeyList() ) {
			mExpSettingLegend->AddText(static_cast<TString>(setting + "= " + mSettingConfig.find(setting)));
		}

		mExpSettingLegend->SetTextAlign(11);
		mExpSettingLegend->SetTextFont(42);
	}
}

void TAnalyser::clusterize() {
	TClusterization originalClusterizer(mOriginEventSet);
	originalClusterizer.clusterize();
	mOriginClusterSet = originalClusterizer.getClusters();

	// TClusterization TSCutClusterizer(mTSCutEventSet);
	// TSCutClusterizer.clusterize();
	// mTSCutClusterSet = TSCutClusterizer.getClusters();

	// TClusterization TSCutNoiseClusterizer(mTSCutNoiseEventSet);
	// TSCutNoiseClusterizer.clusterize();
	// mTSCutNoiseClusterSet = TSCutNoiseClusterizer.getClusters();

	// Get Clusters of High Hit cut events.
	TClusterization HHCutClusterizer(mHHCutEventSet);
	HHCutClusterizer.clusterize();
	mHHCutClusterSet = HHCutClusterizer.getClusters();

	// // // Get Clusters of High Hit cut noises.
	// TClusterization HHCutNoiseClusterizer(mHHCutNoiseEventSet);
	// HHCutNoiseClusterizer.clusterize();
	// mHHCutNoiseClusterSet = HHCutNoiseClusterizer.getClusters();
}

void TAnalyser::getClusterinfos() {
	Int_t rawBins = 1024;
	Int_t rawMin = 0;
	Int_t rawMax = 1024;
	Int_t columnBins = 512;
	Int_t columnMin = 0;
	Int_t columnMax = 512;

	mOriginClustermap = new TH2D("originClustermap", "Origin Hitmap", rawBins, rawMin, rawMax, columnBins, columnMin, columnMax);
	mOriginClustersize = new TH1D("originClustersize", "", 120, 0.5, 120.5);
	for ( const TCluster* cluster : mOriginClusterSet ) {
		const std::pair<double, double> center = cluster->getCenter();
		mOriginClustermap->Fill(center.first, center.second);
		mOriginClustersize->Fill(cluster->getSize());
	}

	mHHCutClustermap = new TH2D("highHitCutClustermap", "", rawBins, rawMin, rawMax, columnBins, columnMin, columnMax);
	mHHCutClustersize = new TH1D("highHitCutClustersize", "", 120, 0.5, 120.5);
	for ( const TCluster* cluster : mHHCutClusterSet ) {
		const std::pair<double, double> center = cluster->getCenter();
		mHHCutClustermap->Fill(center.first, center.second);
		mHHCutClustersize->Fill(cluster->getSize());
	}

	getOriginClustermap();
	getOriginClustersize();
	getHHCutClustermap();
	getHHCutClustersize();

	if ( isHighClusterCut ) {
		HCMasking();
		mHHHCCutClustermap = new TH2D("highHitNClusterCutClustermap", "", rawBins, rawMin, rawMax, columnBins, columnMin, columnMax);
		mHHHCCutClustersize = new TH1D("highHitNClusterCutClustersize", "", 120, 0.5, 120.5);
		for ( const TCluster* cluster : mHHHCCutClusterSet ) {
			const std::pair<double, double> center = cluster->getCenter();
			mHHHCCutClustermap->Fill(center.first, center.second);
			mHHHCCutClustersize->Fill(cluster->getSize());
		}
		getHHHCCutClustermap();
		getHHHCCutClustersize();
	}
}

void TAnalyser::HCMasking() {
	TMatrix2D<int> matrix(1024, 512);
	for ( int x = 0; x < 1024; x++ ) {
		for ( int y = 0; y < 512; y++ ) {
			if ( mHHCutClustermap->GetBinContent(x, y) > mHighClusterCut ) {
				matrix.setElement(x - 1, y - 1, 1);
			}
		}
	}
	for ( TCluster* cluster : mHHCutClusterSet ) {
		std::pair<double, double> center = cluster->getCenter();
		if ( matrix.getElement(floor(center.first), floor(center.second)) != 1 ) {
			mHHHCCutClusterSet.push_back(cluster);
		}
	}
}
















// void TAnalyser::doDivideBySize(std::string_view typeName) {
// 	TClusterDivideData* clusterDivideData = new TClusterDivideData(mExpData.find(std::string(typeName))->second->getClusters());
// 	mDivideData.insert_or_assign(std::string(typeName), clusterDivideData);
// }


// /**
//  * @brief Generalized function to draw clustermap
//  *
//  * @param config Draw configuration for map title, directory and filename
//  * @param clusters Dataset to draw
//  * @return const TH2*
// */
// TH2D* TAnalyser::getClusterPlot(const CppConfigDictionary& config, const std::vector<TCluster*>& clusters) {
// 	// Static variable for numbering
// 	static int iMap = 0;
// 	// Allocate a 2d histogram.
// 	std::string plotTitle = "";
// 	if ( config.hasKey("title") ) {
// 		plotTitle += config.find("title");
// 	}
// 	if ( config.hasKey("x_title") ) {
// 		plotTitle += "; " + config.find("x_title");
// 	} else {
// 		plotTitle += "; ";
// 	}
// 	if ( config.hasKey("y_title") ) {
// 		plotTitle += "; " + config.find("y_title");
// 	} else {
// 		plotTitle += "; ";
// 	}

// 	TH2D* map = new TH2D(Form("map%d", iMap), static_cast<TString>(plotTitle), 1024, 0, 1024, 512, 0, 512);
// 	// Fill data
// 	std::vector<int> interestSizeSet = getClusterSizeRange(config);
// 	for ( const TCluster* cluster : clusters ) {
// 		if ( !interestSizeSet.empty() ) {
// 			if ( std::find(interestSizeSet.begin(), interestSizeSet.end(), cluster->getSize()) != interestSizeSet.end() ) {
// 				map->Fill(cluster->getCenter().first, cluster->getCenter().second);

// 			}
// 		} else {
// 			map->Fill(cluster->getCenter().first, cluster->getCenter().second);
// 		}
// 	}
// 	// Canvas setting
// 	TCanvas* canvas = new TCanvas(Form("mapCan%d", iMap), "", 2500, 1000);
// 	canvas->SetMargin(.07, .35, .12, .08);
// 	map->GetXaxis()->SetTitleOffset(1.4);
// 	map->GetXaxis()->SetLabelOffset(0.003);
// 	// Find directory for saving clustermap. If it doesn't exist, then make the directory with mother directories.
// 	std::filesystem::path filePath(config.find("output_path"));
// 	filePath /= config.find("subdirectory");
// 	std::filesystem::create_directories(filePath);

// 	// Draw plot with options
// 	if ( config.hasKey("options") ) {
// 		for ( const std::string& optionName : config.getSubConfig("options").getValueList() ) {
// 			map->Draw(static_cast<TString>(optionName));
// 			mExpSettingLegend->Draw("SAME");
// 			std::filesystem::path file = filePath / (config.find("filename") + "_" + optionName);
// 			if ( config.hasKey("extension") ) {
// 				file.replace_extension(config.find("extension"));
// 			} else {
// 				file.replace_extension("png");
// 			}
// 			canvas->SaveAs(static_cast<TString>(file));
// 		}
// 	} else {
// 		map->Draw();
// 		mExpSettingLegend->Draw("SAME");
// 		std::filesystem::path file = filePath / (config.find("filename"));
// 		if ( config.hasKey("extension") ) {
// 			file.replace_extension(config.find("extension"));
// 		} else {
// 			file.replace_extension("png");
// 		}
// 		canvas->SaveAs(static_cast<TString>(file));
// 	}
// 	// Delete canvas;
// 	delete canvas;
// 	iMap++;
// 	return map;
// }
// /**
//  * @brief
//  *
//  * @param config
//  * @param clusters
//  * @return TH1D*
// */
// TH1D* TAnalyser::getClustersizePlot(const CppConfigDictionary& config, const std::vector<TCluster*>& clusters) {
// 	static int iDistribution = 0;
// 	TString distName = Form("distribution%d", iDistribution);
// 	TString distTitle = "";
// 	distTitle += config.hasKey("title") ? config.find("title") : "";
// 	distTitle += config.hasKey("x_title") ? "; " + config.find("x_title") : ";";
// 	distTitle += config.hasKey("y_title") ? "; " + config.find("y_title") : ";";
// 	Int_t nBins = 0;
// 	Float_t maxBin = 0;
// 	Float_t minBin = 0;
// 	if ( config.hasKey("distribution_info") ) {
// 		nBins = config.getSubConfig("distribution_info").hasKey("nbins") ? stoi(config.getSubConfig("distribution_info").find("nbins")) : 80;
// 		maxBin = config.getSubConfig("distribution_info").hasKey("max") ? stoi(config.getSubConfig("distribution_info").find("max")) + .5 : 80.5;
// 		minBin = config.getSubConfig("distribution_info").hasKey("min") ? stoi(config.getSubConfig("distribution_info").find("min")) - .5 : .5;
// 	} else {
// 		nBins = 80;
// 		maxBin = 80.5;
// 		minBin = 0.5;
// 	}
// 	TH1D* distribution = new TH1D(distName, distTitle, nBins, minBin, maxBin);
// 	for ( const TCluster* cluster : clusters ) {
// 		distribution->Fill(cluster->getSize()); // Fill clustersize to clustersize distribution
// 	}
// 	TCanvas* canvas = new TCanvas(Form("distributionCan%d", iDistribution), "", 2500, 1000);
// 	canvas->SetMargin(.07, .28, .12, .08);
// 	distribution->GetXaxis()->SetTitleOffset(1.4);
// 	distribution->GetXaxis()->SetLabelOffset(0.003);

// 	std::filesystem::path filePath(config.find("output_path"));
// 	filePath /= config.find("subdirectory");
// 	std::filesystem::create_directories(filePath);

// 	if ( config.hasKey("options") ) {
// 		for ( const std::string& optionName : config.getSubConfig("options").getValueList() ) {
// 			if ( optionName == "logy" ) {
// 				distribution->Draw();
// 				mExpSettingLegend->Draw("SAME");
// 				canvas->SetLogy();
// 				std::filesystem::path file = filePath / (config.find("filename") + "_" + optionName);
// 				if ( config.hasKey("extension") ) {
// 					file.replace_extension(config.find("extension"));
// 				} else {
// 					file.replace_extension("png");
// 				}
// 				canvas->SaveAs(static_cast<TString>(file));
// 			} else if ( optionName == "basic" ) {
// 				distribution->Draw();
// 				mExpSettingLegend->Draw("SAME");
// 				std::filesystem::path file = filePath / config.find("filename");
// 				if ( config.hasKey("extension") ) {
// 					file.replace_extension(config.find("extension"));
// 				} else {
// 					file.replace_extension("png");
// 				}
// 				canvas->SetLogy(0);
// 				canvas->SaveAs(static_cast<TString>(file));
// 			}
// 		}
// 	} else {
// 		distribution->Draw();
// 		mExpSettingLegend->Draw("SAME");
// 		std::filesystem::path file = filePath / (config.find("filename"));
// 		if ( config.hasKey("extension") ) {
// 			file.replace_extension(config.find("extension"));
// 		} else {
// 			file.replace_extension("png");
// 		}
// 		canvas->SaveAs(static_cast<TString>(file));
// 	}
// 	delete canvas;
// 	iDistribution++;
// 	return distribution;
// }

// void TAnalyser::saveClustermap(std::string typeName, const CppConfigDictionary& config) {
// 	std::clog << "Generating \033[1;32mClustermap\033[1;0m..." << std::endl;
// 	if ( !mClustermaps.count(typeName) ) {
// 		mClustermaps.insert_or_assign(typeName, getClusterPlot(config, mExpData.find(typeName)->second->getClusters()));
// 		if ( mIsOutputGraph ) {
// 			mDirectorySet.find(std::string(typeName))->second->cd();
// 			mClustermaps.find(std::string(typeName))->second->Write("clustermap");
// 			mOutputFile->cd();
// 		}
// 	} else {
// 		getClusterPlot(config, mExpData.find(typeName)->second->getClusters());
// 	}
// }

// void TAnalyser::saveClustersize(std::string typeName, const CppConfigDictionary& config) {
// 	std::clog << "Generating \033[1;32mCluster Size Distribution\033[1;0m..." << std::endl;
// 	if ( !mClustersizes.count(typeName) ) {
// 		mClustersizes.insert_or_assign(typeName, getClustersizePlot(config, mExpData.find(typeName)->second->getClusters()));
// 		if ( mIsOutputGraph ) {
// 			mDirectorySet.find(std::string(typeName))->second->cd();
// 			mClustersizes.find(std::string(typeName))->second->Write("clustersize");
// 			mOutputFile->cd();
// 		}
// 	} else {
// 		getClustersizePlot(config, mExpData.find(typeName)->second->getClusters());
// 	}
// }

// void TAnalyser::setClusterDataWithShape(const std::vector<int>& clusterSizeRange) {
// 	for ( const int clusterSize : clusterSizeRange ) {
// 		std::vector<TCluster*> clustersWithShape;
// 	}
// }

// std::vector<int> TAnalyser::getClusterSizeRange(const CppConfigDictionary& privateProperty) {
// 	std::vector<int> clusterSizeRange;
// 	if ( privateProperty.hasKey("interest_size") ) {
// 		for ( const std::string& rangeStr : privateProperty.getSubConfig("interest_size").getValueList() ) {
// 			if ( rangeStr.find('.') != std::string::npos ) {
// 				for ( int i = stoi(rangeStr.substr(0, rangeStr.find('.'))); i < stoi(rangeStr.substr(rangeStr.find('.') + 3)) + 1; i++ ) {
// 					clusterSizeRange.push_back(i);
// 				}
// 			} else {
// 				clusterSizeRange.push_back(stoi(rangeStr));
// 			}
// 		}
// 	} else {
// 		for ( int i = 0; i < 100; i++ ) {
// 			clusterSizeRange.push_back(i);
// 		}
// 	}
// 	return clusterSizeRange;
// }


// /**
//  * @brief Store clusters to objects of TClusterShape for extracting shape informations
//  *
//  * @param typeName
//  * @param clusterSizeRange
// */
// void TAnalyser::doShaping(std::string_view typeName, const std::vector<int>& clusterSizeRange) {
// 	// The number of total shape and the maximum number of shapes of each cluster sizes.
// 	int nTotalShape = 0;
// 	int maxMode = 0;
// 	// The array of objects of TClusterShape class.
// 	std::vector<TClusterShape*> clusterShapes;
// 	// This stores and extracts clusters shape informations.
// 	for ( const int clusterSize : clusterSizeRange ) {
// 		if ( mClustersizes.find(std::string(typeName))->second->GetBinContent(clusterSize) != 0 ) {
// 			// This gets cluster bunch from TClusterDivideData object which classifies clusters by their size.
// 			const std::vector<TCluster*> divideData = mDivideData.find(std::string(typeName))->second->getClusterOfSize(clusterSize);
// 			// This inserts cluster size information and clusters to TClusterShape objects.
// 			TClusterShape* clusterShape = new TClusterShape(clusterSize, divideData);
// 			// This extracts shape informations.
// 			clusterShape->identifyShapes();
// 			// This sorts shapes according to its size of long axis.
// 			clusterShape->sortShapes();
// 			// 
// 			clusterShapes.push_back(clusterShape);
// 			nTotalShape += clusterShape->getClusterShapeInfos().size();
// 			maxMode = std::max(maxMode, static_cast<int>(clusterShape->getClusterShapeInfos().size()));
// 		}
// 	}
// 	mClusterShapeSet.insert_or_assign(std::string(typeName), clusterShapes);
// 	mNTotalShapeSet.insert_or_assign(std::string(typeName), nTotalShape);
// 	mMaxModeSet.insert_or_assign(std::string(typeName), maxMode);
// }
// /**
//  * @brief Drawing individual shapes.
//  *
//  * @param typeName
//  * @param config
// */
// void TAnalyser::saveIndividualShapes(std::string_view typeName, const CppConfigDictionary config) {
// 	// Print log message
// 	std::clog << "Generating " << "\033[1;32m" << "individual shapes" << "\033[1;0m" << "..." << std::endl;

// 	// Get the total number of shapes for progress bar.
// 	int nTotalShape = mNTotalShapeSet.find(std::string(typeName))->second;

// 	// Get save path from config file.
// 	std::filesystem::path filePath(config.find("output_path"));
// 	filePath /= config.find("subdirectory");
// 	std::filesystem::create_directories(filePath);

// 	// Init progress bar
// 	ProgressBar pBar(nTotalShape);

// 	for ( const TClusterShape* clusterShape : mClusterShapeSet.find(std::string(typeName))->second ) {
// 		int clusterSize = clusterShape->getClusterSize(); // The cluster size of shape set.
// 		int iClusterShape = 0; // The i-th cluster shape
// 		for ( const TShapeInfo& shapeInfo : clusterShape->getClusterShapeInfos() ) {
// 			pBar.printProgress(); // Print progress bar.

// 			int shapeWidth = shapeInfo.mClusterMatrix->getNRow(); // Shape width of matrix
// 			int shapeHeight = shapeInfo.mClusterMatrix->getNColumn(); // Shape height of matrix

// 			// Initialize canvas
// 			TCanvas* canvas = new TCanvas(Form("%d_%d.png", clusterSize, iClusterShape), "", (shapeWidth + 2) * 500, (shapeHeight + 2) * 500);
// 			// Set canvas margin
// 			canvas->SetMargin(0., 0., 0., 0.);
// 			// Draw shape on canvas
// 			shapeInfo.mClusterMap->Draw();
// 			// Fill colour
// 			shapeInfo.mClusterMap->SetDrawOption("COLZ");
// 			// Remove histogram axis
// 			shapeInfo.mClusterMap->GetXaxis()->SetAxisColor(0);
// 			shapeInfo.mClusterMap->GetYaxis()->SetAxisColor(0);

// 			// Fill line for seperating pixels
// 			TLine* line = new TLine();
// 			line->SetLineColorAlpha(kRed, 6. / 8);
// 			for ( int i = 1; i <= shapeInfo.mClusterMap->GetNbinsX(); ++i ) {
// 				for ( int j = 1; j <= shapeInfo.mClusterMap->GetNbinsY(); ++j ) {
// 					if ( shapeInfo.mClusterMap->GetBinContent(i, j) > 0 ) {
// 						double xlow = shapeInfo.mClusterMap->GetXaxis()->GetBinLowEdge(i);
// 						double xup = shapeInfo.mClusterMap->GetXaxis()->GetBinUpEdge(i);
// 						double ylow = shapeInfo.mClusterMap->GetYaxis()->GetBinLowEdge(j);
// 						double yup = shapeInfo.mClusterMap->GetYaxis()->GetBinUpEdge(j);
// 						line->DrawLine(xlow, ylow, xup, ylow); // Bottom
// 						line->DrawLine(xlow, yup, xup, yup); // Top
// 						line->DrawLine(xlow, ylow, xlow, yup); // Left
// 						line->DrawLine(xup, ylow, xup, yup); // Right
// 					}
// 				}
// 			}

// 			// Set title of shape
// 			TText* title = new TText(.5, 1. - .5 / (shapeHeight + 2), Form("%d-th Cluster Shape of Cluster Size %d", iClusterShape, clusterSize));
// 			title->SetTextAlign(23);
// 			title->SetNDC();
// 			title->Draw();

// 			// Save shape figure
// 			std::filesystem::path file = filePath / (config.find("filename") + "_" + std::to_string(clusterSize) + "_" + std::to_string(iClusterShape));
// 			if ( config.hasKey("extension") ) {
// 				file.replace_extension(config.find("extension"));
// 			} else {
// 				file.replace_extension("png");
// 			}
// 			canvas->SaveAs(static_cast<TString>(file));

// 			iClusterShape++;

// 			delete title;
// 			title = nullptr;
// 			delete line;
// 			line = nullptr;
// 			delete canvas;
// 			canvas = nullptr;
// 		}
// 	}
// }

// int calNIncludePixel(const TMatrix2D<int>* matrix) {
// 	int centreX = 0, centreY = 0;
// 	int clusterSize = 0;
// 	for ( int pixelX = 0; pixelX < matrix->getNRow(); pixelX++ ) {
// 		for ( int pixelY = 0; pixelY < matrix->getNColumn(); pixelY++ ) {
// 			if ( matrix->getElement(pixelX, pixelY) == 1 ) {
// 				centreX += pixelX * 2;
// 				centreY += pixelY * 2;
// 				clusterSize++;
// 			}
// 		}
// 	}
// 	int radiusSquare = 0;
// 	for ( int pixelX = 0; pixelX < matrix->getNRow(); pixelX++ ) {
// 		for ( int pixelY = 0; pixelY < matrix->getNColumn(); pixelY++ ) {
// 			if ( matrix->getElement(pixelX, pixelY) == 1 ) {
// 				int distance = pow(std::abs(2 * pixelX * clusterSize - centreX) + clusterSize, 2) + pow(std::abs(2 * pixelY * clusterSize - centreY) + clusterSize, 2);
// 				radiusSquare = std::max(radiusSquare, distance);
// 			}
// 		}
// 	}
// 	int count = 0;
// 	for ( int x = floor((centreX - sqrt(radiusSquare)) / (2 * clusterSize)); x < ceil((centreX + sqrt(radiusSquare)) / (2 * clusterSize)) + 1; x++ ) {
// 		for ( int y = floor((centreY - sqrt(radiusSquare)) / (2 * clusterSize)); y < ceil((centreY + sqrt(radiusSquare)) / (2 * clusterSize)) + 1; y++ ) {
// 			if ( static_cast<int>(std::pow(std::abs(2 * x * clusterSize - centreX) + clusterSize, 2)) + static_cast<int>(std::pow(std::abs(2 * y * clusterSize - centreY) + clusterSize, 2)) <= radiusSquare ) {
// 				count++;
// 			}
// 		}
// 	}
// 	return count;
// }

// double calRatioOfRadius(const TMatrix2D<int>* matrix) {
// 	int centreX = 0, centreY = 0;
// 	int clusterSize = 0;
// 	for ( int pixelX = 0; pixelX < matrix->getNRow(); pixelX++ ) {
// 		for ( int pixelY = 0; pixelY < matrix->getNColumn(); pixelY++ ) {
// 			if ( matrix->getElement(pixelX, pixelY) == 1 ) {
// 				centreX += pixelX * 2;
// 				centreY += pixelY * 2;
// 				clusterSize++;
// 			}
// 		}
// 	}
// 	int longRadiusSquare = 0;
// 	int shortRadiusSquare = 100000000;
// 	for ( int pixelX = 0; pixelX < matrix->getNRow(); pixelX++ ) {
// 		for ( int pixelY = 0; pixelY < matrix->getNColumn(); pixelY++ ) {
// 			bool hasValue = matrix->getElement(pixelX, pixelY) == 1;
// 			bool isWorldBorder = pixelX == 0 || pixelY == 0 || pixelX == matrix->getNRow() - 1 || pixelY == matrix->getNColumn() - 1;
// 			bool isBorder = isWorldBorder ? true : matrix->getElement(pixelX - 1, pixelY) == 0 || matrix->getElement(pixelX, pixelY - 1) == 0 || matrix->getElement(pixelX + 1, pixelY) == 0 || matrix->getElement(pixelX, pixelY + 1) == 0;
// 			if ( hasValue ) {
// 				int distance = pow(2 * pixelX * clusterSize - centreX, 2) + pow(2 * pixelY * clusterSize - centreY, 2);
// 				longRadiusSquare = std::max(longRadiusSquare, distance);
// 			}
// 			if ( (hasValue && isWorldBorder) || (hasValue && !isWorldBorder && isBorder) ) {
// 				int distance = pow(2 * pixelX * clusterSize - centreX, 2) + pow(2 * pixelY * clusterSize - centreY, 2);
// 				shortRadiusSquare = std::min(shortRadiusSquare, distance);
// 			}
// 		}
// 	}
// 	if ( matrix->getNRow() == 1 || matrix->getNColumn() == 1 ) {
// 		shortRadiusSquare = 0;
// 	}
// 	return sqrt(static_cast<double>(shortRadiusSquare) / longRadiusSquare);
// }

// /**
//  * @brief Drawing information for classifying multi-cluster
//  *
//  * @param typeName
//  * @param config
// */
// void TAnalyser::saveSameSizeInfos(std::string_view typeName, const CppConfigDictionary config) {
// 	// Print out a log
// 	std::cout << "Generating " << "\033[1;32m" << "Informations of shapes with same size" << "\033[1;0m" << "..." << std::endl;

// 	// Settting output file path
// 	std::filesystem::path filePath(config.find("output_path"));
// 	filePath /= config.find("subdirectory");
// 	// The creation of directories of output path
// 	std::filesystem::create_directories(filePath);
// 	// Call configs. First element is config name and second one is config values.
// 	std::unordered_map<std::string, CppConfigDictionary> plotConfigList = config.getSubConfig("ratio_distribution").getSubConfigSetWithName();
// 	// Call histograms 
// 	std::unordered_map<std::string, TH1D*> distributionSet;
// 	// Set the information of histograms from config file
// 	for ( const auto& plotConfig : plotConfigList ) {
// 		// Plot name is config name. It isn't value of "name" key.
// 		std::string plotName = plotConfig.second.find("name");
// 		// Set plot title and x, y label.
// 		TString plotTitle = plotConfig.second.hasKey("title") ? plotConfig.second.find("title") : "";
// 		TString plotXTitle = plotConfig.second.hasKey("x_title") ? plotConfig.second.find("x_title") : "";
// 		TString plotYTitle = plotConfig.second.hasKey("y_title") ? plotConfig.second.find("y_title") : "";
// 		// Set number of bin and min and max of x-direction range.
// 		Int_t plotNBin = plotConfig.second.hasKey("n_bin") ? stoi(plotConfig.second.find("n_bin")) : 100;
// 		Float_t plotXMin = plotConfig.second.hasKey("x_min") ? stof(plotConfig.second.find("x_min")) : 0.;
// 		Float_t plotXMax = plotConfig.second.hasKey("x_max") ? stof(plotConfig.second.find("x_max")) : 1.;

// 		// Add histograms to map.
// 		distributionSet.insert_or_assign(plotConfig.first, new TH1D(static_cast<TString>(plotName), plotTitle + "; " + plotXTitle + "; " + plotYTitle, plotNBin, plotXMin, plotXMax));
// 	}

// 	ProgressBar pBar(mNTotalShapeSet.find(std::string(typeName))->second);
// 	for ( const TClusterShape* clusterShape : mClusterShapeSet.find(std::string(typeName))->second ) {
// 		// Get cluster size.
// 		int clusterSize = clusterShape->getClusterSize();
// 		if ( clusterSize < stoi(config.getSubConfig("ratio_distribution").find("cluster_size_oi_min")) || clusterSize > stoi(config.getSubConfig("ratio_distribution").find("cluster_size_oi_max")) ) {
// 			continue;
// 		}
// 		// Initialize canvas.
// 		TCanvas* canvas = new TCanvas(Form("shapeEntry%d", clusterSize), "shapeEntry", 2500, 1000);
// 		// int binNum = clusterShape->getClusterShapeInfos().size();
// 		TGraph* areaRatioGraph = new TGraph();
// 		TGraph* radiusRatioGraph = new TGraph();

// 		int iShape = 0;
// 		for ( const TShapeInfo& shapeInfo : clusterShape->getClusterShapeInfos() ) {
// 			// pBar.printProgress();
// 			for ( const auto& plotConfig : plotConfigList ) {
// 				if ( plotConfig.second.find("name") == "area_ratio" ) {
// 					distributionSet.find(plotConfig.first)->second->Fill(static_cast<double>(clusterSize) / calNIncludePixel(shapeInfo.mClusterMatrix));
// 				}
// 				if ( plotConfig.second.find("name") == "area_ratio_with_entry" ) {
// 					distributionSet.find(plotConfig.first)->second->Fill(static_cast<double>(clusterSize) / calNIncludePixel(shapeInfo.mClusterMatrix), shapeInfo.mEntry);
// 				}
// 				if ( plotConfig.second.find("name") == "radius_ratio" ) {
// 					distributionSet.find(plotConfig.first)->second->Fill(calRatioOfRadius(shapeInfo.mClusterMatrix));
// 				}
// 				if ( plotConfig.second.find("name") == "radius_ratio_with_entry" ) {
// 					distributionSet.find(plotConfig.first)->second->Fill(calRatioOfRadius(shapeInfo.mClusterMatrix), shapeInfo.mEntry);
// 				}
// 			}

// 			areaRatioGraph->SetPoint(iShape, iShape, static_cast<double>(clusterSize) / calNIncludePixel(shapeInfo.mClusterMatrix));
// 			radiusRatioGraph->SetPoint(iShape, iShape, calRatioOfRadius(shapeInfo.mClusterMatrix));
// 			// std::cout << clusterSize << "\t" << iShape << "\t" << static_cast<double>(clusterSize) / calNIncludePixel(shapeInfo.mClusterMatrix) << "\t" << calRatioOfRadius(shapeInfo.mClusterMatrix) << "\t" << shapeInfo.mEntry << std::endl;
// 			iShape++;
// 		}
// 		TF1* line1 = new TF1("line1", "0.8", 0, iShape);
// 		TF1* line2 = new TF1("line2", "0.5", 0, iShape);

// 		areaRatioGraph->SetTitle(Form("Informations for cluster shapes in cluster size %d", clusterSize));
// 		areaRatioGraph->GetXaxis()->SetTitle("i-th cluster shape");
// 		areaRatioGraph->GetXaxis()->SetTitleOffset(1.4);
// 		areaRatioGraph->GetXaxis()->SetLabelOffset(0.003);
// 		areaRatioGraph->GetYaxis()->SetTitle("Ratio");
// 		areaRatioGraph->GetYaxis()->SetTitleOffset(0.7);
// 		areaRatioGraph->SetMarkerStyle(45);
// 		areaRatioGraph->SetMarkerSize(4);
// 		areaRatioGraph->SetMarkerColor(kRed);
// 		areaRatioGraph->SetMaximum(1);
// 		areaRatioGraph->SetMinimum(0);
// 		areaRatioGraph->Draw("AP");
// 		radiusRatioGraph->SetMarkerStyle(41);
// 		radiusRatioGraph->SetMarkerSize(4);
// 		radiusRatioGraph->SetMarkerColor(kBlue);
// 		radiusRatioGraph->Draw("P");
// 		mExpSettingLegend->Draw("SAME");
// 		line1->SetLineColor(kPink + 2);
// 		// line1->Draw("SAME");
// 		line2->SetLineColor(kCyan - 7);
// 		// line2->Draw("SAME");
// 		TLegend* legend = new TLegend(.78, .7, .98, .92);
// 		legend->SetHeader("Ratios", "c");
// 		legend->AddEntry(areaRatioGraph, "Pixels / Full pixels", "p");
// 		legend->AddEntry(radiusRatioGraph, "Short radius / Long radius", "p");
// 		legend->Draw();
// 		canvas->SetMargin(.07, .28, .12, .08);
// 		std::filesystem::path file = filePath;

// 		file /= config.getSubConfig("graphs").find("filename") + "_" + std::to_string(clusterSize);
// 		file.replace_extension(config.find("extension"));

// 		canvas->SaveAs(static_cast<TString>(file));

// 		delete line1;
// 		delete line2;
// 		delete areaRatioGraph;
// 		delete radiusRatioGraph;
// 		delete canvas;
// 	}

// 	for ( const auto& plotConfig : plotConfigList ) {
// 		TString canvasName = "can" + plotConfig.first;
// 		Int_t canvasWidth = plotConfig.second.hasKey("canvas_width") ? stoi(plotConfig.second.find("canvas_width")) : 500;
// 		Int_t canvasHeight = plotConfig.second.hasKey("canvas_height") ? stoi(plotConfig.second.find("canvas_height")) : 500;
// 		TCanvas* canvas = new TCanvas(canvasName, "", canvasWidth, canvasHeight);

// 		distributionSet.find(plotConfig.first)->second->SetStats(0);

// 		Int_t y_min = plotConfig.second.hasKey("y_min") ? stoi(plotConfig.second.find("y_min")) : 0;
// 		Int_t y_max = plotConfig.second.hasKey("y_max") ? stoi(plotConfig.second.find("y_max")) : 1000;
// 		distributionSet.find(plotConfig.first)->second->SetMinimum(y_min);
// 		distributionSet.find(plotConfig.first)->second->SetMaximum(y_max);
// 		distributionSet.find(plotConfig.first)->second->Draw();

// 		std::filesystem::path file = filePath;
// 		file /= plotConfig.second.find("filename");
// 		file.replace_extension(config.find("extension"));
// 		canvas->SaveAs(static_cast<TString>(file));

// 		delete canvas;
// 		canvas = nullptr;
// 	}
// }

// void TAnalyser::saveSameSizeShapes(std::string_view typeName, const CppConfigDictionary config) {
// 	std::cout << "Generating " << "\033[1;32m" << "Shapes with same sized" << "\033[1;0m" << "..." << std::endl;

// 	std::filesystem::path filePath(config.find("output_path"));
// 	if ( config.hasKey("subdirectory") ) {
// 		filePath /= config.find("subdirectory");
// 	}
// 	std::filesystem::create_directories(filePath);

// 	int nTotalShape = mNTotalShapeSet.find(std::string(typeName))->second;

// 	const int nominalWidth = 200;
// 	const int nominalHeader = 100;
// 	const int nPlotInRow = 10;
// 	int plotsWidth = nPlotInRow * nominalWidth;

// 	ProgressBar pBar(nTotalShape);
// 	for ( const TClusterShape* clusterShape : mClusterShapeSet.find(std::string(typeName))->second ) {
// 		int clusterSize = clusterShape->getClusterSize();
// 		int nClusterShape = clusterShape->getClusterShapeInfos().size();

// 		int plotsHeight = (floor((nClusterShape - 1.) / 10.) + 1.) * nominalWidth;

// 		TCanvas* canvas = new TCanvas(Form("shapes%d", clusterSize), "", plotsWidth, plotsHeight + nominalHeader);

// 		int iClusterShape = 0;

// 		std::vector<TPad*> padSet;
// 		std::vector<TLine*> lineSet;
// 		std::vector<TText*> textSet;

// 		for ( const TShapeInfo& shapeInfo : clusterShape->getClusterShapeInfos() ) {
// 			pBar.printProgress();

// 			int width = shapeInfo.mClusterMatrix->getNRow() + 2;
// 			int height = shapeInfo.mClusterMatrix->getNColumn() + 2;

// 			double padCenterX = (1. / nPlotInRow) * (.5 + (iClusterShape % 10));
// 			double padCenterY = (static_cast<double>(plotsHeight) / static_cast<double>(plotsHeight + nominalHeader)) / ((floor((nClusterShape - 1) / static_cast<double>(nPlotInRow))) + 1.) * .5 * (2. * (floor((nClusterShape - 1) / static_cast<double>(nPlotInRow)) - floor(iClusterShape / static_cast<double>(nPlotInRow))) + 1.);

// 			double padWidth = (1. / (2. * nPlotInRow));
// 			double padHeight = (static_cast<double>(plotsHeight) / (plotsHeight + nominalHeader)) / (2. * ((floor(static_cast<double>(nClusterShape) / nPlotInRow) + 1.)));

// 			TPad* pad = new TPad(Form("pad%d", iClusterShape), "", padCenterX - padWidth, padCenterY - padHeight, padCenterX + padWidth, padCenterY + padHeight);
// 			pad->SetMargin(0., 0., .5 * (1. - static_cast<double>(height) / width), .5 * (1. - static_cast<double>(height) / width));
// 			pad->Draw();
// 			pad->cd();
// 			shapeInfo.mClusterMap->SetDrawOption("COL");
// 			shapeInfo.mClusterMap->GetXaxis()->SetAxisColor(0);
// 			shapeInfo.mClusterMap->GetYaxis()->SetAxisColor(0);
// 			shapeInfo.mClusterMap->Draw("col");
// 			pad->SetFrameLineWidth(0);
// 			padSet.push_back(pad);

// 			TLine* line = new TLine();
// 			line->SetLineColorAlpha(kRed, 6. / 8);
// 			for ( int i = 1; i <= shapeInfo.mClusterMap->GetNbinsX(); ++i ) {
// 				for ( int j = 1; j <= shapeInfo.mClusterMap->GetNbinsY(); ++j ) {
// 					if ( shapeInfo.mClusterMap->GetBinContent(i, j) > 0 ) {
// 						double xlow = shapeInfo.mClusterMap->GetXaxis()->GetBinLowEdge(i);
// 						double xup = shapeInfo.mClusterMap->GetXaxis()->GetBinUpEdge(i);
// 						double ylow = shapeInfo.mClusterMap->GetYaxis()->GetBinLowEdge(j);
// 						double yup = shapeInfo.mClusterMap->GetYaxis()->GetBinUpEdge(j);
// 						line->DrawLine(xlow, ylow, xup, ylow); // Bottom
// 						line->DrawLine(xlow, yup, xup, yup); // Top
// 						line->DrawLine(xlow, ylow, xlow, yup); // Left
// 						line->DrawLine(xup, ylow, xup, yup); // Right
// 					}
// 				}
// 			}
// 			lineSet.push_back(line);

// 			TText* numberingText = new TText(padCenterX, padCenterY, Form("%d", iClusterShape));
// 			numberingText->SetNDC();
// 			numberingText->SetTextAlign(22);
// 			numberingText->SetTextSize(.3 * nominalWidth / (plotsHeight + nominalHeader));
// 			numberingText->SetTextColorAlpha(kBlack, 5. / 8.);

// 			canvas->cd();
// 			numberingText->Draw();
// 			textSet.push_back(numberingText);
// 			iClusterShape++;
// 		}
// 		TText* titleText = new TText(.5, 1. - .5 * nominalHeader / (nominalHeader + plotsHeight), Form("Cluster shapes with cluster size %d", clusterSize));
// 		titleText->SetTextSize(.5 * nominalHeader / (plotsHeight + nominalHeader));
// 		titleText->SetTextAlign(22);
// 		titleText->SetNDC();
// 		titleText->Draw();
// 		std::filesystem::path file = filePath / (config.find("filename") + "_" + std::to_string(clusterSize));
// 		file.replace_extension(config.find("extension"));
// 		canvas->SaveAs(static_cast<TString>(file));

// 		delete titleText;
// 		titleText = nullptr;

// 		for ( TPad* pad : padSet ) {
// 			delete pad;
// 			pad = nullptr;
// 		}

// 		for ( TLine* line : lineSet ) {
// 			delete line;
// 			line = nullptr;
// 		}

// 		for ( TText* text : textSet ) {
// 			delete text;
// 			text = nullptr;
// 		}

// 		delete canvas;
// 		canvas = nullptr;
// 	}
// }

// void TAnalyser::saveTotalShapes(std::string_view typeName, const CppConfigDictionary config) {
// 	// Print log
// 	std::cout << "Generating \033[1;32mTotal shapes\033[1;0m..." << std::endl;

// 	std::filesystem::path filePath(config.find("output_path"));
// 	if ( config.hasKey("subdirectory") ) {
// 		filePath /= config.find("subdirectory");
// 	}
// 	std::filesystem::create_directories(filePath);

// 	int nWidth = mMaxModeSet.find(std::string(typeName))->second;
// 	int nHeight = mClusterShapeSet.find(std::string(typeName))->second.size();

// 	int nominalWidth = 100;
// 	int nominalHeader = 100;

// 	int plotsWidth = nWidth * nominalWidth;
// 	int plotsHeight = nHeight * nominalWidth;
// 	TCanvas* canvas = new TCanvas("tShape", "cluster shape", plotsWidth + nominalHeader, plotsHeight + nominalHeader);
// 	int iClusterSize = 0;
// 	std::vector<TPad*> padSet;
// 	std::vector<TLine*> lineSet;
// 	std::vector<TText*> textSet;

// 	ProgressBar pBar(mNTotalShapeSet.find(std::string(typeName))->second);
// 	for ( const TClusterShape* clusterShape : mClusterShapeSet.find(std::string(typeName))->second ) {
// 		int clusterSize = clusterShape->getClusterSize();
// 		int iClusterShape = 0;
// 		for ( const TShapeInfo& shapeInfo : clusterShape->getClusterShapeInfos() ) {
// 			pBar.printProgress();
// 			int width = shapeInfo.mClusterMap->GetNbinsX();
// 			int height = shapeInfo.mClusterMap->GetNbinsY();

// 			TPad* pad = new TPad(Form("pad%d_%d", iClusterSize, iClusterShape), "pad", (double) nominalHeader / (plotsWidth + nominalHeader) + ((double) plotsWidth / (plotsWidth + nominalHeader)) * ((double) iClusterShape / nWidth), ((double) plotsHeight / (plotsHeight + nominalHeader)) * (((double) nHeight - iClusterSize - 1) / nHeight), (double) nominalHeader / (plotsWidth + nominalHeader) + ((double) plotsWidth / (plotsWidth + nominalHeader)) * (double) (iClusterShape + 1) / nWidth, ((double) plotsHeight / (plotsHeight + nominalHeader)) * (((double) nHeight - iClusterSize) / nHeight), -1, 1);
// 			padSet.push_back(pad);
// 			pad->Draw();
// 			pad->cd();
// 			pad->SetMargin(0., 0., .5 * (1. - (double) height / width), .5 * (1. - (double) height / width));

// 			shapeInfo.mClusterMap->Draw();
// 			shapeInfo.mClusterMap->SetDrawOption("COL");
// 			shapeInfo.mClusterMap->GetXaxis()->SetAxisColor(0);
// 			shapeInfo.mClusterMap->GetYaxis()->SetAxisColor(0);
// 			TLine* line = new TLine();
// 			lineSet.push_back(line);
// 			line->SetLineColorAlpha(kRed, 6. / 8);
// 			for ( int i = 1; i <= shapeInfo.mClusterMap->GetNbinsX(); ++i ) {
// 				for ( int j = 1; j <= shapeInfo.mClusterMap->GetNbinsY(); ++j ) {
// 					if ( shapeInfo.mClusterMap->GetBinContent(i, j) > 0 ) {
// 						double xlow = shapeInfo.mClusterMap->GetXaxis()->GetBinLowEdge(i);
// 						double xup = shapeInfo.mClusterMap->GetXaxis()->GetBinUpEdge(i);
// 						double ylow = shapeInfo.mClusterMap->GetYaxis()->GetBinLowEdge(j);
// 						double yup = shapeInfo.mClusterMap->GetYaxis()->GetBinUpEdge(j);
// 						line->DrawLine(xlow, ylow, xup, ylow); // Bottom
// 						line->DrawLine(xlow, yup, xup, yup); // Top
// 						line->DrawLine(xlow, ylow, xlow, yup); // Left
// 						line->DrawLine(xup, ylow, xup, yup); // Right
// 					}
// 				}
// 			}
// 			pad->SetFrameLineWidth(0);
// 			canvas->cd();
// 			TText* numberingText = new TText((double) nominalHeader / (plotsWidth + nominalHeader) + ((double) plotsWidth / (plotsWidth + nominalHeader)) * (((double) iClusterShape + .5) / nWidth), ((double) plotsHeight / (plotsHeight + nominalHeader)) * (((double) nHeight - iClusterSize) / nHeight), Form("%d", shapeInfo.mEntry));
// 			textSet.push_back(numberingText);
// 			numberingText->SetNDC();
// 			numberingText->SetTextAlign(22);
// 			numberingText->SetTextSize(.4 * nominalWidth / (plotsHeight + nominalHeader));
// 			numberingText->SetTextColor(kBlack);
// 			numberingText->Draw();
// 			iClusterShape++;
// 		}
// 		TText* sizeText = new TText((double) nominalHeader * .5 / (nominalHeader + plotsWidth), ((double) plotsHeight / (plotsHeight + nominalHeader)) * (((double) nHeight - iClusterSize - .5) / nHeight), Form("%d", clusterSize));
// 		textSet.push_back(sizeText);
// 		sizeText->SetNDC();
// 		sizeText->SetTextAlign(22);
// 		sizeText->SetTextSize(.6 * nominalWidth / (plotsHeight + nominalHeader));
// 		sizeText->SetTextColor(kBlack);
// 		sizeText->Draw();
// 		iClusterSize++;
// 	}
// 	TText* titleText = new TText(.5, 1. - .5 * nominalHeader / (nominalHeader + plotsHeight), static_cast<TString>("Total Cluster Shapes"));
// 	titleText->SetTextSize(.8 * nominalHeader / (plotsHeight + nominalHeader));
// 	titleText->SetTextAlign(22);
// 	titleText->SetNDC();
// 	titleText->Draw();

// 	std::filesystem::path file = filePath / config.find("filename");
// 	file.replace_extension(config.find("extension"));
// 	canvas->SaveAs(static_cast<TString>(file));

// 	delete titleText;
// 	titleText = nullptr;

// 	for ( TPad* pad : padSet ) {
// 		delete pad;
// 		pad = nullptr;
// 	}

// 	for ( TLine* line : lineSet ) {
// 		delete line;
// 		line = nullptr;
// 	}

// 	for ( TText* text : textSet ) {
// 		delete text;
// 		text = nullptr;
// 	}

// 	delete canvas;
// 	canvas = nullptr;
// }

// void TAnalyser::saveSameSizeShapeEntry(std::string_view typeName, const CppConfigDictionary config) {
// 	std::cout << "Generating \033[1;32mEntry of shapes with same size\033[1;0m..." << std::endl;

// 	std::filesystem::path filePath(config.find("output_path"));
// 	if ( config.hasKey("subdirectory") ) {
// 		filePath /= config.find("subdirectory");
// 	}
// 	std::filesystem::create_directories(filePath);

// 	ProgressBar pBar(mNTotalShapeSet.find(std::string(typeName))->second);
// 	for ( const TClusterShape* clusterShape : mClusterShapeSet.find(std::string(typeName))->second ) {
// 		int clusterSize = clusterShape->getClusterSize();
// 		TCanvas* canvas = new TCanvas(Form("shapeEntry%d", clusterSize), "shapeEntry", 2500, 1000);
// 		int binNum = clusterShape->getClusterShapeInfos().size();
// 		TH1D* distribution = new TH1D(Form("shapeEntry%d", clusterSize), Form(static_cast<TString>("Shape Entry with cluster size %d"), clusterSize), binNum, -.5, binNum - .5);
// 		int iShape = 0;
// 		for ( const TShapeInfo& shapeInfo : clusterShape->getClusterShapeInfos() ) {
// 			pBar.printProgress();
// 			distribution->Fill(iShape, shapeInfo.mEntry);
// 			iShape++;
// 		}
// 		distribution->GetXaxis()->SetNdivisions(10, 10, 0, true);
// 		distribution->GetXaxis()->SetTitle("i-th cluster size");
// 		distribution->GetXaxis()->SetTitleOffset(1.4);
// 		distribution->GetXaxis()->SetLabelOffset(0.003);
// 		distribution->GetYaxis()->SetTitle("Entry");
// 		distribution->GetYaxis()->SetTitleOffset(0.7);
// 		distribution->Draw("HIST");
// 		mExpSettingLegend->Draw("SAME");
// 		canvas->SetMargin(.07, .28, .12, .08);

// 		std::filesystem::path file = filePath / (config.find("filename") + "_" + std::to_string(clusterSize));
// 		file.replace_extension(config.find("extension"));
// 		canvas->SaveAs(static_cast<TString>(file));

// 		delete distribution;
// 		delete canvas;
// 	}
// }

// void TAnalyser::saveTotalShapeEntry(std::string_view typeName, const CppConfigDictionary config) {
// 	std::cout << "Generating \033[1;32mEntry of total shapes\033[1;0m..." << std::endl;

// 	std::filesystem::path filePath(config.find("output_path"));
// 	if ( config.hasKey("subdirectory") ) {
// 		filePath /= config.find("subdirectory");
// 	}
// 	std::filesystem::create_directories(filePath);

// 	int nXbin = 0;
// 	int nYbin = 0;
// 	for ( const TClusterShape* clusterShape : mClusterShapeSet.find(std::string(typeName))->second ) {
// 		nYbin = std::max(nYbin, static_cast<int>(clusterShape->getClusterShapeInfos().size()));
// 		nXbin = std::max(nXbin, clusterShape->getClusterSize());
// 	}
// 	TCanvas* canvas = new TCanvas("shapeEntryTotal", "shape entry", 2500, 1000);
// 	TH2D* distribution = new TH2D("shapeEntryTotal", static_cast<TString>(config.find("plot_titles")), nXbin, -nXbin - .5, -.5, nYbin, -nYbin + .5, .5);
// 	ProgressBar pBar(mNTotalShapeSet.find(std::string(typeName))->second);
// 	for ( const TClusterShape* clusterShape : mClusterShapeSet.find(std::string(typeName))->second ) {
// 		int clusterSize = clusterShape->getClusterSize();
// 		int iShape = 0;
// 		for ( const TShapeInfo shapeInfo : clusterShape->getClusterShapeInfos() ) {
// 			pBar.printProgress();
// 			distribution->Fill(-clusterSize, -iShape, shapeInfo.mEntry);
// 			iShape++;
// 		}
// 	}
// 	for ( int iXbin = 1; iXbin < distribution->GetNbinsX(); ++iXbin ) {
// 		distribution->GetXaxis()->SetBinLabel(iXbin, Form("%g", floor(-1 * distribution->GetXaxis()->GetBinLowEdge(iXbin))));
// 	}

// 	for ( int iYbin = 1; iYbin < distribution->GetNbinsY(); ++(++iYbin) ) {
// 		distribution->GetYaxis()->SetBinLabel(iYbin, Form("%g", floor(-1 * distribution->GetYaxis()->GetBinLowEdge(iYbin))));
// 	}

// 	distribution->SetStats(0);
// 	if ( config.hasKey("options") ) {
// 		for ( const std::string& optionName : config.getSubConfig("options").getValueList() ) {
// 			distribution->Draw(static_cast<TString>(optionName));
// 			canvas->SetPhi(10);
// 			canvas->SetTheta(25);
// 			canvas->SetLogz();
// 			std::filesystem::path file = filePath;
// 			file /= (config.find("filename") + "_" + optionName);
// 			file.replace_extension(config.find("extension"));
// 			canvas->SaveAs(static_cast<TString>(file));
// 		}
// 	} else {
// 		distribution->Draw();
// 		std::filesystem::path file = filePath;
// 		file /= config.find("filename");
// 		file.replace_extension(config.find("extension"));
// 		canvas->SaveAs(static_cast<TString>(file));
// 	}

// 	delete distribution;
// 	delete canvas;
// }