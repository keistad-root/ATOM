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
	TMultiGraph* mg = new TMultiGraph();
	while ( csv.read_row(xBins, yBins[0], yBins[1], yBins[2], yBins[3], yBins[4], yBins[5], yBins[6], yBins[7],
		   yBins[8], yBins[9], yBins[10], yBins[11], yBins[12], yBins[13], yBins[14], yBins[15],
		   yBins[16], yBins[17], yBins[18], yBins[19], yBins[20], yBins[21], yBins[22], yBins[23],
		   yBins[24], yBins[25], yBins[26], yBins[27], yBins[28], yBins[29]) ) {
		TGraph* graph = new TGraph();
		for ( int i = 0; i < 30; i++ ) {
			graph->SetPoint(i, i + 3, yBins[i]);
		}
		mg->Add(graph);
	}
	TCanvas* canvas = new TCanvas("canvas", "", 1500, 1000);
	mg->Draw("AL");
	canvas->SaveAs("incident_angle_and_entry.png");

	return 0;
}