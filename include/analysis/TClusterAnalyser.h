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

#include "Headers.h"

#include "TALPIDEEvent.h"
#include "TAnalyser.h"
#include "TClusterN.h"
#include "TCluster.h"
#include "TClusterization.h"
#include "TFileFormat.h"
#include "cppconfig.h"

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
	TTree* mTree; /**<Storing TTree object named as `hits`. It contains `ChipID`, `TimeStamp`, X` and `Y` branches */
	TInputRoot mInput; /**<Object for TInputRoot structure. The kinds of branches are stored in here */
	std::vector<TALPIDEEvent*> mEvents; /**<The array for storing data from raw event */
	std::vector<TALPIDEEvent*> mMaskedEvents; /**<The array for data excluding noises */
	std::vector<TCluster*> mClusters; /**< The array for storing clusterized data from raw event */
	std::vector<TCluster*> mMaskedClusters; /**< The array for storing clusterized data excluding noises */
	std::vector<std::pair<int, int>> mHotPixels; /**< The noise pixel informations */
	std::unordered_map<std::string, TObject*> mPlots; /**< The plot lists*/
	std::vector<TClusterN*> mClusterN; /**< Stored data for cluster shape*/
	std::vector<std::vector<TImage*>> mShapeImages; /**< The array storing cluster shape image*/
	bool mIsMask = false; /**< bool variable whether doing mask or not */
	int mMaskOver = 0; /**< Crieteria for masking */
	TLegend* settingLegend; /**< The setting parameters legend for plots */

public:
	//Constructor
	TClusterAnalyser() = delete;
	TClusterAnalyser(TFile* file);
	TClusterAnalyser(Configuration* conf);
	~TClusterAnalyser();

	void openRootFile();
	void storeRawEvents();
	void refineData();

	void savePlots();

private:
	void setMask(int hot);
	void masking();
	bool isHot(const std::pair<int, int>& pixel);

	void setSettingLegend();
	void saveMaskingFile(std::string title);
	void saveHitmap(const std::filesystem::path& savePath, TFile* saveRootFile);
	void saveClustermap(const std::filesystem::path& savePath, TFile* saveRootFile);
	void saveClustersize(const std::filesystem::path& savePath, TFile* saveRootFile);
	void saveMaskedHitmap(const std::filesystem::path& savePath, TFile* saveRootFile);
	void saveMaskedClustermap(const std::filesystem::path& savePath, TFile* saveRootFile);
	void saveMaskedClustersize(const std::filesystem::path& savePath, TFile* saveRootFile);
	void saveShapes(const std::filesystem::path& savePath, TFile* saveRootFile);
	void saveShapeInformation(const std::filesystem::path& savePath, TFile* saveRootFile);
};

#endif