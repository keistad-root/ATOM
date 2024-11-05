// #include "TEntrySimulation_v2.h"

// #include "TGraph.h"
// #include "TCanvas.h"
// #include "TMath.h"
// #include "TF1.h"
// #include "TFile.h"
// #include "TTree.h"
// #include "TColor.h"
// #include "TLegend.h"
// #include "TMultiGraph.h"
// #include "cppargs.h"
// #include "CppConfigFile.h"
// #include "TGraphErrors.h"
// #include "TGraphUser.h"

// struct InitValue {
// 	double sourceRadius;
// 	double detectorWidth;
// 	double detectorHeight;
// 	double detectorCenterX;
// 	double detectorCenterY;
// 	double sourceToCollimator;
// 	double collimatorLength;
// 	double collimatorToDetector;
// 	double upperCollimatorWidth;
// 	double upperCollimatorX;
// 	double upperCollimatorY;
// 	double lowerCollimatorWidth;
// 	double lowerCollimatorX;
// 	double lowerCollimatorY;
// };

// InitValue initialize(const CppConfigDictionary& config) {
// 	InitValue temp;
// 	temp.sourceRadius = config.hasKey("source_radius") ? stod(config.find("source_radius")) : 1.;
// 	temp.detectorWidth = config.hasKey("detector_x") ? stod(config.find("detector_x")) : 30.;
// 	temp.detectorHeight = config.hasKey("detector_y") ? stod(config.find("detector_y")) : 15.;
// 	temp.detectorCenterX = config.hasKey("detector_coord_x") ? stod(config.find("detector_coord_x")) : 0.;
// 	temp.detectorCenterY = config.hasKey("detector_coord_y") ? stod(config.find("detector_coord_y")) : 0.;
// 	temp.sourceToCollimator = config.hasKey("source_to_collimator") ? stod(config.find("source_to_collimator")) : .5;
// 	temp.collimatorLength = config.hasKey("collimator_length") ? stod(config.find("collimator_length")) : 1.;
// 	temp.collimatorToDetector = config.hasKey("collimator_to_detector") ? stod(config.find("collimator_to_detector")) : 2.9;
// 	return temp;
// }

// ArgumentParser set_parse(int argc, char** argv) {
// 	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Simulation for entry research");
// 	parser.add_argument("config").help("Config file for controling simulation").set_default("").add_finish();
// 	parser.parse_args();
// 	return parser;
// }

int main(int argc, char** argv) {
	// 	// Call setting configs from config file.
	// 	ArgumentParser parser = set_parse(argc, argv);
	// 	std::string configPath = parser.get_value<std::string>("config");
	// 	CppConfigFile config(configPath);

	// 	InitValue initValue = initialize(config.getConfig("Environment"));

	// 	CppConfigDictionary fileConfig = config.getConfig("File");
	// 	CppConfigDictionary environmentConfig = config.getConfig("Environment");
	// 	CppConfigDictionary graphConfig = config.getConfig("Graph");


	// 	TString graphPath = fileConfig.find("graph_path");
	// 	TFile* graphFile = new TFile(graphPath, "RECREATE");
	// 	TTree* tree = new TTree("data", "Data");
	// 	double sourceRadius = initValue.sourceRadius;
	// 	double detectorCenterX = initValue.detectorCenterX;
	// 	double detectorCenterY = initValue.detectorCenterY;
	// 	double sourceToCollimator = initValue.sourceToCollimator;
	// 	double collimatorLength = initValue.collimatorLength;
	// 	double collimatorToDetector = initValue.collimatorToDetector;
	// 	double upperCollimatorWidth = initValue.upperCollimatorWidth;
	// 	double upperCollimatorX = initValue.upperCollimatorX;
	// 	double upperCollimatorY = initValue.upperCollimatorY;
	// 	double upperCollimatorWidth = initValue.upperCollimatorWidth;
	// 	double upperCollimatorX = initValue.upperCollimatorX;
	// 	double upperCollimatorY = initValue.upperCollimatorY;
	// 	double ratio;
	// 	tree->Branch("source_radius", &sourceRadius);
	// 	tree->Branch("detector_center_x", &detectorCenterX);
	// 	tree->Branch("detector_center_y", &detectorCenterY);
	// 	tree->Branch("source_to_collimator", &sourceToCollimator);
	// 	tree->Branch("collimator_length", &collimatorLength);
	// 	tree->Branch("collimator_to_detector", &collimatorToDetector);
	// 	tree->Branch("ratio", &ratio);


	// 	TAdvancedEntrySimulation* simulation = new TAdvancedEntrySimulation(.25);

	// 	for ( int step = -2; step = 2; step++ ) {
	// 		sourceRadius = initValue.sourceRadius + .1 * step;

	// 		simulation->setSource(sourceRadius);
	// 		simulation->setDetector({detectorCenterX, detectorCenterY, -(sourceToCollimator + collimatorToDetector + collimatorLength)}, 30., 15.);


	// 		simulation->setCollimator({0., 0., -sourceToCollimator}, collimatorRadius, {0., 0., -(sourceToCollimator + collimatorLength)}, collimatorRadius);
	// 		simulation->extractTrack(360);
	// 	}









	// 	// TGraph* graph = new TGraph();
	// 	std::vector<std::pair<std::string, std::vector<std::array<double, 2>>>> ratio;
	// 	double divideCriteria = 0.;
	// 	if ( graphConfig.find("type") == "comprehensive_point" ) {
	// 		int nGraph = graphConfig.getSubConfig("graphs").getSubConfigSet().size();
	// 		int previousLength = 0;
	// 		for ( int iGraph = 0; iGraph < nGraph; iGraph++ ) {
	// 			std::vector<std::array<double, 2>> tempRatio;
	// 			double length = stod(graphConfig.getSubConfig("graphs").getSubConfigSet()[iGraph].find("length"));
	// 			std::string widthStr = graphConfig.getSubConfig("graphs").getSubConfigSet()[iGraph].find("width");
	// 			double min = stod(widthStr.substr(0, widthStr.find(':')));
	// 			widthStr = widthStr.substr(widthStr.find(':') + 1);
	// 			double max = stod(widthStr.substr(0, widthStr.find(':')));
	// 			widthStr = widthStr.substr(widthStr.find(':') + 1);
	// 			double gap = stod(widthStr.substr(0, widthStr.find(':')));
	// 			std::cout << "Collimator Length = " << length << std::endl;
	// 			for ( double width = min; width < max; width += gap ) {
	// 				simulation->setDetector({detectorCoordX, detectorCoordY, -(S2CDistance + C2DDistance + length)}, detectorX, detectorY);
	// 				simulation->setCollimator({0., 0., -S2CDistance}, sqrt(width / TMath::Pi()), {0., 0., -(S2CDistance + length)}, sqrt(width / TMath::Pi()));
	// 				if ( previousLength != length ) {
	// 					simulation->extractTrack(360);
	// 				}
	// 				double entryRatio = simulation->calculateEntry();
	// 				tempRatio.push_back({width, entryRatio});
	// 				if ( width > 34.9 && width < 35.1 && length > 0.9 && length < 1.1 ) {
	// 					divideCriteria = entryRatio;
	// 				}
	// 				std::cout << length << "\t" << width << "\t" << entryRatio << std::endl;

	// 				previousLength = static_cast<int>(length * 10);
	// 			}
	// 			ratio.push_back({std::string(graphConfig.getSubConfig("graphs").getSubConfigSet()[iGraph].getConfigName()), tempRatio});
	// 		}
	// 		std::vector<TGraph*> entryRatioGraph;

	// 		double totalActivity = stod(environmentConfig.find("source_becquerel")) * stod(environmentConfig.find("time"));

	// 		TGraphUser* graph1 = new TGraphUser(graphConfig.getSubConfig("entryRatio"));
	// 		for ( const std::pair<std::string, std::vector<std::array<double, 2>>>&ratioSet : ratio ) {
	// 			entryRatioGraph.push_back(new TGraph());
	// 			for ( const std::array<double, 2>&point : ratioSet.second ) {
	// 				// entryRatioGraph.back()->AddPoint(point[0], point[1] * totalActivity);
	// 				entryRatioGraph.back()->AddPoint(point[0], point[1] * 420200 / 0.099267);
	// 			}
	// 			int nGraph = graphConfig.getSubConfig("graphs").getSubConfigSet().size();
	// 			for ( int iGraph = 0; iGraph < nGraph; iGraph++ ) {
	// 				if ( graphConfig.getSubConfig("graphs").getSubConfigSet()[iGraph].getConfigName() == ratioSet.first ) {
	// 					graph1->AddGraph(entryRatioGraph.back(), graphConfig.getSubConfig("graphs").getSubConfigSet()[iGraph]);
	// 				}
	// 			}
	// 		}

	// 		for ( const CppConfigDictionary& expData : graphConfig.getSubConfig("exp_data").getSubConfigSet() ) {
	// 			TGraphErrors* eGraph = new TGraphErrors();
	// 			int i = 0;
	// 			for ( const CppConfigDictionary& point : expData.getSubConfig("point").getSubConfigSet() ) {
	// 				Double_t width = stod(point.find("width"));
	// 				Double_t width_error = stod(point.find("width_error"));
	// 				Double_t value = stod(point.find("value"));
	// 				Double_t error = stod(point.find("error"));
	// 				eGraph->SetPoint(i, width, value);
	// 				eGraph->SetPointError(i, width_error, error);
	// 				i++;
	// 			}
	// 			graph1->AddGraph(eGraph, expData);
	// 		}


	// 		// Set output path and create directories if it isn't.
	// 		std::filesystem::path outputPath = fileConfig.find("output_path");
	// 		std::filesystem::create_directories(outputPath);
	// 		graph1->Save(outputPath);

	// 	}
	// 	return 0;
}