#ifndef __TGEANTPLOT__
#define __TGEANTPLOT__

#include "TPlotter.h"

class TFile;

class TGeantPlot : public TPlotter {
public:
	TGeantPlot() = default;
	~TGeantPlot();

private:
	std::filesystem::path mOutputDirectory;
	std::unique_ptr<TFile> mInputFile;
	std::unordered_map<std::string, std::unique_ptr<TH1D>> m1DHistograms;
	std::unordered_map<std::string, std::unique_ptr<TH2D>> m2DHistograms;

public:
	void readInputFile(std::filesystem::path inputFilePath);
	void getHistorams();
	void setOutputDirectory(std::string_view outputDirectory);
	void saveHistorams(const std::vector<CppConfigDictionary>& configList);
	void getEntries();
};

#endif