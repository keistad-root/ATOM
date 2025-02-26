#include<iostream>
#include<filesystem>
#include<tuple>

#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TText.h"
#include "TMath.h"
#include "cppargs.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TMultiGraph.h"

#include<csv.h>

int main(int argc, char** argv) {
	std::filesystem::path csvPath = "../Data/entry.csv";
	io::CSVReader<9> csv(csvPath);

	csv.read_header(io::ignore_extra_column, "Collimator", "Length", "Width", "Area", "Area_Error", "Region_A", "Region_B", "Region_C", "Region_D");

	std::string collimator;
	int length, width;
	double area, areaError;
	double a, b, c, d;

	TGraphErrors* l1AGraph = new TGraphErrors();
	TGraphErrors* l1BGraph = new TGraphErrors();
	TGraphErrors* l1CGraph = new TGraphErrors();
	TGraphErrors* l1DGraph = new TGraphErrors();
	TGraphErrors* l3AGraph = new TGraphErrors();
	TGraphErrors* l3BGraph = new TGraphErrors();
	TGraphErrors* l3CGraph = new TGraphErrors();
	TGraphErrors* l3DGraph = new TGraphErrors();
	TGraphErrors* l7AGraph = new TGraphErrors();
	TGraphErrors* l7BGraph = new TGraphErrors();
	TGraphErrors* l7CGraph = new TGraphErrors();
	TGraphErrors* l7DGraph = new TGraphErrors();
	TGraphErrors* l20AGraph = new TGraphErrors();
	TGraphErrors* l20BGraph = new TGraphErrors();
	TGraphErrors* l20CGraph = new TGraphErrors();
	TGraphErrors* l20DGraph = new TGraphErrors();

	while ( csv.read_row(collimator, length, width, area, areaError, a, b, c, d) ) {
		if ( length == 1 ) {
			l1AGraph->SetPoint(l1AGraph->GetN(), area, a);
			l1AGraph->SetPointError(l1AGraph->GetN() - 1, areaError, 0);
			l1BGraph->SetPoint(l1BGraph->GetN(), area, b);
			l1BGraph->SetPointError(l1BGraph->GetN() - 1, areaError, 0);
			l1CGraph->SetPoint(l1CGraph->GetN(), area, c);
			l1CGraph->SetPointError(l1CGraph->GetN() - 1, areaError, 0);
			l1DGraph->SetPoint(l1DGraph->GetN(), area, d);
			l1DGraph->SetPointError(l1DGraph->GetN() - 1, areaError, 0);
		} else if ( length == 3 ) {
			l3AGraph->SetPoint(l3AGraph->GetN(), area, a);
			l3AGraph->SetPointError(l3AGraph->GetN() - 1, areaError, 0);
			l3BGraph->SetPoint(l3BGraph->GetN(), area, b);
			l3BGraph->SetPointError(l3BGraph->GetN() - 1, areaError, 0);
			l3CGraph->SetPoint(l3CGraph->GetN(), area, c);
			l3CGraph->SetPointError(l3CGraph->GetN() - 1, areaError, 0);
			l3DGraph->SetPoint(l3DGraph->GetN(), area, d);
			l3DGraph->SetPointError(l3DGraph->GetN() - 1, areaError, 0);
		} else if ( length == 7 ) {
			l7AGraph->SetPoint(l7AGraph->GetN(), area, a);
			l7AGraph->SetPointError(l7AGraph->GetN() - 1, areaError, 0);
			l7BGraph->SetPoint(l7BGraph->GetN(), area, b);
			l7BGraph->SetPointError(l7BGraph->GetN() - 1, areaError, 0);
			l7CGraph->SetPoint(l7CGraph->GetN(), area, c);
			l7CGraph->SetPointError(l7CGraph->GetN() - 1, areaError, 0);
			l7DGraph->SetPoint(l7DGraph->GetN(), area, d);
			l7DGraph->SetPointError(l7DGraph->GetN() - 1, areaError, 0);
		} else if ( length == 20 ) {
			l20AGraph->SetPoint(l20AGraph->GetN(), area, a);
			l20AGraph->SetPointError(l20AGraph->GetN() - 1, areaError, 0);
			l20BGraph->SetPoint(l20BGraph->GetN(), area, b);
			l20BGraph->SetPointError(l20BGraph->GetN() - 1, areaError, 0);
			l20CGraph->SetPoint(l20CGraph->GetN(), area, c);
			l20CGraph->SetPointError(l20CGraph->GetN() - 1, areaError, 0);
			l20DGraph->SetPoint(l20DGraph->GetN(), area, d);
			l20DGraph->SetPointError(l20DGraph->GetN() - 1, areaError, 0);
		}
	}

	TCanvas* c1 = new TCanvas("aCanvas", "", 1000, 1000);
	TMultiGraph* aGraph = new TMultiGraph();
	l1AGraph->SetLineWidth(2);
	l1AGraph->SetLineColor(kRed);
	aGraph->Add(l1AGraph);
	l3AGraph->SetLineWidth(2);
	l3AGraph->SetLineColor(kBlue);
	aGraph->Add(l3AGraph);
	l7AGraph->SetLineWidth(2);
	l7AGraph->SetLineColor(kGreen);
	aGraph->Add(l7AGraph);
	l20AGraph->SetLineWidth(2);
	l20AGraph->SetLineColor(kOrange);
	aGraph->Add(l20AGraph);

	aGraph->SetMinimum(0);
	aGraph->SetMaximum(1);

	aGraph->Draw("APL");
	aGraph->SetTitle("Region A; Area[mm^{2}]; Collimator / No collimator");

	TLegend* legend = new TLegend(0.1, 0.7, 0.5, 0.9);
	legend->AddEntry(l1AGraph, "L=1mm", "l");
	legend->AddEntry(l3AGraph, "L=3mm", "l");
	legend->AddEntry(l7AGraph, "L=7mm", "l");
	legend->AddEntry(l20AGraph, "L=20mm", "l");
	legend->Draw("SAME");

	c1->SaveAs("RegionA.png");

	TCanvas* c2 = new TCanvas("bCanvas", "", 1000, 1000);
	TMultiGraph* bGraph = new TMultiGraph();
	l1BGraph->SetLineWidth(2);
	l1BGraph->SetLineColor(kRed);
	bGraph->Add(l1BGraph);
	l3BGraph->SetLineWidth(2);
	l3BGraph->SetLineColor(kBlue);
	bGraph->Add(l3BGraph);
	l7BGraph->SetLineWidth(2);
	l7BGraph->SetLineColor(kGreen);
	bGraph->Add(l7BGraph);
	l20BGraph->SetLineWidth(2);
	l20BGraph->SetLineColor(kOrange);
	bGraph->Add(l20BGraph);

	bGraph->SetMinimum(0);
	bGraph->SetMaximum(1);
	bGraph->Draw("APL");
	bGraph->SetTitle("Region B; Area[mm^{2}]; Collimator / No collimator");

	TLegend* legend2 = new TLegend(0.1, 0.7, 0.5, 0.9);
	legend2->AddEntry(l1BGraph, "L=1mm", "l");
	legend2->AddEntry(l3BGraph, "L=3mm", "l");
	legend2->AddEntry(l7BGraph, "L=7mm", "l");
	legend2->AddEntry(l20BGraph, "L=20mm", "l");
	legend2->Draw("SAME");

	c2->SaveAs("RegionB.png");

	TCanvas* c3 = new TCanvas("cCanvas", "", 1000, 1000);
	TMultiGraph* cGraph = new TMultiGraph();
	l1CGraph->SetLineWidth(2);
	l1CGraph->SetLineColor(kRed);
	cGraph->Add(l1CGraph);
	l3CGraph->SetLineWidth(2);
	l3CGraph->SetLineColor(kBlue);
	cGraph->Add(l3CGraph);
	l7CGraph->SetLineWidth(2);
	l7CGraph->SetLineColor(kGreen);
	cGraph->Add(l7CGraph);
	l20CGraph->SetLineWidth(2);
	l20CGraph->SetLineColor(kOrange);
	cGraph->Add(l20CGraph);

	cGraph->SetMinimum(0);
	cGraph->SetMaximum(1);
	cGraph->Draw("APL");
	cGraph->SetTitle("Region C; Area[mm^{2}]; Collimator / No collimator");

	TLegend* legend3 = new TLegend(0.1, 0.7, 0.5, 0.9);
	legend3->AddEntry(l1CGraph, "L=1mm", "l");
	legend3->AddEntry(l3CGraph, "L=3mm", "l");
	legend3->AddEntry(l7CGraph, "L=7mm", "l");
	legend3->AddEntry(l20CGraph, "L=20mm", "l");
	legend3->Draw("SAME");

	c3->SaveAs("RegionC.png");

	TCanvas* c4 = new TCanvas("dCanvas", "", 1000, 1000);
	TMultiGraph* dGraph = new TMultiGraph();
	l1DGraph->SetLineWidth(2);
	l1DGraph->SetLineColor(kRed);
	dGraph->Add(l1DGraph);
	l3DGraph->SetLineWidth(2);
	l3DGraph->SetLineColor(kBlue);
	dGraph->Add(l3DGraph);
	l7DGraph->SetLineWidth(2);
	l7DGraph->SetLineColor(kGreen);
	dGraph->Add(l7DGraph);
	l20DGraph->SetLineWidth(2);
	l20DGraph->SetLineColor(kOrange);
	dGraph->Add(l20DGraph);

	dGraph->SetMinimum(0);
	dGraph->SetMaximum(1);
	dGraph->Draw("APL");
	dGraph->SetTitle("Region D; Area[mm^{2}]; Collimator / No collimator");

	TLegend* legend4 = new TLegend(0.1, 0.7, 0.5, 0.9);
	legend4->AddEntry(l1DGraph, "L=1mm", "l");
	legend4->AddEntry(l3DGraph, "L=3mm", "l");
	legend4->AddEntry(l7DGraph, "L=7mm", "l");
	legend4->AddEntry(l20DGraph, "L=20mm", "l");
	legend4->Draw("SAME");

	c4->SaveAs("RegionD.png");

	TCanvas* c5 = new TCanvas("l1Canvas", "", 1000, 1000);
	TMultiGraph* l1Graph = new TMultiGraph();
	l1AGraph->SetLineWidth(2);
	l1AGraph->SetLineColor(kRed);
	l1Graph->Add(l1AGraph);
	l1BGraph->SetLineWidth(2);
	l1BGraph->SetLineColor(kBlue);
	l1Graph->Add(l1BGraph);
	l1CGraph->SetLineWidth(2);
	l1CGraph->SetLineColor(kGreen);
	l1Graph->Add(l1CGraph);
	l1DGraph->SetLineWidth(2);
	l1DGraph->SetLineColor(kOrange);
	l1Graph->Add(l1DGraph);

	l1Graph->SetMinimum(0);
	l1Graph->SetMaximum(1);
	l1Graph->Draw("APL");
	l1Graph->SetTitle("L=1mm; Area[mm^{2}]; Collimator / No collimator");

	TLegend* legend5 = new TLegend(0.1, 0.7, 0.5, 0.9);
	legend5->AddEntry(l1AGraph, "Region A", "l");
	legend5->AddEntry(l1BGraph, "Region B", "l");
	legend5->AddEntry(l1CGraph, "Region C", "l");
	legend5->AddEntry(l1DGraph, "Region D", "l");
	legend5->Draw("SAME");

	c5->SaveAs("L1.png");

	TCanvas* c6 = new TCanvas("l3Canvas", "", 1000, 1000);
	TMultiGraph* l3Graph = new TMultiGraph();
	l3AGraph->SetLineWidth(2);
	l3AGraph->SetLineColor(kRed);
	l3Graph->Add(l3AGraph);
	l3BGraph->SetLineWidth(2);
	l3BGraph->SetLineColor(kBlue);
	l3Graph->Add(l3BGraph);
	l3CGraph->SetLineWidth(2);
	l3CGraph->SetLineColor(kGreen);
	l3Graph->Add(l3CGraph);
	l3DGraph->SetLineWidth(2);
	l3DGraph->SetLineColor(kOrange);
	l3Graph->Add(l3DGraph);

	l3Graph->SetMinimum(0);
	l3Graph->SetMaximum(1);
	l3Graph->Draw("APL");
	l3Graph->SetTitle("L=3mm; Area[mm^{2}]; Collimator / No collimator");

	TLegend* legend6 = new TLegend(0.1, 0.7, 0.5, 0.9);
	legend6->AddEntry(l3AGraph, "Region A", "l");
	legend6->AddEntry(l3BGraph, "Region B", "l");
	legend6->AddEntry(l3CGraph, "Region C", "l");
	legend6->AddEntry(l3DGraph, "Region D", "l");
	legend6->Draw("SAME");

	c6->SaveAs("L3.png");

	TCanvas* c7 = new TCanvas("l7Canvas", "", 1000, 1000);
	TMultiGraph* l7Graph = new TMultiGraph();
	l7AGraph->SetLineWidth(2);
	l7AGraph->SetLineColor(kRed);
	l7Graph->Add(l7AGraph);
	l7BGraph->SetLineWidth(2);
	l7BGraph->SetLineColor(kBlue);
	l7Graph->Add(l7BGraph);
	l7CGraph->SetLineWidth(2);
	l7CGraph->SetLineColor(kGreen);
	l7Graph->Add(l7CGraph);
	l7DGraph->SetLineWidth(2);
	l7DGraph->SetLineColor(kOrange);
	l7Graph->Add(l7DGraph);

	l7Graph->SetMinimum(0);
	l7Graph->SetMaximum(1);
	l7Graph->Draw("APL");
	l7Graph->SetTitle("L=7mm; Area[mm^{2}]; Collimator / No collimator");

	TLegend* legend7 = new TLegend(0.1, 0.7, 0.5, 0.9);
	legend7->AddEntry(l7AGraph, "Region A", "l");
	legend7->AddEntry(l7BGraph, "Region B", "l");
	legend7->AddEntry(l7CGraph, "Region C", "l");
	legend7->AddEntry(l7DGraph, "Region D", "l");
	legend7->Draw("SAME");

	c7->SaveAs("L7.png");

	TCanvas* c8 = new TCanvas("l20Canvas", "", 1000, 1000);
	TMultiGraph* l20Graph = new TMultiGraph();
	l20AGraph->SetLineWidth(2);
	l20AGraph->SetLineColor(kRed);
	l20Graph->Add(l20AGraph);
	l20BGraph->SetLineWidth(2);
	l20BGraph->SetLineColor(kBlue);
	l20Graph->Add(l20BGraph);
	l20CGraph->SetLineWidth(2);
	l20CGraph->SetLineColor(kGreen);
	l20Graph->Add(l20CGraph);
	l20DGraph->SetLineWidth(2);
	l20DGraph->SetLineColor(kOrange);
	l20Graph->Add(l20DGraph);

	l20Graph->SetMinimum(0);
	l20Graph->SetMaximum(1);
	l20Graph->Draw("APL");
	l20Graph->SetTitle("L=20mm; Area[mm^{2}]; Collimator / No collimator");

	TLegend* legend8 = new TLegend(0.1, 0.7, 0.5, 0.9);
	legend8->AddEntry(l20AGraph, "Region A", "l");
	legend8->AddEntry(l20BGraph, "Region B", "l");
	legend8->AddEntry(l20CGraph, "Region C", "l");
	legend8->AddEntry(l20DGraph, "Region D", "l");
	legend8->Draw("SAME");

	c8->SaveAs("L20.png");

	return 0;
}