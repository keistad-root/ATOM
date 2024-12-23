#include "TGeantComparison.h"

TGeantComparison::TGeantComparison(const CppConfigFile& config) : TPlotter(&config), mConfig(config) {
	mOutputPath = mConfig.getConfig("File").find("output_directory");


}