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
#include "TBranch.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TError.h"
#include "TPaveText.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TGraph.h"
#include "TF1.h"
#include "TLegend.h"

#include "cpptqdm.h"
#include "CppConfigFile.h"

#include "TExperimentData.h"
#include "TMatrix2D.h"
#include "TALPIDEEvent.h"
#include "TCluster.h"
#include "TClusterDivideData.h"
#include "TClusterShape.h"
#endif

#include<string>
#include<filesystem>
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
	TTree* mTree;
	TInputRoot mInput;


protected:
	TFile* mOutputFile = nullptr;
	bool mIsOutputGraph = false;
	TPaveText* mExpSettingLegend;
	std::unordered_map<std::string, TH2D*> mHitmaps;
	std::unordered_map<std::string, TH2D*> mClustermaps;
	std::unordered_map<std::string, TH1D*> mClustersizes;
	std::unordered_map<std::string, std::unordered_map<int, std::vector<TCluster*>>> mClusterDataWithShape;
	std::unordered_map<std::string, TDirectory*> mDirectorySet;
	std::unordered_map<std::string, TExperimentData*> mExpData;
	std::unordered_map<std::string, TClusterDivideData*> mDivideData;
	std::unordered_map<std::string, std::vector<TClusterShape*>> mClusterShapeSet;
	std::unordered_map<std::string, int> mNTotalShapeSet;
	std::unordered_map<std::string, int> mMaxModeSet;
public:
	TAnalyser() = default;
	TAnalyser(TFile* inputFile, std::unordered_map<std::string, TExperimentData*> expData);
	TAnalyser(const TAnalyser& copy);
	~TAnalyser();

	TTree* openTree(std::string treeName);
	void storeEvents();
	void doMasking(int mMaskOver);
	void openOutputGraphFile(std::string_view fileName);
	void openDirectory(std::string_view typeName);

	// void setSavePath(const std::filesystem::path& savePath);
	void setExpSettingLegend(CppConfigDictionary settingConfig);

	void doDivideBySize(std::string_view typeName);

	TExperimentData* getAnEventSet(std::string_view typeName) const;

	TH2D* getHitPlot(const CppConfigDictionary& config, const std::vector<TALPIDEEvent*>& events);

	void saveHitmap(std::string typeName, const CppConfigDictionary& config);

	TH2D* getClusterPlot(const CppConfigDictionary& config, const std::vector<TCluster*>& clusters);
	TH1D* getClustersizePlot(const CppConfigDictionary& config, const std::vector<TCluster*>& clusters);
	void setClusterDataWithShape(const std::vector<int>& clusterSizeRange);

	void saveClustermap(std::string typeName, const CppConfigDictionary& config);
	void saveClustersize(std::string typeName, const CppConfigDictionary& config);
	std::vector<int> getClusterSizeRange(const CppConfigDictionary& privateProperty);

	void doShaping(std::string_view typeName, const std::vector<int>& clusterSizeRange);
	void saveIndividualShapes(std::string_view typeName, const CppConfigDictionary config);
	void saveSameSizeInfos(std::string_view typeName, const CppConfigDictionary config);
	void saveSameSizeShapes(std::string_view typeName, const CppConfigDictionary config);
	void saveTotalShapes(std::string_view typeName, const CppConfigDictionary config);
	void saveSameSizeShapeEntry(std::string_view typeName, const CppConfigDictionary config);
	void saveTotalShapeEntry(std::string_view typeName, const CppConfigDictionary config);

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