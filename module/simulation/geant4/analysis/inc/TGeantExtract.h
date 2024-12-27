#ifndef __TGEANTEXTRACT__
#define __TGEANTEXTRACT__

#include<filesystem>
#include "CppConfigFile.h"

#include "TFile.h"
#include "TTree.h"
#include "cpptqdm.h"

#include "TGeantTuple.h"
class TGeantExtract {
public:
	TGeantExtract(const CppConfigDictionary& config);
	~TGeantExtract();
private:
	std::filesystem::path mOutputPath;

	// From input file
	std::unique_ptr<TFile> mInputFile;
	std::unique_ptr<TTree> mTrackTree;
	TTrackTuple mTrackTuple;
	std::unique_ptr<TTree> mIncidentTree;
	TIncidentTuple mIncidentTuple;

	// For output file
	std::unique_ptr<TFile> mPrimaryAnalysisOutputFile;
	std::unique_ptr<TTree> mPrimaryAnalysisTree;
	TPrimaryAnalysisTuple mPrimaryAnalysisTuple;
	std::unique_ptr<TFile> mIncidentAnalysisOutputFile;
	std::unique_ptr<TTree> mIncidentAnalysisTree;
	TIncidentAnalysisTuple mIncidentAnalysisTuple;

public:
	void initTrackTree();
	void initIncidentTree();

	void initPrimaryAnalysisTree();
	void initIncidentAnalysisTree();

	void extractTrack();
	void getPrimaryAnalysisInformation();
	void getIncidentAnalysisInformation();

};

#endif