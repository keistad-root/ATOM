#include <iostream>
#include <fstream>
#include <filesystem>

// #include "TEvent.h"
#include "cppconfig.h"
#include "TClusterAnalyser.h"
#include "TClusterShapeAnalyser.h"
#include "TExperimentData.h"
#include "cppargs.h"
#include "TClusterDivideData.h"
#include "TClusterization.h"

#include "TCanvas.h"
#include "TFile.h"

class ControlExperimentAnalysis {
	ArgumentParser mParser;
	Configuration* mConfig = nullptr;
	bool mHasConfig = false;
	std::vector<std::string> mTypeNameSet;
	std::unordered_map<std::string, Configuration*> mSubConfigSet;
	std::unordered_map<std::string, TExperimentData*> mExpDataSet;

	TFile* mInputFile = nullptr;
	TAnalyser* mAnalyser;
	TClusterAnalyser* mClusterAnalyser;
	TClusterShapeAnalyser* mClusterShapeAnalyser;
	std::vector<int> mClusterRange;

	Configurable* mSharedProperty;


public:
	ControlExperimentAnalysis(int argc, char** argv);
	~ControlExperimentAnalysis();
	void openOutputGraphFile();
	void setConfig();
	void openInputFile();
	void setSubConfigSet();
	void setExpDataSet();
	void doBasicAnalysis();
	void doMasking();
	void drawHitmap();
	void clusterization();
	void drawClustermapAndClustersize();
	void doDivideBySize();
	void drawClusterShapeInfos();

	std::filesystem::path getOutputPathFromConfig(const Configurable* privateProperty);
	std::string getPlotTitleFromConfig(const Configurable* privateProperty);
	std::string getOptionFromConfig(const Configurable* privateProperty);
	std::vector<int> getClusterSizeRange(const Configurable* privateProperty);

	Configurable* mHitmapPrivateConfigGenerator(const Configurable* privateProperty);
	Configurable* mClustermapPrivateConfigGenerator(const Configurable* privateProperty);
	Configurable* mClustersizePrivateConfigGenerator(const Configurable* privateProperty);
	Configurable* mIndividualClusterShapePrivateGenerator(const Configurable* privateProperty);
	Configurable* mSameSizeClusterShapePrivateGenerator(const Configurable* privateProperty);
	Configurable* mTotalClusterShapePrivateGenerator(const Configurable* privateProperty);
	Configurable* mSameSizeClusterShapeEntryPrivateGenerator(const Configurable* privateProperty);
	Configurable* mTotalClusterShapeEntryPrivateGenerator(const Configurable* privateProperty);

};

ControlExperimentAnalysis::ControlExperimentAnalysis(int argc, char** argv) : mParser(argc, argv) {
	mParser.setDescription("Draw plots for analysis data");
	mParser.add_argument("config").help("Config file").set_default("default").add_finish();
	mParser.parse_args();
}

ControlExperimentAnalysis::~ControlExperimentAnalysis() {
	if ( mHasConfig ) {
		delete mConfig;
	}
	for ( auto& key : mSubConfigSet ) {
		delete key.second;
		key.second = nullptr;
	}
	for ( auto& key : mExpDataSet ) {
		delete key.second;
		key.second = nullptr;
	}
	if ( mInputFile != nullptr && !mInputFile->IsDestructed() ) {
		mInputFile->Close();
		delete mInputFile;
		mInputFile = nullptr;
	}
	if ( mClusterShapeAnalyser != nullptr && !mClusterShapeAnalyser->IsDestructed() ) {
		delete mClusterShapeAnalyser;
		mClusterShapeAnalyser = nullptr;
	}
	if ( mClusterAnalyser != nullptr && !mClusterAnalyser->IsDestructed() ) {
		delete mClusterAnalyser;
		mClusterAnalyser = nullptr;
	}
	if ( mAnalyser != nullptr && !mAnalyser->IsDestructed() ) {
		delete mAnalyser;
		mAnalyser = nullptr;
	}
}

void ControlExperimentAnalysis::setConfig() {
	std::string configPath = mParser.get_value<std::string>("config");
	mConfig = new Configuration(configPath);
	mHasConfig = true;
}

void ControlExperimentAnalysis::openInputFile() {
	const Configurable* fileConfig = mConfig->getConfig("File");
	std::string inputFilePath = fileConfig->find("input_file");
	mInputFile = new TFile(static_cast<TString>(inputFilePath), "READ");
}

void ControlExperimentAnalysis::setSubConfigSet() {
	for ( std::string_view mainKey : mConfig->getConfigurableNameList() ) {
		const Configurable* mainConfig = mConfig->getConfig(mainKey);
		Configuration* subConfiguration = new Configuration();
		int configSize = 0;
		for ( std::string_view subKey : mainConfig->getKeyList() ) {
			if ( mainConfig->findlist(subKey).size() > 1 ) {
				std::vector<std::string> subValue = mainConfig->findlist(subKey);
				subConfiguration->addConfig(subKey, subValue);
				configSize++;
			}
		}
		if ( configSize != 0 ) {
			mTypeNameSet.push_back(std::string(mainKey));
			mSubConfigSet.insert_or_assign(std::string(mainKey), subConfiguration);
		} else {
			delete subConfiguration;
		}
		subConfiguration = nullptr;
		mainConfig = nullptr;
	}
	mSharedProperty = const_cast<Configurable*>(mConfig->getConfig("SharedProperty"));
}

void ControlExperimentAnalysis::setExpDataSet() {
	for ( const auto& subConfig : mSubConfigSet ) {
		std::string typeName = subConfig.first;
		mExpDataSet.insert_or_assign(typeName, new TExperimentData());
	}
}

void ControlExperimentAnalysis::doBasicAnalysis() {
	mAnalyser = new TAnalyser(mInputFile, mExpDataSet);
	mAnalyser->storeEvents();
	mAnalyser->setExpSettingLegend(*mConfig->getConfig("ExperimentSetting"));
	if ( mConfig->getConfig("File")->hasKey("output_graph") ) {
		mAnalyser->openOutputGraphFile(mConfig->getConfig("File")->find("output_graph"));
		for ( std::string_view typeName : mTypeNameSet ) {
			mAnalyser->openDirectory(typeName);
		}
	}
}

void ControlExperimentAnalysis::doMasking() {
	mAnalyser->doMasking(stoi(mConfig->getConfig("Masking")->find("cut")));
	for ( std::string_view typeName : mTypeNameSet ) {
		if ( typeName != "Basic" ) {
			mExpDataSet.insert_or_assign(std::string(typeName), mAnalyser->getAnEventSet(typeName));
		}
	}
}

void ControlExperimentAnalysis::drawHitmap() {
	std::vector<std::string> plotNameSet = mSubConfigSet.find("Basic")->second->getConfigurableNameList();
	for ( std::string_view plotName : plotNameSet ) {
		if ( mSubConfigSet.find("Basic")->second->getConfig(plotName)->find("type") == "hitmap" ) {
			mAnalyser->saveHitmap("Basic", *mHitmapPrivateConfigGenerator(mSubConfigSet.find("Basic")->second->getConfig(plotName)));
		}
	}
	doMasking();
	for ( const std::string& typeName : mTypeNameSet ) {
		if ( typeName != "Basic" ) {
			for ( std::string_view plotName : mSubConfigSet.find(typeName)->second->getConfigurableNameList() ) {
				if ( mSubConfigSet.find(typeName)->second->getConfig(plotName)->find("type") == "hitmap" ) {
					mAnalyser->saveHitmap(typeName, *mHitmapPrivateConfigGenerator(mSubConfigSet.find(typeName)->second->getConfig(plotName)));
				}
			}
		}
	}
}

void ControlExperimentAnalysis::clusterization() {
	for ( std::string_view typeName : mTypeNameSet ) {
		TClusterization clusterization(mExpDataSet.find(std::string(typeName))->second->getEvents());
		clusterization.clusterize();
		mExpDataSet.find(std::string(typeName))->second->setClusters(clusterization.getClusters());
	}
}

void ControlExperimentAnalysis::drawClustermapAndClustersize() {
	mClusterAnalyser = new TClusterAnalyser(*mAnalyser);

	for ( std::string_view typeName : mTypeNameSet ) {
		for ( std::string_view plotName : mSubConfigSet.find(std::string(typeName))->second->getConfigurableNameList() ) {
			if ( mSubConfigSet.find(std::string(typeName))->second->getConfig(plotName)->find("type") == "clustermap" ) {
				mClusterAnalyser->saveClustermap(std::string(typeName), *mHitmapPrivateConfigGenerator(mSubConfigSet.find(std::string(typeName))->second->getConfig(plotName)));
			}
			if ( mSubConfigSet.find(std::string(typeName))->second->getConfig(plotName)->find("type") == "clustersize" ) {
				mClusterAnalyser->saveClustersize(std::string(typeName), *mHitmapPrivateConfigGenerator(mSubConfigSet.find(std::string(typeName))->second->getConfig(plotName)));
			}
		}
	}
}

void ControlExperimentAnalysis::doDivideBySize() {
	mClusterRange = getClusterSizeRange(mConfig->getConfig("ShapeCut"));
	for ( std::string_view typeName : mTypeNameSet ) {
		mClusterAnalyser->doDivideBySize(typeName);
	}
}

void ControlExperimentAnalysis::drawClusterShapeInfos() {
	mClusterShapeAnalyser = new TClusterShapeAnalyser(*mClusterAnalyser);
	for ( std::string_view typeName : mTypeNameSet ) {
		mClusterShapeAnalyser->doShaping(typeName, mClusterRange);
		for ( std::string_view plotName : mSubConfigSet.find(std::string(typeName))->second->getConfigurableNameList() ) {
			if ( mSubConfigSet.find(std::string(typeName))->second->getConfig(plotName)->find("type") == "shape_individual" ) {
				mClusterShapeAnalyser->saveIndividualShapes(std::string(typeName), mIndividualClusterShapePrivateGenerator(mSubConfigSet.find(std::string(typeName))->second->getConfig(plotName)));
			}
			if ( mSubConfigSet.find(std::string(typeName))->second->getConfig(plotName)->find("type") == "shape_same_size" ) {
				mClusterShapeAnalyser->saveSameSizeShapes(std::string(typeName), mSameSizeClusterShapePrivateGenerator(mSubConfigSet.find(std::string(typeName))->second->getConfig(plotName)));
			}
			if ( mSubConfigSet.find(std::string(typeName))->second->getConfig(plotName)->find("type") == "shape_total" ) {
				mClusterShapeAnalyser->saveTotalShapes(std::string(typeName), mTotalClusterShapePrivateGenerator(mSubConfigSet.find(std::string(typeName))->second->getConfig(plotName)));
			}
			if ( mSubConfigSet.find(std::string(typeName))->second->getConfig(plotName)->find("type") == "shape_same_size_entry" ) {
				mClusterShapeAnalyser->saveSameSizeShapeEntry(std::string(typeName), mSameSizeClusterShapeEntryPrivateGenerator(mSubConfigSet.find(std::string(typeName))->second->getConfig(plotName)));
			}
			if ( mSubConfigSet.find(std::string(typeName))->second->getConfig(plotName)->find("type") == "shape_total_entry" ) {
				mClusterShapeAnalyser->saveTotalShapeEntry(std::string(typeName), mTotalClusterShapeEntryPrivateGenerator(mSubConfigSet.find(std::string(typeName))->second->getConfig(plotName)));
			}
		}
	}
}








std::vector<int> ControlExperimentAnalysis::getClusterSizeRange(const Configurable* privateProperty) {
	std::vector<int> clusterSizeRange;
	if ( privateProperty->hasKey("cluster_size_oi") ) {
		for ( const std::string& rangeStr : privateProperty->findlist("cluster_size_oi") ) {
			if ( rangeStr.find('.') != std::string::npos ) {
				for ( int i = stoi(rangeStr.substr(0, rangeStr.find('.'))); i < stoi(rangeStr.substr(rangeStr.find('.') + 3)) + 1; i++ ) {
					clusterSizeRange.push_back(i);
				}
			} else {
				clusterSizeRange.push_back(stoi(rangeStr));
			}
		}
	} else {
		for ( int i = 0; i < 100; i++ ) {
			clusterSizeRange.push_back(i);
		}
	}
	return clusterSizeRange;
}

std::filesystem::path ControlExperimentAnalysis::getOutputPathFromConfig(const Configurable* privateProperty) {
	std::filesystem::path outputPath = std::filesystem::absolute(mSharedProperty->find("output_path"));
	if ( privateProperty->hasKey("subdirectory") ) {
		outputPath /= privateProperty->find("subdirectory");
	}
	outputPath /= privateProperty->find("filename");
	if ( mSharedProperty->hasKey("extension") || privateProperty->hasKey("extension") ) {
		std::string extension = privateProperty->hasKey("extension") ? privateProperty->find("extinsion") : mSharedProperty->find("extension");
		outputPath.replace_extension(extension);
	} else {
		outputPath.replace_extension("root");
	}
	return outputPath;
}

std::string ControlExperimentAnalysis::getPlotTitleFromConfig(const Configurable* privateProperty) {
	std::string plotTitle;
	if ( privateProperty->hasKey("title") ) {
		plotTitle = privateProperty->find("title");
	} else {
		plotTitle = "";
	}
	if ( privateProperty->hasKey("x_title") ) {
		plotTitle += ";" + privateProperty->find("x_title");
	}
	if ( privateProperty->hasKey("y_title") ) {
		plotTitle += ";" + privateProperty->find("y_title");
	}
	if ( privateProperty->hasKey("z_title") ) {
		plotTitle += ";" + privateProperty->find("z_title");
	}
	return plotTitle;
}

std::string ControlExperimentAnalysis::getOptionFromConfig(const Configurable* privateProperty) {
	std::string option;
	if ( privateProperty->hasKey("options") ) {
		option = privateProperty->find("options");
	} else {
		option = "";
	}
	return option;
}

Configurable* ControlExperimentAnalysis::mHitmapPrivateConfigGenerator(const Configurable* privateProperty) {
	Configurable* config = new Configurable("Map");
	// Set path for hitmap plot
	std::filesystem::path outputPath = getOutputPathFromConfig(privateProperty);
	config->addDictionary("file_path", std::string(outputPath));
	// Set title for hitmap plot
	std::string plotTitles = getPlotTitleFromConfig(privateProperty);
	config->addDictionary("plot_titles", plotTitles);
	// Set option for hitmap plot
	std::string option = getOptionFromConfig(privateProperty);
	config->addDictionary("options", option);

	return config;
}

Configurable* ControlExperimentAnalysis::mClustermapPrivateConfigGenerator(const Configurable* privateProperty) {
	Configurable* config = new Configurable("Map");
	// Set path for hitmap plot
	std::filesystem::path outputPath = getOutputPathFromConfig(privateProperty);
	config->addDictionary("file_path", std::string(outputPath));
	// Set title for hitmap plot
	std::string plotTitles = getPlotTitleFromConfig(privateProperty);
	config->addDictionary("plot_titles", plotTitles);
	// Set option for hitmap plot
	std::string option = getOptionFromConfig(privateProperty);
	config->addDictionary("options", option);

	return config;
}

Configurable* ControlExperimentAnalysis::mClustersizePrivateConfigGenerator(const Configurable* privateProperty) {
	Configurable* config = new Configurable("Map");
	// Set path for hitmap plot
	std::filesystem::path outputPath = getOutputPathFromConfig(privateProperty);
	config->addDictionary("file_path", std::string(outputPath));
	// Set title for hitmap plot
	std::string plotTitles = getPlotTitleFromConfig(privateProperty);
	config->addDictionary("plot_titles", plotTitles);
	// Set option for hitmap plot
	std::string option = getOptionFromConfig(privateProperty);
	config->addDictionary("options", option);

	return config;
}


Configurable* ControlExperimentAnalysis::mIndividualClusterShapePrivateGenerator(const Configurable* privateProperty) {
	Configurable* config = new Configurable("Map");
	// Set path for hitmap plot
	std::filesystem::path outputPath = getOutputPathFromConfig(privateProperty);
	config->addDictionary("file_path", std::string(outputPath));
	// Set title for hitmap plot
	std::string plotTitles = getPlotTitleFromConfig(privateProperty);
	config->addDictionary("plot_titles", plotTitles);
	// Set option for hitmap plot
	std::string option = getOptionFromConfig(privateProperty);
	config->addDictionary("options", option);

	return config;
}

Configurable* ControlExperimentAnalysis::mSameSizeClusterShapePrivateGenerator(const Configurable* privateProperty) {
	Configurable* config = new Configurable("Map");
	// Set path for hitmap plot
	std::filesystem::path outputPath = getOutputPathFromConfig(privateProperty);
	config->addDictionary("file_path", std::string(outputPath));
	// Set title for hitmap plot
	std::string plotTitles = getPlotTitleFromConfig(privateProperty);
	config->addDictionary("plot_titles", plotTitles);
	// Set option for hitmap plot
	std::string option = getOptionFromConfig(privateProperty);
	config->addDictionary("options", option);

	return config;
}

Configurable* ControlExperimentAnalysis::mTotalClusterShapePrivateGenerator(const Configurable* privateProperty) {
	Configurable* config = new Configurable("Map");
	// Set path for hitmap plot
	std::filesystem::path outputPath = getOutputPathFromConfig(privateProperty);
	config->addDictionary("file_path", std::string(outputPath));
	// Set title for hitmap plot
	std::string plotTitles = getPlotTitleFromConfig(privateProperty);
	config->addDictionary("plot_titles", plotTitles);
	// Set option for hitmap plot
	std::string option = getOptionFromConfig(privateProperty);
	config->addDictionary("options", option);

	return config;
}

Configurable* ControlExperimentAnalysis::mSameSizeClusterShapeEntryPrivateGenerator(const Configurable* privateProperty) {
	Configurable* config = new Configurable("Map");
	// Set path for hitmap plot
	std::filesystem::path outputPath = getOutputPathFromConfig(privateProperty);
	config->addDictionary("file_path", std::string(outputPath));
	// Set title for hitmap plot
	std::string plotTitles = getPlotTitleFromConfig(privateProperty);
	config->addDictionary("plot_titles", plotTitles);
	// Set option for hitmap plot
	std::string option = getOptionFromConfig(privateProperty);
	config->addDictionary("options", option);

	return config;
}

Configurable* ControlExperimentAnalysis::mTotalClusterShapeEntryPrivateGenerator(const Configurable* privateProperty) {
	Configurable* config = new Configurable("Map");
	// Set path for hitmap plot
	std::filesystem::path outputPath = getOutputPathFromConfig(privateProperty);
	config->addDictionary("file_path", std::string(outputPath));
	// Set title for hitmap plot
	std::string plotTitles = getPlotTitleFromConfig(privateProperty);
	config->addDictionary("plot_titles", plotTitles);
	// Set option for hitmap plot
	std::string option = getOptionFromConfig(privateProperty);
	config->addDictionary("options", option);

	return config;
}