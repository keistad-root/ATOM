#ifndef __TTHRESHOLDANALYSER__
#define __TTHRESHOLDANALYSER__

#include "Headers.h"
#include "TThreshold.h"
#include "cpptqdm.h"

class TThresholdAnalyser {
private:
    std::ifstream mFile;
    int mVcasn, mIthr;

    std::vector<std::unique_ptr<TThreshold>> mThresholds;

    std::unique_ptr<TH1> mThresholdDistribution;
    std::unique_ptr<TH1> mErrorDistribution;
    std::unique_ptr<TH2> mThresholdmap;
    std::unique_ptr<TH1> mChi2NdfDistribution;
public:
    TThresholdAnalyser();
    TThresholdAnalyser(std::ifstream& file);
    //Copy Constructor
    TThresholdAnalyser(const TThresholdAnalyser& copy);
    //Copy Assignment
    TThresholdAnalyser& operator=(const TThresholdAnalyser& copy);
    //Move Constructor
    TThresholdAnalyser(TThresholdAnalyser&& move);
    //Move Assignment
    TThresholdAnalyser& operator=(TThresholdAnalyser&& move);
    //Destructor
    ~TThresholdAnalyser();

    void openFile(std::ifstream& file);

    void refineData();

    void saveThresholdDistribution(std::string_view title) const;
    void saveErrorDistribution(std::string_view title) const;
    void saveThresholdmap(std::string_view title) const;
    void saveQualityDistribution(std::string_view title) const;
};

#endif