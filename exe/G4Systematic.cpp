#include "TGraph.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TGraphErrors.h"

#include "TGeantInfoSet.h"

#include<string>

int main() {
	TGeantInfoSet set;

	std::vector<TGeantInfo> interestSet;
	TGraphErrors* graph[3] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};

	for ( auto& entry : set.getGeantSet() ) {
		std::string tag = entry.getTag();
		if ( tag.find("DX") != std::string::npos ) {
			interestSet.push_back(entry);
		}
	}

	for ( auto& entry : interestSet ) {
		graph[0]->SetPoint(graph[0]->GetN(), entry.getAlpidePositionX(), entry.getEEM()[0]);
		graph[0]->SetPointError(graph[0]->GetN() - 1, 0, entry.getEEM()[1]);
		graph[1]->SetPoint(graph[1]->GetN(), entry.getAlpidePositionX(), entry.getEAE()[0]);
		graph[1]->SetPointError(graph[1]->GetN() - 1, 0, entry.getEAE()[1]);
		graph[2]->SetPoint(graph[2]->GetN(), entry.getAlpidePositionX(), entry.getDouble()[0]);
		graph[2]->SetPointError(graph[2]->GetN() - 1, 0, entry.getDouble()[1]);
	}

	TCanvas* canvas[3] = {new TCanvas("eemCanvas", "", 1000, 1000), new TCanvas("eaeCanvas", "", 1000, 1000), new TCanvas("eaeCanvas", "", 1000, 1000)};
	TF1* fitFunc[3] = {new TF1("fitFunc1", "[0]*x+[1]", 0, 100), new TF1("fitFunc2", "[0]*x+[1]", 0, 100), new TF1("fitFunc3", "[0]*x+[1]", 0, 100)};
	graph[0]->Fit(fitFunc[0], "R");
	graph[1]->Fit(fitFunc[1], "R");
	graph[2]->Fit(fitFunc[2], "R");



	// Double_t width[11] = {0.82, 0.85, 0.88, 0.91, 0.94, 0.97, 1.00, 1.03, 1.06, 1.09, 1.12};
	// Double_t Double[11] = {0, 1, 1, 0, 1, 0, 0, 2, 1, 0, 3};
	// Double_t EAM[11] = {720, 785, 805, 846, 962, 932, 1056, 1067, 1047, 1134, 1226};
	// Double_t EEM[11] = {260, 236, 267, 270, 298, 309, 312, 290, 328, 320, 313};
	// Double_t EAE[11] = {626, 678, 699, 738, 846, 807, 894, 923, 931, 975, 1077};

	// TGraph* doubleGraph = new TGraph();
	// TGraph* eamGraph = new TGraph();
	// TGraph* eemGraph = new TGraph();
	// TGraph* eaeGraph = new TGraph();

	// for ( int i = 0; i < 11; i++ ) {
	// 	doubleGraph->SetPoint(i, width[i], Double[i]);
	// 	eamGraph->SetPoint(i, width[i], EAM[i]);
	// 	eemGraph->SetPoint(i, width[i], EEM[i]);
	// 	eaeGraph->SetPoint(i, width[i], EAE[i]);
	// }

	// TF1* fitFunc[4] = {new TF1("fitFunc1", "[0]*x+[1]", 0.8, 1.2), new TF1("fitFunc2", "[0]*x+[1]", 0.8, 1.2), new TF1("fitFunc3", "[0]*x+[1]", 0.8, 1.2), new TF1("fitFunc4", "[0]*x+[1]", 0.8, 1.2)};

	// doubleGraph->Fit(fitFunc[0], "R");
	// eamGraph->Fit(fitFunc[1], "R");
	// eemGraph->Fit(fitFunc[2], "R");
	// eaeGraph->Fit(fitFunc[3], "R");

	// TCanvas* canvas = new TCanvas("doubleCanvas", "", 1000, 1000);
	// doubleGraph->Draw("APL");
	// canvas->SaveAs("doubleGraph.png");
	// TCanvas* canvas2 = new TCanvas("eamCanvas", "", 1000, 1000);
	// eamGraph->Draw("APL");
	// canvas2->SaveAs("eamGraph.png");
	// TCanvas* canvas3 = new TCanvas("eemCanvas", "", 1000, 1000);
	// eemGraph->Draw("APL");
	// canvas3->SaveAs("eemGraph.png");
	// TCanvas* canvas4 = new TCanvas("eaeCanvas", "", 1000, 1000);
	// eaeGraph->Draw("APL");
	// canvas4->SaveAs("eaeGraph.png");

	return 0;
}