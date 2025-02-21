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

const int ALPIDECOLUMN = 1024;
const int ALPIDEROW = 512;

TDataPlotter::TDataPlotter(const CppConfigFile& config) : mConfig(config) {
	mOutputPath = mConfig.getConfig("CONFIG").find("OUTPUT_DIRECTORY");
	std::filesystem::create_directories(mOutputPath);

	gStyle->SetOptStat(0);

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
		mHitmap = new TH2D("hitmap", "Hitmap; Column; Row", ALPIDECOLUMN, 0, ALPIDECOLUMN, ALPIDEROW, 0, ALPIDEROW);
	}
	if ( isHitmapProjectionX ) {
		mHitmapProjectionX = new TH1D("hitmapProjectionX", "Hitmap Projection X; Column; Entry", ALPIDECOLUMN / 2, 0, ALPIDECOLUMN);
	}
	if ( isHitmapProjectionY ) {
		mHitmapProjectionY = new TH1D("hitmapProjectionY", "Hitmap Projection Y; Row; Entry", ALPIDEROW / 2, 0, ALPIDEROW);
	}
	if ( isClustermap ) {
		mClustermap = new TH2D("clustermap", "Clustermap; Column; Row", ALPIDECOLUMN, 0, ALPIDECOLUMN, ALPIDEROW, 0, ALPIDEROW);
	}
	if ( isClustermapProjectionX ) {
		mClustermapProjectionX = new TH1D("clustermapProjectionX", "Clustermap Projection X; Column; Row", ALPIDECOLUMN / 4, 0, ALPIDECOLUMN);
	}
	if ( isClustermapProjectionY ) {
		mClustermapProjectionY = new TH1D("clustermapProjectionY", "Clustermap Projection Y; Column; Row", ALPIDEROW / 4, 0, ALPIDEROW);
	}
	if ( isClustersize ) {
		mClustersize = new TH1D("clustersize", "Cluster size; Cluster size; Entry", 100, .5, 100.5);
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
	TH1D* clustermapSliceX[11];
	TH1D* clustermapSliceY[11];
	for ( int i = 0; i < 11; i++ ) {
		clustermapSliceX[i] = new TH1D(Form("clustermapSliceX_%d", i), "", ALPIDECOLUMN / 2, 0, ALPIDECOLUMN);
		clustermapSliceY[i] = new TH1D(Form("clustermapSliceY_%d", i), "", ALPIDEROW / 2, 0, ALPIDEROW);
	}
	for ( int iCluster = 0; iCluster < nCluster; iCluster++ ) {
		clusterTree->GetEntry(iCluster);
		if ( isClustermap ) mClustermap->Fill(x, y);
		if ( isClustersize ) mClustersize->Fill(size);
		if ( isClustermapProjectionX ) mClustermapProjectionX->Fill(x);
		if ( isClustermapProjectionY ) mClustermapProjectionY->Fill(y);

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
		if ( isClustermapSliceX ) {
			for ( int i = 0; i < 11; i++ ) {
				if ( std::abs(y - 300 + 50 - 10 * i) < 5 ) {
					clustermapSliceX[i]->Fill(x);
				}
			}
		}
		if ( isClustermapSliceY ) {
			for ( int i = 0; i < 11; i++ ) {
				if ( std::abs(x - 500 + 50 - 10 * i) < 5 ) {
					clustermapSliceY[i]->Fill(y);
				}
			}
		}
	}
	TGraphErrors* graphX = new TGraphErrors();
	TGraphErrors* graphAmplitudeX = new TGraphErrors();
	TGraphErrors* graphY = new TGraphErrors();
	TGraphErrors* graphAmplitudeY = new TGraphErrors();
	for ( int i = 0; i < 11; i++ ) {
		TCanvas* canvas = new TCanvas(Form("sliceX_%d", i), "", 2000, 1000);
		clustermapSliceX[i]->SetBinContent(259, (clustermapSliceX[i]->GetBinContent(257) + clustermapSliceX[i]->GetBinContent(261)) / 2);
		clustermapSliceX[i]->SetBinContent(260, (clustermapSliceX[i]->GetBinContent(258) + clustermapSliceX[i]->GetBinContent(262)) / 2);
		TF1* fitFunc = new TF1(Form("sliceXFitFunc_%d", i), "[0]*e^(-((x-[1])/[2])^2)+[3]", 0, ALPIDECOLUMN);
		fitFunc->SetParameters(clustermapSliceX[i]->GetMaximum(), clustermapSliceX[i]->GetMean(), clustermapSliceX[i]->GetStdDev() / 10, clustermapSliceX[i]->GetMinimum());
		clustermapSliceX[i]->Fit(fitFunc, "R");
		clustermapSliceX[i]->SetTitle(Form("Row: %d#pm10; Column; Entry", 300 - 50 + (10 * i)));
		clustermapSliceX[i]->Draw();
		canvas->SaveAs(Form("Plot/sliceX_%d.png", i));
		delete canvas;
		graphX->SetPoint(i, 300 - 50 + (10 * i), fitFunc->GetParameter(1));
		graphX->SetPointError(i, 5, fitFunc->GetParError(1));
		graphAmplitudeX->SetPoint(i, 300 - 50 + (10 * i), fitFunc->GetParameter(0));
		graphAmplitudeX->SetPointError(i, 5, fitFunc->GetParError(0));
		delete fitFunc;

		TCanvas* canvas2 = new TCanvas(Form("sliceY_%d", i), "", 2000, 1000);
		TF1* fitFunc2 = new TF1(Form("sliceYFitFunc_%d", i), "[0]*e^(-((x-[1])/[2])^2)+[3]", 0, ALPIDEROW);
		fitFunc2->SetParameters(clustermapSliceY[i]->GetMaximum(), clustermapSliceY[i]->GetMean(), clustermapSliceY[i]->GetStdDev() / 10, clustermapSliceY[i]->GetMinimum());
		clustermapSliceY[i]->Fit(fitFunc2, "R");
		clustermapSliceY[i]->SetTitle(Form("Column: %d#pm10; Row; Entry", 500 - 50 + (10 * i)));
		clustermapSliceY[i]->Draw();
		canvas2->SaveAs(Form("Plot/sliceY_%d.png", i));
		delete canvas2;
		graphY->SetPoint(i, 500 - 50 + (10 * i), fitFunc2->GetParameter(1));
		graphY->SetPointError(i, 5, fitFunc2->GetParError(1));
		graphAmplitudeY->SetPoint(i, 500 - 50 + (10 * i), fitFunc2->GetParameter(0));
		graphAmplitudeY->SetPointError(i, 5, fitFunc2->GetParError(0));
		delete fitFunc2;
	}
	TCanvas* canvas = new TCanvas("sliceX", "", 1000, 1000);
	graphX->SetMarkerStyle(24);
	graphX->SetMarkerSize(2);
	graphX->SetTitle("Slicing in Row Direction; Row; Mean of Gaussian Fit");
	graphX->Draw("AP");
	canvas->SaveAs("Plot/sliceX.png");
	delete canvas;

	canvas = new TCanvas("sliceAmplitudeX", "", 1000, 1000);
	graphAmplitudeX->SetMarkerStyle(24);
	graphAmplitudeX->SetMarkerSize(2);
	graphAmplitudeX->SetMaximum(200);
	graphAmplitudeX->SetMinimum(0);
	graphAmplitudeX->SetTitle("Slicing in Row Direction; Row; Amplitude of Gaussian Fit");
	graphAmplitudeX->Draw("AP");
	canvas->SaveAs("Plot/amplitudeX.png");
	delete canvas;

	canvas = new TCanvas("sliceY", "", 1000, 1000);
	graphY->SetMarkerStyle(24);
	graphY->SetMarkerSize(2);
	graphY->SetTitle("Slicing in Column Direction; Column; Mean of Gaussian Fit");
	graphY->Draw("AP");
	canvas->SaveAs("Plot/sliceY.png");
	delete canvas;

	canvas = new TCanvas("sliceAmplitudeY", "", 1000, 1000);
	graphAmplitudeY->SetMarkerStyle(24);
	graphAmplitudeY->SetMarkerSize(2);
	graphAmplitudeY->SetMaximum(200);
	graphAmplitudeY->SetMinimum(0);
	graphAmplitudeY->SetTitle("Slicing in Column Direction; Column; Amplitude of Gaussian Fit");
	graphAmplitudeY->Draw("AP");
	canvas->SaveAs("Plot/amplitudeY.png");
	delete canvas;
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
		TPlotter::drawPlot(canvas, mHitmap, mConfig.getConfig("HITMAP"), " ");
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
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("HITMAP"));
		delete canvas;
		canvas = nullptr;
	}
	if ( isHitmapProjectionX ) {
		TCanvas* canvas = new TCanvas("hitmapProjectionXCanvas", "", 3000, 1500);
		mHitmapProjectionX->SetBinContent(259, (mHitmapProjectionX->GetBinContent(258) + mHitmapProjectionX->GetBinContent(260)) / 2);
		// mHitmapProjectionX->SetBinContent(517, (mHitmapProjectionX->GetBinContent(515) + mHitmapProjectionX->GetBinContent(519)) / 2);
		// mHitmapProjectionX->SetBinContent(518, (mHitmapProjectionX->GetBinContent(516) + mHitmapProjectionX->GetBinContent(520)) / 2);
		TF1* fitFunc = new TF1("fitFunc", "[0]*e^(-((x-[1])/[2])^[3])", 0, ALPIDECOLUMN);
		fitFunc->SetParameters(1000, 500, 100, 2);
		mHitmapProjectionX->Fit(fitFunc, "RQ");

		TPlotter::drawPlot(canvas, mHitmapProjectionX, mConfig.getConfig("HITMAP_PROJECTION_X"), "HISTE");
		TText* text = new TText(0.5, 0.5, Form("Mean: %.2f", fitFunc->GetParameter(1)));
		text->SetNDC();
		text->SetTextAlign(22);
		text->Draw("SAME");
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("HITMAP_PROJECTION_X"));
		delete canvas;
		canvas = nullptr;
	}
	if ( isHitmapProjectionY ) {
		TCanvas* canvas = new TCanvas("hitmapProjectionYCanvas", "", 3000, 1500);
		TPlotter::initCanvas(canvas, mConfig.getConfig("HITMAP_PROJECTION_Y"));
		TF1* fitFunc = new TF1("fitFunc", "[0]*e^(-((x-[1])/[2])^2)", 0, ALPIDEROW);
		fitFunc->SetParameters(1000, 250, 50);
		mHitmapProjectionY->Fit(fitFunc, "RQ");
		TText* text = new TText(0.5, 0.5, Form("Mean: %.2f", fitFunc->GetParameter(1)));
		text->SetNDC();
		text->SetTextAlign(22);
		text->Draw("SAME");
		TPlotter::drawPlot(canvas, mHitmapProjectionY, mConfig.getConfig("HITMAP_PROJECTION_Y"), "HISTE");
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("HITMAP_PROJECTION_Y"));
		delete canvas;
		canvas = nullptr;
	}
	if ( isClustermapProjectionX ) {
		TCanvas* canvas = new TCanvas("clustermapProjectionXCanvas", "", 3000, 1500);
		std::vector<int> center = TPlotter::getIntegerSetFromString(mConfig.getConfig("CLUSTERSIZE_REGION").find("center"));
		mClustermapProjectionX->SetBinContent(130, (mClustermapProjectionX->GetBinContent(128) + mClustermapProjectionX->GetBinContent(132)) / 2);
		mClustermapProjectionX->SetBinContent(129, (mClustermapProjectionX->GetBinContent(127) + mClustermapProjectionX->GetBinContent(131)) / 2);
		mClustermapProjectionX->SetMinimum(0);

		TF1* fitFunc = new TF1("fitFunc", "[0]*e^(-((x-[1])/[2])^2)+[3]", mClustermapProjectionX->GetMean() - 150, mClustermapProjectionX->GetMean() + 150);
		fitFunc->SetParLimits(0, 0, mClustermapProjectionX->GetMaximum() * 10);
		fitFunc->SetParameter(1, mClustermapProjectionX->GetMean());
		fitFunc->SetParameter(2, mClustermapProjectionX->GetStdDev());
		fitFunc->SetParameter(3, mClustermapProjectionX->GetMinimum());
		mClustermapProjectionX->Fit(fitFunc, "R");

		TPaveText* latex = new TPaveText(.8, .5, .95, .95, "NDC");
		latex->AddText(Form("[0]: %.0f #pm %.0f", fitFunc->GetParameter(0), fitFunc->GetParError(0)));
		latex->AddText(Form("[1]: %.0f #pm %.0f", fitFunc->GetParameter(1), fitFunc->GetParError(1)));
		latex->AddText(Form("[2]: %.0f #pm %.0f", fitFunc->GetParameter(2), fitFunc->GetParError(2)));
		latex->AddText(Form("[3]: %.0f #pm %.0f", fitFunc->GetParameter(3), fitFunc->GetParError(3)));
		latex->AddText(Form("#chi^{2}/NDoF: %.2f", fitFunc->GetChisquare() / fitFunc->GetNDF()));
		latex->SetLabel("Fit parameters");

		TPlotter::drawPlot(canvas, mClustermapProjectionX, mConfig.getConfig("CLUSTERMAP_PROJECTION_X"), " ");
		latex->Draw();
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("CLUSTERMAP_PROJECTION_X"));
		delete canvas;
		delete fitFunc;
	}
	if ( isClustermapProjectionY ) {
		TCanvas* canvas = new TCanvas("clustermapProjectionYCanvas", "", 3000, 1500);
		std::vector<int> center = TPlotter::getIntegerSetFromString(mConfig.getConfig("CLUSTERSIZE_REGION").find("center"));
		mClustermapProjectionY->SetMinimum(0);

		TF1* fitFunc = new TF1("fitFunc", "[0]*e^(-((x-[1])/[2])^2)+[3]", mClustermapProjectionY->GetMean() - 150, mClustermapProjectionY->GetMean() + 150);
		fitFunc->SetParLimits(0, 0, mClustermapProjectionY->GetMaximum() * 10);
		fitFunc->SetParameter(1, mClustermapProjectionY->GetMean());
		fitFunc->SetParameter(2, mClustermapProjectionY->GetStdDev());
		fitFunc->SetParameter(3, mClustermapProjectionY->GetMinimum());
		mClustermapProjectionY->Fit(fitFunc, "R");

		TPaveText* latex = new TPaveText(.8, .5, .95, .95, "NDC");
		latex->AddText(Form("[0]: %.0f #pm %.0f", fitFunc->GetParameter(0), fitFunc->GetParError(0)));
		latex->AddText(Form("[1]: %.0f #pm %.0f", fitFunc->GetParameter(1), fitFunc->GetParError(1)));
		latex->AddText(Form("[2]: %.0f #pm %.0f", fitFunc->GetParameter(2), fitFunc->GetParError(2)));
		latex->AddText(Form("[3]: %.0f #pm %.0f", fitFunc->GetParameter(3), fitFunc->GetParError(3)));
		latex->AddText(Form("#chi^{2}/NDoF: %.2f", fitFunc->GetChisquare() / fitFunc->GetNDF()));
		latex->SetLabel("Fit parameters");

		TPlotter::drawPlot(canvas, mClustermapProjectionY, mConfig.getConfig("CLUSTERMAP_PROJECTION_Y"), " ");
		latex->Draw();
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("CLUSTERMAP_PROJECTION_Y"));
		delete canvas;
		delete fitFunc;
	}
	if ( isClustermap ) {
		TCanvas* canvas = new TCanvas("clustermapCanvas", "", 3000, 1500);
		TPlotter::drawPlot(canvas, mClustermap, mConfig.getConfig("CLUSTERMAP"), "COLZ");
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
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("CLUSTERMAP"));
		delete canvas;
		canvas = nullptr;
	}
	if ( isClustersize ) {
		TCanvas* canvas = new TCanvas("clustersizeCanvas", "", 3000, 1500);
		TPlotter::drawPlot(canvas, mClustersize, mConfig.getConfig("CLUSTERSIZE"), "HISTE");
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("CLUSTERSIZE"));
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
		TPlotter::drawPlot(canvas, mClusterSizeOfRegion[0], mConfig.getConfig("CLUSTERSIZE_REGION"), "HISTE");
		mClusterSizeOfRegion[1]->Draw("SAME");
		mClusterSizeOfRegion[2]->Draw("SAME");
		mClusterSizeOfRegion[3]->Draw("SAME");
		mClustersize->Draw("SAME");
		TPlotter::saveLegend(canvas, legend);
		TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("CLUSTERSIZE_REGION"));
		delete canvas;
		canvas = nullptr;
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
	TH2D* clusterSizeWithTime = new TH2D();
	TPlotter::initPlot(clusterSizeWithTime, mConfig.getConfig("ClusterSizeWithTime"));

	UInt_t timeStamp;
	UInt_t size;

	TTree* clusterTree = static_cast<TTree*>(mInputFile->Get("cluster"));
	clusterTree->SetBranchAddress("TimeStamp", &timeStamp);
	clusterTree->SetBranchAddress("Size", &size);

	for ( int i = 0; i < clusterTree->GetEntries(); i++ ) {
		clusterTree->GetEntry(i);
		clusterSizeWithTime->Fill(timeStamp, size);
	}

	TPlotter::drawPlot(canvas, clusterSizeWithTime, mConfig.getConfig("ClusterSizeWithTime"), " ");

	TPlotter::saveCanvas(canvas, mOutputPath, mConfig.getConfig("ClusterSizeWithTime"));
}

std::array<int, 60> TDataPlotter::getEntry() {
	std::array<int, 60> entry;
	for ( int i = 0; i < 60; i++ ) {
		entry[i] = mClustersize->GetBinContent(i + 1);
	}
	return entry;
}