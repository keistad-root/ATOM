#include "TEntrySimulation_v2.h"

#include<array>
#include<vector>

#include "cppargs.h"
#include "CppConfigFile.h"
#include "TMathUser.h"
#include "TGraph.h"
#include "TH2.h"
#include "TSimulSource.h"
#include "TSimulDetector.h"
#include "TSimulCollimator.h"
#include "TTrack.h"
#include "TCanvas.h"
#include "cpptqdm.h"

ArgumentParser set_parse(int argc, char** argv) {
	ArgumentParser parser = ArgumentParser(argc, argv).setDescription("Simulation for entry research");
	parser.add_argument("config").help("Config file for controling simulation").set_default("").add_finish();
	parser.parse_args();
	return parser;
}

TSimulSource* setSource(const CppConfigDictionary& config) {
	double x = 0., y = 0., z = 0.;
	double radius = config.hasKey("source_radius") ? stod(config.find("source_radius")) : 2.5;
	int step = config.hasKey("source_step") ? stoi(config.find("source_step")) : 1;

	TSimulSource* source = new TSimulSource(x, y, z, radius, step);
	source->extractCoordinate();

	return source;
}

TSimulDetector* setDetector(const CppConfigDictionary& config) {
	double x = config.hasKey("x") ? stod(config.find("x")) : 0.;
	double y = config.hasKey("y") ? stod(config.find("y")) : 0.;
	double z = config.hasKey("z") ? stod(config.find("z")) : 0.;
	double width = 30., height = 15.;

	TSimulDetector* detector = new TSimulDetector(x, y, z, width, height);
	return detector;
}

TSimulCollimator* setCollimator(const CppConfigDictionary& config) {
	double uX = config.hasKey("upper_x") ? stod(config.find("upper_x")) : 0.;
	double uY = config.hasKey("upper_y") ? stod(config.find("upper_y")) : 0.;
	double uZ = config.hasKey("upper_z") ? stod(config.find("upper_z")) : 0.;
	double uRadius;
	if ( config.hasKey("upper_radius") ) {
		uRadius = stod(config.find("upper_radius"));
	} else if ( config.hasKey("upper_area") ) {
		uRadius = sqrt(stod(config.find("upper_area")) / TMath::Pi());
	} else {
		uRadius = 0.;
	}

	double lX = config.hasKey("lower_x") ? stod(config.find("lower_x")) : 0.;
	double lY = config.hasKey("lower_y") ? stod(config.find("lower_y")) : 0.;
	double lZ = config.hasKey("lower_z") ? stod(config.find("lower_z")) : 0.;
	double lRadius;
	if ( config.hasKey("lower_radius") ) {
		lRadius = stod(config.find("lower_radius"));
	} else if ( config.hasKey("lower_area") ) {
		lRadius = sqrt(stod(config.find("lower_area")) / TMath::Pi());
	} else {
		lRadius = 0.;
	}

	TSimulCollimator* collimator = new TSimulCollimator(uX, uY, uZ, uRadius, lX, lY, lZ, lRadius);
	return collimator;
}



int main(int argc, char** argv) {
	ArgumentParser parser = set_parse(argc, argv);
	std::string configPath = parser.get_value<std::string>("config");
	CppConfigFile config(configPath);


	TSimulSource* source = setSource(config.getConfig("Source"));
	TSimulDetector* detector = setDetector(config.getConfig("Detector"));
	TSimulCollimator* collimator = setCollimator(config.getConfig("Collimator"));

	std::vector<TTrack*> trackSet;

	CppConfigDictionary envConfig = config.getConfig("Environment");
	int thetaStep = envConfig.hasKey("theta_step") ? stoi(envConfig.find("theta_step")) : 18.;
	int phiStep = envConfig.hasKey("phi_step") ? stoi(envConfig.find("phi_step")) : 36.;

	int nSourcePoint = source->getSourceCoordinate().size();
	double factor = (TMath::Pi() / thetaStep) * (2 * TMath::Pi() / phiStep) * (1. / nSourcePoint) / (4 * TMath::Pi());
	double ratio = 0.;
	ProgressBar pBar(nSourcePoint);
	for ( int i = 0; i < nSourcePoint; i++ ) {
		pBar.printProgress();
		int x = source->getSourceCoordinate()[i].first;
		int y = source->getSourceCoordinate()[i].second;

		for ( int iTheta = ((thetaStep / 2) + 1); iTheta < thetaStep; iTheta++ ) {
			double theta = iTheta * TMath::Pi() / thetaStep;
			for ( int iPhi = 0; iPhi < phiStep; iPhi++ ) {
				double phi = iPhi * (2 * TMath::Pi()) / phiStep;
				TTrack* track = new TTrack(x, y, 0, theta, phi);
				if ( collimator->isInclude(track) && detector->isInclude(track) ) {
					double weight = sin(theta) * factor;
					ratio += weight;
				}
				delete track;
			}
		}
	}
	std::cout << ratio << std::endl;

	return 0;
}