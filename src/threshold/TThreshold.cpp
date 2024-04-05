#include "TThreshold.h"

TThreshold::TThreshold() {}

TThreshold::TThreshold(int x, int y) : mX(x), mY(y) {}

TThreshold::TThreshold(const std::array<int,2>& coordinate) : mX(coordinate[0]), mY(coordinate[1]) {}

TThreshold::TThreshold(int x, int y, const std::array<Int_t, 50>& dacs) : mX(x), mY(y) {
    std::copy(std::begin(dacs), std::end(dacs), std::begin(mDacs));
    calculateThreshold();
}

TThreshold::TThreshold(const std::array<int,2>& coordinate, const std::array<Int_t, 50>& dacs) : mX(coordinate[0]), mY(coordinate[1]) {
    std::copy(std::begin(dacs), std::end(dacs), std::begin(mDacs));
}

TThreshold::TThreshold(int x, int y, std::array<Int_t, 50>&& dacs) : mX(x), mY(y) {
    std::move(std::begin(dacs), std::end(dacs), std::begin(mDacs));
}

TThreshold::TThreshold(const std::array<int,2>& coordinate, std::array<Int_t, 50>&& dacs) : mX(coordinate[0]), mY(coordinate[1]) {
    std::move(std::begin(dacs), std::end(dacs), std::begin(mDacs));
}

TThreshold::TThreshold(const TThreshold& copy) : mX(copy.mX), mY(copy.mY), mThr(copy.mThr), mErr(copy.mErr) {
    std::copy(std::begin(copy.mDacs), std::end(copy.mDacs), std::begin(mDacs));
}

TThreshold& TThreshold::operator=(const TThreshold& copy) {
    mX = copy.mX;
    mY = copy.mY;
    std::copy(std::begin(copy.mDacs), std::end(copy.mDacs), std::begin(mDacs));
    mThr = copy.mThr;
    mErr = copy.mErr;
    return *this;
}

TThreshold::TThreshold(TThreshold&& move) : mX(move.mX), mY(move.mY), mThr(move.mThr), mErr(move.mErr) {
    std::move(std::begin(move.mDacs), std::end(move.mDacs), std::begin(mDacs));
}

TThreshold& TThreshold::operator=(TThreshold&& move) {
    mX = move.mX;
    mY = move.mY;
    std::move(std::begin(move.mDacs), std::end(move.mDacs), std::begin(mDacs));
    mThr = move.mThr;
    mErr = move.mErr;
    return *this;
}

TThreshold::~TThreshold() {}

void TThreshold::calculateThreshold() {
    if (*std::begin(mDacs) > 25 || *(std::end(mDacs)-1) < 25 ) {
        mThr = -1;
        mErr = -1;
    } else {
        std::array<Int_t,50> adcs;
        std::iota(std::begin(adcs), std::end(adcs), 1);
        thresholdGraph.reset(new TGraph(50, std::begin(adcs), std::begin(mDacs)));
        fitFunction.reset(new TF1("fitFunction","[0]*TMath::Erf((x-[1])/[2])+[3]",0.,50.));
        bool quality = false;
        int count = 0;
        while(!quality) {
            fitFunction->SetParameters(20,10*count + 10,10,25);
            thresholdGraph->Fit("fitFunction","q");
            mThr = fitFunction->GetParameter(1);
            mErr = fitFunction->GetParameter(2);
            mQualityFactor = fitFunction->GetChisquare() / fitFunction->GetNDF();
            if (mQualityFactor < 20.) {
                quality = true;
            }
            if (count > 4) {
                mThr = -1;
                mErr = -1;
                break;
            }
            count++;
        }
        if (!quality) {
            std::cout << fitFunction->GetChisquare() / fitFunction->GetNDF() << std::endl;
            savePlot();
        }
    }
}

void TThreshold::savePlot() {
    std::unique_ptr<TCanvas> can(new TCanvas("can","can",500,500));
    thresholdGraph->SetTitle(static_cast<TString>("Threshold Graph at " + std::to_string(mX) + ", " + std::to_string(mY)+"; ADC[$500 \times e^-$]; DAC[# of Fire]"));
    thresholdGraph->Draw();
    can->SaveAs(static_cast<TString>("data/" + std::to_string(mX) + "_" + std::to_string(mY) + ".png"));
}

const double TThreshold::getX() const {
    return mX;
}

const double TThreshold::getY() const {
    return mY;
}

const double TThreshold::getThreshold() const { 
    return mThr;
}

const double TThreshold::getError() const { 
    return mErr;
}

const double TThreshold::getQualityFactor() const {
    return mQualityFactor;
}