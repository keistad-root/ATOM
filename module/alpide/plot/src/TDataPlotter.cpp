#include "TDataPlotter.h"

TDataPlotter::TDataPlotter(const CppConfigFile& config) : TPlotter(), mConfig(config) {
	mOutputPath = mConfig.getConfig("CONFIG").find("OUTPUT_DIRECTORY");
	std::filesystem::create_directories(mOutputPath);

	// gStyle->SetOptStat(0);

	if ( mConfig.hasConfig("HITMAP") ) isHitmap = true;
	if ( mConfig.hasConfig("HITMAP_PROJECTION_X") ) isHitmapProjectionX = true;
	if ( mConfig.hasConfig("HITMAP_PROJECTION_Y") ) isHitmapProjectionY = true;
	if ( mConfig.hasConfig("CLUSTERMAP") ) isClustermap = true;
	if ( mConfig.hasConfig("CLUSTERSIZE") ) isClustersize = true;
	if ( mConfig.hasConfig("CLUSTERSIZE_REGION") ) isClustersizeRegion = true;
}

TDataPlotter::~TDataPlotter() { }

void TDataPlotter::openInputFile() {
	std::filesystem::path inputPath = mConfig.getConfig("CONFIG").find("MASKED_FILE");
	mInputFile = std::make_unique<TFile>(static_cast<TString>(inputPath));
}

void TDataPlotter::InitPlot() {
	if ( isHitmap ) {
		mHitmap = new TH2D("hitmap", "Hitmap; Column; Row", ALPIDECOLUMN, 0, ALPIDECOLUMN, ALPIDEROW, 0, ALPIDEROW);
		setBins(mHitmap, mConfig.getConfig("HITMAP"));
	}
	if ( isHitmapProjectionX ) {
		mHitmapProjectionX = new TH1D("hitmapProjectionX", "Hitmap Projection X; Column; Entry", ALPIDECOLUMN, 0, ALPIDECOLUMN);
		setBins(mHitmapProjectionX, mConfig.getConfig("HITMAP_PROJECTION_X"));
	}
	if ( isHitmapProjectionY ) {
		mHitmapProjectionY = new TH1D("hitmapProjectionY", "Hitmap Projection Y; Row; Entry", ALPIDEROW, 0, ALPIDEROW);
		setBins(mHitmapProjectionY, mConfig.getConfig("HITMAP_PROJECTION_Y"));
	}
	if ( isClustermap ) {
		mClustermap = new TH2D("clustermap", "Clustermap; Column; Row", ALPIDECOLUMN, 0, ALPIDECOLUMN, ALPIDEROW, 0, ALPIDEROW);
		setBins(mClustermap, mConfig.getConfig("CLUSTERMAP"));
	}
	if ( isClustersize ) {
		mClustersize = new TH1D("clustersize", "Cluster size; Cluster size; Entry", 100, .5, 100.5);
		setBins(mClustersize, mConfig.getConfig("CLUSTERSIZE"));
	}
	if ( isClustersizeRegion ) {
		mClusterSizeOfRegion.push_back(new TH1D("R=2mm", "", 100, .5, 100.5));
		mClusterSizeOfRegion.push_back(new TH1D("R=4mm", "", 100, .5, 100.5));
		mClusterSizeOfRegion.push_back(new TH1D("R=6mm", "", 100, .5, 100.5));
		mClusterSizeOfRegion.push_back(new TH1D("R=8mm", "", 100, .5, 100.5));
	}
}

void TDataPlotter::FillHitInfo() {
	UInt_t timeStamp;
	UShort_t x, y;
	TTree* hitTree = static_cast<TTree*>(mInputFile->Get("hit"));
	hitTree->SetBranchAddress("TimeStamp", &timeStamp);
	hitTree->SetBranchAddress("X", &x);
	hitTree->SetBranchAddress("Y", &y);

	Int_t nHit = hitTree->GetEntries();

	for ( int iHit = 0; iHit < nHit; iHit++ ) {
		hitTree->GetEntry(iHit);
		if ( isHitmap ) mHitmap->Fill(x, y);
		if ( isHitmapProjectionX ) mHitmapProjectionX->Fill(x);
		if ( isHitmapProjectionY ) mHitmapProjectionY->Fill(y);
	}
}

void TDataPlotter::FillClusterInfo() {
	UInt_t timeStamp;
	Double_t x, y;
	UInt_t size;

	TTree* clusterTree = static_cast<TTree*>(mInputFile->Get("cluster"));
	clusterTree->SetBranchAddress("TimeStamp", &timeStamp);
	clusterTree->SetBranchAddress("X", &x);
	clusterTree->SetBranchAddress("Y", &y);
	clusterTree->SetBranchAddress("Size", &size);

	Int_t nCluster = clusterTree->GetEntries();

	for ( int iCluster = 0; iCluster < nCluster; iCluster++ ) {
		clusterTree->GetEntry(iCluster);
		if ( isClustermap ) mClustermap->Fill(x, y);
		if ( isClustersize ) mClustersize->Fill(size);
		if ( isClustersizeRegion ) {
			std::vector<double> center = TPlotter::getDoubleSetFromString(mConfig.getConfig("CLUSTERSIZE_REGION").find("center"));

			if ( std::abs(x - center[0]) < 2 * (1 / 0.028) && std::abs(y - center[1]) < 2 * (1 / 0.028) ) {
				mClusterSizeOfRegion[0]->Fill(size);
				mClusterSizeOfRegion[1]->Fill(size);
				mClusterSizeOfRegion[2]->Fill(size);
				mClusterSizeOfRegion[3]->Fill(size);
			} else if ( std::abs(x - center[0]) < 4 * (1 / 0.028) && std::abs(y - center[1]) < 4 * (1 / 0.028) ) {
				mClusterSizeOfRegion[1]->Fill(size);
				mClusterSizeOfRegion[2]->Fill(size);
				mClusterSizeOfRegion[3]->Fill(size);
			} else if ( std::abs(x - center[0]) < 6 * (1 / 0.028) && std::abs(y - center[1]) < 6 * (1 / 0.028) ) {
				mClusterSizeOfRegion[2]->Fill(size);
				mClusterSizeOfRegion[3]->Fill(size);
			} else if ( std::abs(x - center[0]) < 8 * (1 / 0.028) && std::abs(y - center[1]) < 8 * (1 / 0.028) ) {
				mClusterSizeOfRegion[3]->Fill(size);
			}
		}
	}
}

void TDataPlotter::FillShapeInfo() {
	Int_t clusterSize;
	Int_t nShape;
	TH2I* pixelMap = nullptr;

	TTree* shapeTree = static_cast<TTree*>(mInputFile->Get("shape"));
	shapeTree->SetBranchAddress("ClusterSize", &clusterSize);
	shapeTree->SetBranchAddress("NShape", &nShape);
	shapeTree->SetBranchAddress("PixelMap", &pixelMap);

	Int_t n = shapeTree->GetEntries();

	int prevClusterSize = -1;
	for ( int iShape = 0; iShape < n; iShape++ ) {
		shapeTree->GetEntry(iShape);
		mShapeSet.push_back({new TH2I(*pixelMap), nShape});
	}
}

void TDataPlotter::savePlots() {
	if ( isHitmap ) {
		TCanvas* canvas = new TCanvas("hitmapCanvas", "", 3000, 1500);
		savePlot(canvas, mHitmap, mConfig.getConfig("HITMAP"));
		if ( isClustersizeRegion ) {
			std::vector<double> center = TPlotter::getDoubleSetFromString(mConfig.getConfig("CLUSTERSIZE_REGION").find("center"));
			TEllipse* circle2mm = new TEllipse(center[0], center[1], 2 * (1 / 0.028));
			circle2mm->SetLineColor(kRed);
			circle2mm->SetLineWidth(2);
			circle2mm->SetFillStyle(0);
			circle2mm->Draw("SAME");
			TEllipse* circle4mm = new TEllipse(center[0], center[1], 4 * (1 / 0.028));
			circle4mm->SetLineColor(kRed);
			circle4mm->SetLineWidth(2);
			circle4mm->SetFillStyle(0);
			circle4mm->Draw("SAME");
			TEllipse* circle6mm = new TEllipse(center[0], center[1], 6 * (1 / 0.028));
			circle6mm->SetLineColor(kRed);
			circle6mm->SetLineWidth(2);
			circle6mm->SetFillStyle(0);
			circle6mm->Draw("SAME");
			TEllipse* circle8mm = new TEllipse(center[0], center[1], 8 * (1 / 0.028));
			circle8mm->SetLineColor(kRed);
			circle8mm->SetLineWidth(2);
			circle8mm->SetFillStyle(0);
			circle8mm->Draw("SAME");
		}
		saveCanvas(canvas, mOutputPath, mConfig.getConfig("HITMAP"));
		delete canvas;
		canvas = nullptr;
	}
	if ( isHitmapProjectionX ) {
		TCanvas* canvas = new TCanvas("hitmapProjectionXCanvas", "", 3000, 1500);
		savePlot(canvas, mHitmapProjectionX, mConfig.getConfig("HITMAP_PROJECTION_X"));
		saveCanvas(canvas, mOutputPath, mConfig.getConfig("HITMAP_PROJECTION_X"));
		delete canvas;
		canvas = nullptr;
	}
	if ( isHitmapProjectionY ) {
		TCanvas* canvas = new TCanvas("hitmapProjectionYCanvas", "", 3000, 1500);
		savePlot(canvas, mHitmapProjectionY, mConfig.getConfig("HITMAP_PROJECTION_Y"));
		saveCanvas(canvas, mOutputPath, mConfig.getConfig("HITMAP_PROJECTION_Y"));
		delete canvas;
		canvas = nullptr;
	}
	if ( isClustermap ) {
		TCanvas* canvas = new TCanvas("clustermapCanvas", "", 3000, 1500);
		savePlot(canvas, mClustermap, mConfig.getConfig("CLUSTERMAP"));
		saveCanvas(canvas, mOutputPath, mConfig.getConfig("CLUSTERMAP"));
		delete canvas;
		canvas = nullptr;
	}
	if ( isClustersize ) {
		TCanvas* canvas = new TCanvas("clustersizeCanvas", "", 3000, 1500);
		savePlot(canvas, mClustersize, mConfig.getConfig("CLUSTERSIZE"));
		saveCanvas(canvas, mOutputPath, mConfig.getConfig("CLUSTERSIZE"));
		delete canvas;
		canvas = nullptr;
	}
	if ( isClustersizeRegion ) {
		TCanvas* canvas = new TCanvas("clustersizeRegionCanvas", "", 3000, 1500);
		TLegend* legend = new TLegend(0.7, 0.6, 0.9, 0.9);
		mClusterSizeOfRegion[0]->SetLineColor(kRed);
		mClusterSizeOfRegion[0]->SetLineWidth(4);
		legend->AddEntry(mClusterSizeOfRegion[0], "R=2mm", "l");
		mClusterSizeOfRegion[1]->SetLineColor(kBlue);
		mClusterSizeOfRegion[1]->SetLineWidth(4);
		legend->AddEntry(mClusterSizeOfRegion[1], "R=4mm", "l");
		mClusterSizeOfRegion[2]->SetLineColor(kMagenta);
		mClusterSizeOfRegion[2]->SetLineWidth(4);
		legend->AddEntry(mClusterSizeOfRegion[2], "R=6mm", "l");
		mClusterSizeOfRegion[3]->SetLineColor(kGreen + 3);
		mClusterSizeOfRegion[3]->SetLineWidth(4);
		legend->AddEntry(mClusterSizeOfRegion[3], "R=8mm", "l");
		mClustersize->SetLineColor(kBlack);
		mClustersize->SetLineWidth(4);
		legend->AddEntry(mClustersize, "No Radius", "l");
		savePlot(canvas, mClusterSizeOfRegion[0], mConfig.getConfig("CLUSTERSIZE_REGION"));
		mClusterSizeOfRegion[1]->Draw("SAME");
		mClusterSizeOfRegion[2]->Draw("SAME");
		mClusterSizeOfRegion[3]->Draw("SAME");
		mClustersize->Draw("SAME");
		saveLegend(canvas, legend);
		saveCanvas(canvas, mOutputPath, mConfig.getConfig("CLUSTERSIZE_REGION"));
		delete canvas;
		canvas = nullptr;
	}
}

void TDataPlotter::getMeanX() {
	TH1D* meanX = new TH1D("meanX", "Mean X; Column; Entry", ALPIDECOLUMN, 0, ALPIDECOLUMN);
	for ( int i = 0; i < ALPIDEROW; i++ ) {
		TH1D* sliceX = new TH1D("sliceX", "Slice X; Column; Entry", ALPIDECOLUMN, 0, ALPIDECOLUMN);
		for ( int j = 0; j < ALPIDECOLUMN; j++ ) {
			sliceX->SetBinContent(j + 1, mHitmap->GetBinContent(j + 1, i + 1));
		}
		sliceX->SetBinContent(517, (sliceX->GetBinContent(515) + sliceX->GetBinContent(519)) / 2);
		sliceX->SetBinContent(518, (sliceX->GetBinContent(516) + sliceX->GetBinContent(520)) / 2);

		TF1* fitFunc = new TF1("fitFunc", "gaus", 0, ALPIDECOLUMN);
		sliceX->Fit(fitFunc, "R");
		meanX->SetBinContent(i, fitFunc->GetParameter(1));
	}
}

void TDataPlotter::saveTotalShape() {
	int prevClusterSize = -1;
	int maxClusterSize = 0;
	int currentClusterSize = 0;
	int nClusterSize = 0;
	for ( const auto& shape : mShapeSet ) {
		if ( prevClusterSize != shape.first->GetEntries() ) {
			prevClusterSize = shape.first->GetEntries();
			maxClusterSize = std::max(currentClusterSize, maxClusterSize);
			currentClusterSize = 1;
			nClusterSize++;
		} else {
			currentClusterSize++;
		}
	}

	int nWidth = maxClusterSize;
	int nHeight = nClusterSize;

	int nominalWidth = 100;
	int nominalHeader = 100;

	int plotsWidth = nWidth * nominalWidth;
	int plotsHeight = nHeight * nominalWidth;

	TCanvas* canvas = new TCanvas("tShape", "", plotsWidth + nominalHeader, plotsHeight + nominalHeader);
	std::vector<TText*> mTextSet;
	std::vector<TPad*> mPadSet;
	std::vector<TLine*> mLineSet;

	prevClusterSize = 1;
	int iClusterSize = 0;
	int iClusterShape = 0;
	for ( const auto& shape : mShapeSet ) {
		int clusterSize = shape.first->GetEntries();
		if ( prevClusterSize != shape.first->GetEntries() ) {
			iClusterSize++;
			prevClusterSize = shape.first->GetEntries();
			TText* sizeText = new TText((double) nominalHeader * .5 / (nominalHeader + plotsWidth), ((double) plotsHeight / (plotsHeight + nominalHeader)) * (((double) nHeight - iClusterSize - .5) / nHeight), Form("%d", clusterSize));
			mTextSet.push_back(sizeText);
			sizeText->SetNDC();
			sizeText->SetTextAlign(22);
			sizeText->SetTextSize(.6 * nominalWidth / (plotsHeight + nominalHeader));
			sizeText->SetTextColor(kBlack);
			sizeText->Draw();
			iClusterShape = 0;
		}
		int width = shape.first->GetNbinsX();
		int height = shape.first->GetNbinsY();

		TPad* pad = new TPad(Form("pad&d_%d", clusterSize, iClusterShape), "", (double) nominalHeader / (plotsWidth + nominalHeader) + ((double) plotsWidth / (plotsWidth + nominalHeader)) * ((double) iClusterShape / nWidth), ((double) plotsHeight / (plotsHeight + nominalHeader)) * (((double) nHeight - iClusterSize - 1) / nHeight), (double) nominalHeader / (plotsWidth + nominalHeader) + ((double) plotsWidth / (plotsWidth + nominalHeader)) * (double) (iClusterShape + 1) / nWidth, ((double) plotsHeight / (plotsHeight + nominalHeader)) * (((double) nHeight - iClusterSize) / nHeight), -1, 1);
		mPadSet.push_back(pad);
		pad->Draw();
		pad->cd();
		pad->SetMargin(0., 0., .5 * (1. - (double) height / width), .5 * (1. - (double) height / width));

		shape.first->Draw("COL");
		shape.first->GetXaxis()->SetAxisColor(0);
		shape.first->GetYaxis()->SetAxisColor(0);
		TLine* line = new TLine();
		mLineSet.push_back(line);
		line->SetLineColorAlpha(kRed, 6. / 8);
		for ( int i = 1; i <= shape.first->GetNbinsX(); ++i ) {
			for ( int j = 1; j <= shape.first->GetNbinsY(); ++j ) {
				if ( shape.first->GetBinContent(i, j) > 0 ) {
					double xlow = shape.first->GetXaxis()->GetBinLowEdge(i);
					double xup = shape.first->GetXaxis()->GetBinUpEdge(i);
					double ylow = shape.first->GetYaxis()->GetBinLowEdge(j);
					double yup = shape.first->GetYaxis()->GetBinUpEdge(j);
					line->DrawLine(xlow, ylow, xup, ylow); // Bottom
					line->DrawLine(xlow, yup, xup, yup); // Top
					line->DrawLine(xlow, ylow, xlow, yup); // Left
					line->DrawLine(xup, ylow, xup, yup); // Right
				}
			}
		}
		pad->SetFrameLineWidth(0);
		canvas->cd();
		TText* numberingText = new TText((double) nominalHeader / (plotsWidth + nominalHeader) + ((double) plotsWidth / (plotsWidth + nominalHeader)) * (((double) iClusterShape + .5) / nWidth), ((double) plotsHeight / (plotsHeight + nominalHeader)) * (((double) nHeight - iClusterSize) / nHeight), Form("%d", shape.second));
		mTextSet.push_back(numberingText);
		numberingText->SetNDC();
		numberingText->SetTextAlign(22);
		numberingText->SetTextSize(.4 * nominalWidth / (plotsHeight + nominalHeader));
		numberingText->SetTextColor(kBlack);
		numberingText->Draw();
		iClusterShape++;

	}
	saveCanvas(canvas, mOutputPath, mConfig.getConfig("TotalShape"));

	// for ( TText* text : mTextSet ) {
	// 	delete text;
	// 	text = nullptr;
	// }
	// for ( TPad* pad : mPadSet ) {
	// 	delete pad;
	// 	pad = nullptr;
	// }
	// for ( TLine* line : mLineSet ) {
	// 	delete line;
	// 	line = nullptr;
	// }
}

void TDataPlotter::saveTop10Shape() {
	std::sort(mShapeSet.begin(), mShapeSet.end(), [ ](const std::pair<TH2I*, int>& a, const std::pair<TH2I*, int>& b) {
		return a.second > b.second;
			  });
	int nShape = std::min((int) mShapeSet.size(), 10);
	TCanvas* canvas = new TCanvas("top10Shape", "", 5000, 500);
	for ( int i = 0; i < nShape; i++ ) {
		TH2I* shape = mShapeSet[i].first;

		TPad* pad = new TPad(Form("pad_%d", i), "", 0.1 * i, 0., .1 + .1 * i, 1.);
		pad->Draw();
		pad->cd();
		pad->SetMargin(0., 0., .5 * (1. - (double) shape->GetNbinsY() / shape->GetNbinsX()), .5 * (1. - (double) shape->GetNbinsY() / shape->GetNbinsX()));
		shape->Draw("COL");
		shape->GetXaxis()->SetAxisColor(0);
		shape->GetYaxis()->SetAxisColor(0);

		TLine* line = new TLine();
		line->SetLineColorAlpha(kRed, 6. / 8);
		for ( int i = 0; i < shape->GetNbinsX(); i++ ) {
			for ( int j = 0; j < shape->GetNbinsY(); j++ ) {
				if ( shape->GetBinContent(i, j) == 1 ) {
					double xlow = shape->GetXaxis()->GetBinLowEdge(i);
					double xup = shape->GetXaxis()->GetBinUpEdge(i);
					double ylow = shape->GetYaxis()->GetBinLowEdge(j);
					double yup = shape->GetYaxis()->GetBinUpEdge(j);
					line->DrawLine(xlow, ylow, xup, ylow); // Bottom
					line->DrawLine(xlow, yup, xup, yup); // Top
					line->DrawLine(xlow, ylow, xlow, yup); // Left
					line->DrawLine(xup, ylow, xup, yup); // Right
				}
			}
		}
		pad->SetFrameLineWidth(0);
		canvas->cd();
		TText* numberingText = new TText(.05 + .1 * i, .95, Form("%d", mShapeSet[i].second));
		numberingText->SetNDC();
		numberingText->SetTextAlign(22);
		numberingText->SetTextSize(.1);
		numberingText->SetTextColor(kBlack);
		numberingText->Draw();
	}
	saveCanvas(canvas, mOutputPath, mConfig.getConfig("Top10Shape"));
}

void TDataPlotter::saveClusterSizeWithTime() {
	TCanvas* canvas = new TCanvas("clusterSizeWithTime", "", 1000, 1000);
	TH2D* clusterSizeWithTime = new TH2D();
	initHist(clusterSizeWithTime, mConfig.getConfig("ClusterSizeWithTime"));

	UInt_t timeStamp;
	UInt_t size;

	TTree* clusterTree = static_cast<TTree*>(mInputFile->Get("cluster"));
	clusterTree->SetBranchAddress("TimeStamp", &timeStamp);
	clusterTree->SetBranchAddress("Size", &size);

	for ( int i = 0; i < clusterTree->GetEntries(); i++ ) {
		clusterTree->GetEntry(i);
		clusterSizeWithTime->Fill(timeStamp, size);
	}

	savePlot(canvas, clusterSizeWithTime, "ClusterSizeWithTime");

	saveCanvas(canvas, mOutputPath, mConfig.getConfig("ClusterSizeWithTime"));
}

std::array<int, 60> TDataPlotter::getEntry() {
	std::array<int, 60> entry;
	for ( int i = 0; i < 60; i++ ) {
		entry[i] = mClustersize->GetBinContent(i + 1);
	}
	return entry;
}