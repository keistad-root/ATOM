#define __TCLUSTERSHAPEANALYSER_HEADER__

#include "TClusterShapeAnalyser.h"

TClusterShapeAnalyser::TClusterShapeAnalyser(const TClusterAnalyser& analyser) : TClusterAnalyser(analyser), fBits(kNotDeleted) {
	std::clog << "TClusterShapeAnalyser object is armed." << std::endl;
}

TClusterShapeAnalyser::~TClusterShapeAnalyser() {
	for ( const auto& key : mClusterShapeSet ) {
		for ( auto& ele : key.second ) {
			if ( !ele->IsDestructed() ) {
				delete ele;
			}
		}
	}
	std::clog << "TClusterShapeAnalyser object is terminated." << std::endl;
}

void TClusterShapeAnalyser::doShaping(std::string_view typeName, const std::vector<int>& clusterSizeRange) {
	int nTotalShape = 0;
	int maxMode = 0;
	std::vector<TClusterShape*> clusterShapes;
	for ( const int clusterSize : clusterSizeRange ) {
		if ( mClustersizes.find(std::string(typeName))->second->GetBinContent(clusterSize) != 0 ) {
			const std::vector<TCluster*> divideData = mDivideData.find(std::string(typeName))->second->getClusterOfSize(clusterSize);
			TClusterShape* clusterShape = new TClusterShape(clusterSize, divideData);
			clusterShape->identifyShapes();
			clusterShape->sortShapes();
			clusterShapes.push_back(clusterShape);
			nTotalShape += clusterShape->getClusterShapeInfos().size();
			maxMode = std::max(maxMode, static_cast<int>(clusterShape->getClusterShapeInfos().size()));
		}
	}
	mClusterShapeSet.insert_or_assign(std::string(typeName), clusterShapes);
	mNTotalShapeSet.insert_or_assign(std::string(typeName), nTotalShape);
	mMaxModeSet.insert_or_assign(std::string(typeName), maxMode);
}

void TClusterShapeAnalyser::saveIndividualShapes(std::string_view typeName, const Configurable* config) {
	std::clog << "Generating " << "\033[1;32m" << "individual shapes" << "\033[1;0m" << "..." << std::endl;

	int nTotalShape = mNTotalShapeSet.find(std::string(typeName))->second;
	std::filesystem::path filePath(config->find("file_path"));
	std::filesystem::create_directories(filePath.parent_path());
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

			std::filesystem::path file = filePath.parent_path() / (filePath.stem().string() + "_" + std::to_string(clusterSize) + "_" + std::to_string(iClusterShape) + filePath.extension().string());
			canvas->SaveAs(static_cast<TString>(file));

			iClusterShape++;

			delete title;
			title = nullptr;
			delete canvas;
			canvas = nullptr;
		}
	}
}

void TClusterShapeAnalyser::saveSameSizeShapes(std::string_view typeName, const Configurable* config) {
	std::cout << "Generating " << "\033[1;32m" << "Shapes with same sized" << "\033[1;0m" << "..." << std::endl;

	std::filesystem::path filePath(config->find("file_path"));
	std::filesystem::create_directories(filePath.parent_path());

	int nTotalShape = mNTotalShapeSet.find(std::string(typeName))->second;

	const int nominalWidth = 100;
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

			TText* numberingText = new TText(padCenterX, padCenterY, Form("%d", shapeInfo.mEntry));
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

void TClusterShapeAnalyser::saveTotalShapes(std::string_view typeName, const Configurable* config) {
	std::cout << "Generating \033[1;32mTotal shapes\033[1;0m..." << std::endl;

	std::filesystem::path filePath(config->find("file_path"));
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

void TClusterShapeAnalyser::saveSameSizeShapeEntry(std::string_view typeName, const Configurable* config) {
	std::cout << "Generating \033[1;32mEntry of shapes with same size\033[1;0m..." << std::endl;

	std::filesystem::path filePath(config->find("file_path"));
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

void TClusterShapeAnalyser::saveTotalShapeEntry(std::string_view typeName, const Configurable* config) {
	std::cout << "Generating \033[1;32mEntry of total shapes\033[1;0m..." << std::endl;
	std::filesystem::path filePath(config->find("file_path"));
	std::filesystem::create_directories(filePath.parent_path());
	int nXbin = 0;
	int nYbin = 0;
	for ( const TClusterShape* clusterShape : mClusterShapeSet.find(std::string(typeName))->second ) {
		nYbin = std::max(nYbin, static_cast<int>(clusterShape->getClusterShapeInfos().size()));
		nXbin = std::max(nXbin, clusterShape->getClusterSize());
	}
	TCanvas* canvas = new TCanvas("shapeEntryTotal", "shape entry", 2500, 1000);
	TH2D* distribution = new TH2D("shapeEntryTotal", static_cast<TString>(config->find("plot_titles")), nXbin, -nXbin - .5, -.5, nYbin, -nYbin + .5, .5);
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
	if ( config->hasKey("options") ) {
		for ( const std::string& optionName : config->findlist("options") ) {
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