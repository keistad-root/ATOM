#define __TCLUSTERSHAPEANALYSER_HEADER__

#include "TClusterShapeAnalyser.h"

TClusterShapeAnalyser::TClusterShapeAnalyser(const TClusterAnalyser& analyser) : TClusterAnalyser(analyser) {
	std::cout << "TClusterShapeAnalyser object is armed." << std::endl;
}

TClusterShapeAnalyser::~TClusterShapeAnalyser() {
	for ( TClusterShape* clusterShape : mClusterShapes ) {
		delete clusterShape;
		clusterShape = nullptr;
	}
	std::cout << "TClusterShapeAnalyser object is terminated." << std::endl;
}

void TClusterShapeAnalyser::doShaping(int lower, int upper) {
	mNClusterShapes = 0;
	mNMaskedClusterShapes = 0;
	mNNoisePixelClusterShapes = 0;
	mClusterMode = 0;
	mMaskedClusterMode = 0;
	mNoisePixelClusterMode = 0;

	for ( int clusterSize = lower; clusterSize < upper; clusterSize++ ) {
		if ( static_cast<int>(mClustersize->GetBinContent(clusterSize)) != 0 ) {
			mClusterShapes.push_back(new TClusterShape(mExpData->getClusters(), clusterSize));
			mClusterShapes.back()->identifyShapes();
			mClusterShapes.back()->sortShapes();
			mNClusterShapes += mClusterShapes.back()->getClusterShapeInfos().size();
			mClusterMode = std::max(mMode, static_cast<int>(mClusterShapes.back()->getClusterShapeInfos().size()));
		}
		if ( static_cast<int>(mMaskedClustersize->GetBinContent(clusterSize)) != 0 ) {
			mMaskedClusterShapes.push_back(new TClusterShape(mExpData->getMaskedClusters(), clusterSize));
			mMaskedClusterShapes.back()->identifyShapes();
			mMaskedClusterShapes.back()->sortShapes();
			mNMaskedClusterShapes += mMaskedClusterShapes.back()->getClusterShapeInfos().size();
			mMaskedClusterMode = std::max(mMode, static_cast<int>(mMaskedClusterShapes.back()->getClusterShapeInfos().size()));
		}
		if ( static_cast<int>(mNoisePixelClustersize->GetBinContent(clusterSize)) != 0 ) {
			mNoisePixelClusterShapes.push_back(new TClusterShape(mExpData->getNoiseClusters(), clusterSize));
			mNoisePixelClusterShapes.back()->identifyShapes();
			mNoisePixelClusterShapes.back()->sortShapes();
			mNNoisePixelClusterShapes += mNoisePixelClusterShapes.back()->getClusterShapeInfos().size();
			mNoisePixelClusterMode = std::max(mMode, static_cast<int>(mNoisePixelClusterShapes.back()->getClusterShapeInfos().size()));
		}
	}
}

void TClusterShapeAnalyser::doShaping(const std::vector<int>& clusterSizeSet) {
	mNClusterShapes = 0;
	mNMaskedClusterShapes = 0;
	mNNoisePixelClusterShapes = 0;
	mClusterMode = 0;
	mMaskedClusterMode = 0;
	mNoisePixelClusterMode = 0;

	for ( const int clusterSize : clusterSizeSet ) {
		if ( static_cast<int>(mClustersize->GetBinContent(clusterSize)) != 0 ) {
			mClusterShapes.push_back(new TClusterShape(mExpData->getClusters(), clusterSize));
			mClusterShapes.back()->identifyShapes();
			mClusterShapes.back()->sortShapes();
			mNClusterShapes += mClusterShapes.back()->getClusterShapeInfos().size();
			mClusterMode = std::max(mClusterMode, static_cast<int>(mClusterShapes.back()->getClusterShapeInfos().size()));
		}
		if ( static_cast<int>(mMaskedClustersize->GetBinContent(clusterSize)) != 0 ) {
			mMaskedClusterShapes.push_back(new TClusterShape(mExpData->getMaskedClusters(), clusterSize));
			mMaskedClusterShapes.back()->identifyShapes();
			mMaskedClusterShapes.back()->sortShapes();
			mNMaskedClusterShapes += mMaskedClusterShapes.back()->getClusterShapeInfos().size();
			mMaskedClusterMode = std::max(mMaskedClusterMode, static_cast<int>(mMaskedClusterShapes.back()->getClusterShapeInfos().size()));
		}
		if ( static_cast<int>(mNoisePixelClustersize->GetBinContent(clusterSize)) != 0 ) {
			mNoisePixelClusterShapes.push_back(new TClusterShape(mExpData->getNoiseClusters(), clusterSize));
			mNoisePixelClusterShapes.back()->identifyShapes();
			mNoisePixelClusterShapes.back()->sortShapes();
			mNNoisePixelClusterShapes += mNoisePixelClusterShapes.back()->getClusterShapeInfos().size();
			mNoisePixelClusterMode = std::max(mNoisePixelClusterMode, static_cast<int>(mNoisePixelClusterShapes.back()->getClusterShapeInfos().size()));
		}
	}
}

void TClusterShapeAnalyser::saveIndividualShapes() {
	std::cout << "Generating \033[1;32mindividual shapes\033[1;0m..." << std::endl;
	std::filesystem::create_directories(mSavePath / (mDataSetName + "shapes") / "individual");
	ProgressBar* pBar = new ProgressBar(mNTotalShapes);
	for ( const TClusterShape* clusterShape : mClusterShapes ) {
		int clusterSize = clusterShape->getClusterSize();
		int iClusterShape = 0;
		for ( const TShapeInfo& shapeInfo : clusterShape->getClusterShapeInfos() ) {
			pBar->printProgress();
			int width = shapeInfo.mClusterMatrix->getNRow();
			int height = shapeInfo.mClusterMatrix->getNColumn();
			TCanvas* canvas = new TCanvas(Form("%d_%d.png", clusterSize, iClusterShape), Form("cluster shape"), width * 500, height * 500);
			canvas->SetMargin(0., 0., 0., 0.);
			TText* titleText = new TText(.5, 1. - .5 / (shapeInfo.mClusterMatrix->getNColumn() + 2), Form("%d-th Cluster Shape of Cluster Size %d", iClusterShape, clusterSize));
			titleText->SetTextAlign(23);
			titleText->SetNDC();
			shapeInfo.mClusterMap->Draw("colz");
			titleText->Draw();
			canvas->SaveAs(static_cast<TString>(mSavePath / (mDataSetName + "shapes") / "individual" / static_cast<std::string>(Form("%d_%d.png", clusterSize, iClusterShape))));
			iClusterShape++;
			delete titleText;
			delete canvas;
		}
	}
	delete pBar;
}

void TClusterShapeAnalyser::saveSameSizeShapes() {
	std::cout << "Generating \033[1;32mShapes with same sized\033[1;0m..." << std::endl;
	std::filesystem::create_directories(mSavePath / (mDataSetName + "shapes") / "same_size");
	ProgressBar* pBar = new ProgressBar(mNTotalShapes);
	for ( const TClusterShape* clusterShape : mClusterShapes ) {
		int clusterSize = clusterShape->getClusterSize();
		int iClusterShape = 0;
		int nClusterShape = clusterShape->getClusterShapeInfos().size();
		int nominalWidth = 200;
		int nominalHeader = 200;
		int plotsWidth = 10 * nominalWidth;
		int plotsHeight = (floor((nClusterShape - 1) / 10) + 1) * nominalWidth + 25;
		TCanvas* canvas = new TCanvas(Form("shapes%d", clusterSize), "cluster shapes", plotsWidth, plotsHeight + nominalHeader);
		for ( const TShapeInfo& shapeInfo : clusterShape->getClusterShapeInfos() ) {
			pBar->printProgress();
			int width = shapeInfo.mClusterMatrix->getNRow();
			int height = shapeInfo.mClusterMatrix->getNColumn();
			int factor = std::max(width, height);
			double padCenterX = (1 / 10.) * (.5 + (iClusterShape % 10));
			double padCenterY = (((double) plotsHeight / (plotsHeight + nominalHeader)) / (floor((nClusterShape - 1) / 10.) + 1.)) * ((floor((nClusterShape - 1) / 10.) + 1.) + .5 - (floor(iClusterShape / 10.) + 1.));
			double padWidth = (1 / 20.) * width / factor;
			double padHeight = (((double) plotsHeight / (plotsHeight + nominalHeader)) / ((floor((nClusterShape - 1) / 10) + 1.) * 2.)) * height / factor;
			TPad* pad = new TPad(Form("pad%d", iClusterShape), "pad", padCenterX - padWidth, padCenterY - padHeight, padCenterX + padWidth, padCenterY + padHeight);
			pad->Draw();
			pad->cd();
			pad->SetMargin(0., 0., .5 * (1. - (double) height / width), .5 * (1. - (double) height / width));
			shapeInfo.mClusterMap->Draw("colz");
			pad->SetGrid();
			TText* numberingText = new TText(padCenterX, padCenterY, Form("%d", iClusterShape));
			numberingText->SetNDC();
			numberingText->SetTextAlign(22);
			numberingText->SetTextSize(.5 * nominalWidth / (plotsHeight + nominalHeader));
			numberingText->SetTextColorAlpha(kBlack, 5. / 8);
			canvas->cd();
			numberingText->Draw();
			iClusterShape++;
		}
		TText* titleText = new TText(.5, 1. - .5 * nominalHeader / (nominalHeader + plotsHeight), Form(static_cast<TString>("Cluster shapes with cluster size %d_" + mPostfix), clusterSize));
		titleText->SetTextSize(.5 * nominalHeader / (plotsHeight + nominalHeader));
		titleText->SetTextAlign(22);
		titleText->SetNDC();
		titleText->Draw();
		canvas->SaveAs(static_cast<TString>(mSavePath / (mDataSetName + "shapes") / "same_size" / Form("shapes_size_%d.png", clusterSize)));
		delete titleText;
		delete canvas;
	}
	delete pBar;
}

void TClusterShapeAnalyser::saveTotalShapes() {
	std::cout << "Generating \033[1;32mTotal shapes\033[1;0m..." << std::endl;
	int nWidth = mMode;
	int nHeight = mClusterShapes.size();
	int nominalWidth = 100;
	int nominalHeader = 100;
	int plotsWidth = nWidth * nominalWidth;
	int plotsHeight = nHeight * nominalWidth;
	TCanvas* canvas = new TCanvas("tShape", "cluster shape", plotsWidth + nominalHeader, plotsHeight + nominalHeader);
	int iClusterSize = 0;
	ProgressBar* pBar = new ProgressBar(mNTotalShapes);
	for ( const TClusterShape* clusterShape : mClusterShapes ) {
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
	titleText->SetTextAlign(21);
	titleText->SetNDC();
	titleText->Draw();
	TText* titleText2 = new TText(.5, 1. - .6 * nominalHeader / (nominalHeader + plotsHeight), static_cast<TString>(mPostfix));
	titleText2->SetTextSize(.5 * nominalHeader / (plotsHeight + nominalHeader));
	titleText2->SetTextAlign(23);
	titleText2->SetNDC();
	titleText2->Draw();
	canvas->SaveAs(static_cast<TString>(mSavePath / (mDataSetName + "shapes") / "total_shape.png"));
	delete titleText;
	delete titleText2;
	delete canvas;
}

void TClusterShapeAnalyser::saveSameSizeShapeEntry() {
	std::cout << "Generating \033[1;32mEntry of shapes with same size\033[1;0m..." << std::endl;
	std::filesystem::create_directories(mSavePath / (mDataSetName + "shapes") / "same_size");

	ProgressBar* pBar = new ProgressBar(mNTotalShapes);
	for ( const TClusterShape* clusterShape : mClusterShapes ) {
		int clusterSize = clusterShape->getClusterSize();
		TCanvas* canvas = new TCanvas(Form("shapeEntry%d", clusterSize), "shapeEntry", 2500, 1000);
		int binNum = clusterShape->getClusterShapeInfos().size();
		TH1D* distribution = new TH1D(Form("shapeEntry%d", clusterSize), Form(static_cast<TString>("Shape Entry with cluster size %d_" + mPostfix), clusterSize), binNum, -.5, binNum - .5);
		int iShape = 0;
		for ( const TShapeInfo& shapeInfo : clusterShape->getClusterShapeInfos() ) {
			pBar->printProgress();
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
		canvas->SaveAs(static_cast<TString>(mSavePath / (mDataSetName + "shapes") / "same_size" / Form("shapes_entry_size_%d.png", clusterSize)));
		delete distribution;
		delete canvas;
	}
	delete pBar;
}

void TClusterShapeAnalyser::saveTotalShapeEntry() {
	std::cout << "Generating \033[1;32mEntry of total shapes\033[1;0m..." << std::endl;
	int nXbin = 0;
	int nYbin = 0;
	for ( const TClusterShape* clusterShape : mClusterShapes ) {
		nYbin = std::max(nYbin, static_cast<int>(clusterShape->getClusterShapeInfos().size()));
		nXbin = std::max(nXbin, clusterShape->getClusterSize());
	}
	TCanvas* canvas = new TCanvas("shapeEntryTotal", "shape entry", 2500, 1000);
	TH2D* distribution = new TH2D("shapeEntryTotal", static_cast<TString>("The entries of shapes_" + mPostfix + "; Cluster Size; i-th cluster shape; Entry"), nXbin, -nXbin - .5, -.5, nYbin, -nYbin + .5, .5);
	ProgressBar* pBar = new ProgressBar(mNTotalShapes);
	for ( const TClusterShape* clusterShape : mClusterShapes ) {
		int clusterSize = clusterShape->getClusterSize();
		int iShape = 0;
		for ( const TShapeInfo shapeInfo : clusterShape->getClusterShapeInfos() ) {
			pBar->printProgress();
			distribution->Fill(-clusterSize, -iShape, shapeInfo.mEntry);
			iShape++;
		}
	}
	delete pBar;
	for ( int iXbin = 1; iXbin < distribution->GetNbinsX(); ++iXbin ) {
		distribution->GetXaxis()->SetBinLabel(iXbin, Form("%g", floor(-1 * distribution->GetXaxis()->GetBinLowEdge(iXbin))));
	}

	for ( int iYbin = 1; iYbin < distribution->GetNbinsY(); ++(++iYbin) ) {
		distribution->GetYaxis()->SetBinLabel(iYbin, Form("%g", floor(-1 * distribution->GetYaxis()->GetBinLowEdge(iYbin))));
	}
	distribution->SetStats(0);
	distribution->Draw("COLZ");
	canvas->SetGrid();
	canvas->SaveAs(static_cast<TString>(mSavePath / (mDataSetName + "shapes") / "shape_entry_total_colz.png"));

	distribution->Draw("LEGO2");
	canvas->SetPhi(10);
	canvas->SetTheta(25);
	canvas->SetLogz();
	canvas->SaveAs(static_cast<TString>(mSavePath / (mDataSetName + "shapes") / "shape_entry_total_lego.png"));
	delete distribution;
	delete canvas;
}