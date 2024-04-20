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

#include "Headers.h"
#include "TFileFormat.h"
#include "cpptqdm.h"

class MergeFileOpen : public std::exception {
public:
	std::string message;
public:
	MergeFileOpen(std::string_view fileName) {
		message = static_cast<std::string>(fileName) + " cannot open";
	}
	const char* what() const throw() {
		return message.c_str();
	}
};

class MergeTreeOpen : public std::exception {
public:
	std::string message;
public:
	MergeTreeOpen(std::string_view fileName) {
		message = "Tree of " + static_cast<std::string>(fileName) + " cannot be accessed";
	}
	const char* what() const throw() {
		return message.c_str();
	}
};

class TMerge {
protected:
	std::string mOutputFileName;
	std::vector<std::string> mInputFileNames;
public:
	TMerge(std::string_view outputFileName, const std::vector<std::string>& inputFileNames);
};

class TMergeExperimentROOT : public TMerge {
private:
	TFile* mOutputFile;
	TInputRoot mInputValue;
	TInputRoot mOutputValue;
public:
	TMergeExperimentROOT(std::string_view outputFileName, const std::vector<std::string>& inputFileNames);
	void mergeFile();
	~TMergeExperimentROOT();
};

#endif