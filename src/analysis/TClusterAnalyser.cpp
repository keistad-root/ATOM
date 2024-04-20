#include "TClusterAnalyser.h"
/**
 * @brief Construct a new TClusterAnalyser::TClusterAnalyser object
 *
 * @param analyser
*/
TClusterAnalyser::TClusterAnalyser(const TAnalyser& analyser) : TAnalyser(analyser), fBits(kNotDeleted) {
	std::clog << "TClusterAnalyser object is armed." << std::endl;
}
/**
 * @brief Destroy the TClusterAnalyser::TClusterAnalyser object
 *
*/
TClusterAnalyser::~TClusterAnalyser() {
	if ( mClustermap != nullptr && !mClustermap->IsDestructed() ) {
		delete mClustermap;
		mClustermap = nullptr;
	}
	if ( mMaskedClustermap != nullptr && !mMaskedClustermap->IsDestructed() ) {
		delete mMaskedClustermap;
		mMaskedClustermap = nullptr;
	}
	if ( mNoisePixelClustermap != nullptr && !mNoisePixelClustermap->IsDestructed() ) {
		delete mNoisePixelClustermap;
		mNoisePixelClustermap = nullptr;
	}
	if ( mClustersize != nullptr && !mClustersize->IsDestructed() ) {
		delete mClustersize;
		mClustersize = nullptr;
	}
	if ( mMaskedClustersize != nullptr && !mMaskedClustersize->IsDestructed() ) {
		delete mMaskedClustersize;
		mMaskedClustersize = nullptr;
	}
	if ( mNoisePixelClustersize != nullptr && !mNoisePixelClustersize->IsDestructed() ) {
		delete mNoisePixelClustersize;
		mNoisePixelClustersize = nullptr;
	}
	std::clog << "TClusterAnalyser object is terminated." << std::endl;
}

/**
 * @brief Generalized function to draw clustermap
 *
 * @param config Draw configuration for map title, directory and filename
 * @param clusters Dataset to draw
 * @return const TH2*
*/
TH2D* TClusterAnalyser::getClusterPlot(const Configurable& config, const std::vector<TCluster*>& clusters) {
	// Static variable for numbering
	static int iMap = 0;
	// Allocate a 2d histogram.
	TH2D* map = new TH2D(Form("map%d", iMap), static_cast<TString>(config.find("plot_titles")), 1024, 0, 1024, 512, 0, 512);
	// Fill data
	for ( const TCluster* cluster : clusters ) {
		map->Fill(cluster->getCenter().first, cluster->getCenter().second);
	}
	// Canvas setting
	TCanvas* canvas = new TCanvas(Form("mapCan%d", iMap), "", 2500, 1000);
	canvas->SetMargin(.07, .35, .12, .08);
	map->GetXaxis()->SetTitleOffset(1.4);
	map->GetXaxis()->SetLabelOffset(0.003);
	// Find directory for saving clustermap. If it doesn't exist, then make the directory with mother directories.
	std::filesystem::path filePath(config.find("file_path"));
	std::filesystem::create_directories(filePath.parent_path());

	// Draw plot with options
	if ( config.hasKey("options") ) {
		for ( const std::string& optionName : config.findlist("options") ) {
			map->Draw(static_cast<TString>(optionName));
			mExpSettingLegend->Draw("SAME");
			std::filesystem::path file = filePath.parent_path() / (filePath.stem().string() + "_" + optionName + filePath.extension().string());
			canvas->SaveAs(static_cast<TString>(file));
		}
	} else {
		map->Draw();
		mExpSettingLegend->Draw("SAME");
		canvas->SaveAs(static_cast<TString>(filePath));
	}
	// Delete canvas;
	delete canvas;
	iMap++;
	return map;
}
/**
 * @brief
 *
 * @param config
 * @param clusters
 * @return TH1D*
*/
TH1D* TClusterAnalyser::getClustersizePlot(const Configurable& config, const std::vector<TCluster*>& clusters) {
	static int iDistribution = 0;
	TH1D* distribution = new TH1D(Form("distribution%d", iDistribution), static_cast<TString>(config.find("plot_titles")), 80, .5, 80.5);
	for ( const TCluster* cluster : clusters ) {
		distribution->Fill(cluster->getSize()); // Fill clustersize to clustersize distribution
	}
	TCanvas* canvas = new TCanvas(Form("distributionCan%d", iDistribution), "", 2500, 1000);
	canvas->SetMargin(.07, .28, .12, .08);
	distribution->GetXaxis()->SetTitleOffset(1.4);
	distribution->GetXaxis()->SetLabelOffset(0.003);

	std::filesystem::path filePath(config.find("file_path"));
	std::filesystem::create_directories(filePath.parent_path());

	if ( config.hasKey("options") ) {
		for ( const std::string& optionName : config.findlist("options") ) {
			if ( optionName == "logy" ) {
				distribution->Draw();
				mExpSettingLegend->Draw("SAME");
				canvas->SetLogy();
				std::filesystem::path file = filePath.parent_path() / (filePath.stem().string() + "_" + optionName + filePath.extension().string());
				canvas->SaveAs(static_cast<TString>(file));
			} else if ( optionName == "basic" ) {
				distribution->Draw();
				mExpSettingLegend->Draw("SAME");
				canvas->SaveAs(static_cast<TString>(filePath));
			}
		}
	} else {
		distribution->Draw();
		mExpSettingLegend->Draw("SAME");
		canvas->SaveAs(static_cast<TString>(filePath));
	}
	delete canvas;
	iDistribution++;
	return distribution;
}

void TClusterAnalyser::saveClustermap(const Configurable& config) {
	std::clog << "Generating \033[1;32mClustermap\033[1;0m..." << std::endl;
	mClustermap = getClusterPlot(config, mExpData->getClusters());
}

void TClusterAnalyser::saveMaskedClustermap(const Configurable& config) {
	std::clog << "Generating \033[1;32mMasked Clustermap\033[1;0m..." << std::endl;
	mMaskedClustermap = getClusterPlot(config, mExpData->getMaskedClusters());
}

void TClusterAnalyser::saveNoisePixelClustermap(const Configurable& config) {
	std::clog << "Generating \033[1;32mNoise Pixel Clustermap\033[1;0m..." << std::endl;
	mNoisePixelClustermap = getClusterPlot(config, mExpData->getNoiseClusters());
}

void TClusterAnalyser::saveClustersize(const Configurable& config) {
	std::clog << "Generating \033[1;32mCluster Size Distribution\033[1;0m..." << std::endl;
	mClustersize = getClustersizePlot(config, mExpData->getClusters());
}

void TClusterAnalyser::saveMaskedClustersize(const Configurable& config) {
	std::clog << "Generating \033[1;32mMasked Cluster Size Distribution\033[1;0m..." << std::endl;
	mMaskedClustersize = getClustersizePlot(config, mExpData->getMaskedClusters());
}

void TClusterAnalyser::saveNoisePixelClustersize(const Configurable& config) {
	std::clog << "Generating \033[1;32mNoise Pixel cluster Size Distribution\033[1;0m..." << std::endl;
	mNoisePixelClustersize = getClustersizePlot(config, mExpData->getNoiseClusters());
}

void TClusterAnalyser::saveHitmapByClustersize(const Configurable& config) {
	std::filesystem::create_directories(mSavePath / "hitmap_by_cluster_size");
	for ( int clusterSize = 1; clusterSize < 80; clusterSize++ ) {
		TH2D* clusterHitmap = new TH2D(Form("hitmap%d", clusterSize), Form("Hitmap of cluster size %d", clusterSize), 1024, 0, 1024, 512, 0, 512);
		TH2D* clusterClustermap = new TH2D(Form("clustermap%d", clusterSize), Form("Clustermap of cluster size %d", clusterSize), 1024, 0, 1024, 512, 0, 512);
		TH1D* clusterBinFire = new TH1D(Form("fired%d", clusterSize), Form("The number of fired of each bins in cluster size %d", clusterSize), 50, 0, 50);
		for ( const TCluster* cluster : mExpData->getClusters() ) {
			if ( cluster->getSize() == clusterSize ) {
				for ( const std::pair<int, int>& pixel : cluster->getPixels() ) {
					clusterHitmap->Fill(pixel.first, pixel.second);
				}
				clusterClustermap->Fill(cluster->getCenter().first, cluster->getCenter().second);
			}
		}
		for ( int iRow = 0; iRow < 1024; iRow++ ) {
			for ( int iColumn = 0; iColumn < 512; iColumn++ ) {
				if ( clusterHitmap->GetBinContent(iRow, iColumn) != 0 ) {
					clusterBinFire->Fill(clusterHitmap->GetBinContent(iRow, iColumn));
				}
			}
		}
		if ( clusterBinFire->GetEntries() != 0 ) {
			TCanvas* hcanvas = new TCanvas(Form("hcan%d", clusterSize), "", 2000, 1000);
			clusterHitmap->Draw();
			hcanvas->SaveAs(static_cast<TString>(mSavePath / "hitmap_by_cluster_size" / ("hitmap_cs_" + std::to_string(clusterSize) + ".png")));
			TCanvas* ccanvas = new TCanvas(Form("ccan%d", clusterSize), "", 2000, 1000);
			clusterClustermap->Draw();
			ccanvas->SaveAs(static_cast<TString>(mSavePath / "hitmap_by_cluster_size" / ("clustermap_cs_" + std::to_string(clusterSize) + ".png")));
			TCanvas* dcanvas = new TCanvas(Form("dcan%d", clusterSize), "", 1000, 1000);
			clusterBinFire->Draw();
			dcanvas->SetLogy();
			dcanvas->SaveAs(static_cast<TString>(mSavePath / "hitmap_by_cluster_size" / ("fire_distribution_cs_" + std::to_string(clusterSize) + ".png")));

		}
	}
}
