#include<iostream>
#include<filesystem>
#include<csv.h>
#include<vector>

#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TLegend.h"
#include<array>

int main(int argc, char* argv) {
	io::CSVReader<31> csv("/home/ychoi/ATOM/Data/incident_angle_and_entry.csv");
	csv.read_header(io::ignore_extra_column, "x_bin", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13",
					"14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28",
					"29", "30", "31", "32");
	int xBins;
	std::array<double, 30> yBins;
	TGraph* graph[30];
	for ( int i = 0; i < 30; i++ ) {
		graph[i] = new TGraph();
	}
	int num = 0;
	while ( csv.read_row(xBins, yBins[0], yBins[1], yBins[2], yBins[3], yBins[4], yBins[5], yBins[6], yBins[7],
		   yBins[8], yBins[9], yBins[10], yBins[11], yBins[12], yBins[13], yBins[14], yBins[15],
		   yBins[16], yBins[17], yBins[18], yBins[19], yBins[20], yBins[21], yBins[22], yBins[23],
		   yBins[24], yBins[25], yBins[26], yBins[27], yBins[28], yBins[29]) ) {
		for ( int i = 0; i < 30; i++ ) {
			graph[i]->SetPoint(num, xBins, yBins[i]);
		}
		num++;
	}
	for ( int i = 0; i < 30; i++ ) {
		TCanvas* canvas = new TCanvas("canvas", "", 1000, 1000);
		graph[i]->SetTitle(static_cast<TString>("Cluster Size " + std::to_string(i + 3) + "; Max Incident Angle; N_{cluster size = " + std::to_string(i + 3) + "} / N_{" + std::to_string(i + 1) + " < cluster size < " + std::to_string(i + 5) + " }"));
		graph[i]->Draw("AL");

		canvas->SaveAs(static_cast<TString>("incident_angle_and_entry_" + std::to_string(i) + ".png"));
		delete canvas;
	}

	TLegend* increaseLegend = new TLegend(0.15, 0.7, 0.45, 0.9);

	TMultiGraph* increaseGraphs = new TMultiGraph();
	graph[7]->SetLineColor(kRed);
	graph[7]->SetLineWidth(2);
	increaseLegend->AddEntry(graph[7], "Cluster Size 10", "l");
	increaseGraphs->Add(graph[7]);
	graph[10]->SetLineColor(kBlue);
	graph[10]->SetLineWidth(2);
	increaseLegend->AddEntry(graph[10], "Cluster Size 13", "l");
	increaseGraphs->Add(graph[10]);
	graph[15]->SetLineColor(kViolet);
	graph[15]->SetLineWidth(2);
	increaseLegend->AddEntry(graph[15], "Cluster Size 18", "l");
	increaseGraphs->Add(graph[15]);
	graph[19]->SetLineColor(kGreen);
	graph[19]->SetLineWidth(2);
	increaseLegend->AddEntry(graph[19], "Cluster Size 22", "l");
	increaseGraphs->Add(graph[19]);
	graph[23]->SetLineColor(kOrange);
	graph[23]->SetLineWidth(2);
	increaseLegend->AddEntry(graph[23], "Cluster Size 26", "l");
	increaseGraphs->Add(graph[23]);

	TCanvas* canvas = new TCanvas("canvas", "", 1000, 1000);
	increaseGraphs->SetMinimum(0.1);
	increaseGraphs->SetMaximum(0.35);
	increaseGraphs->Draw("AL");
	increaseGraphs->SetTitle("Increased Cases; Max Incident Angle; N_{cluster size = x} / N_{x - 2#leq cluster size#leq x + 2}");
	increaseLegend->Draw();
	canvas->SetGrid();
	canvas->SetLeftMargin(0.15);
	canvas->SaveAs("increaseTendency.pdf");
	delete canvas;

	TLegend* decreaseLegend = new TLegend(0.6, 0.6, 0.9, 0.9);
	TMultiGraph* decreaseGraphs = new TMultiGraph();
	graph[8]->SetLineColor(kRed);
	graph[8]->SetLineWidth(2);
	decreaseLegend->AddEntry(graph[8], "Cluster Size 11", "l");
	decreaseGraphs->Add(graph[8]);
	graph[9]->SetLineColor(kBlue);
	graph[9]->SetLineWidth(2);
	decreaseLegend->AddEntry(graph[9], "Cluster Size 12", "l");
	decreaseGraphs->Add(graph[9]);
	graph[12]->SetLineColor(kViolet);
	graph[12]->SetLineWidth(2);
	decreaseLegend->AddEntry(graph[12], "Cluster Size 15", "l");
	decreaseGraphs->Add(graph[12]);
	graph[17]->SetLineColor(kGreen);
	graph[17]->SetLineWidth(2);
	decreaseLegend->AddEntry(graph[17], "Cluster Size 20", "l");
	decreaseGraphs->Add(graph[17]);
	graph[21]->SetLineColor(kOrange);
	graph[21]->SetLineWidth(2);
	decreaseLegend->AddEntry(graph[21], "Cluster Size 24", "l");
	decreaseGraphs->Add(graph[21]);
	graph[24]->SetLineColor(kYellow);
	graph[24]->SetLineWidth(2);
	decreaseLegend->AddEntry(graph[24], "Cluster Size 27", "l");
	decreaseGraphs->Add(graph[24]);
	graph[25]->SetLineColor(kBlack);
	graph[25]->SetLineWidth(2);
	decreaseLegend->AddEntry(graph[25], "Cluster Size 28", "l");
	decreaseGraphs->Add(graph[25]);

	canvas = new TCanvas("canvas", "", 1000, 1000);
	decreaseGraphs->SetMinimum(0.1);
	decreaseGraphs->SetMaximum(0.35);
	decreaseGraphs->SetTitle("Decreased Cases; Max Incident Angle; N_{cluster size = x} / N_{x - 2#leq cluster size#leq x + 2}");
	decreaseGraphs->Draw("AL");
	decreaseLegend->Draw();
	canvas->SetLeftMargin(0.15);
	canvas->SetGrid();
	canvas->SaveAs("decreaseTendency.pdf");
	delete canvas;

	return 0;
}