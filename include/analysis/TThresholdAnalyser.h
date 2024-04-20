/**
 * @file TThresholdAnalyser.h
 * @author Yongjun Choi (yochoi@cern.ch)
 * @brief The tools for threshold analysis
 * @version 0.1
 * @date 2024-04-13
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __TTHRESHOLDANALYSER__
#define __TTHRESHOLDANALYSER__

#ifdef __TTHRESHOLDANALYSER_HEADER__
#include <iostream>
#include <sstream>

#include "RtypesCore.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"

#include "cpptqdm.h"

#include "TThreshold.h"
#endif

#include <fstream>
 // #include "TThreshold.h"
 // #include "cpptqdm.h"
class TThreshold;
class TH1;
class TH2;

class TThresholdAnalyser {
private:
	std::ifstream mFile; /** Dat file */
	int mVcasn, mIthr; /**< vcasn and ithr value. They are key values for determine threshold */

	std::vector<std::unique_ptr<TThreshold>> mThresholds; /**< The array in which the informations about threshold of ALPIDE are stored */

	std::unique_ptr<TH1> mThresholdDistribution; /**< Threshold value distribution plot*/
	std::unique_ptr<TH1> mErrorDistribution; /**< Error value distribution plot*/
	std::unique_ptr<TH2> mThresholdmap; /**< Thresholdmap*/
	std::unique_ptr<TH1> mChi2NdfDistribution; /** Fitting quality distribution ( Chi2 / Ndof ) */
public:
	TThresholdAnalyser();
	TThresholdAnalyser(std::ifstream& file);
	TThresholdAnalyser(const TThresholdAnalyser& copy);
	TThresholdAnalyser& operator=(const TThresholdAnalyser& copy);
	TThresholdAnalyser(TThresholdAnalyser&& move);
	TThresholdAnalyser& operator=(TThresholdAnalyser&& move);
	~TThresholdAnalyser();

	void openFile(std::ifstream& file);

	void refineData();

	void saveThresholdDistribution(std::string_view title) const;
	void saveErrorDistribution(std::string_view title) const;
	void saveThresholdmap(std::string_view title) const;
	void saveQualityDistribution(std::string_view title) const;
};

#endif