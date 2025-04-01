#include "TDataPlotter.h"

// ROOT header
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TStyle.h"
#include "TLine.h"
#include "TPad.h"
#include "TText.h"
#include "TEllipse.h"
#include "TF1.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "TGraphErrors.h"

// User header
#include "TALPIDEEvent.h"
#include "TPlotter.h"
#include "CppConfigFile.h"
#include "TClusterShape.h"

#include<unistd.h>
#include<utility>

const int ALPIDECOLUMN = 1024;
const int ALPIDEROW = 512;

TDataPlotter::TDataPlotter(const CppConfigFile& config) : mConfig(config) {
	mOutputPath = mConfig.getConfig("CONFIG").find("OUTPUT_DIRECTORY");
	std::filesystem::create_directories(mOutputPath);

	// gStyle->SetOptStat(0);

	if ( mConfig.hasConfig("HITMAP") ) isHitmap = true;
	if ( mConfig.hasConfig("HITMAP_PROJECTION_X") ) isHitmapProjectionX = true;
	if ( mConfig.hasConfig("HITMAP_PROJECTION_Y") ) isHitmapProjectionY = true;
	if ( mConfig.hasConfig("CLUSTERMAP") ) isClustermap = true;
	if ( mConfig.hasConfig("CLUSTERSIZE") ) isClustersize = true;
	if ( mConfig.hasConfig("CLUSTERSIZE_REGION") ) isClustersizeRegion = true;
	if ( mConfig.hasConfig("CLUSTERMAP_PROJECTION_X") ) isClustermapProjectionX = true;
	if ( mConfig.hasConfig("CLUSTERMAP_PROJECTION_Y") ) isClustermapProjectionY = true;
	if ( mConfig.hasConfig("CLUSTERMAP_SLICE_X") ) isClustermapSliceX = true;
	if ( mConfig.hasConfig("CLUSTERMAP_SLICE_Y") ) isClustermapSliceY = true;
	if ( mConfig.hasConfig("HITMAP_SLICE_X") ) isHitmapSliceX = true;
	if ( mConfig.hasConfig("HITMAP_SLICE_Y") ) isHitmapSliceY = true;

	mCenter = mConfig.getConfig("CONFIG").hasKey("CENTER") ? TPlotter::getDoubleSetFromString(mConfig.getConfig("CONFIG").find("CENTER")) : std::vector<double>{512, 256};
	mROI = mConfig.getConfig("CONFIG").hasKey("ROI") ? TPlotter::getDoubleSetFromString(mConfig.getConfig("CONFIG").find("ROI")) : std::vector<double>{512, 256};
	mROITheta = mConfig.getConfig("CONFIG").hasKey("ROI_THETA") ? std::tan(stod(mConfig.getConfig("CONFIG").find("ROI_THETA")) * TMath::Pi() / 180) : 0;
}

TDataPlotter::~TDataPlotter() {
	mInputFile->Close();
}

void TDataPlotter::openInputFile() {
	std::filesystem::path inputPath = mConfig.getConfig("CONFIG").find("MASKED_FILE");
	mInputFile = new TFile(static_cast<TString>(inputPath));
}

void TDataPlotter::InitPlot() {
	if ( isHitmap ) {
		mHitmap = TPlotter::init2DHist(mConfig.getConfig("HITMAP"));
	}
	if ( isHitmapProjectionX ) {
		mHitmapProjectionX = TPlotter::init1DHist(mConfig.getConfig("HITMAP_PROJECTION_X"));
	}
	if ( isHitmapProjectionY ) {
		mHitmapProjectionY = TPlotter::init1DHist(mConfig.getConfig("HITMAP_PROJECTION_Y"));
	}
	if ( isClustermap ) {
		mClustermap = TPlotter::init2DHist(mConfig.getConfig("CLUSTERMAP"));
	}
	if ( isClustermapProjectionX ) {
		mClustermapProjectionX = TPlotter::init1DHist(mConfig.getConfig("CLUSTERMAP_PROJECTION_X"));
	}
	if ( isClustermapProjectionY ) {
		mClustermapProjectionY = TPlotter::init1DHist(mConfig.getConfig("CLUSTERMAP_PROJECTION_Y"));
	}
	if ( isClustersize ) {
		mClustersize = TPlotter::init1DHist(mConfig.getConfig("CLUSTERSIZE"));
	}

	if ( isClustersizeRegion ) {
		mClusterSizeOfRegion.push_back(new TH1D("R=2mm", "", 100, .5, 100.5));
		mClusterSizeOfRegion.push_back(new TH1D("R=4mm", "", 100, .5, 100.5));
		mClusterSizeOfRegion.push_back(new TH1D("R=6mm", "", 100, .5, 100.5));
		mClusterSizeOfRegion.push_back(new TH1D("R=8mm", "", 100, .5, 100.5));
	}
	if ( isHitmapSliceX ) {
		int nPlot = mConfig.getConfig("HITMAP_SLICE_X").getSubConfig("SUB_PLOTS").getSubConfigSet().size();
		for ( int i = 0; i < nPlot; i++ ) {
			mHitmapSliceX.push_back(TPlotter::init1DHist(mConfig.getConfig("HITMAP_SLICE_X").getSubConfig("SUB_PLOTS").getSubConfigSet()[i]));
		}
		mHitmapSliceXMean = new TGraphErrors();
		mHitmapSliceXAmplitude = new TGraphErrors();
	}
	if ( isHitmapSliceY ) {
		int nPlot = mConfig.getConfig("HITMAP_SLICE_Y").getSubConfig("SUB_PLOTS").getSubConfigSet().size();
		for ( int i = 0; i < nPlot; i++ ) {
			mHitmapSliceY.push_back(TPlotter::init1DHist(mConfig.getConfig("HITMAP_SLICE_Y").getSubConfig("SUB_PLOTS").getSubConfigSet()[i]));
		}
		mHitmapSliceYMean = new TGraphErrors();
		mHitmapSliceYAmplitude = new TGraphErrors();
	}
	if ( isClustermapSliceX ) {
		int nPlot = mConfig.getConfig("CLUSTERMAP_SLICE_X").getSubConfig("SUB_PLOTS").getSubConfigSet().size();
		for ( int i = 0; i < nPlot; i++ ) {
			mClustermapSliceX.push_back(TPlotter::init1DHist(mConfig.getConfig("CLUSTERMAP_SLICE_X").getSubConfig("SUB_PLOTS").getSubConfigSet()[i]));
		}
		mClustermapSliceXMean = new TGraphErrors();
		mClustermapSliceXAmplitude = new TGraphErrors();
	}
	if ( isClustermapSliceY ) {
		int nPlot = mConfig.getConfig("CLUSTERMAP_SLICE_Y").getSubConfig("SUB_PLOTS").getSubConfigSet().size();
		for ( int i = 0; i < nPlot; i++ ) {
			mClustermapSliceY.push_back(TPlotter::init1DHist(mConfig.getConfig("CLUSTERMAP_SLICE_Y").getSubConfig("SUB_PLOTS").getSubConfigSet()[i]));
		}
		mClustermapSliceYMean = new TGraphErrors();
		mClustermapSliceYAmplitude = new TGraphErrors();
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
		if ( isInsideRegion(x - mCenter[0], y - mCenter[1]) ) {
			if ( isHitmap ) { mHitmap->Fill(x, y); }
			if ( isHitmapProjectionX ) { mHitmapProjectionX->Fill(x); }
			if ( isHitmapProjectionY ) { mHitmapProjectionY->Fill(y); }
		}
		if ( isHitmapSliceX ) {
			int nPlot = mConfig.getConfig("HITMAP_SLICE_X").getSubConfig("SUB_PLOTS").getSubConfigSet().size();
			for ( int i = 0; i < nPlot; i++ ) {
				std::vector<double> range = TPlotter::getDoubleSetFromString(mConfig.getConfig("HITMAP_SLICE_X").getSubConfig("SUB_PLOTS").getSubConfigSet()[i].find("ROW_RANGE"));
				if ( range[0] < y && y < range[1] ) {
					mHitmapSliceX[i]->Fill(x);
				}
			}
		}
		if ( isHitmapSliceY ) {
			int nPlot = mConfig.getConfig("HITMAP_SLICE_Y").getSubConfig("SUB_PLOTS").getSubConfigSet().size();
			for ( int i = 0; i < nPlot; i++ ) {
				std::vector<double> range = TPlotter::getDoubleSetFromString(mConfig.getConfig("HITMAP_SLICE_Y").getSubConfig("SUB_PLOTS").getSubConfigSet()[i].find("COLUMN_RANGE"));
				if ( range[0] < x && x < range[1] ) {
					mHitmapSliceY[i]->Fill(y);
				}
			}
		}
	}
}

bool TDataPlotter::isInsideRegion(const double x, const double y) {
	double cosTheta = 1 / std::sqrt(1 + std::pow(mROITheta, 2));
	double sinTheta = mROITheta / std::sqrt(1 + std::pow(mROITheta, 2));

	std::pair<double, double> p1 = {-mROI[0] * cosTheta - mROI[1] * sinTheta, -mROI[0] * sinTheta + mROI[1] * cosTheta};
	std::pair<double, double> p2 = {mROI[0] * cosTheta - mROI[1] * sinTheta, mROI[0] * sinTheta + mROI[1] * cosTheta};
	std::pair<double, double> p3 = {mROI[0] * cosTheta + mROI[1] * sinTheta, mROI[0] * sinTheta - mROI[1] * cosTheta};
	std::pair<double, double> p4 = {-mROI[0] * cosTheta + mROI[1] * sinTheta, -mROI[0] * sinTheta - mROI[1] * cosTheta};

	std::pair<double, double> v1 = {p2.first - p1.first, p2.second - p1.second};
	std::pair<double, double> v2 = {p3.first - p2.first, p3.second - p2.second};
	std::pair<double, double> v3 = {p4.first - p3.first, p4.second - p3.second};
	std::pair<double, double> v4 = {p1.first - p4.first, p1.second - p4.second};

	std::pair<double, double> vp1 = {x - p1.first, y - p1.second};
	std::pair<double, double> vp2 = {x - p2.first, y - p2.second};
	std::pair<double, double> vp3 = {x - p3.first, y - p3.second};
	std::pair<double, double> vp4 = {x - p4.first, y - p4.second};

	double cross1 = v1.first * vp1.second - v1.second * vp1.first;
	double cross2 = v2.first * vp2.second - v2.second * vp2.first;
	double cross3 = v3.first * vp3.second - v3.second * vp3.first;
	double cross4 = v4.first * vp4.second - v4.second * vp4.first;

	if ( cross1 > 0 && cross2 > 0 && cross3 > 0 && cross4 > 0 ) {
		return true;
	} else if ( cross1 < 0 && cross2 < 0 && cross3 < 0 && cross4 < 0 ) { return true; } else {
		return false;
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
		if ( isInsideRegion(x - mCenter[0], y - mCenter[1]) ) {
			if ( isClustermap ) { mClustermap->Fill(x, y); }
			if ( isClustersize ) { mClustersize->Fill(size); }
			if ( isClustermapProjectionX ) mClustermapProjectionX->Fill(x);
			if ( isClustermapProjectionY ) mClustermapProjectionY->Fill(y);
		}
		if ( isClustersizeRegion ) {
			if ( std::abs(x - mCenter[0]) < 2 * (1 / 0.028) && std::abs(y - mCenter[1]) < 2 * (1 / 0.028) ) {
				mClusterSizeOfRegion[0]->Fill(size);
				mClusterSizeOfRegion[1]->Fill(size);
				mClusterSizeOfRegion[2]->Fill(size);
				mClusterSizeOfRegion[3]->Fill(size);
			} else if ( std::abs(x - mCenter[0]) < 4 * (1 / 0.028) && std::abs(y - mCenter[1]) < 4 * (1 / 0.028) ) {
				mClusterSizeOfRegion[1]->Fill(size);
				mClusterSizeOfRegion[2]->Fill(size);
				mClusterSizeOfRegion[3]->Fill(size);
			} else if ( std::abs(x - mCenter[0]) < 6 * (1 / 0.028) && std::abs(y - mCenter[1]) < 6 * (1 / 0.028) ) {
				mClusterSizeOfRegion[2]->Fill(size);
				mClusterSizeOfRegion[3]->Fill(size);
			} else if ( std::abs(x - mCenter[0]) < 8 * (1 / 0.028) && std::abs(y - mCenter[1]) < 8 * (1 / 0.028) ) {
				mClusterSizeOfRegion[3]->Fill(size);
			}
		}
		if ( isClustermapSliceX ) {
			int nPlot = mConfig.getConfig("CLUSTERMAP_SLICE_X").getSubConfig("SUB_PLOTS").getSubConfigSet().size();
			for ( int i = 0; i < nPlot; i++ ) {
				std::vector<double> range = TPlotter::getDoubleSetFromString(mConfig.getConfig("CLUSTERMAP_SLICE_X").getSubConfig("SUB_PLOTS").getSubConfigSet()[i].find("ROW_RANGE"));
				if ( range[0] < y && y < range[1] ) {
					mClustermapSliceX[i]->Fill(x);
				}
			}
		}
		if ( isClustermapSliceY ) {
			int nPlot = mConfig.getConfig("CLUSTERMAP_SLICE_Y").getSubConfig("SUB_PLOTS").getSubConfigSet().size();
			for ( int i = 0; i < nPlot; i++ ) {
				std::vector<double> range = TPlotter::getDoubleSetFromString(mConfig.getConfig("CLUSTERMAP_SLICE_Y").getSubConfig("SUB_PLOTS").getSubConfigSet()[i].find("COLUMN_RANGE"));
				if ( range[0] < x && x < range[1] ) {
					mClustermapSliceY[i]->Fill(y);
				}
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
	for ( int iShape = 0; iShape < n; iShape++ ) {
		shapeTree->GetEntry(iShape);
		mShapeSet.push_back({new TH2I(*pixelMap), nShape});
	}
}

void TDataPlotter::drawCircle(TCanvas* canvas) {
	canvas->cd();
	TEllipse* circle2mm = new TEllipse(mCenter[0], mCenter[1], 2 * (1 / 0.028));
	circle2mm->SetLineColor(kRed);
	circle2mm->SetLineWidth(2);
	circle2mm->SetFillStyle(0);
	circle2mm->Draw("SAME");
	TEllipse* circle4mm = new TEllipse(mCenter[0], mCenter[1], 4 * (1 / 0.028));
	circle4mm->SetLineColor(kRed);
	circle4mm->SetLineWidth(2);
	circle4mm->SetFillStyle(0);
	circle4mm->Draw("SAME");
	TEllipse* circle6mm = new TEllipse(mCenter[0], mCenter[1], 6 * (1 / 0.028));
	circle6mm->SetLineColor(kRed);
	circle6mm->SetLineWidth(2);
	circle6mm->SetFillStyle(0);
	circle6mm->Draw("SAME");
	TEllipse* circle8mm = new TEllipse(mCenter[0], mCenter[1], 8 * (1 / 0.028));
	circle8mm->SetLineColor(kRed);
	circle8mm->SetLineWidth(2);
	circle8mm->SetFillStyle(0);
	circle8mm->Draw("SAME");
}

void TDataPlotter::savePlots() {
	if ( isHitmap ) {
		TCanvas* canvas = TPlotter::initCanvas(mConfig.getConfig("HITMAP"));
		TPlotter::drawPlot(canvas, mHitmap, mConfig.getConfig("HITMAP"));
		if ( mConfig.getConfig("HITMAP").hasKey("circle") && mConfig.getConfig("HITMAP").find("circle") == "true" ) { drawCircle(canvas); }
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("HITMAP"));
		delete canvas;
	}

	if ( isHitmapProjectionX ) {
		TCanvas* canvas = TPlotter::initCanvas(mConfig.getConfig("HITMAP_PROJECTION_X"));
		mHitmapProjectionX->SetBinContent(259, (mHitmapProjectionX->GetBinContent(258) + mHitmapProjectionX->GetBinContent(260)) / 2);
		TPlotter::drawPlot(canvas, mHitmapProjectionX, mConfig.getConfig("HITMAP_PROJECTION_X"));
		if ( mConfig.getConfig("HITMAP_PROJECTION_X").hasKey("FIT_FUNC") ) {
			TF1* fitFunc = TPlotter::initFunction(mConfig.getConfig("HITMAP_PROJECTION_X"));
			mHitmapProjectionX->Fit(fitFunc, "RQ");
			TText* text = new TText(0.5, 0.5, Form("Mean: %.2f", fitFunc->GetParameter(1)));
			text->SetNDC();
			text->SetTextAlign(22);
			text->Draw("SAME");
		}
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("HITMAP_PROJECTION_X"));

		delete canvas;
	}

	if ( isHitmapProjectionY ) {
		TCanvas* canvas = TPlotter::initCanvas(mConfig.getConfig("HITMAP_PROJECTION_Y"));
		TPlotter::drawPlot(canvas, mHitmapProjectionY, mConfig.getConfig("HITMAP_PROJECTION_Y"));
		if ( mConfig.getConfig("HITMAP_PROJECTION_Y").hasKey("FIT_FUNC") ) {
			TF1* fitFunc = TPlotter::initFunction(mConfig.getConfig("HITMAP_PROJECTION_Y"));
			mHitmapProjectionY->Fit(fitFunc, "RQ");
			TText* text = new TText(0.5, 0.5, Form("Mean: %.2f", fitFunc->GetParameter(1)));
			text->SetNDC();
			text->SetTextAlign(22);
			text->Draw("SAME");
		}
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("HITMAP_PROJECTION_Y"));
		delete canvas;
	}

	if ( isClustermapProjectionX ) {
		TCanvas* canvas = TPlotter::initCanvas(mConfig.getConfig("CLUSTERMAP_PROJECTION_X"));
		mClustermapProjectionX->SetBinContent(130, (mClustermapProjectionX->GetBinContent(128) + mClustermapProjectionX->GetBinContent(132)) / 2);
		mClustermapProjectionX->SetBinContent(129, (mClustermapProjectionX->GetBinContent(127) + mClustermapProjectionX->GetBinContent(131)) / 2);
		TPlotter::drawPlot(canvas, mClustermapProjectionX, mConfig.getConfig("CLUSTERMAP_PROJECTION_X"));
		if ( mConfig.getConfig("CLUSTERMAP_PROJECTION_X").hasKey("FIT_FUNC") ) {
			TF1* fitFunc = TPlotter::initFunction(mConfig.getConfig("CLUSTERMAP_PROJECTION_X"));
			mClustermapProjectionX->Fit(fitFunc, "RQ");
			TPaveText* latex = new TPaveText(.8, .5, .95, .95, "NDC");
			latex->AddText(Form("[0]: %.0f #pm %.0f", fitFunc->GetParameter(0), fitFunc->GetParError(0)));
			latex->AddText(Form("[1]: %.0f #pm %.0f", fitFunc->GetParameter(1), fitFunc->GetParError(1)));
			latex->AddText(Form("[2]: %.0f #pm %.0f", fitFunc->GetParameter(2), fitFunc->GetParError(2)));
			latex->AddText(Form("[3]: %.0f #pm %.0f", fitFunc->GetParameter(3), fitFunc->GetParError(3)));
			latex->AddText(Form("#chi^{2}/NDoF: %.2f", fitFunc->GetChisquare() / fitFunc->GetNDF()));
			latex->SetLabel("Fit parameters");
			latex->Draw();
		}
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("CLUSTERMAP_PROJECTION_X"));
		delete canvas;
	}

	if ( isClustermapProjectionY ) {
		TCanvas* canvas = TPlotter::initCanvas(mConfig.getConfig("CLUSTERMAP_PROJECTION_Y"));
		TPlotter::drawPlot(canvas, mClustermapProjectionY, mConfig.getConfig("CLUSTERMAP_PROJECTION_Y"));
		if ( mConfig.getConfig("CLUSTERMAP_PROJECTION_Y").hasKey("FIT_FUNC") ) {
			TF1* fitFunc = TPlotter::initFunction(mConfig.getConfig("CLUSTERMAP_PROJECTION_Y"));
			mClustermapProjectionY->Fit(fitFunc, "RQ");
			TPaveText* latex = new TPaveText(.8, .5, .95, .95, "NDC");
			latex->AddText(Form("[0]: %.0f #pm %.0f", fitFunc->GetParameter(0), fitFunc->GetParError(0)));
			latex->AddText(Form("[1]: %.0f #pm %.0f", fitFunc->GetParameter(1), fitFunc->GetParError(1)));
			latex->AddText(Form("[2]: %.0f #pm %.0f", fitFunc->GetParameter(2), fitFunc->GetParError(2)));
			latex->AddText(Form("[3]: %.0f #pm %.0f", fitFunc->GetParameter(3), fitFunc->GetParError(3)));
			latex->AddText(Form("#chi^{2}/NDoF: %.2f", fitFunc->GetChisquare() / fitFunc->GetNDF()));
			latex->SetLabel("Fit parameters");
			latex->Draw();
		}
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("CLUSTERMAP_PROJECTION_Y"));
		delete canvas;
	}

	if ( isClustermap ) {
		TCanvas* canvas = TPlotter::initCanvas(mConfig.getConfig("CLUSTERMAP"));
		TPlotter::drawPlot(canvas, mClustermap, mConfig.getConfig("CLUSTERMAP"));
		if ( mConfig.getConfig("CLUSTERMAP").hasKey("circle") && mConfig.getConfig("CLUSTERMAP").find("circle") == "true" ) { drawCircle(canvas); }
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("CLUSTERMAP"));
		delete canvas;
	}

	if ( isClustersize ) {
		TCanvas* canvas = TPlotter::initCanvas(mConfig.getConfig("CLUSTERSIZE"));
		TPlotter::drawPlot(canvas, mClustersize, mConfig.getConfig("CLUSTERSIZE"));
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("CLUSTERSIZE"));
		delete canvas;
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
		TPlotter::drawPlot(canvas, mClusterSizeOfRegion[0], mConfig.getConfig("CLUSTERSIZE_REGION"));
		mClusterSizeOfRegion[1]->Draw("SAME");
		mClusterSizeOfRegion[2]->Draw("SAME");
		mClusterSizeOfRegion[3]->Draw("SAME");
		mClustersize->Draw("SAME");
		TPlotter::saveLegend(canvas, legend);
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("CLUSTERSIZE_REGION"));
		delete canvas;
		delete legend;
	}
	if ( isHitmapSliceX ) {
		int nPlot = mConfig.getConfig("HITMAP_SLICE_X").getSubConfig("SUB_PLOTS").getSubConfigSet().size();
		for ( int i = 0; i < nPlot; i++ ) {
			CppConfigDictionary plotConfig = mConfig.getConfig("HITMAP_SLICE_X").getSubConfig("SUB_PLOTS").getSubConfigSet()[i];

			TCanvas* canvas = TPlotter::initCanvas(plotConfig);
			TPlotter::drawPlot(canvas, mHitmapSliceX[i], plotConfig);
			// mHitmapSliceX[i]->SetBinContent(259, (mHitmapSliceX[i]->GetBinContent(258) + mHitmapSliceX[i]->GetBinContent(260)) / 2);
			// mHitmapSliceX[i]->SetBinContent(260, (mHitmapSliceX[i]->GetBinContent(259) + mHitmapSliceX[i]->GetBinContent(261)) / 2);
			TF1* fitFunc = nullptr;
			TLegend* fitText = nullptr;
			std::cout << mHitmapSliceX[i]->GetMaximum() << std::endl;
			if ( plotConfig.hasKey("FIT_FUNC") ) {
				fitFunc = TPlotter::initFunction(plotConfig);
				mHitmapSliceX[i]->Fit(fitFunc, "RQ");
				fitFunc->Draw("SAME");

				fitText = TPlotter::initLegend(plotConfig);
				TPlotter::saveFitLegend(fitText, fitFunc);
				fitText->SetBorderSize(0);
				fitText->SetFillStyle(4000);
				fitText->Draw();
				std::vector<double> range = TPlotter::getDoubleSetFromString(plotConfig.find("ROW_RANGE"));
				mHitmapSliceXMean->SetPoint(i, (range[0] + range[1]) / 2, fitFunc->GetParameter(1));
				mHitmapSliceXMean->SetPointError(i, (range[0] - range[1]) / 2, fitFunc->GetParError(1));
				mHitmapSliceXAmplitude->SetPoint(i, (range[0] + range[1]) / 2, fitFunc->GetParameter(0));
				mHitmapSliceXAmplitude->SetPointError(i, (range[0] - range[1]) / 2, fitFunc->GetParError(0));
			}

			TPlotter::saveCanvas(canvas, mOutputPath, plotConfig);
			delete canvas;
		}
		if ( mConfig.getConfig("HITMAP_SLICE_X").hasKey("MEAN_PLOT") ) {
			TCanvas* canvas = TPlotter::initCanvas(mConfig.getConfig("HITMAP_SLICE_X").getSubConfig("MEAN_PLOT"));
			TPlotter::drawPlot(canvas, mHitmapSliceXMean, mConfig.getConfig("HITMAP_SLICE_X").getSubConfig("MEAN_PLOT"));
			TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("HITMAP_SLICE_X").getSubConfig("MEAN_PLOT"));
			delete canvas;
		}
		if ( mConfig.getConfig("HITMAP_SLICE_X").hasKey("AMPLITUDE_PLOT") ) {
			TCanvas* canvas = TPlotter::initCanvas(mConfig.getConfig("HITMAP_SLICE_X").getSubConfig("AMPLITUDE_PLOT"));
			TPlotter::drawPlot(canvas, mHitmapSliceXAmplitude, mConfig.getConfig("HITMAP_SLICE_X").getSubConfig("AMPLITUDE_PLOT"));
			TF1* fitFunc = TPlotter::initFunction(mConfig.getConfig("HITMAP_SLICE_X").getSubConfig("AMPLITUDE_PLOT"));
			mHitmapSliceXAmplitude->Fit(fitFunc, "RQ");
			fitFunc->Draw("SAME");
			TLegend* fitText = TPlotter::initLegend(mConfig.getConfig("HITMAP_SLICE_X").getSubConfig("AMPLITUDE_PLOT"));
			TPlotter::saveFitLegend(fitText, fitFunc);
			fitText->SetBorderSize(0);
			fitText->SetFillStyle(4000);
			fitText->Draw();

			TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("HITMAP_SLICE_X").getSubConfig("AMPLITUDE_PLOT"));
			delete canvas;
		}
	}
	if ( isHitmapSliceY ) {
		int nPlot = mConfig.getConfig("HITMAP_SLICE_Y").getSubConfig("SUB_PLOTS").getSubConfigSet().size();
		for ( int i = 0; i < nPlot; i++ ) {
			CppConfigDictionary plotConfig = mConfig.getConfig("HITMAP_SLICE_Y").getSubConfig("SUB_PLOTS").getSubConfigSet()[i];

			TCanvas* canvas = TPlotter::initCanvas(plotConfig);
			TF1* fitFunc = TPlotter::initFunction(plotConfig);
			TPlotter::drawPlot(canvas, mHitmapSliceY[i], plotConfig);
			mHitmapSliceY[i]->Fit(fitFunc, "RQ");
			fitFunc->Draw("SAME");

			TLegend* fitText = TPlotter::initLegend(plotConfig);
			fitText->SetBorderSize(0);
			TPlotter::saveFitLegend(fitText, fitFunc);
			fitText->SetFillStyle(4000);
			fitText->Draw();

			std::vector<double>range = TPlotter::getDoubleSetFromString(plotConfig.find("COLUMN_RANGE"));
			mHitmapSliceYMean->SetPoint(i, (range[0] + range[1]) / 2, fitFunc->GetParameter(1));
			mHitmapSliceYMean->SetPointError(i, (range[0] - range[1]) / 2, fitFunc->GetParError(1));
			mHitmapSliceYAmplitude->SetPoint(i, (range[0] + range[1]) / 2, fitFunc->GetParameter(0));
			mHitmapSliceYAmplitude->SetPointError(i, (range[0] - range[1]) / 2, fitFunc->GetParError(0));

			TPlotter::saveCanvas(canvas, mOutputPath, plotConfig);
			delete canvas;
			delete fitFunc;
		}
		if ( mConfig.getConfig("HITMAP_SLICE_Y").hasKey("MEAN_PLOT") ) {
			TCanvas* canvas = TPlotter::initCanvas(mConfig.getConfig("HITMAP_SLICE_Y").getSubConfig("MEAN_PLOT"));
			TPlotter::drawPlot(canvas, mHitmapSliceYMean, mConfig.getConfig("HITMAP_SLICE_Y").getSubConfig("MEAN_PLOT"));
			TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("HITMAP_SLICE_Y").getSubConfig("MEAN_PLOT"));
			delete canvas;
		}
		if ( mConfig.getConfig("HITMAP_SLICE_Y").hasKey("AMPLITUDE_PLOT") ) {
			TCanvas* canvas = TPlotter::initCanvas(mConfig.getConfig("HITMAP_SLICE_Y").getSubConfig("AMPLITUDE_PLOT"));
			TPlotter::drawPlot(canvas, mHitmapSliceYAmplitude, mConfig.getConfig("HITMAP_SLICE_Y").getSubConfig("AMPLITUDE_PLOT"));
			TF1* fitFunc = TPlotter::initFunction(mConfig.getConfig("HITMAP_SLICE_Y").getSubConfig("AMPLITUDE_PLOT"));
			mHitmapSliceYAmplitude->Fit(fitFunc, "RQ");
			fitFunc->Draw("SAME");
			TLegend* fitText = TPlotter::initLegend(mConfig.getConfig("HITMAP_SLICE_Y").getSubConfig("AMPLITUDE_PLOT"));
			TPlotter::saveFitLegend(fitText, fitFunc);
			fitText->SetBorderSize(0);
			fitText->SetFillStyle(4000);
			fitText->Draw();

			TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("HITMAP_SLICE_Y").getSubConfig("AMPLITUDE_PLOT"));
			delete canvas;
		}
	}

	if ( isClustermapSliceX ) {
		int nPlot = mConfig.getConfig("CLUSTERMAP_SLICE_X").getSubConfig("SUB_PLOTS").getSubConfigSet().size();
		for ( int i = 0; i < nPlot; i++ ) {
			CppConfigDictionary plotConfig = mConfig.getConfig("CLUSTERMAP_SLICE_X").getSubConfig("SUB_PLOTS").getSubConfigSet()[i];

			TCanvas* canvas = TPlotter::initCanvas(plotConfig);
			mClustermapSliceX[i]->SetBinContent(259, (mClustermapSliceX[i]->GetBinContent(258) + mClustermapSliceX[i]->GetBinContent(260)) / 2);
			mClustermapSliceX[i]->SetBinContent(260, (mClustermapSliceX[i]->GetBinContent(259) + mClustermapSliceX[i]->GetBinContent(261)) / 2);

			TF1* fitFunc = TPlotter::initFunction(plotConfig);
			TPlotter::drawPlot(canvas, mClustermapSliceX[i], plotConfig);
			mClustermapSliceX[i]->Fit(fitFunc, "RQ");
			fitFunc->Draw("SAME");

			TLegend* fitText = TPlotter::initLegend(plotConfig);
			TPlotter::saveFitLegend(fitText, fitFunc);
			fitText->SetBorderSize(0);
			fitText->SetFillStyle(4000);
			fitText->Draw();

			std::vector<double> range = TPlotter::getDoubleSetFromString(plotConfig.find("ROW_RANGE"));
			mClustermapSliceXMean->SetPoint(i, (range[0] + range[1]) / 2, fitFunc->GetParameter(1));
			mClustermapSliceXMean->SetPointError(i, (range[0] - range[1]) / 2, fitFunc->GetParError(1));
			mClustermapSliceXAmplitude->SetPoint(i, (range[0] + range[1]) / 2, fitFunc->GetParameter(0));
			mClustermapSliceXAmplitude->SetPointError(i, (range[0] - range[1]) / 2, fitFunc->GetParError(0));

			TPlotter::saveCanvas(canvas, mOutputPath, plotConfig);
			delete canvas;
			delete fitFunc;
		}
		if ( mConfig.getConfig("CLUSTERMAP_SLICE_X").hasKey("MEAN_PLOT") ) {
			TCanvas* canvas = TPlotter::initCanvas(mConfig.getConfig("CLUSTERMAP_SLICE_X").getSubConfig("MEAN_PLOT"));
			TPlotter::drawPlot(canvas, mClustermapSliceXMean, mConfig.getConfig("CLUSTERMAP_SLICE_X").getSubConfig("MEAN_PLOT"));
			TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("CLUSTERMAP_SLICE_X").getSubConfig("MEAN_PLOT"));
			delete canvas;
		}
		if ( mConfig.getConfig("CLUSTERMAP_SLICE_X").hasKey("AMPLITUDE_PLOT") ) {
			TCanvas* canvas = TPlotter::initCanvas(mConfig.getConfig("CLUSTERMAP_SLICE_X").getSubConfig("AMPLITUDE_PLOT"));
			TPlotter::drawPlot(canvas, mClustermapSliceXAmplitude, mConfig.getConfig("CLUSTERMAP_SLICE_X").getSubConfig("AMPLITUDE_PLOT"));
			TF1* fitFunc = TPlotter::initFunction(mConfig.getConfig("CLUSTERMAP_SLICE_X").getSubConfig("AMPLITUDE_PLOT"));
			mClustermapSliceXAmplitude->Fit(fitFunc, "RQ");
			fitFunc->Draw("SAME");
			TLegend* fitText = TPlotter::initLegend(mConfig.getConfig("CLUSTERMAP_SLICE_X").getSubConfig("AMPLITUDE_PLOT"));
			TPlotter::saveFitLegend(fitText, fitFunc);
			fitText->SetBorderSize(0);
			fitText->SetFillStyle(4000);
			fitText->Draw();

			TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("CLUSTERMAP_SLICE_X").getSubConfig("AMPLITUDE_PLOT"));
			delete canvas;
		}
	}

	if ( isClustermapSliceY ) {
		int nPlot = mConfig.getConfig("CLUSTERMAP_SLICE_Y").getSubConfig("SUB_PLOTS").getSubConfigSet().size();
		for ( int i = 0; i < nPlot; i++ ) {
			CppConfigDictionary plotConfig = mConfig.getConfig("CLUSTERMAP_SLICE_Y").getSubConfig("SUB_PLOTS").getSubConfigSet()[i];

			TCanvas* canvas = TPlotter::initCanvas(plotConfig);
			TF1* fitFunc = TPlotter::initFunction(plotConfig);
			TPlotter::drawPlot(canvas, mClustermapSliceY[i], plotConfig);
			mClustermapSliceY[i]->Fit(fitFunc, "RQ");
			fitFunc->Draw("SAME");

			TLegend* fitText = TPlotter::initLegend(plotConfig);
			fitText->SetBorderSize(0);
			TPlotter::saveFitLegend(fitText, fitFunc);
			fitText->SetFillStyle(4000);
			fitText->Draw();

			std::vector<double>range = TPlotter::getDoubleSetFromString(plotConfig.find("COLUMN_RANGE"));
			mClustermapSliceYMean->SetPoint(i, (range[0] + range[1]) / 2, fitFunc->GetParameter(1));
			mClustermapSliceYMean->SetPointError(i, (range[0] - range[1]) / 2, fitFunc->GetParError(1));
			mClustermapSliceYAmplitude->SetPoint(i, (range[0] + range[1]) / 2, fitFunc->GetParameter(0));
			mClustermapSliceYAmplitude->SetPointError(i, (range[0] - range[1]) / 2, fitFunc->GetParError(0));

			TPlotter::saveCanvas(canvas, mOutputPath, plotConfig);
			delete canvas;
			delete fitText;
			delete fitFunc;
		}
		if ( mConfig.getConfig("CLUSTERMAP_SLICE_Y").hasKey("MEAN_PLOT") ) {
			TCanvas* canvas = TPlotter::initCanvas(mConfig.getConfig("CLUSTERMAP_SLICE_Y").getSubConfig("MEAN_PLOT"));
			TPlotter::drawPlot(canvas, mClustermapSliceYMean, mConfig.getConfig("CLUSTERMAP_SLICE_Y").getSubConfig("MEAN_PLOT"));
			TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("CLUSTERMAP_SLICE_Y").getSubConfig("MEAN_PLOT"));
			delete canvas;
		}
		if ( mConfig.getConfig("CLUSTERMAP_SLICE_Y").hasKey("AMPLITUDE_PLOT") ) {
			TCanvas* canvas = TPlotter::initCanvas(mConfig.getConfig("CLUSTERMAP_SLICE_Y").getSubConfig("AMPLITUDE_PLOT"));
			TPlotter::drawPlot(canvas, mClustermapSliceYAmplitude, mConfig.getConfig("CLUSTERMAP_SLICE_Y").getSubConfig("AMPLITUDE_PLOT"));
			TF1* fitFunc = TPlotter::initFunction(mConfig.getConfig("CLUSTERMAP_SLICE_Y").getSubConfig("AMPLITUDE_PLOT"));
			mClustermapSliceYAmplitude->Fit(fitFunc, "RQ");
			fitFunc->Draw("SAME");

			TLegend* fitText = TPlotter::initLegend(mConfig.getConfig("CLUSTERMAP_SLICE_Y").getSubConfig("AMPLITUDE_PLOT"));
			TPlotter::saveFitLegend(fitText, fitFunc);
			fitText->SetBorderSize(0);
			fitText->SetFillStyle(4000);
			fitText->Draw();

			TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("CLUSTERMAP_SLICE_Y").getSubConfig("AMPLITUDE_PLOT"));
			delete canvas;
		}
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

		TPad* pad = new TPad(Form("pad%d_%d", clusterSize, iClusterShape), "", (double) nominalHeader / (plotsWidth + nominalHeader) + ((double) plotsWidth / (plotsWidth + nominalHeader)) * ((double) iClusterShape / nWidth), ((double) plotsHeight / (plotsHeight + nominalHeader)) * (((double) nHeight - iClusterSize - 1) / nHeight), (double) nominalHeader / (plotsWidth + nominalHeader) + ((double) plotsWidth / (plotsWidth + nominalHeader)) * (double) (iClusterShape + 1) / nWidth, ((double) plotsHeight / (plotsHeight + nominalHeader)) * (((double) nHeight - iClusterSize) / nHeight), -1, 1);
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
	TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("TotalShape"));
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
	TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("Top10Shape"));
}

void TDataPlotter::saveClusterSizeWithTime() {
	TCanvas* canvas = new TCanvas("clusterSizeWithTime", "", 1000, 1000);
	TH2D* clusterSizeWithTime = TPlotter::init2DHist(mConfig.getConfig("ClusterSizeWithTime"));

	UInt_t timeStamp;
	UInt_t size;

	TTree* clusterTree = static_cast<TTree*>(mInputFile->Get("cluster"));
	clusterTree->SetBranchAddress("TimeStamp", &timeStamp);
	clusterTree->SetBranchAddress("Size", &size);

	for ( int i = 0; i < clusterTree->GetEntries(); i++ ) {
		clusterTree->GetEntry(i);
		clusterSizeWithTime->Fill(timeStamp, size);
	}

	TPlotter::drawPlot(canvas, clusterSizeWithTime, mConfig.getConfig("ClusterSizeWithTime"));

	TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("ClusterSizeWithTime"));
}

std::array<int, 60> TDataPlotter::getEntry() {
	std::array<int, 60> entry;
	for ( int i = 0; i < 60; i++ ) {
		entry[i] = mClustersize->GetBinContent(i + 1);
	}
	return entry;
}