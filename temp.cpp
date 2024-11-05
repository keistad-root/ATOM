#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLegend.h"

void temp() {
	double simulation[8] = {
		100,
		3000,
		40000,
		100000,
		220000,
		100000,
		37000,
		6100
	};
	double simulationError[8] = {
		80,
		1300,
		14000,
		40000,
		70000,
		40000,
		15000,
		1800
	};
	double measurementError[8] = {
		0.8,
		17,
		110,
		300,
		500,
		200,
		110,
		17
	};
	double measurement[8] = {
		76.3,
		4136,
		33010,
		95600,
		223800,
		112800,
		35400,
		5157
	};

	TGraphErrors* simulationGraph = new TGraphErrors();
	TGraphErrors* experimentGraph = new TGraphErrors();

	for ( Int_t i = 0; i < 8; i++ ) {
		simulationGraph->SetPoint(i, i + 1, simulation[i]);
		simulationGraph->SetPointError(i, 0, simulationError[i]);
		experimentGraph->SetPoint(i, i + 1, measurement[i]);
		experimentGraph->SetPointError(i, 0, measurementError[i]);
	}

	TCanvas* canvas = new TCanvas("canvas", "", 2000, 1000);
	simulationGraph->SetLineColor(kRed);
	simulationGraph->SetLineWidth(3);
	simulationGraph->SetTitle("Silver ALPIDE Entry Comparison (Simulation VS Experiment); Collimator; Entry");
	simulationGraph->Draw();
	experimentGraph->SetLineColor(kBlue);
	experimentGraph->SetLineWidth(3);
	experimentGraph->Draw("SAME");

	TLegend* legend = new TLegend(0.1, 0.7, 0.3, 0.9, "Graph");
	legend->SetHeader("Graph", "C");
	legend->AddEntry(simulationGraph, "Simulation", "l");
	legend->AddEntry(experimentGraph, "Experiment", "l");
	legend->Draw();

	canvas->SaveAs("/mnt/homes/ychoi/PIDproject/plots/final/entry_test/clustersize_total/compareGraph.png");
}