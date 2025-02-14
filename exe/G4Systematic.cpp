#include "TGraph.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TGraphErrors.h"

#include "TGeantInfoSet.h"
#include "CppConfigFile.h"
#include "TPlotter.h"
#include "TText.h"
#include<string>

void setGraph(const CppConfigFile& config, const TGeantInfoSet& set, const std::string& name) {
	std::vector<TGeantInfo> interestData;
	TGraphErrors* graph[3] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};

	for ( auto& entry : set.getGeantSet() ) {
		std::string tag = entry.getTag();
		if ( tag.find(name) != std::string::npos ) {
			interestData.push_back(entry);
		}
	}
	for ( auto& entry : interestData ) {
		double xAxis;
		if ( name == "DX" ) xAxis = entry.getAlpidePositionX();
		else if ( name == "DY" ) xAxis = entry.getAlpidePositionY();
		else if ( name == "_AX" ) xAxis = entry.getAlpideAngleX();
		else if ( name == "AY" ) xAxis = entry.getAlpideAngleY();
		else if ( name == "LS" ) xAxis = entry.getDistanceSourceCollimator();
		graph[0]->SetPoint(graph[0]->GetN(), xAxis, entry.getEEM()[0]);
		graph[0]->SetPointError(graph[0]->GetN() - 1, 0, entry.getEEM()[1]);
		graph[1]->SetPoint(graph[1]->GetN(), xAxis, entry.getEAE()[0]);
		graph[1]->SetPointError(graph[1]->GetN() - 1, 0, entry.getEAE()[1]);
		graph[2]->SetPoint(graph[2]->GetN(), xAxis, entry.getDouble()[0]);
		graph[2]->SetPointError(graph[2]->GetN() - 1, 0, entry.getDouble()[1]);
	}

	TF1* fitFunc[3] = {new TF1("fitFunc1", "[0]*x+[1]", 0, 10), new TF1("fitFunc2", "[0]*x+[1]", 0, 10), new TF1("fitFunc3", "[0]*x+[1]", 0, 10)};
	graph[0]->Fit(fitFunc[0], "R");
	graph[1]->Fit(fitFunc[1], "R");
	graph[2]->Fit(fitFunc[2], "R");

	TCanvas* canvas[3] = {new TCanvas("eemCanvas", "", 1000, 1000), new TCanvas("eaeCanvas", "", 1000, 1000), new TCanvas("doubleCanvas", "", 1000, 1000)};

	TPlotter plotter;
	std::filesystem::path outputPath = "/home/ychoi/ATOM/build/Data/";
	plotter.savePlot(canvas[0], graph[0], config.getConfig(name + "_EEM"));
	plotter.saveCanvas(canvas[0], outputPath, config.getConfig(name + "_EEM"));
	plotter.savePlot(canvas[1], graph[1], config.getConfig(name + "_EAE"));
	plotter.saveCanvas(canvas[1], outputPath, config.getConfig(name + "_EAE"));
	plotter.savePlot(canvas[2], graph[2], config.getConfig(name + "_DOUBLE"));
	plotter.saveCanvas(canvas[2], outputPath, config.getConfig(name + "_DOUBLE"));
	delete graph[0];
	delete graph[1];
	delete graph[2];
	delete fitFunc[0];
	delete fitFunc[1];
	delete fitFunc[2];
	delete canvas[0];
	delete canvas[1];
	delete canvas[2];
}

int main() {
	CppConfigFile config("/home/ychoi/ATOM/build/config/Systematic.conf");
	TGeantInfoSet set;

	std::vector<TGeantInfo> interestSet;
	// setGraph(config, set, "DX");
	// setGraph(config, set, "DY");
	// setGraph(config, set, "_AX");
	// setGraph(config, set, "AY");
	setGraph(config, set, "LS");

	return 0;
}