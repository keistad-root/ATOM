/**
 * @file TMerge.h
 * @author Yongjun Choi (yochoi@cern.ch)
 * @brief Tools for integrate same configue but seperate file
 * @version 0.1
 * @date 17-04-2024
 *
 * @copyright Copyright (c) 2024
 *
*/

#ifndef __TMERGE__
#define __TMERGE__

#include<vector>
#include<string>

class TMerge {
public:
	TMerge(std::string_view outputFileName, const std::vector<std::string>& inputFileNames);
	~TMerge();
private:
	std::string mOutputFileName;
	std::vector<std::string> mInputFileNameSet;

public:
	void checkFile();
	void mergeFileALPIDEROOT();
};

#endif