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

#include "Headers.h"
#include "TKey.h"
#include "TObject.h"
#include "cppconfig.h"
#include "TFileFormat.h"
#include "TExperimentData.h"

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
	TH2D* mHitmap;
protected:
	TExperimentData* mExpData;
	std::filesystem::path mSavePath;
	TPaveText* mExpSettingLegend;
public:
	TAnalyser() = default;
	TAnalyser(TFile* inputFile, TExperimentData* expData);
	TAnalyser(const TAnalyser& copy);
	~TAnalyser();

	void storeEvents();

	TTree* openTree(std::string treeName);
	void setSavePath(const std::filesystem::path& savePath);
	void setExpSettingLegend(Configurable settingConfig);
	void saveHitmap();
	void saveMaskedHitmap();
	void doMasking(int mMaskOver);
};

#endif