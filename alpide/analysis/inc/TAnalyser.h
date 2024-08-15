/**
 * @file TAnalyser.h
 * @author Yongjun Choi (yochoi@cern.ch)
 * @brief The class for controlling ROOT and config file when analysing
 * @version 0.1
 * @date 13-04-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __TANALYSER__
#define __TANALYSER__

#ifdef __TANALYSER_HEADERS__
#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TPaveText.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TGraph.h"
#include "TF1.h"
#include "TLegend.h"
#include "TH1IUser.h"

#include "cpptqdm.h"
#include "CppConfigFile.h"

#include "TExperimentData.h"
#include "TMatrix2D.h"
#include "TALPIDEEvent.h"
#include "TCluster.h"
#include "TClusterDivideData.h"
#include "TClusterShape.h"
#include "TClusterization.h"
#endif

#include<string>
#include<filesystem>
#include<unordered_map>
#include<vector>
#include "TFileFormat.h"

class TFile;
class TTree;
class TH2D;
class TH1D;
class TPaveText;

class Configurable;

// struct TInputRoot;
class TALPIDEEvent;
class TExperimentData;
class TClusterDivideData;
class TClusterShape;
class TDirectory;

class CppConfigDictionary;
class TCluster;
typedef unsigned int UInt_t;

/**
 * @class TAnalyser
 * @brief For ROOT and config file when analysis
 * @details It sotre ROOT file and Config file.
 * It provide open and access such kind files.
 * It is made for being mother class of Analysis class.
 * @warning
 * @bug
 * @todo Add template for plots. Map, distribution, etc.
 */
class TAnalyser {
private:
	TFile* mInputFile = nullptr; /**< Input file with ROOT extension. */
	TTree* mTree = nullptr;
	TInputRoot mInput;

	std::filesystem::path mOutputPath;

	CppConfigFile mConfigFile;
	CppConfigDictionary mFileConfig;
	CppConfigDictionary mMaskingConfig;
	CppConfigDictionary mBasicAnalysisConfig;
	CppConfigDictionary mSettingConfig;

	std::vector<TALPIDEEvent*> mOriginEventSet;
	std::vector<TCluster*> mOriginClusterSet;
	// Cut methods: TS(TimeStamp), HH(High Hits), HC(High Clusters)
	// Adjusting Only Time Stamp Cut
	std::vector<TALPIDEEvent*> mTSCutEventSet;
	std::vector<TALPIDEEvent*> mTSCutNoiseEventSet;
	std::vector<TCluster*> mTSCutClusterSet;
	std::vector<TCluster*> mTSCutNoiseClusterSet;
	// Adjusting Only High Hit Cut
	// std::vector< TALPIDEEvent*> mTS
	std::vector<TALPIDEEvent*> mHHCutEventSet;
	std::vector<TALPIDEEvent*> mHHCutNoiseEventSet;
	std::vector<TCluster*> mHHCutClusterSet;
	std::vector<TCluster*> mHHCutNoiseClusterSet;
	std::vector<TCluster*> mHHHCCutClusterSet;
	std::vector<TCluster*> mHHHCCutNoiseClusterSet;

	std::vector<int> nPixelPerFrame;

	bool isTimeStampCut = false;
	bool isHitmapCut = false;
	bool isHighClusterCut = false;
	bool isPixelPerTimeStampMap = false;
	int mTimeStampCut = 0;
	int mHitmapCut = 0;
	int mHighClusterCut = 0;

	TPaveText* mExpSettingLegend;

	TH1D* mPixelPerTimeStampMap;
	TH2D* mOriginHitmap;
	TH2D* mHitmapMaskedHitmap;
	TH2D* mHitmapMaskedNoisemap;

	TH2D* mOriginClustermap;
	TH2D* mHHCutClustermap;
	TH2D* mHHHCCutClustermap;

public:
	TAnalyser(const CppConfigFile& configFile);
	~TAnalyser();

	void setConfig();
	void storeEvents();
	void getHitmaps();

	void doMasking();
	void HCMasking();
	void openOutputGraphFile(std::string_view fileName);

	// void setSavePath(const std::filesystem::path& savePath);
	void setExpSettingLegend();

	void getOriginHitmap();
	void getHitmapMaskedHitmap();
	void getHitmapMaskedNoisemap();

	void clusterize();

	void getClustermaps();
	void getOriginClustermap();
	void getHHCutClustermap();
	void getHHHCCutClustermap();

private:
	UInt_t fBits;
public:
	enum {
		kNotDeleted = 0x02000000
	};
	bool IsDestructed() const { return !TestBit(kNotDeleted); }
	bool TestBit(UInt_t f) const { return (bool) ((fBits & f) != 0); }
};

#endif