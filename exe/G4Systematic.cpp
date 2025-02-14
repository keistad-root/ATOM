#include "TGraph.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TGraphErrors.h"

#include "TGeantInfoSet.h"
#include "CppConfigFile.h"
#include "TPlotter.h"
#include "TText.h"
#include<string>

void setGraph(const std::string& tag) {

}

int main() {
	CppConfigFile config("/home/ychoi/ATOM/config/Systematic.conf");
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

	TCanvas* canvas[3] = {new TCanvas("eemCanvas", "", 1000, 1000), new TCanvas("eaeCanvas", "", 1000, 1000), new TCanvas("doubleCanvas", "", 1000, 1000)};
	TF1* fitFunc[3] = {new TF1("fitFunc1", "[0]*x+[1]", 0, 3), new TF1("fitFunc2", "[0]*x+[1]", 0, 3), new TF1("fitFunc3", "[0]*x+[1]", 0, 3)};
	graph[0]->Fit(fitFunc[0], "R");
	graph[1]->Fit(fitFunc[1], "R");
	graph[2]->Fit(fitFunc[2], "R");
	TText* text = new TText(.5, .5, static_cast<TString>("Slope: " + std::to_string(fitFunc[0]->GetParameter(0))));
	text->SetTextSize(2);

	TPlotter plotter;
	std::filesystem::path outputPath = "/home/ychoi/ATOM/build/Data/";
	plotter.savePlot(canvas[0], graph[0], config.getConfig("ALPIDE_POSITION_X_EEM"));
	canvas[0]->cd();
	text->Draw("SAME");
	plotter.saveCanvas(canvas[0], outputPath, config.getConfig("ALPIDE_POSITION_X_EEM"));
	plotter.savePlot(canvas[1], graph[1], config.getConfig("ALPIDE_POSITION_X_EAE"));
	plotter.saveCanvas(canvas[1], outputPath, config.getConfig("ALPIDE_POSITION_X_EAE"));
	plotter.savePlot(canvas[2], graph[2], config.getConfig("ALPIDE_POSITION_X_DOUBLE"));
	plotter.saveCanvas(canvas[2], outputPath, config.getConfig("ALPIDE_POSITION_X_DOUBLE"));


	return 0;
}