#include<iostream>

#include "TExperimentInfoSet.h"
#include "TGeantInfoSet.h"

#include "TGraph.h"

int main(int argc, char** argv) {
	TExperimentInfoSet expInfo;
	TGeantInfoSet geantInfo;

	std::string eutTag = "L20F2";
	std::string refTag = "L20REFAX";
	TExperimentInfo eutAlphaExp = expInfo.getExperimentInfo(eutTag + "X");
	TExperimentInfo eutGammaExp = expInfo.getExperimentInfo(eutTag + "O");
	TExperimentInfo refAlphaExp = expInfo.getExperimentInfo(refTag + "X");
	TExperimentInfo refGammaExp = expInfo.getExperimentInfo(refTag + "O");

	TGeantInfo eutAlphaGeant = geantInfo.getGeantInfo(eutTag);
	TGeantInfo refAlphaGeant = geantInfo.getGeantInfo(refTag);

	TGraph* expGraph = new TGraph();


	return 0;
}