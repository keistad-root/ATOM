#include "TClusterShapeAnalyser.h"

TClusterShapeAnalyser::TClusterShapeAnalyser(const TClusterAnalyser& analyser) : TClusterAnalyser(analyser) {
	std::filesystem::create_directory(mSavePath / "shapes");
}

void TClusterShapeAnalyser::doShaping(int lower, int upper) {
	for ( int clusterSize = lower; clusterSize < upper; clusterSize++ ) { // For looking shape, cut is important because cluster size 1 or 2 omit meanless results.
		if ( static_cast<int>(mMaskedClustersize->GetBinContent(clusterSize)) != 0 ) {
			clusterShapes.push_back(new TClusterShape(mExpData->getMaskedClusters(), clusterSize));
			clusterShapes.back()->identifyShapes();
			clusterShapes.back()->sortShapes();
		}
	}
}

void TClusterShapeAnalyser::saveIndividualShapes() {
	std::filesystem::create_directory(mSavePath / "shapes" / "individual");
	for ( const TClusterShape* clusterShape : clusterShapes ) {
		int clusterSize = clusterShape->getClusterSize();
		int iClusterShape = 0;
		for ( const TShapeInfo& shapeInfo : clusterShape->getClusterShapeInfos() ) {
			int width = shapeInfo.mClusterImage->GetWidth();
			int height = shapeInfo.mClusterImage->GetHeight() + 25;
			TCanvas* canvas = new TCanvas(Form("%d_%d.png", clusterSize, iClusterShape), Form("cluster shape"), width, height);
			canvas->SetLeftMargin(0.);
			canvas->SetTopMargin(0.);
			TText* titleText = new TText(.5, 1. - .5 / (shapeInfo.mPresidentCluster->getShape().getNColumn() + 2), Form("%d-th Cluster Shape of Cluster Size %d", iClusterShape, clusterSize));
			titleText->SetTextAlign(23);
			titleText->SetNDC();
			shapeInfo.mClusterImage->Draw();
			titleText->Draw();
			canvas->SaveAs(static_cast<TString>(mSavePath / "shapes" / "individual" / static_cast<std::string>(Form("%d_%d.png", clusterSize, iClusterShape))));
			iClusterShape++;
		}
	}
}

void TClusterShapeAnalyser::saveSameSizeShapes() {
	std::filesystem::create_directory(mSavePath / "shapes" / "same_size");
	for ( const TClusterShape* clusterShape : clusterShapes ) {
		int clusterSize = clusterShape->getClusterSize();
		int iClusterShape = 0;
		int nClusterShape = clusterShape->getClusterShapeInfos().size();
		int nominalWidth = 100;
		int nominalHeader = 100;
		int plotsWidth = 10 * nominalWidth;
		int plotsHeight = (floor((nClusterShape - 1) / 10) + 1) * nominalWidth + 25;
		TCanvas* canvas = new TCanvas(Form("shapes%d", clusterSize), "cluster shapes", plotsWidth, plotsHeight + nominalHeader);
		for ( const TShapeInfo& shapeInfo : clusterShape->getClusterShapeInfos() ) {
			int width = shapeInfo.mClusterImage->GetWidth();
			int height = shapeInfo.mClusterImage->GetHeight();
			int factor = std::max(width, height);
			double padCenterX = (1 / 10.) * (.5 + (iClusterShape % 10));
			double padCenterY = (((double) plotsHeight / (plotsHeight + nominalHeader)) / (floor((nClusterShape - 1) / 10.) + 1.)) * ((floor((nClusterShape - 1) / 10.) + 1.) + .5 - (floor(iClusterShape / 10.) + 1.));
			double padWidth = (1 / 20.) * width / factor;
			double padHeight = (((double) plotsHeight / (plotsHeight + nominalHeader)) / ((floor((nClusterShape - 1) / 10) + 1.) * 2.)) * height / factor;
			TPad* pad = new TPad(Form("pad%d", iClusterShape), "pad", padCenterX - padWidth, padCenterY - padHeight, padCenterX + padWidth, padCenterY + padHeight);
			shapeInfo.mClusterImage->Scale(width * nominalWidth / factor, height * nominalWidth / factor);
			pad->Draw();
			pad->cd();
			pad->SetMargin(0., 0., 0., 0.);
			shapeInfo.mClusterImage->Draw();
			TText* numberingText = new TText(padCenterX, padCenterY, Form("%d", iClusterShape));
			numberingText->SetNDC();
			numberingText->SetTextAlign(22);
			numberingText->SetTextSize(.5 * nominalWidth / (plotsHeight + nominalHeader));
			numberingText->SetTextColorAlpha(kBlack, 5. / 8);
			canvas->cd();
			numberingText->Draw();
			iClusterShape++;
		}
		TText* titleText = new TText(.5, 1. - .5 * nominalHeader / (nominalHeader + plotsHeight), Form("Cluster shapes with cluster size %d", clusterSize));
		titleText->SetTextSize(.5 * nominalHeader / (plotsHeight + nominalHeader));
		titleText->SetTextAlign(22);
		titleText->SetNDC();
		titleText->Draw();
		canvas->SaveAs(static_cast<TString>(mSavePath / "shapes" / "same_size" / Form("shapes_size_%d.png", clusterSize)));
	}
}

void TClusterShapeAnalyser::saveTotalShapes() {
	int nWidth = 0;
	int nHeight = clusterShapes.size();
	for ( const TClusterShape* clusterShape : clusterShapes ) {
		nWidth = std::max(nWidth, static_cast<int>(clusterShape->getClusterShapeInfos().size()));
	}
	int nominalWidth = 100.;
	int nominalHeader = 100.;
	int plotsWidth = nWidth * nominalWidth;
	int plotsHeight = nHeight * nominalWidth;
	TCanvas* canvas = new TCanvas("tShape", "cluster shape", plotsWidth + nominalHeader, plotsHeight + nominalHeader);
	int iClusterSize = 0;
	for ( const TClusterShape* clusterShape : clusterShapes ) {
		int clusterSize = clusterShape->getClusterSize();
		int iClusterShape = 0;
		for ( const TShapeInfo& shapeInfo : clusterShape->getClusterShapeInfos() ) {
			int width = shapeInfo.mClusterImage->GetWidth();
			int height = shapeInfo.mClusterImage->GetHeight();
			int factor = std::max(width, height);
			TPad* pad = new TPad(Form("pad%d_%d", iClusterSize, iClusterShape), "pad", (double) nominalHeader / (plotsWidth + nominalHeader) + ((double) plotsWidth / (plotsWidth + nominalHeader)) * ((double) iClusterShape / nWidth), ((double) plotsHeight / (plotsHeight + nominalHeader)) * (((double) nHeight - iClusterSize - 1) / nHeight), (double) nominalHeader / (plotsWidth + nominalHeader) + ((double) plotsWidth / (plotsWidth + nominalHeader)) * (double) (iClusterShape + 1) / nWidth, ((double) plotsHeight / (plotsHeight + nominalHeader)) * (((double) nHeight - iClusterSize) / nHeight));
			pad->Draw();
			pad->cd();
			pad->SetMargin(.5 * (1. - (double) width / factor), .5 * (1. - (double) width / factor), .5 * (1. - (double) height / factor), .5 * (1. - (double) height / factor));
			shapeInfo.mClusterImage->Scale(width * nominalWidth / factor, height * nominalWidth / factor);
			shapeInfo.mClusterImage->Draw();
			canvas->cd();
			TText* numberingText = new TText((double) nominalHeader / (plotsWidth + nominalHeader) + ((double) plotsWidth / (plotsWidth + nominalHeader)) * (((double) iClusterShape + .5) / nWidth), ((double) plotsHeight / (plotsHeight + nominalHeader)) * (((double) nHeight - iClusterSize - .5) / nHeight), Form("%d", iClusterShape));
			numberingText->SetNDC();
			numberingText->SetTextAlign(22);
			numberingText->SetTextSize(.4 * nominalWidth / (plotsHeight + nominalHeader));
			numberingText->SetTextColorAlpha(kBlack, 5. / 8);
			numberingText->Draw();
			iClusterShape++;
		}
		TText* sizeText = new TText((double) nominalHeader * .7 / (nominalHeader + plotsHeight), ((double) plotsHeight / (plotsHeight + nominalHeader)) * (((double) nHeight - iClusterSize - .5) / nHeight), Form("%d", clusterSize));
		sizeText->SetNDC();
		sizeText->SetTextAlign(12);
		sizeText->SetTextSize(.6 * nominalWidth / (plotsHeight + nominalHeader));
		sizeText->SetTextColorAlpha(kBlack, 7. / 8);
		sizeText->Draw();
		iClusterSize++;
	}
	TText* titleText = new TText(.5, 1. - .5 * nominalHeader / (nominalHeader + plotsHeight), "Total Cluster shapes");
	titleText->SetTextSize(.8 * nominalHeader / (plotsHeight + nominalHeader));
	titleText->SetTextAlign(22);
	titleText->SetNDC();
	titleText->Draw();
	canvas->SaveAs(static_cast<TString>(mSavePath / "shapes" / "total_shape.png"));
}

void TClusterShapeAnalyser::saveSameSizeShapeEntry() {
	std::filesystem::create_directory(mSavePath / "shapes" / "same_size");
	for ( const TClusterShape* clusterShape : clusterShapes ) {
		int clusterSize = clusterShape->getClusterSize();
		TCanvas* canvas = new TCanvas(Form("shapeEntry%d", clusterSize), "shapeEntry", 2500, 1000);
		int binNum = clusterShape->getClusterShapeInfos().size();
		TH1D* distribution = new TH1D(Form("shapeEntry%d", clusterSize), "shapeEntry", binNum, -.5, binNum - .5);
		int iShape = 0;
		for ( const TShapeInfo& shapeInfo : clusterShape->getClusterShapeInfos() ) {
			distribution->Fill(iShape, shapeInfo.mEntry);
			iShape++;
		}
		distribution->GetXaxis()->SetNdivisions(binNum, 0, 0, true);
		distribution->GetXaxis()->SetTitle("i-th cluster size");
		distribution->GetXaxis()->SetTitleOffset(1.4);
		distribution->GetXaxis()->SetLabelOffset(0.003);
		distribution->GetYaxis()->SetTitle("Entry");
		distribution->GetYaxis()->SetTitleOffset(0.7);
		distribution->Draw("HIST");
		mExpSettingLegend->Draw("SAME");
		canvas->SetMargin(.07, .28, .12, .08);
		canvas->SaveAs(static_cast<TString>(mSavePath / "shapes" / "same_size" / Form("shapes_entry_size_%d.png", clusterSize)));
	}
}

void TClusterShapeAnalyser::saveTotalShapeEntry() {
	int nXbin = 0;
	int nYbin = 0;
	for ( const TClusterShape* clusterShape : clusterShapes ) {
		nYbin = std::max(nYbin, static_cast<int>(clusterShape->getClusterShapeInfos().size()));
		nXbin = std::max(nXbin, clusterShape->getClusterSize());
	}
	TCanvas* canvas = new TCanvas("shapeEntryTotal", "shape entry", 2500, 1000);
	TH2D* distribution = new TH2D("shapeEntryTotal", "The entries of shapes; Cluster Size; i-th cluster shape in each cluster size; Entry", nXbin, -nXbin - .5, -.5, nYbin, -nYbin - .5, -.5);
	for ( const TClusterShape* clusterShape : clusterShapes ) {
		int clusterSize = clusterShape->getClusterSize();
		int iShape = 0;
		for ( const TShapeInfo shapeInfo : clusterShape->getClusterShapeInfos() ) {
			distribution->Fill(-clusterSize, -iShape, shapeInfo.mEntry);
			iShape++;
		}
	}
	for ( int iXbin = 1; iXbin < distribution->GetNbinsX(); ++iXbin ) {
		distribution->GetXaxis()->SetBinLabel(iXbin, Form("%g", floor(-1 * distribution->GetXaxis()->GetBinLowEdge(iXbin))));
	}
	for ( int iYbin = 1; iYbin < distribution->GetNbinsX(); ++iYbin ) {
		distribution->GetXaxis()->SetBinLabel(iYbin, Form("%g", floor(-1 * distribution->GetXaxis()->GetBinLowEdge(iYbin))));
	}
	distribution->SetStats(0);
	distribution->Draw("LEGO2");
	canvas->SetLogz();
	canvas->SaveAs(static_cast<TString>(mSavePath / "shapes" / "shape_entry_total.png"));
}