#include "TGraph.h"
#include "TF1.h"
#include "TCanvas.h"

int main() {
	Double_t width[11] = {0.82, 0.85, 0.88, 0.91, 0.94, 0.97, 1.00, 1.03, 1.06, 1.09, 1.12};
	Double_t Double[11] = {0, 1, 1, 0, 1, 0, 0, 2, 1, 0, 3};
	Double_t EAM[11] = {720, 785, 805, 846, 962, 932, 1056, 1067, 1047, 1134, 1226};
	Double_t EEM[11] = {260, 236, 267, 270, 298, 309, 312, 290, 328, 320, 313};
	Double_t EAE[11] = {626, 678, 699, 738, 846, 807, 894, 923, 931, 975, 1077};

	TGraph* doubleGraph = new TGraph();
	TGraph* eamGraph = new TGraph();
	TGraph* eemGraph = new TGraph();
	TGraph* eaeGraph = new TGraph();

	for ( int i = 0; i < 11; i++ ) {
		doubleGraph->SetPoint(i, width[i], Double[i]);
		eamGraph->SetPoint(i, width[i], EAM[i]);
		eemGraph->SetPoint(i, width[i], EEM[i]);
		eaeGraph->SetPoint(i, width[i], EAE[i]);
	}

	TF1* fitFunc[4] = {new TF1("fitFunc1", "[0]*x+[1]", 0.8, 1.2), new TF1("fitFunc2", "[0]*x+[1]", 0.8, 1.2), new TF1("fitFunc3", "[0]*x+[1]", 0.8, 1.2), new TF1("fitFunc4", "[0]*x+[1]", 0.8, 1.2)};

	doubleGraph->Fit(fitFunc[0], "R");
	eamGraph->Fit(fitFunc[1], "R");
	eemGraph->Fit(fitFunc[2], "R");
	eaeGraph->Fit(fitFunc[3], "R");

	TCanvas* canvas = new TCanvas("doubleCanvas", "", 1000, 1000);
	doubleGraph->Draw("APL");
	canvas->SaveAs("doubleGraph.png");
	TCanvas* canvas2 = new TCanvas("eamCanvas", "", 1000, 1000);
	eamGraph->Draw("APL");
	canvas2->SaveAs("eamGraph.png");
	TCanvas* canvas3 = new TCanvas("eemCanvas", "", 1000, 1000);
	eemGraph->Draw("APL");
	canvas3->SaveAs("eemGraph.png");
	TCanvas* canvas4 = new TCanvas("eaeCanvas", "", 1000, 1000);
	eaeGraph->Draw("APL");
	canvas4->SaveAs("eaeGraph.png");

	return 0;
}