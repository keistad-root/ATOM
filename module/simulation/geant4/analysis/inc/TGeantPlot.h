#ifndef __TGEANTPLOT__
#define __TGEANTPLOT__

#include "TPlotter.h"

class TFile;
class TH1D;
class TH2D;

class TGeantPlot {
public:
	TGeantPlot() = default;
	~TGeantPlot() = default;

private:
	std::filesystem::path mOutputDirectory;
	TFile* mInputFile;
	std::unordered_map<std::string, TH1D*> m1DHistograms;
	std::unordered_map<std::string, TH2D*> m2DHistograms;

	std::array<TString, 12> mParticleName = {"UKN", "#alpha", "e^{-}", "#gamma", "p", "n", "^{12}C", "^{13}C", "N", "^{16}O", "^{18}O", "Si"};
	std::array<TString, 7> mVolumeName = {"UKN", "ALPIDE Metal", "ALPIDE Epitaxial", "ALPIDE Substrate", "Collimator", "Al Shield", "World"};

public:
	void readInputFile(std::filesystem::path inputFilePath);
	void getHistorams();
	void setOutputDirectory(std::string_view outputDirectory);
	void saveHistorams(const std::vector<CppConfigDictionary>& configList);
	void getEntries();
};

#endif