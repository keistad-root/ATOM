#include <iostream>
#include <fstream>
#include <filesystem>

// #include "TEvent.h"
#include "CppConfigFile.h"
#include "TAnalyser.h"
#include "TExperimentData.h"
#include "cppargs.h"
#include "TClusterDivideData.h"
#include "TClusterization.h"

#include "TCanvas.h"
#include "TFile.h"

// class ControlExperimentAnalysis {
// 	ArgumentParser mParser;
// 	CppConfigFile* mConfig;
// 	std::vector<std::string> mTypeNameSet;
// 	std::unordered_map<std::string, CppConfigDictionary*> mSubConfigSet;
// 	std::unordered_map<std::string, TExperimentData*> mExpDataSet;

// 	std::string mInputFilePath;
// 	TFile* mInputFile = nullptr;
// 	TAnalyser* mAnalyser;
// 	std::vector<int> mClusterRange;

	// public:
	// 	ControlExperimentAnalysis(int argc, char** argv);
	// 	~ControlExperimentAnalysis();
	// 	void setConfig();
	// 	void openInputFile();
	// 	void setExpDataSet();
	// 	void doBasicAnalysis();
	// 	void doMasking();
	// 	void drawHitmap();
	// 	void clusterization();
	// 	void drawClustermapAndClustersize();
	// 	void doDivideBySize();
	// 	void drawClusterShapeInfos();

	// 	std::vector<int> getClusterSizeRange(const CppConfigDictionary privateProperty);
	// };

	// ControlExperimentAnalysis::ControlExperimentAnalysis(int argc, char** argv) : mParser(argc, argv) {
	// 	mParser.setDescription("Draw plots for analysis data");
	// 	mParser.add_argument("config").help("Config file").set_default("default").add_finish();
	// 	mParser.parse_args();
	// }

	// ControlExperimentAnalysis::~ControlExperimentAnalysis() {
	// 	for ( auto& key : mSubConfigSet ) {
	// 		delete key.second;
	// 		key.second = nullptr;
	// 	}
	// 	for ( auto& key : mExpDataSet ) {
	// 		delete key.second;
	// 		key.second = nullptr;
	// 	}
	// 	if ( mInputFile != nullptr && !mInputFile->IsDestructed() ) {
	// 		mInputFile->Close();
	// 		delete mInputFile;
	// 		mInputFile = nullptr;
	// 	}
	// 	if ( mAnalyser != nullptr && !mAnalyser->IsDestructed() ) {
	// 		delete mAnalyser;
	// 		mAnalyser = nullptr;
	// 	}
	// }

	// void ControlExperimentAnalysis::setConfig() {
	// 	std::string configPath = mParser.get_value<std::string>("config");
	// 	mConfig = new CppConfigFile(configPath);

	// 	mTypeNameSet = mConfig->getConfig("File").getSubConfig("type_name").getValueList();
	// 	mInputFilePath = mConfig->getConfig("File").find("input_file");
	// }

	// void ControlExperimentAnalysis::openInputFile() {
	// 	mInputFile = new TFile(static_cast<TString>(mInputFilePath), "READ");
	// }

	// void ControlExperimentAnalysis::setExpDataSet() {
	// 	for ( const std::string& typeName : mTypeNameSet ) {
	// 		mExpDataSet.insert_or_assign(typeName, new TExperimentData());
	// 	}
	// }

	// void ControlExperimentAnalysis::doBasicAnalysis() {
	// 	mAnalyser = new TAnalyser(mInputFile, mExpDataSet);
	// 	mConfig->getConfig("Masking").hasKey("time_stamp_cut");
	// 	mAnalyser->storeEvents(mConfig->getConfig("SharedProperty"));
	// 	mAnalyser->setExpSettingLegend(mConfig->getConfig("ExperimentSetting"));
	// 	if ( mConfig->getConfig("File").hasKey("output_graph") ) {
	// 		mAnalyser->openOutputGraphFile(mConfig->getConfig("File").find("output_graph"));
	// 		for ( std::string_view typeName : mTypeNameSet ) {
	// 			mAnalyser->openDirectory(typeName);
	// 		}
	// 	}
	// }

	// void ControlExperimentAnalysis::doMasking() {
	// 	mAnalyser->doMasking(stoi(mConfig->getConfig("Masking").find("cut")));
	// 	for ( std::string_view typeName : mTypeNameSet ) {
	// 		if ( typeName != "Basic" ) {
	// 			mExpDataSet.insert_or_assign(std::string(typeName), mAnalyser->getAnEventSet(typeName));
	// 		}
	// 	}
	// }

	// void ControlExperimentAnalysis::drawHitmap() {
	// 	if ( mConfig->getConfig("Basic").hasKey("hitmap") ) {
	// 		CppConfigDictionary hitmapConfig = mConfig->getConfig("Basic").getSubConfig("hitmap");
	// 		hitmapConfig += mConfig->getConfig("SharedProperty");
	// 		mAnalyser->saveHitmap("Basic", hitmapConfig);
	// 	}
	// 	doMasking();
	// 	for ( const std::string& typeName : mTypeNameSet ) {
	// 		if ( typeName != "Basic" ) {
	// 			if ( mConfig->getConfig(typeName).hasKey("hitmap") ) {
	// 				CppConfigDictionary hitmapConfig = mConfig->getConfig(typeName).getSubConfig("hitmap");
	// 				hitmapConfig += mConfig->getConfig("SharedProperty");
	// 				mAnalyser->saveHitmap(typeName, hitmapConfig);
	// 			}
	// 		}
	// 	}
	// }

	// void ControlExperimentAnalysis::clusterization() {
	// 	for ( const std::string& typeName : mTypeNameSet ) {
	// 		TClusterization clusterization(mExpDataSet.find(std::string(typeName))->second->getEvents());
	// 		clusterization.clusterize();
	// 		mExpDataSet.find(std::string(typeName))->second->setClusters(clusterization.getClusters());
	// 	}
	// }

	// void ControlExperimentAnalysis::drawClustermapAndClustersize() {

	// 	for ( const std::string& typeName : mTypeNameSet ) {
	// 		if ( mConfig->getConfig(typeName).hasKey("clustermap") ) {
	// 			CppConfigDictionary hitmapConfig = mConfig->getConfig(typeName).getSubConfig("clustermap");
	// 			hitmapConfig += mConfig->getConfig("SharedProperty");
	// 			mAnalyser->saveClustermap(typeName, hitmapConfig);
	// 		}
	// 		if ( mConfig->getConfig(typeName).hasKey("clustersize") ) {
	// 			CppConfigDictionary hitmapConfig = mConfig->getConfig(typeName).getSubConfig("clustersize");
	// 			hitmapConfig += mConfig->getConfig("SharedProperty");
	// 			mAnalyser->saveClustersize(typeName, hitmapConfig);
	// 		}
	// 	}
	// }

	// void ControlExperimentAnalysis::doDivideBySize() {
	// 	mClusterRange = getClusterSizeRange(mConfig->getConfig("ShapeCut"));
	// 	for ( std::string_view typeName : mTypeNameSet ) {
	// 		mAnalyser->doDivideBySize(typeName);
	// 	}
	// }

	// void ControlExperimentAnalysis::drawClusterShapeInfos() {
	// 	for ( const std::string& typeName : mTypeNameSet ) {
	// 		mAnalyser->doShaping(typeName, mClusterRange);
	// 		if ( mConfig->getConfig(typeName).hasKey("shape_individual") ) {
	// 			CppConfigDictionary shapeConfig = mConfig->getConfig(typeName).getSubConfig("shape_individual");
	// 			shapeConfig += mConfig->getConfig("SharedProperty");
	// 			mAnalyser->saveIndividualShapes(typeName, shapeConfig);
	// 		}
	// 		if ( mConfig->getConfig(typeName).hasKey("shape_same_size") ) {
	// 			CppConfigDictionary shapeConfig = mConfig->getConfig(typeName).getSubConfig("shape_same_size");
	// 			shapeConfig += mConfig->getConfig("SharedProperty");
	// 			mAnalyser->saveSameSizeShapes(typeName, shapeConfig);
	// 		}
	// 		if ( mConfig->getConfig(typeName).hasKey("shape_total") ) {
	// 			CppConfigDictionary shapeConfig = mConfig->getConfig(typeName).getSubConfig("shape_total");
	// 			shapeConfig += mConfig->getConfig("SharedProperty");
	// 			mAnalyser->saveTotalShapes(typeName, shapeConfig);
	// 		}
	// 		if ( mConfig->getConfig(typeName).hasKey("shape_same_size_entry") ) {
	// 			CppConfigDictionary shapeConfig = mConfig->getConfig(typeName).getSubConfig("shape_same_size_entry");
	// 			shapeConfig += mConfig->getConfig("SharedProperty");
	// 			mAnalyser->saveSameSizeShapeEntry(typeName, shapeConfig);
	// 		}
	// 		if ( mConfig->getConfig(typeName).hasKey("shape_total_entry") ) {
	// 			CppConfigDictionary shapeConfig = mConfig->getConfig(typeName).getSubConfig("shape_total_entry");
	// 			shapeConfig += mConfig->getConfig("SharedProperty");
	// 			mAnalyser->saveTotalShapeEntry(typeName, shapeConfig);
	// 		}
	// 		if ( mConfig->getConfig(typeName).hasKey("shape_info") ) {
	// 			CppConfigDictionary shapeConfig = mConfig->getConfig(typeName).getSubConfig("shape_info");
	// 			shapeConfig += mConfig->getConfig("SharedProperty");
	// 			mAnalyser->saveSameSizeInfos(typeName, shapeConfig);
	// 		}
	// 	}
	// }


	// std::vector<int> ControlExperimentAnalysis::getClusterSizeRange(const CppConfigDictionary privateProperty) {
	// 	std::vector<int> clusterSizeRange;
	// 	if ( privateProperty.hasKey("cluster_size_oi") ) {
	// 		for ( const std::string& rangeStr : privateProperty.getSubConfig("cluster_size_oi").getValueList() ) {
	// 			if ( rangeStr.find('.') != std::string::npos ) {
	// 				for ( int i = stoi(rangeStr.substr(0, rangeStr.find('.'))); i < stoi(rangeStr.substr(rangeStr.find('.') + 3)) + 1; i++ ) {
	// 					clusterSizeRange.push_back(i);
	// 				}
	// 			} else {
	// 				clusterSizeRange.push_back(stoi(rangeStr));
	// 			}
	// 		}
	// 	} else {
	// 		for ( int i = 0; i < 100; i++ ) {
	// 			clusterSizeRange.push_back(i);
	// 		}
	// 	}
	// 	return clusterSizeRange;
	// }