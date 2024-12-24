#include <iostream>
#include <vector>
#include <string>

#include "CppConfigFile.h"
#include "TMerge.h"

int main(int argc, char** argv) {
	CppConfigFile config(argv[1]);

	std::string outputFilePath = config.getConfig("Merge").find("output_file");

	std::vector<std::string> inputFilePathList = config.getConfig("Merge").getSubConfig("input_files").getValueList();

	std::sort(inputFilePathList.begin(), inputFilePathList.end());
	TMerge merge(outputFilePath, inputFilePathList);
	merge.checkFile();
	merge.mergeFileALPIDEROOT();

	return 0;
}