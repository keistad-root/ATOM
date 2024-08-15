#include "TEntrySimulation_v2.h"

#include "TGraph.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TF1.h"
#include "TColor.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include "cppargs.h"
#include "CppConfigFile.h"
#include "TGraphErrors.h"
#include "TGraphUser.h"

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Simulation for entry research");
	parser.add_argument("config").help("Config file for controling simulation").set_default("").add_finish();
	parser.parse_args();
	return parser;
}

int main(int argc, char** argv) {
	// Call setting configs from config file.
	ArgumentParser parser = set_parse(argc, argv);
	std::string configPath = parser.get_value<std::string>("config");
	CppConfigFile config(configPath);
	CppConfigDictionary fileConfig = config.getConfig("File");
	CppConfigDictionary environmentConfig = config.getConfig("Environment");
	CppConfigDictionary graphConfig = config.getConfig("Graph");

	// Set source radius
	double sourceRadius = 2.5;
	if ( environmentConfig.hasKey("source_radius") ) {
		sourceRadius = stod(environmentConfig.find("source_radius"));
	}

	//Set detector paramters
	double detectorX = 30.;
	double detectorY = 15.;
	double S2CDistance = .5;
	double C2DDistance = 3.;
	double collimatorLength = 1.;
	double detectorCoordX = 0.;
	double detectorCoordY = 0.;
	if ( environmentConfig.hasKey("detector_x") ) {
		detectorX = stod(environmentConfig.find("detector_x"));
	}
	if ( environmentConfig.hasKey("detector_y") ) {
		detectorY = stod(environmentConfig.find("detector_y"));
	}
	if ( environmentConfig.hasKey("source_to_collimator") ) {
		S2CDistance = stod(environmentConfig.find("source_to_collimator"));
	}
	if ( environmentConfig.hasKey("collimator_to_detector") ) {
		C2DDistance = stod(environmentConfig.find("collimator_to_detector"));
	}
	if ( environmentConfig.hasKey("collimator_length") ) {
		collimatorLength = stod(environmentConfig.find("collimator_length"));
	}
	if ( environmentConfig.hasKey("detector_coord_x") ) {
		detectorCoordX = stod(environmentConfig.find("detector_coord_x"));
	}
	if ( environmentConfig.hasKey("detector_coord_y") ) {
		detectorCoordY = stod(environmentConfig.find("detector_coord_y"));
	}

	// Set collimator paramters
	double collimatorRadius = 1.5;
	if ( environmentConfig.hasKey("collimator_radius") ) {
		collimatorRadius = stod(environmentConfig.find("collimator_radius"));
	}


	TAdvancedEntrySimulation* simulation = new TAdvancedEntrySimulation(.5);
	simulation->setSource(sourceRadius);
	simulation->setDetector({detectorCoordX, detectorCoordY, -(S2CDistance + C2DDistance + collimatorLength)}, detectorX, detectorY);
	simulation->setCollimator({0., 0., -S2CDistance}, collimatorRadius, {0., 0., -(S2CDistance + collimatorLength)}, collimatorRadius);
	simulation->extractTrack(180);

	// TGraph* graph = new TGraph();
	std::vector<std::pair<std::string, std::vector<std::array<double, 2>>>> ratio;
	double divideCriteria = 0.;
	if ( graphConfig.find("type") == "comprehensive_point" ) {
		int nGraph = graphConfig.getSubConfig("graphs").getSubConfigSet().size();
		int previousLength = 0;
		for ( int iGraph = 0; iGraph < nGraph; iGraph++ ) {
			std::vector<std::array<double, 2>> tempRatio;
			double length = stod(graphConfig.getSubConfig("graphs").getSubConfigSet()[iGraph].find("length"));
			std::string widthStr = graphConfig.getSubConfig("graphs").getSubConfigSet()[iGraph].find("width");
			double min = stod(widthStr.substr(0, widthStr.find(':')));
			widthStr = widthStr.substr(widthStr.find(':') + 1);
			double max = stod(widthStr.substr(0, widthStr.find(':')));
			widthStr = widthStr.substr(widthStr.find(':') + 1);
			double gap = stod(widthStr.substr(0, widthStr.find(':')));
			std::cout << "Collimator Length = " << length << std::endl;
			for ( double width = min; width < max; width += gap ) {
				simulation->setDetector({detectorCoordX, detectorCoordY, -(S2CDistance + C2DDistance + length)}, detectorX, detectorY);
				simulation->setCollimator({0., 0., -S2CDistance}, sqrt(width / TMath::Pi()), {0., 0., -(S2CDistance + length)}, sqrt(width / TMath::Pi()));
				if ( previousLength != length ) {
					simulation->extractTrack(360);
				}
				double entryRatio = simulation->calculateEntry();
				tempRatio.push_back({width, entryRatio});
				if ( width > 34.9 && width < 35.1 && length > 0.9 && length < 1.1 ) {
					divideCriteria = entryRatio;
				}
				std::cout << length << "\t" << width << "\t" << entryRatio << std::endl;

				previousLength = static_cast<int>(length * 10);
			}
			ratio.push_back({std::string(graphConfig.getSubConfig("graphs").getSubConfigSet()[iGraph].getConfigName()), tempRatio});
		}
		std::vector<TGraph*> entryRatioGraph;

		double totalActivity = stod(environmentConfig.find("source_becquerel")) * stod(environmentConfig.find("time"));

		TGraphUser* graph1 = new TGraphUser(graphConfig.getSubConfig("entryRatio"));
		for ( const std::pair<std::string, std::vector<std::array<double, 2>>>&ratioSet : ratio ) {
			entryRatioGraph.push_back(new TGraph());
			for ( const std::array<double, 2>&point : ratioSet.second ) {
				// entryRatioGraph.back()->AddPoint(point[0], point[1] * totalActivity);
				entryRatioGraph.back()->AddPoint(point[0], point[1] / 0.255375);
			}
			int nGraph = graphConfig.getSubConfig("graphs").getSubConfigSet().size();
			for ( int iGraph = 0; iGraph < nGraph; iGraph++ ) {
				if ( graphConfig.getSubConfig("graphs").getSubConfigSet()[iGraph].getConfigName() == ratioSet.first ) {
					graph1->AddGraph(entryRatioGraph.back(), graphConfig.getSubConfig("graphs").getSubConfigSet()[iGraph]);
				}
			}
		}

		for ( const CppConfigDictionary& expData : graphConfig.getSubConfig("exp_data").getSubConfigSet() ) {
			TGraphErrors* eGraph = new TGraphErrors();
			int i = 0;
			for ( const CppConfigDictionary& point : expData.getSubConfig("point").getSubConfigSet() ) {
				Double_t width = stod(point.find("width"));
				Double_t width_error = stod(point.find("width_error"));
				Double_t value = stod(point.find("value"));
				Double_t error = stod(point.find("error"));
				eGraph->SetPoint(i, width, value);
				eGraph->SetPointError(i, width_error, error);
				i++;
			}
			graph1->AddGraph(eGraph, expData);
		}


		// Set output path and create directories if it isn't.
		std::filesystem::path outputPath = fileConfig.find("output_path");
		std::filesystem::create_directories(outputPath);

		graph1->Save(outputPath);

	}
	return 0;
}