#define __TCLUSTERANALYSER_HEADERS__

#include "TClusterAnalyser.h"
/**
 * @brief Construct a new TClusterAnalyser::TClusterAnalyser object
 *
 * @param analyser
*/
TClusterAnalyser::TClusterAnalyser(const TAnalyser& analyser) : TAnalyser(analyser), fBits(kNotDeleted) {
	std::clog << "TClusterAnalyser object is armed." << std::endl;
}

TClusterAnalyser::TClusterAnalyser(const TClusterAnalyser& copy) : TAnalyser(copy) {
	std::clog << "Copy TClusterAnalyser object is armed." << std::endl;
}
/**
 * @brief Destroy the TClusterAnalyser::TClusterAnalyser object
 *
*/
TClusterAnalyser::~TClusterAnalyser() {
	// for ( const auto& pair : mClustermaps ) {
	// 	delete pair.second;
	// }
	// for ( const auto& pair : mClustersizes ) {
	// 	delete pair.second;
	// }
	// for ( const auto& pair : mClusterDataWithShape ) {
	// 	for ( const auto& pair2 : pair.second ) {
	// 		for ( const TCluster* cluster : pair2.second ) {
	// 			delete cluster;
	// 		}
	// 	}
	// }
	std::clog << "TClusterAnalyser object is terminated." << std::endl;
}

// std::vector<int> getClusterSizeRange(const CppConfigDictionary& privateProperty) {
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
//  * @brief Generalized function to draw clustermap
//  *
//  * @param config Draw configuration for map title, directory and filename
//  * @param clusters Dataset to draw
//  * @return const TH2*
// */
// TH2D* TClusterAnalyser::getClusterPlot(const CppConfigDictionary& config, const std::vector<TCluster*>& clusters) {
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
// TH1D* TClusterAnalyser::getClustersizePlot(const CppConfigDictionary& config, const std::vector<TCluster*>& clusters) {
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

// void TClusterAnalyser::saveClustermap(std::string typeName, const CppConfigDictionary& config) {
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

// void TClusterAnalyser::saveClustersize(std::string typeName, const CppConfigDictionary& config) {
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

// void TClusterAnalyser::setClusterDataWithShape(const std::vector<int>& clusterSizeRange) {
// 	for ( const int clusterSize : clusterSizeRange ) {
// 		std::vector<TCluster*> clustersWithShape;
// 	}
// }




// void TClusterAnalyser::saveHitmapByClustersize(const CppConfigDictionary& config) {
// 	// std::filesystem::create_directories(mSavePath / "hitmap_by_cluster_size");
// 	// for ( int clusterSize = 1; clusterSize < 80; clusterSize++ ) {
// 	// 	TH2D* clusterHitmap = new TH2D(Form("hitmap%d", clusterSize), Form("Hitmap of cluster size %d", clusterSize), 1024, 0, 1024, 512, 0, 512);
// 	// 	TH2D* clusterClustermap = new TH2D(Form("clustermap%d", clusterSize), Form("Clustermap of cluster size %d", clusterSize), 1024, 0, 1024, 512, 0, 512);
// 	// 	TH1D* clusterBinFire = new TH1D(Form("fired%d", clusterSize), Form("The number of fired of each bins in cluster size %d", clusterSize), 50, 0, 50);
// 	// 	// for ( const TCluster* cluster : mExpData->getClusters() ) {
// 	// 	// 	if ( cluster->getSize() == clusterSize ) {
// 	// 	// 		for ( const std::pair<int, int>& pixel : cluster->getPixels() ) {
// 	// 	// 			clusterHitmap->Fill(pixel.first, pixel.second);
// 	// 	// 		}
// 	// 	// 		clusterClustermap->Fill(cluster->getCenter().first, cluster->getCenter().second);
// 	// 	// 	}
// 	// 	// }
// 	// 	for ( int iRow = 0; iRow < 1024; iRow++ ) {
// 	// 		for ( int iColumn = 0; iColumn < 512; iColumn++ ) {
// 	// 			if ( clusterHitmap->GetBinContent(iRow, iColumn) != 0 ) {
// 	// 				clusterBinFire->Fill(clusterHitmap->GetBinContent(iRow, iColumn));
// 	// 			}
// 	// 		}
// 	// 	}
// 	// 	if ( clusterBinFire->GetEntries() != 0 ) {
// 	// 		TCanvas* hcanvas = new TCanvas(Form("hcan%d", clusterSize), "", 2000, 1000);
// 	// 		clusterHitmap->Draw();
// 	// 		// hcanvas->SaveAs(static_cast<TString>(mSavePath / "hitmap_by_cluster_size" / ("hitmap_cs_" + std::to_string(clusterSize) + ".png")));
// 	// 		TCanvas* ccanvas = new TCanvas(Form("ccan%d", clusterSize), "", 2000, 1000);
// 	// 		clusterClustermap->Draw();
// 	// 		// ccanvas->SaveAs(static_cast<TString>(mSavePath / "hitmap_by_cluster_size" / ("clustermap_cs_" + std::to_string(clusterSize) + ".png")));
// 	// 		TCanvas* dcanvas = new TCanvas(Form("dcan%d", clusterSize), "", 1000, 1000);
// 	// 		clusterBinFire->Draw();
// 	// 		dcanvas->SetLogy();
// 	// 		// dcanvas->SaveAs(static_cast<TString>(mSavePath / "hitmap_by_cluster_size" / ("fire_distribution_cs_" + std::to_string(clusterSize) + ".png")));

// 	// 	}
// 	// }
// }
