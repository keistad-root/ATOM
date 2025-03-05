#ifndef __TGEANTEXTRACT__
#define __TGEANTEXTRACT__

#include<filesystem>
#include<memory>
#include "CppConfigFile.h"

#include "TFile.h"
#include "TTree.h"
#include "cpptqdm.h"

#include "TGeantTuple.h"
class TGeantExtract {
public:
	TGeantExtract(const CppConfigFile& config);
	~TGeantExtract();
private:
	std::filesystem::path mInputFileParentPath;
	std::string mInputFileStem;
	std::string mInputFileExtension;

	std::filesystem::path mPrimaryOutputFilePath;
	std::filesystem::path mIncidentOutputFilePath;
	std::filesystem::path mSecondaryOutputFilePath;

	bool mIsDivide = false;
	int mNumFile = 1;

	// From input file
	std::unique_ptr<TFile> mInputFile;
	TTree* mTrackTree;
	TTrackTuple mTrackTuple;
	TTree* mIncidentTree;
	TIncidentTuple mIncidentTuple;

	// For output file
	std::unique_ptr<TFile> mPrimaryAnalysisOutputFile;
	std::unique_ptr<TTree> mPrimaryAnalysisTree;
	TPrimaryAnalysisTuple mPrimaryAnalysisTuple;
	std::unique_ptr<TFile> mIncidentAnalysisOutputFile;
	std::unique_ptr<TTree> mIncidentAnalysisTree;
	TIncidentAnalysisTuple mIncidentAnalysisTuple;
	std::unique_ptr<TFile> mSecondaryAnalysisOutputFile;
	std::unique_ptr<TTree> mSecondaryAnalysisTree;
	TSecondaryAnalysisTuple mSecondaryAnalysisTuple;

public:
	void openInputFile();
	void openOutputFile();

	void extractTrack();
private:
	void initTrackTree();
	void initIncidentTree();

	void initPrimaryAnalysisTree();
	void initIncidentAnalysisTree();
	void initSecondaryAnalysisTree();
	void getPrimaryAnalysisInformation();
	void getIncidentAnalysisInformation();
	void getSecondaryAnalysisInformation();

	void extractFromAFile();
};

#endif