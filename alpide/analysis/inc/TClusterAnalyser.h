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

#include "TFile.h"
#include "TDirectory.h"
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
protected:
	std::unordered_map<std::string, TH2D*> mClustermaps;
	std::unordered_map<std::string, TH1D*> mClustersizes;
	std::unordered_map<std::string, std::unordered_map<int, std::vector<TCluster*>>> mClusterDataWithShape;
public:
	//Constructor
	TClusterAnalyser() = default;
	TClusterAnalyser(const TAnalyser& analyser);
	TClusterAnalyser(const TClusterAnalyser& copy);
	~TClusterAnalyser();

	TH2D* getClusterPlot(const Configurable& config, const std::vector<TCluster*>& clusters);
	TH1D* getClustersizePlot(const Configurable& config, const std::vector<TCluster*>& clusters);
	void setClusterDataWithShape(const std::vector<int>& clusterSizeRange);

	void saveClustermap(std::string typeName, const Configurable& config);
	void saveClustersize(std::string typeName, const Configurable& config);
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