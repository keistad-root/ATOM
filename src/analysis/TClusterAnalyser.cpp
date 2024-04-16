#include "TClusterAnalyser.h"

TClusterAnalyser::TClusterAnalyser(const TAnalyser& analyser) : TAnalyser(analyser) { }

TClusterAnalyser::~TClusterAnalyser() { }

void TClusterAnalyser::saveClustermap() {
	std::clog << "Generating Clustermap..." << std::endl;
	TH2D* map = new TH2D("clustermap", "clustermap", 1024, 0, 1024, 512, 0, 512);
	for ( const TCluster* cluster : mExpData->getClusters() ) {
		map->Fill(cluster->getCenter().first, cluster->getCenter().second); // Fill cluster center coordinate to clustermap
	}
	TCanvas* canvas = new TCanvas("clustermap", "Clustermap", 2500, 1000);
	canvas->SetMargin(.07, .35, .12, .08);
	map->GetXaxis()->SetTitleOffset(1.4);
	map->GetXaxis()->SetLabelOffset(0.003);
	std::filesystem::create_directories(mSavePath / "clustermap");
	map->Draw("COLZ");
	mExpSettingLegend->Draw("SAME");
	canvas->SaveAs(static_cast<TString>(mSavePath / "clustermap" / "clustermap_colz.png"));
	map->Draw("LEGO0");
	mExpSettingLegend->Draw("SAME");
	canvas->SaveAs(static_cast<TString>(mSavePath / "clustermap" / "clustermap_lego.png"));

	mClustermap = map;
	map = nullptr;
}

void TClusterAnalyser::saveMaskedClustermap() {
	std::clog << "Generating Masked Clustermap..." << std::endl;
	TH2D* map = new TH2D("maskedClustermap", "Masked Clustermap", 1024, 0, 1024, 512, 0, 512);
	for ( const TCluster* cluster : mExpData->getMaskedClusters() ) {
		map->Fill(cluster->getCenter().first, cluster->getCenter().second); // Fill cluster center coordinate to clustermap
	}
	TCanvas* canvas = new TCanvas("maskedClustermap", "Masked Clustermap", 2500, 1000);
	canvas->SetMargin(.07, .35, .12, .08);
	map->GetXaxis()->SetTitleOffset(1.4);
	map->GetXaxis()->SetLabelOffset(0.003);
	std::filesystem::create_directories(mSavePath / "masked_clustermap");
	map->Draw("COLZ");
	mExpSettingLegend->Draw("SAME");
	canvas->SaveAs(static_cast<TString>(mSavePath / "masked_clustermap" / "masked_clustermap_colz.png"));
	map->Draw("LEGO0");
	mExpSettingLegend->Draw("SAME");
	canvas->SaveAs(static_cast<TString>(mSavePath / "masked_clustermap" / "masked_clustermap_lego.png"));

	delete canvas;
	mMaskedClustermap = map;
	map = nullptr;
}

void TClusterAnalyser::saveClustersize() {
	std::clog << "Generating Cluster Size Distribution..." << std::endl;
	TH1D* distribution = new TH1D("clustersize", "Cluster Size", 80, .5, 80.5);
	for ( const TCluster* cluster : mExpData->getClusters() ) {
		distribution->Fill(cluster->getSize()); // Fill clustersize to clustersize distribution
	}
	TCanvas* canvas = new TCanvas("clustersize", "Cluster Size", 2500, 1000);
	canvas->SetMargin(.07, .28, .12, .08);
	distribution->GetXaxis()->SetTitleOffset(1.4);
	distribution->GetXaxis()->SetLabelOffset(0.003);

	std::filesystem::create_directories(mSavePath / "clustersize");
	distribution->Draw();
	mExpSettingLegend->Draw("SAME");
	canvas->SaveAs(static_cast<TString>(mSavePath / "clustersize" / "clustersize_colz.png"));
	canvas->SetLogy();
	canvas->SaveAs(static_cast<TString>(mSavePath / "clustersize" / "clustersize_lego.png"));

	delete canvas;
	mClustersize = distribution;
	distribution = nullptr;
}

void TClusterAnalyser::saveMaskedClustersize() {
	std::clog << "Generating Masked Clsuter Size Distribution..." << std::endl;
	TH1D* distribution = new TH1D("maskedClustersize", "Masked Cluster Size", 80, .5, 80.5);
	for ( const TCluster* cluster : mExpData->getMaskedClusters() ) {
		distribution->Fill(cluster->getSize()); // Fill clustersize to clustersize distribution
	}
	TCanvas* canvas = new TCanvas("maskedClustersize", "Masked Cluster Size", 2500, 1000);
	canvas->SetMargin(.07, .28, .12, .08);
	distribution->GetXaxis()->SetTitleOffset(1.4);
	distribution->GetXaxis()->SetLabelOffset(0.003);

	std::filesystem::create_directories(mSavePath / "masked_clustersize");
	distribution->Draw();
	mExpSettingLegend->Draw("SAME");
	canvas->SaveAs(static_cast<TString>(mSavePath / "masked_clustersize" / "masked_clustersize_colz.png"));
	canvas->SetLogy();
	canvas->SaveAs(static_cast<TString>(mSavePath / "masked_clustersize" / "masked_clustersize_lego.png"));

	delete canvas;
	mMaskedClustersize = distribution;
	distribution = nullptr;
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

 // /**
 //  * @brief Save all picture of shapes and entry of the shape in same cluster size
 //  * @details
 //  *
 //  * @param savePath
 //  * @param saveRootFile
 //  */
 // void TClusterAnalyser::saveShapes(const std::filesystem::path& savePath, TFile* saveRootFile) {
 // 	TDirectory* mainDir = saveRootFile->mkdir("shapes");
 // 	mainDir->cd();
 // 	std::filesystem::create_directory(savePath / "shapes");
 // 	for ( const TClusterShape* clusterN : mClusterN ) {
 // 		int clusterSize = clusterN->getClusterSize();
 // 		std::vector<std::pair<TCluster*, int>> clustersWithShape = clusterN->getClustersWithShape();
 // 		std::vector<TImage*> shapeImages;

 // 		std::filesystem::create_directory(savePath / "shapes" / static_cast<std::string>(Form("ClusterShapes(size=%d)", clusterSize)));
 // 		TDirectory* dir = mainDir->mkdir(Form("ClusterShapes(size=%d)", clusterSize));
 // 		dir->cd();
 // 		int iShape = 0;
 // 		for ( const std::pair<TCluster*, int>& clusterWithShape : clustersWithShape ) {
 // 			TMatrix2D<int> matrix = clusterWithShape.first->getShape();
 // 			int nRow = matrix.getNRow();
 // 			int nColumn = matrix.getNColumn();
 // 			TH2I* shapeMap = new TH2I(Form("shapeMap%d", iShape), "", nRow + 2, 0, nRow + 2, nColumn + 2, 0, nColumn + 2);
 // 			for ( int iRow = 0; iRow < nRow; iRow++ ) {
 // 				for ( int iColumn = 0; iColumn < nColumn; iColumn++ ) {
 // 					if ( matrix.getElement(iRow, iColumn) == 1 ) {
 // 						shapeMap->Fill(iRow + 1, iColumn + 1);
 // 					}
 // 				}
 // 			}
 // 			TCanvas* shapeCan = new TCanvas(Form("can%d_%d", iShape, clusterSize), Form("Cluster Shape with cluster size %d(#: %d)", clusterSize, clusterWithShape.second), 100 * (nRow + 2), 100 * (nColumn + 2));
 // 			shapeCan->SetMargin(0, 0, 0, 0);
 // 			shapeCan->SetGrid();
 // 			shapeMap->GetXaxis()->SetNdivisions(nRow + 2, 0, 0, true);
 // 			shapeMap->GetYaxis()->SetNdivisions(nColumn + 2, 0, 0, true);
 // 			shapeMap->GetZaxis()->SetNdivisions(0, 0, 0, true);
 // 			shapeMap->SetStats(0);
 // 			shapeMap->Draw("COLZ L");
 // 			TImage* shapeImage = TImage::Create();
 // 			shapeImage->FromPad(shapeCan);
 // 			shapeImages.push_back(shapeImage);
 // 			shapeMap->SetTitle(Form("Cluster Shape with cluster size %d(#: %d)", clusterSize, clusterWithShape.second));
 // 			shapeCan->SaveAs(static_cast<TString>(savePath / "shapes" / static_cast<std::string>(Form("ClusterShapes(size=%d)", clusterSize)) / static_cast<std::string>(Form("%dth Cluster Shape(size = %d).png", iShape, clusterSize))));
 // 			shapeCan->Write();
 // 			iShape++;
 // 		}
 // 		mShapeImages.push_back(shapeImages);
 // 		iShape = 0;
 // 		dir->Write();
 // 		mainDir->cd();
 // 		dir = nullptr;
 // 	}
 // }

 // void TClusterAnalyser::saveShapeOrder(const std::filesystem::path& savePath, TFile* saveRootFile) {
 // 	TDirectory* mainDir = saveRootFile->mkdir("shapesOrder");
 // 	mainDir->cd();
 // 	int iClusterSize = 0;
 // 	for ( const TClusterShape* clusterN : mClusterN ) {
 // 		int clusterSize = clusterN->getClusterSize();
 // 		std::vector<std::pair<TCluster*, int>> clustersWithShape = clusterN->getClustersWithShape();
 // 		int plotsWidth = 1000;
 // 		double plotsHeight = (floor((clustersWithShape.size() - 1) / 10) + 1) * 120;
 // 		TCanvas* shapeOrderCan = new TCanvas(Form("shapeOrderCan%d", clusterSize), Form("Cluster shapes of cluster size %d", clusterSize), plotsWidth, plotsHeight + 100);
 // 		TText* title = new TText(.5, (plotsHeight + 60.) / (plotsHeight + 120.), Form("Cluster Shapes of cluster size %d", clusterSize));
 // 		title->SetTextSize(50. / (plotsHeight + 100.));
 // 		title->SetTextAlign(22);
 // 		title->SetTextColor(kBlack);
 // 		title->Draw();
 // 		for ( int i = 0; i < clustersWithShape.size(); i++ ) {
 // 			double scaleFactor = std::max(mShapeImages[iClusterSize][i]->GetWidth(), mShapeImages[iClusterSize][i]->GetHeight());
 // 			double padBottom = ceil(((plotsHeight - 60.) / (plotsHeight + 120.) - (120. / (plotsHeight + 120)) * floor(i / 10) - (60. / (plotsHeight + 120.)) * (mShapeImages[iClusterSize][i]->GetHeight() / scaleFactor)) * 1000.) / 1000.;
 // 			double padTop = floor(((plotsHeight - 60.) / (plotsHeight + 120.) - (120. / (plotsHeight + 120)) * floor(i / 10) + (60. / (plotsHeight + 120.)) * (mShapeImages[iClusterSize][i]->GetHeight() / scaleFactor)) * 1000.) / 1000.;
 // 			TPad* pad = new TPad(Form("pad%d", i),
 // 								 "",
 // 								 0.05 + 0.1 * (static_cast<int>(i % 10)) - 0.05 * (mShapeImages[iClusterSize][i]->GetWidth() / scaleFactor),
 // 								 padBottom,
 // 								 0.05 + 0.1 * (static_cast<int>(i % 10)) + 0.05 * (mShapeImages[iClusterSize][i]->GetWidth() / scaleFactor),
 // 								 padTop);
 // 			mShapeImages[iClusterSize][i]->Scale((mShapeImages[iClusterSize][i]->GetWidth() / scaleFactor) * 99, (mShapeImages[iClusterSize][i]->GetHeight() / scaleFactor) * 99);
 // 			pad->Draw();
 // 			pad->cd();
 // 			pad->SetMargin(0, 0, 0, 0);
 // 			mShapeImages[iClusterSize][i]->Draw();
 // 			TText* t = new TText(.5, .5, Form("%d", i));
 // 			t->SetTextSize(.4);
 // 			t->SetTextAlign(22);
 // 			t->SetTextColor(kGray + 2);
 // 			t->Draw();
 // 			shapeOrderCan->cd();
 // 		}
 // 		iClusterSize++;
 // 		shapeOrderCan->SaveAs(static_cast<TString>(savePath / "shapes" / Form("shape_order_%d.png", clusterSize)));
 // 		shapeOrderCan->Write();
 // 	}
 // }

 // void TClusterAnalyser::saveShapeInformation(const std::filesystem::path& savePath, TFile* saveRootFile) {
 // 	TDirectory* mainDir = saveRootFile->mkdir("shapesInfo");
 // 	mainDir->cd();
 // 	int iClusterSize = 0;
 // 	for ( const TClusterShape* clusterN : mClusterN ) {
 // 		int clusterSize = clusterN->getClusterSize();
 // 		std::vector<std::pair<TCluster*, int>> clustersWithShape = clusterN->getClustersWithShape();

 // 		TGraph* rowGraph = new TGraph();
 // 		TGraph* columnGraph = new TGraph();
 // 		TGraph* rowColumnGraph = new TGraph();
 // 		TGraph* stdevXGraph = new TGraph();
 // 		TGraph* stdevYGraph = new TGraph();
 // 		TGraph* stdevGraph = new TGraph();
 // 		TLegend* dotLegend = new TLegend(0.67, 0.57, 0.98, 0.75);
 // 		dotLegend->SetNColumns(3);
 // 		dotLegend->SetTextSize(.02);
 // 		TLegend* graphLegend = new TLegend(0.67, 0.77, 0.98, 0.93);
 // 		graphLegend->AddEntry(rowGraph, "Row", "p");
 // 		graphLegend->AddEntry(columnGraph, "Column", "p");
 // 		graphLegend->AddEntry(rowColumnGraph, "Row + Column", "p");
 // 		graphLegend->AddEntry(stdevXGraph, "Stdev Row", "p");
 // 		graphLegend->AddEntry(stdevYGraph, "Stdev Column", "p");
 // 		graphLegend->AddEntry(stdevGraph, "RMS of distance", "p");

 // 		int iShape = 0;
 // 		for ( const std::pair<TCluster*, int>& clusterWithShape : clustersWithShape ) {
 // 			TMatrix2D<int> matrix = clusterWithShape.first->getShape();
 // 			int nRow = matrix.getNRow();
 // 			int nColumn = matrix.getNColumn();
 // 			rowGraph->SetPoint(iShape, iShape, nRow);
 // 			columnGraph->SetPoint(iShape, iShape + (1. / 16), nColumn);
 // 			rowColumnGraph->SetPoint(iShape, iShape, nRow + nColumn);
 // 			stdevXGraph->SetPoint(iShape, iShape, clusterWithShape.first->getStdevInAxis().first);
 // 			stdevYGraph->SetPoint(iShape, iShape, clusterWithShape.first->getStdevInAxis().second);
 // 			stdevGraph->SetPoint(iShape, iShape, clusterWithShape.first->getStdev());
 // 			dotLegend->AddEntry(rowGraph, Form("%dth Entry: %d", iShape, clusterWithShape.second), "");
 // 			iShape++;
 // 		}

 // 		TCanvas* shapeRowCan = new TCanvas(Form("shapeRow%d", clusterSize), "The number of rows in same cluster shape", 1500, 950);
 // 		rowColumnGraph->GetXaxis()->SetLimits(0, iShape);
 // 		rowColumnGraph->GetXaxis()->SetNdivisions(iShape, 0, 0, true);
 // 		rowColumnGraph->SetTitle(Form("Cluster Shape Informations (Cluster Size: %d)", clusterSize));
 // 		rowColumnGraph->GetXaxis()->SetTitle("Shape Number");
 // 		rowColumnGraph->GetYaxis()->SetTitle("\\mathrm{Length}(1 = 28\\mathrm{\\mu m})");
 // 		rowColumnGraph->GetYaxis()->SetLabelOffset(0.005);
 // 		rowColumnGraph->GetYaxis()->SetTitleOffset(0.7);
 // 		rowColumnGraph->SetMinimum(0);
 // 		rowColumnGraph->SetMarkerStyle(20);
 // 		rowColumnGraph->SetMarkerColor(kRed);
 // 		rowColumnGraph->SetMarkerSize(1);
 // 		rowColumnGraph->Draw("AP");
 // 		rowGraph->SetMarkerStyle(20);
 // 		rowGraph->SetMarkerColor(kMagenta);
 // 		rowGraph->SetMarkerSize(1);
 // 		rowGraph->Draw("P");
 // 		columnGraph->SetMarkerStyle(20);
 // 		columnGraph->SetMarkerColor(kOrange);
 // 		columnGraph->SetMarkerSize(1);
 // 		columnGraph->Draw("P");
 // 		stdevXGraph->SetMarkerStyle(20);
 // 		stdevXGraph->SetMarkerColor(kGreen);
 // 		stdevXGraph->SetMarkerSize(1);
 // 		stdevXGraph->Draw("P");
 // 		stdevYGraph->SetMarkerStyle(20);
 // 		stdevYGraph->SetMarkerColor(kCyan);
 // 		stdevYGraph->SetMarkerSize(1);
 // 		stdevYGraph->Draw("P");
 // 		stdevGraph->SetMarkerStyle(20);
 // 		stdevGraph->SetMarkerColor(kBlue);
 // 		stdevGraph->SetMarkerSize(1);
 // 		stdevGraph->Draw("P");
 // 		dotLegend->Draw();
 // 		graphLegend->Draw();
 // 		for ( int i = 0; i < clustersWithShape.size(); i++ ) {
 // 			double scaleFactor = std::max(mShapeImages[iClusterSize][i]->GetWidth(), mShapeImages[iClusterSize][i]->GetHeight());
 // 			TPad* pad = new TPad(Form("pad%d", i),
 // 								 "",
 // 								 0.05 + 0.1 * static_cast<int>(i % 10) - 0.05 * (mShapeImages[iClusterSize][i]->GetWidth() / scaleFactor),
 // 								 0.4 - 0.16 * static_cast<int>(i / 10) - 0.08 * (mShapeImages[iClusterSize][i]->GetHeight() / scaleFactor),
 // 								 0.05 + 0.1 * static_cast<int>(i % 10) + 0.05 * (mShapeImages[iClusterSize][i]->GetWidth() / scaleFactor),
 // 								 0.4 - 0.16 * static_cast<int>(i / 10) + 0.08 * (mShapeImages[iClusterSize][i]->GetHeight() / scaleFactor));
 // 			mShapeImages[iClusterSize][i]->Scale((mShapeImages[iClusterSize][i]->GetWidth() / scaleFactor) * 150, (mShapeImages[iClusterSize][i]->GetHeight() / scaleFactor) * 140);
 // 			pad->Draw();
 // 			pad->cd();
 // 			pad->SetMargin(0, 0, 0, 0);
 // 			mShapeImages[iClusterSize][i]->Draw();
 // 			TText* t = new TText(.5, .5, Form("%d", i));
 // 			t->SetTextSize(.4);
 // 			t->SetTextAlign(22);
 // 			t->SetTextColor(kGray + 2);
 // 			t->Draw();
 // 			shapeRowCan->cd();
 // 		}
 // 		shapeRowCan->SetMargin(0.06, 0.35, 0.57, 0.07);
 // 		shapeRowCan->SaveAs(static_cast<TString>(savePath / "shapes" / Form("shape_info_%d.png", clusterSize)));
 // 		shapeRowCan->Write();
 // 		iShape = 0;
 // 		iClusterSize++;
 // 	}
 // }

 // void TClusterAnalyser::saveShapeEntry(const std::filesystem::path& savePath, TFile* saveRootFile) {
 // 	TDirectory* mainDir = saveRootFile->mkdir("shapesEntry");
 // 	mainDir->cd();
 // 	int maxX = 0;
 // 	int clusterSize = 0;
 // 	for ( const TClusterShape* clusterN : mClusterN ) {
 // 		clusterSize = clusterN->getClusterSize();
 // 		std::vector<std::pair<TCluster*, int>> clustersWithShape = clusterN->getClustersWithShape();
 // 		maxX = std::max(maxX, static_cast<int>(clustersWithShape.size()));
 // 		TCanvas* shapeEntryCan = new TCanvas(Form("shapeEntryCan%d", clusterSize), Form("The entries of shapes which cluster sizes are %d", clusterSize), 2500, 1000);
 // 		TH1D* shapeEntry = new TH1D(Form("shapeEntry%d", clusterSize), Form("The entries of shapes which cluster sizes are %d", clusterSize), clustersWithShape.size(), -0.5, clustersWithShape.size() - 0.5);
 // 		int iShape = 0;
 // 		for ( const std::pair<TCluster*, int>& clusterWithShape : clustersWithShape ) {
 // 			shapeEntry->Fill(iShape, clusterWithShape.second);
 // 			iShape++;
 // 		}
 // 		shapeEntry->GetXaxis()->SetNdivisions(clustersWithShape.size(), 0, 0, true);
 // 		shapeEntry->GetXaxis()->SetTitle("i-th cluster size");
 // 		shapeEntry->GetXaxis()->SetTitleOffset(1.4);
 // 		shapeEntry->GetXaxis()->SetLabelOffset(0.003);
 // 		shapeEntry->GetYaxis()->SetTitle("Entry");
 // 		shapeEntry->GetYaxis()->SetTitleOffset(0.7);
 // 		shapeEntry->Draw("HIST");
 // 		settingLegend->Draw("SAME");
 // 		shapeEntryCan->SetMargin(.07, .35, .12, .08);
 // 		shapeEntryCan->SaveAs(static_cast<TString>(savePath / "shapes" / Form("shape_entry_%d.png", clusterSize)));
 // 		shapeEntryCan->Write();
 // 	}
 // 	TCanvas* shapeEntry2DCan = new TCanvas("shapeEntry2DCan", "The entries of shapes in this experiments", 2500, 1000);
 // 	TH2D* shapeEntry2D = new TH2D("shapeEntry2D", "The entries of shapes in this experiments", clusterSize, -clusterSize - .5, -.5, maxX, -maxX + .5, .5);
 // 	for ( const TClusterShape* clusterN : mClusterN ) {
 // 		std::vector<std::pair<TCluster*, int>> clustersWithShape = clusterN->getClustersWithShape();
 // 		int iShape = 0;
 // 		for ( const std::pair<TCluster*, int>& clusterWithShape : clustersWithShape ) {
 // 			int clusterSize = clusterN->getClusterSize();
 // 			shapeEntry2D->Fill(-clusterSize, -iShape, clusterWithShape.second);
 // 			iShape++;
 // 		}
 // 	}
 // 	for ( int i = 1; i <= shapeEntry2D->GetNbinsX(); ++i ) {
 // 		shapeEntry2D->GetXaxis()->SetBinLabel(i, Form("%g", floor(-1 * shapeEntry2D->GetXaxis()->GetBinLowEdge(i))));
 // 	}
 // 	for ( int j = 1; j <= shapeEntry2D->GetNbinsX(); ++j ) {
 // 		shapeEntry2D->GetYaxis()->SetBinLabel(j, Form("%g", floor(-1 * shapeEntry2D->GetYaxis()->GetBinLowEdge(j))));
 // 	}
 // 	shapeEntry2D->SetStats(0);
 // 	shapeEntry2D->GetXaxis()->SetTitle("cluster size");
 // 	shapeEntry2D->GetYaxis()->SetTitle("i-th cluster shape in each cluster size");
 // 	shapeEntry2D->GetZaxis()->SetTitle("Entry");
 // 	shapeEntry2D->Draw("LEGO2");
 // 	shapeEntry2DCan->SetLogz();
 // 	shapeEntry2DCan->SetGrid();
 // 	shapeEntry2DCan->SaveAs(static_cast<TString>(savePath / "shapes" / "Shape_entry_total.png"));
 // 	shapeEntry2DCan->Write();
 // }

 // void TClusterAnalyser::saveMaskingFile(std::string title) {
 // 	std::ofstream maskFile(title);
 // 	for ( const std::pair<int, int>& hotPixel : mHotPixels ) {
 // 		maskFile << hotPixel.first << " " << hotPixel.second << std::endl;
 // 	}
 // 	maskFile.close();
 // }