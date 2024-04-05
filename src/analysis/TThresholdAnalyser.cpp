#include "TThresholdAnalyser.h"

TThresholdAnalyser::TThresholdAnalyser() {}

TThresholdAnalyser::TThresholdAnalyser(std::ifstream& file) {
    openFile(file);
}

TThresholdAnalyser::TThresholdAnalyser(const TThresholdAnalyser& copy) : mVcasn(copy.mVcasn), mIthr(copy.mIthr) {
    for (const std::unique_ptr<TThreshold>& threshold : copy.mThresholds) {
        std::unique_ptr<TThreshold> temp(threshold.get());
        mThresholds.push_back(std::move(temp));
    }
    mThresholdDistribution.reset(copy.mThresholdDistribution.get());
    mErrorDistribution.reset(copy.mErrorDistribution.get());
    mThresholdmap.reset(copy.mThresholdmap.get());
}

TThresholdAnalyser& TThresholdAnalyser::operator=(const TThresholdAnalyser& copy) {
    mVcasn = copy.mVcasn;
    mIthr = copy.mIthr;
    for (const std::unique_ptr<TThreshold>& threshold : copy.mThresholds) {
        std::unique_ptr<TThreshold> temp(threshold.get());
        mThresholds.push_back(std::move(temp));
    }
    mThresholdDistribution.reset(copy.mThresholdDistribution.get());
    mErrorDistribution.reset(copy.mErrorDistribution.get());
    mThresholdmap.reset(copy.mThresholdmap.get());
    return *this;
}

TThresholdAnalyser::TThresholdAnalyser(TThresholdAnalyser&& move) : mVcasn(move.mVcasn), mIthr(move.mIthr) {
    std::move(move.mThresholds.begin(), move.mThresholds.end(), mThresholds.begin());
    mThresholdDistribution.reset(move.mThresholdDistribution.get());
    mErrorDistribution.reset(move.mErrorDistribution.get());
    mThresholdmap.reset(move.mThresholdmap.get());
}

TThresholdAnalyser& TThresholdAnalyser::operator=(TThresholdAnalyser&& move) {
    mVcasn = move.mVcasn;
    mIthr = move.mIthr;
    std::move(move.mThresholds.begin(), move.mThresholds.end(), mThresholds.begin());
    mThresholdDistribution.reset(move.mThresholdDistribution.get());
    mErrorDistribution.reset(move.mErrorDistribution.get());
    mThresholdmap.reset(move.mThresholdmap.get());
    return *this;
}

TThresholdAnalyser::~TThresholdAnalyser() {
    std::cout << "The threshold analyser is terminated" << std::endl;
}

void TThresholdAnalyser::openFile(std::ifstream& file) {
    std::string line;
    std::array<Int_t, 50> dacs;
    std::streampos originalPos = file.tellg();
    int numLines = 0;
    char ch;
    while (file.get(ch)) {
        if (ch == '\n') {
            ++numLines;
        }
    }
    file.clear();
    file.seekg(originalPos);
    ProgressBar bar(numLines + 1);
    while(file) {
        bar.printProgress();
        getline(file, line);
        std::stringstream values(line);
        int x, y, iDac, dac;
        values >> y >> x >> iDac >> dac;
        dacs[iDac] = dac;
        if (iDac == 49) {
            std::unique_ptr<TThreshold> temp(new TThreshold(x,y,dacs));
            mThresholds.push_back(std::move(temp));
            temp.release();
        }
    }
}

void TThresholdAnalyser::refineData() {
    mThresholdDistribution.reset(new TH1D("thrDist", "Threshold Distribution", 500, 0, 50));
    mErrorDistribution.reset(new TH1D("errDist", "Error Distribution", 300, 0, 30));
    mThresholdmap.reset(new TH2D("thrmap", "Threshold map", 1024, -0.5, 1023.5, 512, -0.5, 511.5));
    mChi2NdfDistribution.reset(new TH1D("chi2", "Chi2 and NDoF", 200,0,200));

    for (const std::unique_ptr<TThreshold>& threshold : mThresholds) {
        if (threshold->getThreshold() > 0) {
            mThresholdDistribution->Fill(threshold->getThreshold());
            mErrorDistribution->Fill(threshold->getError());
            mThresholdmap->SetBinContent(threshold->getX(),threshold->getY(),threshold->getThreshold());
            mChi2NdfDistribution->Fill(threshold->getQualityFactor());
        }
    }
}

void TThresholdAnalyser::saveThresholdDistribution(std::string_view title) const {
    std::unique_ptr<TCanvas> can(new TCanvas("thrDist","Threshold Distribution; Threshold [$500 \times e^-$]; Entry", 1000, 500));
    mThresholdDistribution->Draw();
    can->SaveAs(static_cast<const TString>(title));
}

void TThresholdAnalyser::saveErrorDistribution(std::string_view title) const {
    std::unique_ptr<TCanvas> can(new TCanvas("errDist","Error Distribution;Threshold [$e^-$]; Entry", 1000, 500));
    mErrorDistribution->Draw();
    can->SaveAs(static_cast<const TString>(title));
}

void TThresholdAnalyser::saveThresholdmap(std::string_view title) const {
    std::unique_ptr<TCanvas> can(new TCanvas("thrmap","Threshold Distribution; rows; coloums", 1000, 500));
    mThresholdmap->Draw();
    can->SaveAs(static_cast<const TString>(title));
}

void TThresholdAnalyser::saveQualityDistribution(std::string_view title) const {
    std::unique_ptr<TCanvas> can(new TCanvas("chi2","Fit quality Distribution; Chi2/NDoF; entry", 1000, 500));
    mChi2NdfDistribution->Draw();
    can->SetLogx();
    can->SetLogy();
    can->SaveAs(static_cast<const TString>(title));
}