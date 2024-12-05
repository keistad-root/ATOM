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
#include "TH1User.h"

#include "cpptqdm.h"

#include "Constants.h"

#include "TExperimentData.h"
#include "TMatrix2D.h"
#include "TALPIDEEvent.h"
#include "TCluster.h"
#include "TClusterDivideData.h"
#include "TClusterShape.h"
#include "TClusterization.h"

#include<string>
#include<fstream>
#include<iostream>
#include<filesystem>
#include<unordered_map>
#include<vector>
#include<array>
#include "TFileFormat.h"
#include "CppConfigFile.h"

 /**
  * @class TAnalyser
  * @brief For ROOT and config file when analysis
  * @details It stores ROOT file and Config file.
  * It provides open and access such kind files.
  * It is made for being mother class of Analysis class.
  */
class TAnalyser {
private:
	TFile* mInputFile;
	TFile* mGraphFile;
	TTree* mTree;
	TInputRoot mInput;


	// TFile* mInputFile = nullptr; /**< Input file with ROOT extension. */
	// TFile* mGraphFile = nullptr;
	// TTree* mTree = nullptr;
	// TInputRoot mInput;

	std::filesystem::path mOutputPath;

	CppConfigFile mConfigFile;
	CppConfigDictionary mFileConfig;
	CppConfigDictionary mMaskingConfig;
	CppConfigDictionary mBasicAnalysisConfig;
	CppConfigDictionary mSettingConfig;

	std::vector<std::pair<int, int>> mMaskedPixelSet;
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


	std::array<int, 4> roiSet;
	std::vector<int> nPixelPerFrame;

	bool isGraphFile = false;
	bool isTimeStampCut = false;
	bool isROICut = false;
	bool isHitmapCut = false;
	bool isHighClusterCut = false;
	bool isPixelPerTimeStampMap = false;
	bool isPedestalPrint = false;
	bool isExpSetting = false;
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

	TH1D* mOriginClustersize;
	TH1D* mHHCutClustersize;
	TH1D* mHHHCCutClustersize;

public:
	TAnalyser(const CppConfigFile& configFile);
	~TAnalyser();

	void setConfig();
	void storeEvents();
	void getHitmaps();

	void setROI();
	void originMasking();
	void doMasking();
	void HCMasking();
	void openOutputGraphFile(std::string_view fileName);

	// void setSavePath(const std::filesystem::path& savePath);
	void setExpSettingLegend();

	void getOriginHitmap();
	void getHitmapMaskedHitmap();
	void getHitmapMaskedNoisemap();

	void clusterize();

	void getClusterinfos();
	void getOriginClustermap();
	void getHHCutClustermap();
	void getHHHCCutClustermap();

	void getOriginClustersize();
	void getHHCutClustersize();
	void getHHHCCutClustersize();

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