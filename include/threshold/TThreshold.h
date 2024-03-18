#ifndef __TTHRESHOLD__
#define __TTHRESHOLD__

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <numeric>

#include "TGraph.h"
#include "TMath.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TH1.h"
#include "TH2.h"

struct ThrFormat {
    int x;
    int y;
    Int_t dac[50];
    double thr;
    double err;
};

struct ThrPlot {
    std::unique_ptr<TH1> threshold;
    std::unique_ptr<TH1> error;
    std::unique_ptr<TH2> map;
};

class TThreshold {
private:
    int vcasn, ithr;
    std::vector<std::unique_ptr<ThrFormat>> datas;
    Int_t defRegion[50];
    std::unique_ptr<TGraph> graph;
    std::unique_ptr<TF1> fitFunc;
public:
    TThreshold() = default;
    TThreshold(std::ifstream& thrFile);

    void inputConfig(int vcasn, int ithr);
    void openThrFile(std::ifstream& thrFile);
    void getThresholds();
    void drawPlot(std::string savePoint);

    void setVcasn(const int vcasn);
    void setIthr(const int ithr);

    const int getVcasn() const;
    const int getIthr() const;
private:
    void calculateThr(const std::unique_ptr<ThrFormat>& data);
};

#endif