#include "TCanvas.h"
#include "TAxis.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TPaveText.h"
#include "TLatex.h"
#include<utility>

std::pair<double, int> getEffectiveValueAndDigit(double value) {
	// std::cout << round(func->GetParError(0) * pow(10., -floor(log10(func->GetParError(0))))) / pow(10., -floor(log10(func->GetParError(0)))) << std::endl;
	int digit = -floor(log10(value));
	if ( floor(value * pow(10, digit)) == 1 ) {
		return {round(value * pow(10, digit + 1)) / pow(10, digit + 1), digit + 1};
	} else {
		return {floor(value * pow(10, digit)) / pow(10, digit), digit};
	}
}

TString EffectiveExpression(const double value, const double error) {
	std::pair<double, int> ErrorAndDigit = getEffectiveValueAndDigit(error);
	double correctedError = ErrorAndDigit.first;
	int digit = ErrorAndDigit.second;
	double correctedValue = round(value * pow(10, digit)) / pow(10, digit);
	int correctedDigit = floor(log10(correctedValue));
	correctedValue /= pow(10, correctedDigit);
	correctedError /= pow(10, correctedDigit);

	return Form("(%." + TString(std::to_string(correctedDigit + digit)) + "f #pm %.15g) #times 10^{%d}", correctedValue, correctedError, correctedDigit);
}

void graph_draw() {
	// Collimator hole
	std::vector<std::vector<double>>values = {{1450, 1270, 1170, 1253, 1130},
											  {19630, 19810, 19740, 19960},
											  {93800, 88310, 89400, 90050, 88950, 87260},
											  {196300, 198700, 204200, 197900, 202200, 193030},
											  {666500, 708000, 717200, 712500, 741279, 746400},
											  {2660000, 2500000, 2516000, 2536000, 2510000},
											  {2522000, 2518000, 2530000, 2555000},
											  {2620000, 2533000, 2550000, 2566000, 2532000, 2480000},
											  {2510000, 2517000, 2590000, 2514000, 2571000, 2450000},
											  {2510000, 2640000, 2680000, 2650000, 2740000, 2730000},
											  {0.0708, 0.066, 0.0604, 0.0642, 0.0585},
											  {1.011, 1.022, 1.013, 1.015},
											  {4.65, 4.53, 4.55, 4.56, 4.56, 4.57},
											  {10.16, 10.25, 10.24, 10.22, 10.21, 10.24},
											  {34.6, 34.8, 34.8, 34.9, 35.1, 35.5}};

	std::vector<std::vector<double>>errors = {{30, 20, 20, 18, 20},
											  {30, 30, 40, 30},
											  {300, 170, 60, 80, 40, 80},
											  {200, 300, 200, 300, 400, 130},
											  {300, 300, 400, 700, 600, 500},
											  {40000, 20000, 13000, 14000, 30000},
											  {8000, 11000, 10000, 14000, 30000},
											  {40000, 7000, 8000, 10000, 8000, 40000},
											  {20000, 14000, 30000, 7000, 12000, 40000},
											  {70000, 40000, 50000, 30000, 30000, 40000},
											  {0.0019, 0.0012, 0.0011, 0.0011, 0.0013},
											  {0.007, 0.008, 0.007, 0.008},
											  {0.08, 0.03, 0.03, 0.03, 0.03, 0.08},
											  {0.1, 0.09, 0.13, 0.07, 0.08, 0.18},
											  {0.9, 0.6, 0.7, 0.4, 0.4, 0.6}};

	std::vector<TString> titleSet = {
		"#phi 1 collimator; i-th photo; Pixel of collimator hole in photo",
		"#phi 2 collimator; i-th photo; Pixel of collimator hole in photo",
		"#phi 3 collimator; i-th photo; Pixel of collimator hole in photo",
		"#phi 4 collimator; i-th photo; Pixel of collimator hole in photo",
		"#phi 7 collimator; i-th photo; Pixel of collimator hole in photo",
		"#phi 1 collimator; i-th photo; Pixel of collimator case in photo",
		"#phi 2 collimator; i-th photo; Pixel of collimator case in photo",
		"#phi 3 collimator; i-th photo; Pixel of collimator case in photo",
		"#phi 4 collimator; i-th photo; Pixel of collimator case in photo",
		"#phi 7 collimator; i-th photo; Pixel of collimator case in photo",
		"#phi 1 collimator; i-th photo; Area of collimator hole",
		"#phi 2 collimator; i-th photo; Area of collimator hole",
		"#phi 3 collimator; i-th photo; Area of collimator hole",
		"#phi 4 collimator; i-th photo; Area of collimator hole",
		"#phi 7 collimator; i-th photo; Area of collimator hole"};

	std::vector<TString> pathSet = {
		"build/output/hole_size/phi1_collimator_hole_pixels.pdf",
		"build/output/hole_size/phi2_collimator_hole_pixels.pdf",
		"build/output/hole_size/phi3_collimator_hole_pixels.pdf",
		"build/output/hole_size/phi4_collimator_hole_pixels.pdf",
		"build/output/hole_size/phi7_collimator_hole_pixels.pdf",
		"build/output/hole_size/phi1_collimator_case_pixels.pdf",
		"build/output/hole_size/phi2_collimator_case_pixels.pdf",
		"build/output/hole_size/phi3_collimator_case_pixels.pdf",
		"build/output/hole_size/phi4_collimator_case_pixels.pdf",
		"build/output/hole_size/phi7_collimator_case_pixels.pdf",
		"build/output/hole_size/phi1_collimator_area.pdf",
		"build/output/hole_size/phi2_collimator_area.pdf",
		"build/output/hole_size/phi3_collimator_area.pdf",
		"build/output/hole_size/phi4_collimator_area.pdf",
		"build/output/hole_size/phi7_collimator_area.pdf"
	};

	for ( int iPlot = 0; iPlot < values.size(); iPlot++ ) {
		TCanvas* canvas = new TCanvas("can", "", 600, 600);
		TGraphErrors* graph = new TGraphErrors();

		double maxValue = 0.;
		double minValue = 10E+12;

		for ( int i = 0; i < values[iPlot].size(); i++ ) {
			maxValue = maxValue < values[iPlot][i] + errors[iPlot][i] ? values[iPlot][i] + errors[iPlot][i] : maxValue;
			minValue = minValue > values[iPlot][i] - errors[iPlot][i] ? values[iPlot][i] - errors[iPlot][i] : minValue;
			graph->SetPoint(i, i, values[iPlot][i]);
			graph->SetPointError(i, 0, errors[iPlot][i]);
		}

		graph->SetTitle(titleSet[iPlot]);
		graph->SetMarkerStyle(8);
		graph->GetXaxis()->SetNdivisions(values[iPlot].size() + 1, 0, 0, kTRUE);
		graph->GetXaxis()->SetLimits(-1, values[iPlot].size());
		graph->SetMaximum(maxValue + (maxValue - minValue) * .5);
		graph->SetMinimum(minValue - (maxValue - minValue) * .1);
		graph->Draw("AP");

		TF1* func = new TF1("func", "[0]", 0, values[iPlot].size() - 1);
		graph->Fit(func, "R");
		func->Draw("SAME");

		TPaveText* legend = new TPaveText(.3, .7, .9, .9, "NDC");
		legend->AddText("Fit parameter");
		legend->AddText("Value: " + EffectiveExpression(func->GetParameter(0), func->GetParError(0)));
		legend->AddText(Form("Chi2/NDoF: %f", sqrt(func->GetChisquare() / func->GetNDF())));
		legend->Draw();

		canvas->SetLeftMargin(0.15);
		canvas->SaveAs(pathSet[iPlot]);

		delete func;
		func = nullptr;
		delete graph;
		graph = nullptr;
		delete canvas;
		canvas = nullptr;
	}

	TGraphErrors* graph2 = new TGraphErrors();
	graph2->SetPoint(0, 0.0631, 0.0001415);
	graph2->SetPointError(0, 0.0005, 0.0000006);
	graph2->SetPoint(1, 1.015, 0.00708);
	graph2->SetPointError(1, 0.003, 0.00003);
	graph2->SetPoint(2, 4.554, 0.0525);
	graph2->SetPointError(2, 0.014, 0.0003);
	graph2->SetPoint(3, 10.22, 0.1404);
	graph2->SetPointError(3, 0.03, 0.0005);
	graph2->SetPoint(4, 35.0, 0.3343);
	graph2->SetPointError(4, 0.2, 0.0005);

	TCanvas* canvas2 = new TCanvas("can2", "", 600, 600);
	graph2->SetTitle("Experiment vs Simulation; Collimator hole width [mm^{2}]; Experiment / Simulation");
	graph2->SetMarkerStyle(8);
	graph2->SetMarkerSize(.1);
	graph2->SetMinimum(0);
	graph2->SetMaximum(.4);
	graph2->Draw("AP");
	canvas2->SetLeftMargin(.12);
	// canvas2->SetLogx();
	// canvas2->SetLogy();
	canvas2->SaveAs("build/output/hole_size/compare_exp_simul.pdf");
}