/**
 * @file TClusterAnalyser.h
 * @author Yongjun Choi (yochoi@cern.ch)
 * @brief Control cluster analysis process and save plots.
 * @version 0.1
 * @date 2024-04-09
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __TCLUSTERANALYSER__
#define __TCLUSTERANALYSER__

#ifdef __TCLUSTERANALYSER_HEADERS__
#include <iostream>

#include "TCanvas.h"
#include "TPaveText.h"
#include "TH1D.h"
#include "TH2D.h"

#include "cppconfig.h"
#include "cpptqdm.h"

#include "TCluster.h"
#include "TExperimentData.h"
#endif

#include <vector>

#include "TAnalyser.h"

class TH1D;
class TH2D;

class Configurable;
class TCluster;

/**
 * @brief Communicating execute file for controlling cluster research.
 * @details It takes ROOT file which is result of experiment.
 * The analysis data about raw data, clusterized data and masked data are stored as data member.
 * From these data, the plots for imformations about hitmap, clustermap, size and shapes are drawn.
 * @warning
 * @bug
 * @todo Add Legend about experiment setting
 * @todo Make more plots about cluster information
 */
class TClusterAnalyser : public TAnalyser {
private:
	TH2D* mClustermap;
	TH2D* mMaskedClustermap;
	TH2D* mNoisePixelClustermap;
protected:
	TH1D* mClustersize;
	TH1D* mMaskedClustersize;
	TH1D* mNoisePixelClustersize;
public:
	//Constructor
	TClusterAnalyser() = default;
	TClusterAnalyser(const TAnalyser& analyser);
	~TClusterAnalyser();

	TH2D* getClusterPlot(const Configurable& config, const std::vector<TCluster*>& clusters);
	TH1D* getClustersizePlot(const Configurable& config, const std::vector<TCluster*>& clusters);

	void saveClustermap(const Configurable& config);
	void saveMaskedClustermap(const Configurable& config);
	void saveNoisePixelClustermap(const Configurable& config);

	void saveClustersize(const Configurable& config);
	void saveMaskedClustersize(const Configurable& config);
	void saveNoisePixelClustersize(const Configurable& config);

	void saveHitmapByClustersize(const Configurable& config);

private:
	unsigned int fBits;
public:
	enum {
		kNotDeleted = 0x02000000
	};
	bool IsDestructed() const { return !TestBit(kNotDeleted); }
	bool TestBit(unsigned int f) const { return (bool) ((fBits & f) != 0); }
};

#endif