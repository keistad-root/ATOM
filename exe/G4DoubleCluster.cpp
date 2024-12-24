#include<csv.h>

#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TH1.h"
#include "TCanvas.h"

#include "CppConfigFile.h"
#include "TGeantComparison.h"

CppConfigFile setEnvironment(const std::string& iTag) {
	CppConfigFile config("/home/ychoi/ATOM/config/g4simulation/g4comparison.conf");

	io::CSVReader<3> mainCSV("/home/ychoi/ATOM/config/g4simulation/g4comparison.csv");
	std::string tag, fileListStr, referenceListStr;
	while ( mainCSV.read_row(tag, fileListStr, referenceListStr) ) {
		if ( tag == iTag ) {
			config.modifyConfig("File").addDictionary("file_list", fileListStr);
			config.modifyConfig("File").addDictionary("reference_list", referenceListStr);
			break;
		}
	}
	return config;
}

int main(int argc, char** argv) {
	TGeantComparison* geantComparison = new TGeantComparison(setEnvironment(argv[1]));
	geantComparison->drawNAlphaGraph();
	geantComparison->drawNElectronGraph();
	geantComparison->drawAlphaElectronRatioGraph();
	geantComparison->drawNDoubleGraph();
	geantComparison->drawDoubleSingleRatioGraph();
	geantComparison->compareDouble();
	geantComparison->compareElectron();
	geantComparison->compareAlpha();
	return 0;
}