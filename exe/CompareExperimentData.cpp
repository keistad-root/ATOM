#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

#include "cppargs.h"
#include "CppConfigFile.h"

#include "TGraphCompare.h"

class ControlExperimentComparison {
private:
	ArgumentParser mParser;
	CppConfigFile* mConfig;
	std::vector<std::string> mFileSet;
	std::vector<std::string> mTypeNameSet;
	TGraphCompare* mCompare;
public:
	ControlExperimentComparison(int argc, char** argv);
	void setConfig();
	void initComparison();
};

ControlExperimentComparison::ControlExperimentComparison(int argc, char** argv) : mParser(argc, argv) {
	mParser.setDescription("Draw plots for analysis data");
	mParser.add_argument("config").help("Config file").set_default("default").add_finish();
	mParser.parse_args();
}

void ControlExperimentComparison::setConfig() {
	std::string configPath = mParser.get_value<std::string>("config");

	mConfig = new CppConfigFile(configPath);
	mTypeNameSet = mConfig->getConfig("File").getSubConfig("type_name").getValueList();
}

void ControlExperimentComparison::initComparison() {
	std::filesystem::path inputPath = mConfig->getConfig("FileList").find("input_path");
	std::vector<std::string> inputNameSet = mConfig->getConfig("FileList").getSubConfig("input_file_name").getValueList();
	for ( std::string_view inputName : inputNameSet ) {
		mFileSet.push_back(std::string((inputPath / inputName).replace_extension(".root")));
	}
	mCompare = new TGraphCompare(mFileSet);
	// mCompare->setObject(mConfig->getConfig("DrawingObject"));
	for ( const std::string& typeName : mTypeNameSet ) {
		CppConfigDictionary typeConfig = mConfig->getConfig(typeName);
		if ( typeConfig.hasKey("clustersize") ) {
			CppConfigDictionary shapeConfig = typeConfig.getSubConfig("clustersize");
			shapeConfig += mConfig->getConfig("SharedProperty");
			mCompare->TCompareClusterSize(typeName, shapeConfig);
		}
		// if ( mConfig->hasConfig("clustersize_ratio") ) {
		// 	CppConfigDictionary shapeConfig = mConfig->getConfig("clustersize_ratio");
		// 	shapeConfig += mConfig->getConfig("SharedProperty");
		// 	mCompare->TCompareClusterSizeRatio(typeName, shapeConfig);
		// }
		// if ( mConfig->hasConfig("clustersize_3D") ) {
		// 	CppConfigDictionary shapeConfig = mConfig->getConfig("clustersize_3D");
		// 	shapeConfig += mConfig->getConfig("SharedProperty");
		// 	mCompare->TCompareClusterSize3D(typeName, shapeConfig);
		// }
	}
}

int main(int argc, char** argv) {
	ControlExperimentComparison controller(argc, argv);
	controller.setConfig();
	controller.initComparison();
}