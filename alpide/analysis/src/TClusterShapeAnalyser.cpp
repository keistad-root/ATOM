/**
 * @file TClusterShapeAnalyser.cpp
 * @author Yongjun Choi (yochoi@cern.ch)
 * @brief Tools for analysising shape property of cluster
 * @version 0.1
 * @date 08-05-2024
 *
 * @copyright Copyright (c) 2024
 *
*/
#define __TCLUSTERSHAPEANALYSER_HEADER__
#include "TClusterShapeAnalyser.h"

/**
 * @brief Construct a new TClusterShapeAnalyser::TClusterShapeAnalyser object
 *
 * @param analyser
*/
TClusterShapeAnalyser::TClusterShapeAnalyser(const TClusterAnalyser& analyser) : TClusterAnalyser(analyser), fBits(kNotDeleted) {
	// Print out a log
	std::clog << "TClusterShapeAnalyser object is armed." << std::endl;
}

/**
 * @brief Destroy the TClusterShapeAnalyser::TClusterShapeAnalyser object
 *
*/
TClusterShapeAnalyser::~TClusterShapeAnalyser() {
	// Destroy objects of TClusterShape if it isn't destructed.
	for ( const auto& key : mClusterShapeSet ) {
		for ( auto& ele : key.second ) {
			if ( !ele->IsDestructed() ) {
				delete ele;
			}
		}
	}
	// Print out a log
	std::clog << "TClusterShapeAnalyser object is terminated." << std::endl;
}

/**
 * @brief Store clusters to objects of TClusterShape for extracting shape informations
 *
 * @param typeName
 * @param clusterSizeRange
*/
void TClusterShapeAnalyser::doShaping(std::string_view typeName, const std::vector<int>& clusterSizeRange) {
	// The number of total shape and the maximum number of shapes of each cluster sizes.
	int nTotalShape = 0;
	int maxMode = 0;
	// The array of objects of TClusterShape class.
	std::vector<TClusterShape*> clusterShapes;
	// This stores and extracts clusters shape informations.
	for ( const int clusterSize : clusterSizeRange ) {
		if ( mClustersizes.find(std::string(typeName))->second->GetBinContent(clusterSize) != 0 ) {
			// This gets cluster bunch from TClusterDivideData object which classifies clusters by their size.
			const std::vector<TCluster*> divideData = mDivideData.find(std::string(typeName))->second->getClusterOfSize(clusterSize);
			// This inserts cluster size information and clusters to TClusterShape objects.
			TClusterShape* clusterShape = new TClusterShape(clusterSize, divideData);
			// This extracts shape informations.
			clusterShape->identifyShapes();
			// This sorts shapes according to its size of long axis.
			clusterShape->sortShapes();
			// 
			clusterShapes.push_back(clusterShape);
			nTotalShape += clusterShape->getClusterShapeInfos().size();
			maxMode = std::max(maxMode, static_cast<int>(clusterShape->getClusterShapeInfos().size()));
		}
	}
	mClusterShapeSet.insert_or_assign(std::string(typeName), clusterShapes);
	mNTotalShapeSet.insert_or_assign(std::string(typeName), nTotalShape);
	mMaxModeSet.insert_or_assign(std::string(typeName), maxMode);
}

void TClusterShapeAnalyser::saveIndividualShapes(std::string_view typeName, const CppConfigDictionary config) {
	std::clog << "Generating " << "\033[1;32m" << "individual shapes" << "\033[1;0m" << "..." << std::endl;

	int nTotalShape = mNTotalShapeSet.find(std::string(typeName))->second;
	std::filesystem::path filePath(config.find("output_path"));
	filePath /= config.find("subdirectory");
	std::filesystem::create_directories(filePath);

	ProgressBar pBar(nTotalShape);
	for ( const TClusterShape* clusterShape : mClusterShapeSet.find(std::string(typeName))->second ) {
		int clusterSize = clusterShape->getClusterSize();
		int iClusterShape = 0;
		for ( const TShapeInfo& shapeInfo : clusterShape->getClusterShapeInfos() ) {
			pBar.printProgress();
			int shapeWidth = shapeInfo.mClusterMatrix->getNRow();
			int shapeHeight = shapeInfo.mClusterMatrix->getNColumn();

			TCanvas* canvas = new TCanvas(Form("%d_%d.png", clusterSize, iClusterShape), "", (shapeWidth + 2) * 500, (shapeHeight + 2) * 500);
			canvas->SetMargin(0., 0., 0., 0.);
			shapeInfo.mClusterMap->Draw();
			shapeInfo.mClusterMap->SetDrawOption("COLZ");
			shapeInfo.mClusterMap->GetXaxis()->SetAxisColor(0);
			shapeInfo.mClusterMap->GetYaxis()->SetAxisColor(0);

			TLine* line = new TLine();
			line->SetLineColorAlpha(kRed, 6. / 8);
			for ( int i = 1; i <= shapeInfo.mClusterMap->GetNbinsX(); ++i ) {
				for ( int j = 1; j <= shapeInfo.mClusterMap->GetNbinsY(); ++j ) {
					if ( shapeInfo.mClusterMap->GetBinContent(i, j) > 0 ) {
						double xlow = shapeInfo.mClusterMap->GetXaxis()->GetBinLowEdge(i);
						double xup = shapeInfo.mClusterMap->GetXaxis()->GetBinUpEdge(i);
						double ylow = shapeInfo.mClusterMap->GetYaxis()->GetBinLowEdge(j);
						double yup = shapeInfo.mClusterMap->GetYaxis()->GetBinUpEdge(j);
						line->DrawLine(xlow, ylow, xup, ylow); // Bottom
						line->DrawLine(xlow, yup, xup, yup); // Top
						line->DrawLine(xlow, ylow, xlow, yup); // Left
						line->DrawLine(xup, ylow, xup, yup); // Right
					}
				}
			}

			TText* title = new TText(.5, 1. - .5 / (shapeHeight + 2), Form("%d-th Cluster Shape of Cluster Size %d", iClusterShape, clusterSize));
			title->SetTextAlign(23);
			title->SetNDC();
			title->Draw();

			std::filesystem::path file = filePath / (config.find("filename") + "_" + std::to_string(clusterSize) + "_" + std::to_string(iClusterShape));
			if ( config.hasKey("extension") ) {
				file.replace_extension(config.find("extension"));
			} else {
				file.replace_extension("png");
			}
			canvas->SaveAs(static_cast<TString>(file));

			iClusterShape++;

			delete title;
			title = nullptr;
			delete canvas;
			canvas = nullptr;
		}
	}
}

int calNIncludePixel(const TMatrix2D<int>* matrix) {
	int centreX = 0, centreY = 0;
	int clusterSize = 0;
	for ( int pixelX = 0; pixelX < matrix->getNRow(); pixelX++ ) {
		for ( int pixelY = 0; pixelY < matrix->getNColumn(); pixelY++ ) {
			if ( matrix->getElement(pixelX, pixelY) == 1 ) {
				centreX += pixelX * 2;
				centreY += pixelY * 2;
				clusterSize++;
			}
		}
	}
	int radiusSquare = 0;
	for ( int pixelX = 0; pixelX < matrix->getNRow(); pixelX++ ) {
		for ( int pixelY = 0; pixelY < matrix->getNColumn(); pixelY++ ) {
			if ( matrix->getElement(pixelX, pixelY) == 1 ) {
				int distance = pow(abs(2 * pixelX * clusterSize - centreX) + clusterSize, 2) + pow(abs(2 * pixelY * clusterSize - centreY) + clusterSize, 2);
				radiusSquare = std::max(radiusSquare, distance);
			}
		}
	}
	int count = 0;
	for ( int x = floor((centreX - sqrt(radiusSquare)) / (2 * clusterSize)); x < ceil((centreX + sqrt(radiusSquare)) / (2 * clusterSize)) + 1; x++ ) {
		for ( int y = floor((centreY - sqrt(radiusSquare)) / (2 * clusterSize)); y < ceil((centreY + sqrt(radiusSquare)) / (2 * clusterSize)) + 1; y++ ) {
			if ( static_cast<int>(std::pow(abs(2 * x * clusterSize - centreX) + clusterSize, 2)) + static_cast<int>(std::pow(abs(2 * y * clusterSize - centreY) + clusterSize, 2)) <= radiusSquare ) {
				count++;
			}
		}
	}
	return count;
}

double calRatioOfRadius(const TMatrix2D<int>* matrix) {
	int centreX = 0, centreY = 0;
	int clusterSize = 0;
	for ( int pixelX = 0; pixelX < matrix->getNRow(); pixelX++ ) {
		for ( int pixelY = 0; pixelY < matrix->getNColumn(); pixelY++ ) {
			if ( matrix->getElement(pixelX, pixelY) == 1 ) {
				centreX += pixelX * 2;
				centreY += pixelY * 2;
				clusterSize++;
			}
		}
	}
	int longRadiusSquare = 0;
	int shortRadiusSquare = 100000000;
	for ( int pixelX = 0; pixelX < matrix->getNRow(); pixelX++ ) {
		for ( int pixelY = 0; pixelY < matrix->getNColumn(); pixelY++ ) {
			bool hasValue = matrix->getElement(pixelX, pixelY) == 1;
			bool isWorldBorder = pixelX == 0 || pixelY == 0 || pixelX == matrix->getNRow() - 1 || pixelY == matrix->getNColumn() - 1;
			bool isBorder = isWorldBorder ? true : matrix->getElement(pixelX - 1, pixelY) == 0 || matrix->getElement(pixelX, pixelY - 1) == 0 || matrix->getElement(pixelX + 1, pixelY) == 0 || matrix->getElement(pixelX, pixelY + 1) == 0;
			if ( hasValue ) {
				int distance = pow(2 * pixelX * clusterSize - centreX, 2) + pow(2 * pixelY * clusterSize - centreY, 2);
				longRadiusSquare = std::max(longRadiusSquare, distance);
			}
			if ( (hasValue && isWorldBorder) || (hasValue && !isWorldBorder && isBorder) ) {
				int distance = pow(2 * pixelX * clusterSize - centreX, 2) + pow(2 * pixelY * clusterSize - centreY, 2);
				shortRadiusSquare = std::min(shortRadiusSquare, distance);
			}
		}
	}
	if ( matrix->getNRow() == 1 || matrix->getNColumn() == 1 ) {
		shortRadiusSquare = 0;
	}
	return sqrt(static_cast<double>(shortRadiusSquare) / longRadiusSquare);
}

void TClusterShapeAnalyser::saveSameSizeInfos(std::string_view typeName, const CppConfigDictionary config) {
	// Print out a log
	std::cout << "Generating " << "\033[1;32m" << "Informations of shapes with same size" << "\033[1;0m" << "..." << std::endl;

	// Settting output file path
	std::filesystem::path filePath(config.find("output_path"));
	filePath /= config.find("subdirectory");
	// The creation of directories of output path
	std::filesystem::create_directories(filePath);

	std::unordered_map<std::string, CppConfigDictionary> plotConfigList = config.getSubConfig("plots").getSubConfigSetWithName();
	// Inits for histograms
	std::unordered_map<std::string, TH1D*> distributionSet;
	for ( const auto& plotConfig : plotConfigList ) {
		std::string plotName = plotConfig.second.find("name");
		TString plotTitle = plotConfig.second.hasKey("title") ? plotConfig.second.find("title") : "";
		TString plotXTitle = plotConfig.second.hasKey("x_title") ? plotConfig.second.find("x_title") : "";
		TString plotYTitle = plotConfig.second.hasKey("y_title") ? plotConfig.second.find("y_title") : "";
		Int_t plotNBin = plotConfig.second.hasKey("n_bin") ? stoi(plotConfig.second.find("n_bin")) : 100;
		Float_t plotXMin = plotConfig.second.hasKey("x_min") ? stof(plotConfig.second.find("x_min")) : 0.;
		Float_t plotXMax = plotConfig.second.hasKey("x_max") ? stof(plotConfig.second.find("x_max")) : 1.;

		distributionSet.insert_or_assign(plotConfig.first, new TH1D(static_cast<TString>(plotName), plotTitle + "; " + plotXTitle + "; " + plotYTitle, plotNBin, plotXMin, plotXMax));
	}

	ProgressBar pBar(mNTotalShapeSet.find(std::string(typeName))->second);
	for ( const TClusterShape* clusterShape : mClusterShapeSet.find(std::string(typeName))->second ) {
		int clusterSize = clusterShape->getClusterSize();

		TCanvas* canvas = new TCanvas(Form("shapeEntry%d", clusterSize), "shapeEntry", 2500, 1000);
		// int binNum = clusterShape->getClusterShapeInfos().size();
		TGraph* distribution = new TGraph();
		TGraph* radiusRatioGraph = new TGraph();

		int iShape = 0;
		TH1D* areaDistributionWithSize = new TH1D(Form("dist5_%d", clusterSize), Form("Area Ratio Distribution of cluster size %d", clusterSize), 101, 0, 1.01);
		TH1D* areaWithEntryDistributionWithSize = new TH1D(Form("dist6_%d", clusterSize), Form("Area Ratio With Entry Distribution of cluster size %d", clusterSize), 101, 0, 1.01);
		TH1D* radiusDistributionWithSize = new TH1D(Form("dist7_%d", clusterSize), Form("Radius Ratio Distribution of cluster size %d", clusterSize), 101, 0, 1.01);
		TH1D* radiusWithEntryDistributionWithSize = new TH1D(Form("dist8_%d", clusterSize), Form("Radius Ratio With Entry Distribution of cluster size %d", clusterSize), 101, 0, 1.01);

		for ( const TShapeInfo& shapeInfo : clusterShape->getClusterShapeInfos() ) {
			pBar.printProgress();

			for ( const auto& plotConfig : plotConfigList ) {
				std::cout << plotConfig.second.find("name") << std::endl;
				if ( plotConfig.second.find("name") == "area_ratio" ) {
					distributionSet.find(plotConfig.first)->second->Fill(static_cast<double>(clusterSize) / calNIncludePixel(shapeInfo.mClusterMatrix));
				}
				if ( plotConfig.second.find("name") == "area_ratio_with_entry" ) {
					distributionSet.find(plotConfig.first)->second->Fill(static_cast<double>(clusterSize) / calNIncludePixel(shapeInfo.mClusterMatrix), shapeInfo.mEntry);
				}
				if ( plotConfig.second.find("name") == "radius_ratio" ) {
					distributionSet.find(plotConfig.first)->second->Fill(calRatioOfRadius(shapeInfo.mClusterMatrix));
				}
				if ( plotConfig.second.find("name") == "radius_ratio_with_entry" ) {
					distributionSet.find(plotConfig.first)->second->Fill(calRatioOfRadius(shapeInfo.mClusterMatrix), shapeInfo.mEntry);
				}
			}

			distribution->SetPoint(iShape, iShape, static_cast<double>(clusterSize) / calNIncludePixel(shapeInfo.mClusterMatrix));
			radiusRatioGraph->SetPoint(iShape, iShape, calRatioOfRadius(shapeInfo.mClusterMatrix));

			areaDistributionWithSize->Fill(static_cast<double>(clusterSize) / calNIncludePixel(shapeInfo.mClusterMatrix));
			areaWithEntryDistributionWithSize->Fill(static_cast<double>(clusterSize) / calNIncludePixel(shapeInfo.mClusterMatrix), shapeInfo.mEntry);
			radiusDistributionWithSize->Fill(calRatioOfRadius(shapeInfo.mClusterMatrix));
			radiusWithEntryDistributionWithSize->Fill(calRatioOfRadius(shapeInfo.mClusterMatrix), shapeInfo.mEntry);

			iShape++;
		}
		TF1* line1 = new TF1("line1", "0.8", 0, iShape);
		TF1* line2 = new TF1("line2", "0.5", 0, iShape);
		distribution->SetTitle(Form("Informations for cluster shapes in cluster size %d", clusterSize));
		distribution->GetXaxis()->SetTitle("i-th cluster shape");
		distribution->GetXaxis()->SetTitleOffset(1.4);
		distribution->GetXaxis()->SetLabelOffset(0.003);
		distribution->GetYaxis()->SetTitle("Ratio");
		distribution->GetYaxis()->SetTitleOffset(0.7);
		distribution->SetMarkerStyle(45);
		distribution->SetMarkerSize(4);
		distribution->SetMarkerColor(kRed);
		distribution->SetMaximum(1);
		distribution->SetMinimum(0);
		distribution->Draw("AP");
		radiusRatioGraph->SetMarkerStyle(41);
		radiusRatioGraph->SetMarkerSize(4);
		radiusRatioGraph->SetMarkerColor(kBlue);
		radiusRatioGraph->Draw("P");
		mExpSettingLegend->Draw("SAME");
		line1->SetLineColor(kPink + 2);
		line1->Draw("SAME");
		line2->SetLineColor(kCyan - 7);
		line2->Draw("SAME");
		TLegend* legend = new TLegend(.78, .7, .98, .92);
		legend->SetHeader("Ratios", "c");
		legend->AddEntry(distribution, "Pixels / Full pixels", "p");
		legend->AddEntry(radiusRatioGraph, "Short radius / Long radius", "p");
		legend->Draw();
		canvas->SetMargin(.07, .28, .12, .08);
		std::filesystem::path file = filePath.parent_path() / (filePath.stem().string() + "_" + std::to_string(clusterSize) + filePath.extension().string());
		// canvas->SaveAs(static_cast<TString>(file));

		TCanvas* canvas6 = new TCanvas(Form("canvas6_%d", clusterSize), "", 2000, 1000);
		areaDistributionWithSize->SetStats(0);
		areaDistributionWithSize->Draw();
		std::filesystem::path file6 = filePath.parent_path() / (filePath.stem().string() + "_area_" + std::to_string(clusterSize) + filePath.extension().string());
		// canvas6->SaveAs(static_cast<TString>(file6));

		TCanvas* canvas7 = new TCanvas(Form("canvas7_%d", clusterSize), "", 2000, 1000);
		areaWithEntryDistributionWithSize->SetStats(0);
		areaWithEntryDistributionWithSize->Draw("HIST");
		std::filesystem::path file7 = filePath.parent_path() / (filePath.stem().string() + "_area_with_entry_" + std::to_string(clusterSize) + filePath.extension().string());
		canvas7->SetLogy();
		// canvas7->SaveAs(static_cast<TString>(file7));

		TCanvas* canvas8 = new TCanvas(Form("canvas8_%d", clusterSize), "", 2000, 1000);
		radiusDistributionWithSize->SetStats(0);
		radiusDistributionWithSize->Draw();
		std::filesystem::path file8 = filePath.parent_path() / (filePath.stem().string() + "_radius_" + std::to_string(clusterSize) + filePath.extension().string());
		// canvas8->SaveAs(static_cast<TString>(file8));


		TCanvas* canvas9 = new TCanvas(Form("canvas9_%d", clusterSize), "", 2000, 1000);
		radiusWithEntryDistributionWithSize->SetStats(0);
		radiusWithEntryDistributionWithSize->Draw("HIST");
		std::filesystem::path file9 = filePath.parent_path() / (filePath.stem().string() + "_radius_with_entry_" + std::to_string(clusterSize) + filePath.extension().string());
		canvas9->SetLogy();
		// canvas9->SaveAs(static_cast<TString>(file9));

		delete distribution;
		delete canvas;
	}

	for ( const auto& plotConfig : plotConfigList ) {
		TString canvasName = "can" + plotConfig.first;
		Int_t canvasWidth = plotConfig.second.hasKey("canvas_width") ? stoi(plotConfig.second.find("canvas_width")) : 500;
		Int_t canvasHeight = plotConfig.second.hasKey("canvas_height") ? stoi(plotConfig.second.find("canvas_height")) : 500;
		TCanvas* canvas = new TCanvas(canvasName, "", canvasWidth, canvasHeight);

		distributionSet.find(plotConfig.first)->second->SetStats(0);

		Int_t y_min = plotConfig.second.hasKey("y_min") ? stoi(plotConfig.second.find("y_min")) : 0;
		Int_t y_max = plotConfig.second.hasKey("y_max") ? stoi(plotConfig.second.find("y_max")) : 1000;
		distributionSet.find(plotConfig.first)->second->SetMinimum(y_min);
		distributionSet.find(plotConfig.first)->second->SetMaximum(y_max);
		distributionSet.find(plotConfig.first)->second->Draw();

		std::filesystem::path file = filePath;
		file /= plotConfig.second.find("filename");
		file.replace_extension(config.find("extension"));
		canvas->SaveAs(static_cast<TString>(file));
	}

	// TCanvas* canvas2 = new TCanvas("canvas2", "", 2000, 1000);
	// areaDistribution->SetStats(0);
	// areaDistribution->SetMaximum(100);
	// areaDistribution->Draw();
	// std::filesystem::path file2 = filePath.parent_path() / (filePath.stem().string() + "_area" + filePath.extension().string());
	// canvas2->SaveAs(static_cast<TString>(file2));

	// TCanvas* canvas3 = new TCanvas("canvas3", "", 2000, 1000);
	// areaWithEntryDistribution->SetStats(0);
	// areaWithEntryDistribution->SetMaximum(300000);
	// areaWithEntryDistribution->Draw("HIST");
	// std::filesystem::path file3 = filePath.parent_path() / (filePath.stem().string() + "_area_with_entry" + filePath.extension().string());
	// canvas3->SetLogy();
	// canvas3->SaveAs(static_cast<TString>(file3));

	// TCanvas* canvas4 = new TCanvas("canvas4", "", 2000, 1000);
	// radiusDistribution->SetStats(0);
	// radiusDistribution->SetMaximum(50);
	// radiusDistribution->Draw();
	// std::filesystem::path file4 = filePath.parent_path() / (filePath.stem().string() + "_radius" + filePath.extension().string());
	// canvas4->SaveAs(static_cast<TString>(file4));

	// TCanvas* canvas5 = new TCanvas("canvas5", "", 2000, 1000);
	// radiusWithEntryDistribution->SetStats(0);
	// radiusWithEntryDistribution->SetMaximum(50000);
	// radiusWithEntryDistribution->Draw("HIST");
	// std::filesystem::path file5 = filePath.parent_path() / (filePath.stem().string() + "_radius_with_entry" + filePath.extension().string());
	// canvas5->SetLogy();
	// canvas5->SaveAs(static_cast<TString>(file5));

}

void TClusterShapeAnalyser::saveSameSizeShapes(std::string_view typeName, const CppConfigDictionary config) {
	std::cout << "Generating " << "\033[1;32m" << "Shapes with same sized" << "\033[1;0m" << "..." << std::endl;

	std::filesystem::path filePath(config.find("file_path"));
	std::filesystem::create_directories(filePath.parent_path());

	int nTotalShape = mNTotalShapeSet.find(std::string(typeName))->second;

	const int nominalWidth = 200;
	const int nominalHeader = 100;
	const int nPlotInRow = 10;
	int plotsWidth = nPlotInRow * nominalWidth;

	ProgressBar pBar(nTotalShape);
	for ( const TClusterShape* clusterShape : mClusterShapeSet.find(std::string(typeName))->second ) {
		int clusterSize = clusterShape->getClusterSize();
		int nClusterShape = clusterShape->getClusterShapeInfos().size();

		int plotsHeight = (floor((nClusterShape - 1.) / 10.) + 1.) * nominalWidth;

		TCanvas* canvas = new TCanvas(Form("shapes%d", clusterSize), "", plotsWidth, plotsHeight + nominalHeader);

		int iClusterShape = 0;
		for ( const TShapeInfo& shapeInfo : clusterShape->getClusterShapeInfos() ) {
			pBar.printProgress();

			int width = shapeInfo.mClusterMatrix->getNRow() + 2;
			int height = shapeInfo.mClusterMatrix->getNColumn() + 2;

			double padCenterX = (1. / nPlotInRow) * (.5 + (iClusterShape % 10));
			double padCenterY = (static_cast<double>(plotsHeight) / static_cast<double>(plotsHeight + nominalHeader)) / ((floor((nClusterShape - 1) / static_cast<double>(nPlotInRow))) + 1.) * .5 * (2. * (floor((nClusterShape - 1) / static_cast<double>(nPlotInRow)) - floor(iClusterShape / static_cast<double>(nPlotInRow))) + 1.);

			double padWidth = (1. / (2. * nPlotInRow));
			double padHeight = (static_cast<double>(plotsHeight) / (plotsHeight + nominalHeader)) / (2. * ((floor(static_cast<double>(nClusterShape) / nPlotInRow) + 1.)));

			TPad* pad = new TPad(Form("pad%d", iClusterShape), "", padCenterX - padWidth, padCenterY - padHeight, padCenterX + padWidth, padCenterY + padHeight);
			pad->SetMargin(0., 0., .5 * (1. - static_cast<double>(height) / width), .5 * (1. - static_cast<double>(height) / width));
			pad->Draw();
			pad->cd();
			shapeInfo.mClusterMap->SetDrawOption("COL");
			shapeInfo.mClusterMap->GetXaxis()->SetAxisColor(0);
			shapeInfo.mClusterMap->GetYaxis()->SetAxisColor(0);
			shapeInfo.mClusterMap->Draw("col");
			pad->SetFrameLineWidth(0);

			TLine* line = new TLine();
			line->SetLineColorAlpha(kRed, 6. / 8);
			for ( int i = 1; i <= shapeInfo.mClusterMap->GetNbinsX(); ++i ) {
				for ( int j = 1; j <= shapeInfo.mClusterMap->GetNbinsY(); ++j ) {
					if ( shapeInfo.mClusterMap->GetBinContent(i, j) > 0 ) {
						double xlow = shapeInfo.mClusterMap->GetXaxis()->GetBinLowEdge(i);
						double xup = shapeInfo.mClusterMap->GetXaxis()->GetBinUpEdge(i);
						double ylow = shapeInfo.mClusterMap->GetYaxis()->GetBinLowEdge(j);
						double yup = shapeInfo.mClusterMap->GetYaxis()->GetBinUpEdge(j);
						line->DrawLine(xlow, ylow, xup, ylow); // Bottom
						line->DrawLine(xlow, yup, xup, yup); // Top
						line->DrawLine(xlow, ylow, xlow, yup); // Left
						line->DrawLine(xup, ylow, xup, yup); // Right
					}
				}
			}

			TText* numberingText = new TText(padCenterX, padCenterY, Form("%d", iClusterShape));
			numberingText->SetNDC();
			numberingText->SetTextAlign(22);
			numberingText->SetTextSize(.3 * nominalWidth / (plotsHeight + nominalHeader));
			numberingText->SetTextColorAlpha(kBlack, 5. / 8.);

			canvas->cd();
			numberingText->Draw();
			iClusterShape++;
		}
		TText* titleText = new TText(.5, 1. - .5 * nominalHeader / (nominalHeader + plotsHeight), Form("Cluster shapes with cluster size %d", clusterSize));
		titleText->SetTextSize(.5 * nominalHeader / (plotsHeight + nominalHeader));
		titleText->SetTextAlign(22);
		titleText->SetNDC();
		titleText->Draw();
		std::filesystem::path file = filePath.parent_path() / (filePath.stem().string() + "_" + std::to_string(clusterSize) + filePath.extension().string());
		canvas->SaveAs(static_cast<TString>(file));
	}
}

void TClusterShapeAnalyser::saveTotalShapes(std::string_view typeName, const CppConfigDictionary config) {
	std::cout << "Generating \033[1;32mTotal shapes\033[1;0m..." << std::endl;

	std::filesystem::path filePath(config.find("file_path"));
	std::filesystem::create_directories(filePath.parent_path());

	int nWidth = mMaxModeSet.find(std::string(typeName))->second;
	int nHeight = mClusterShapeSet.find(std::string(typeName))->second.size();

	int nominalWidth = 100;
	int nominalHeader = 100;

	int plotsWidth = nWidth * nominalWidth;
	int plotsHeight = nHeight * nominalWidth;
	TCanvas* canvas = new TCanvas("tShape", "cluster shape", plotsWidth + nominalHeader, plotsHeight + nominalHeader);
	int iClusterSize = 0;
	ProgressBar* pBar = new ProgressBar(mNTotalShapeSet.find(std::string(typeName))->second);
	for ( const TClusterShape* clusterShape : mClusterShapeSet.find(std::string(typeName))->second ) {
		int clusterSize = clusterShape->getClusterSize();
		int iClusterShape = 0;
		for ( const TShapeInfo& shapeInfo : clusterShape->getClusterShapeInfos() ) {
			pBar->printProgress();
			int width = shapeInfo.mClusterMap->GetNbinsX();
			int height = shapeInfo.mClusterMap->GetNbinsY();

			TPad* pad = new TPad(Form("pad%d_%d", iClusterSize, iClusterShape), "pad", (double) nominalHeader / (plotsWidth + nominalHeader) + ((double) plotsWidth / (plotsWidth + nominalHeader)) * ((double) iClusterShape / nWidth), ((double) plotsHeight / (plotsHeight + nominalHeader)) * (((double) nHeight - iClusterSize - 1) / nHeight), (double) nominalHeader / (plotsWidth + nominalHeader) + ((double) plotsWidth / (plotsWidth + nominalHeader)) * (double) (iClusterShape + 1) / nWidth, ((double) plotsHeight / (plotsHeight + nominalHeader)) * (((double) nHeight - iClusterSize) / nHeight), -1, 1);
			pad->Draw();
			pad->cd();
			pad->SetMargin(0., 0., .5 * (1. - (double) height / width), .5 * (1. - (double) height / width));
			shapeInfo.mClusterMap->Draw();
			shapeInfo.mClusterMap->SetDrawOption("COL");
			shapeInfo.mClusterMap->GetXaxis()->SetAxisColor(0);
			shapeInfo.mClusterMap->GetYaxis()->SetAxisColor(0);
			TLine* line = new TLine();
			line->SetLineColorAlpha(kRed, 6. / 8);
			for ( int i = 1; i <= shapeInfo.mClusterMap->GetNbinsX(); ++i ) {
				for ( int j = 1; j <= shapeInfo.mClusterMap->GetNbinsY(); ++j ) {
					if ( shapeInfo.mClusterMap->GetBinContent(i, j) > 0 ) {
						double xlow = shapeInfo.mClusterMap->GetXaxis()->GetBinLowEdge(i);
						double xup = shapeInfo.mClusterMap->GetXaxis()->GetBinUpEdge(i);
						double ylow = shapeInfo.mClusterMap->GetYaxis()->GetBinLowEdge(j);
						double yup = shapeInfo.mClusterMap->GetYaxis()->GetBinUpEdge(j);
						line->DrawLine(xlow, ylow, xup, ylow); // Bottom
						line->DrawLine(xlow, yup, xup, yup); // Top
						line->DrawLine(xlow, ylow, xlow, yup); // Left
						line->DrawLine(xup, ylow, xup, yup); // Right
					}
				}
			}
			pad->SetFrameLineWidth(0);
			canvas->cd();
			TText* numberingText = new TText((double) nominalHeader / (plotsWidth + nominalHeader) + ((double) plotsWidth / (plotsWidth + nominalHeader)) * (((double) iClusterShape + .5) / nWidth), ((double) plotsHeight / (plotsHeight + nominalHeader)) * (((double) nHeight - iClusterSize) / nHeight), Form("%d", shapeInfo.mEntry));
			numberingText->SetNDC();
			numberingText->SetTextAlign(22);
			numberingText->SetTextSize(.4 * nominalWidth / (plotsHeight + nominalHeader));
			numberingText->SetTextColor(kBlack);
			numberingText->Draw();
			iClusterShape++;
		}
		TText* sizeText = new TText((double) nominalHeader * .5 / (nominalHeader + plotsWidth), ((double) plotsHeight / (plotsHeight + nominalHeader)) * (((double) nHeight - iClusterSize - .5) / nHeight), Form("%d", clusterSize));
		sizeText->SetNDC();
		sizeText->SetTextAlign(22);
		sizeText->SetTextSize(.6 * nominalWidth / (plotsHeight + nominalHeader));
		sizeText->SetTextColor(kBlack);
		sizeText->Draw();
		iClusterSize++;
	}
	delete pBar;
	TText* titleText = new TText(.5, 1. - .5 * nominalHeader / (nominalHeader + plotsHeight), static_cast<TString>("Total Cluster Shapes"));
	titleText->SetTextSize(.8 * nominalHeader / (plotsHeight + nominalHeader));
	titleText->SetTextAlign(22);
	titleText->SetNDC();
	titleText->Draw();

	canvas->SaveAs(static_cast<TString>(filePath));

	delete titleText;
	delete canvas;
}

void TClusterShapeAnalyser::saveSameSizeShapeEntry(std::string_view typeName, const CppConfigDictionary config) {
	std::cout << "Generating \033[1;32mEntry of shapes with same size\033[1;0m..." << std::endl;

	std::filesystem::path filePath(config.find("file_path"));
	std::filesystem::create_directories(filePath.parent_path());

	ProgressBar pBar(mNTotalShapeSet.find(std::string(typeName))->second);
	for ( const TClusterShape* clusterShape : mClusterShapeSet.find(std::string(typeName))->second ) {
		int clusterSize = clusterShape->getClusterSize();
		TCanvas* canvas = new TCanvas(Form("shapeEntry%d", clusterSize), "shapeEntry", 2500, 1000);
		int binNum = clusterShape->getClusterShapeInfos().size();
		TH1D* distribution = new TH1D(Form("shapeEntry%d", clusterSize), Form(static_cast<TString>("Shape Entry with cluster size %d"), clusterSize), binNum, -.5, binNum - .5);
		int iShape = 0;
		for ( const TShapeInfo& shapeInfo : clusterShape->getClusterShapeInfos() ) {
			pBar.printProgress();
			distribution->Fill(iShape, shapeInfo.mEntry);
			iShape++;
		}
		distribution->GetXaxis()->SetNdivisions(10, 10, 0, true);
		distribution->GetXaxis()->SetTitle("i-th cluster size");
		distribution->GetXaxis()->SetTitleOffset(1.4);
		distribution->GetXaxis()->SetLabelOffset(0.003);
		distribution->GetYaxis()->SetTitle("Entry");
		distribution->GetYaxis()->SetTitleOffset(0.7);
		distribution->Draw("HIST");
		mExpSettingLegend->Draw("SAME");
		canvas->SetMargin(.07, .28, .12, .08);
		std::filesystem::path file = filePath.parent_path() / (filePath.stem().string() + "_" + std::to_string(clusterSize) + filePath.extension().string());
		canvas->SaveAs(static_cast<TString>(file));
		delete distribution;
		delete canvas;
	}
}

void TClusterShapeAnalyser::saveTotalShapeEntry(std::string_view typeName, const CppConfigDictionary config) {
	std::cout << "Generating \033[1;32mEntry of total shapes\033[1;0m..." << std::endl;
	std::filesystem::path filePath(config.find("file_path"));
	std::filesystem::create_directories(filePath.parent_path());
	int nXbin = 0;
	int nYbin = 0;
	for ( const TClusterShape* clusterShape : mClusterShapeSet.find(std::string(typeName))->second ) {
		nYbin = std::max(nYbin, static_cast<int>(clusterShape->getClusterShapeInfos().size()));
		nXbin = std::max(nXbin, clusterShape->getClusterSize());
	}
	TCanvas* canvas = new TCanvas("shapeEntryTotal", "shape entry", 2500, 1000);
	TH2D* distribution = new TH2D("shapeEntryTotal", static_cast<TString>(config.find("plot_titles")), nXbin, -nXbin - .5, -.5, nYbin, -nYbin + .5, .5);
	ProgressBar pBar(mNTotalShapeSet.find(std::string(typeName))->second);
	for ( const TClusterShape* clusterShape : mClusterShapeSet.find(std::string(typeName))->second ) {
		int clusterSize = clusterShape->getClusterSize();
		int iShape = 0;
		for ( const TShapeInfo shapeInfo : clusterShape->getClusterShapeInfos() ) {
			pBar.printProgress();
			distribution->Fill(-clusterSize, -iShape, shapeInfo.mEntry);
			iShape++;
		}
	}
	for ( int iXbin = 1; iXbin < distribution->GetNbinsX(); ++iXbin ) {
		distribution->GetXaxis()->SetBinLabel(iXbin, Form("%g", floor(-1 * distribution->GetXaxis()->GetBinLowEdge(iXbin))));
	}

	for ( int iYbin = 1; iYbin < distribution->GetNbinsY(); ++(++iYbin) ) {
		distribution->GetYaxis()->SetBinLabel(iYbin, Form("%g", floor(-1 * distribution->GetYaxis()->GetBinLowEdge(iYbin))));
	}
	distribution->SetStats(0);
	if ( config.hasKey("options") ) {
		for ( const std::string& optionName : config.getValueList() ) {
			distribution->Draw(static_cast<TString>(optionName));
			canvas->SetPhi(10);
			canvas->SetTheta(25);
			canvas->SetLogz();
			std::filesystem::path file = filePath.parent_path() / (filePath.stem().string() + "_" + optionName + filePath.extension().string());
			canvas->SaveAs(static_cast<TString>(file));
		}
	} else {
		distribution->Draw();
		canvas->SaveAs(static_cast<TString>(filePath));
	}

	delete distribution;
	delete canvas;
}