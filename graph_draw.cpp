#include "TCanvas.h"
#include "TAxis.h"
#include "TGraphErrors.h"
#include "TF1.h"

// Collimator hole
// double value[ ] = {1450,1270,1170,1253,1130,1460};
// double value[ ] = {21620,19630,19810,19740,19960,20500};
// double value[ ] = {93800,88310,89400,90050,88950,87260};
// double value[ ] = {196300,198700,204200,197900,202200,193030};
// double error[ ] = {30,20,20,18,20,140};
// double error[ ] = {10,30,30,40,30,300};
// double error[ ] = {300,170,60,80,40,80};
// double error[ ] = {200,300,200,300,400,130};

// Case
// double value[ ] = {2660000,2500000,2516000,2536000,2510000,2610000};
// double value[ ] = {2680000,2522000,2518000,2530000,2555000,2590000};
// double value[ ] = {2620000,2533000,2550000,2566000,2532000,2480000};
// double value[ ] = {2510000,2517000,2590000,2514000,2571000,2450000};
// double error[ ] = {40000,20000,13000,14000,30000,30000};
// double error[ ] = {30000,8000,11000,10000,14000,30000};
// double error[ ] = {40000,7000,8000,10000,8000,40000};
// double error[ ] = {20000,14000,30000,7000,12000,40000};

// Area
// double value[ ] = {0.0708,0.066,0.0604,0.0642,0.0585};
// double value[ ] = {1.048,1.011,1.022,1.013,1.015};
// double value[ ] = {4.65,4.53,4.55,4.56,4.56,4.57};
// double value[ ] = {10.16,10.25,10.24,10.22,10.21,10.24};
// double error[ ] = {0.0019,0.0012,0.0011,0.0011,0.0013};
// double error[ ] = {0.013,0.007,0.008,0.007,0.008};
// double error[ ] = {0.08,0.03,0.03,0.03,0.03,0.08};
// double error[ ] = {0.1,0.09,0.13,0.07,0.08,0.18};

void graph_draw() {
	// double value[ ] = {0.0708, 0.066, 0.0604, 0.0642, 0.0585};
	// double value[ ] = {1.048, 1.011, 1.022, 1.013, 1.015};
	// double value[ ] = {4.65, 4.53, 4.55, 4.56, 4.56, 4.57};
	double value[ ] = {10.16, 10.25, 10.24, 10.22, 10.21, 10.24};
	// double error[ ] = {0.0019, 0.0012, 0.0011, 0.0011, 0.0013};
	// double error[ ] = {0.013, 0.007, 0.008, 0.007, 0.008};
	// double error[ ] = {0.08, 0.03, 0.03, 0.03, 0.03, 0.08};
	double error[ ] = {0.1, 0.09, 0.13, 0.07, 0.08, 0.18};


	TCanvas* canvas = new TCanvas("can", "", 600, 600);

	TGraphErrors* graph = new TGraphErrors();
	for ( int i = 0; i < sizeof(value) / sizeof(*value); i++ ) {
		graph->SetPoint(i, i, value[i]);
		graph->SetPointError(i, 0, error[i]);
	}
	// graph->SetTitle("Phi-1; i-th photo; Photo-Pixel");
	// graph->SetTitle("Phi-2; i-th photo; Photo-Pixel");
	// graph->SetTitle("Phi-3; i-th photo; Photo-Pixel");
	graph->SetTitle("Phi-4; i-th photo; Photo-Pixel");
	// graph->SetTitle("Phi-1; i-th photo; Area[mm^2]");
	// graph->SetTitle("Phi-2; i-th photo; Area[mm^2]");
	// graph->SetTitle("Phi-3; i-th photo; Area[mm^2]");
	// graph->SetTitle("Phi-4; i-th photo; Area[mm^2]");
	graph->SetMarkerStyle(8);
	graph->GetXaxis()->SetLimits(-.5, 5.5);
	graph->Draw("AP");

	TF1* func = new TF1("func", "[0]", -.5, 5.5);
	graph->Fit(func, "R");
	func->Draw("SAME");

	canvas->SetLeftMargin(0.15);
	canvas->SaveAs("graph.pdf");
}