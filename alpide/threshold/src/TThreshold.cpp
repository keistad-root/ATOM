#define __TTHRESHOLD_HEADER__
#include "TThreshold.h"

TThreshold::TThreshold() { }

TThreshold::TThreshold(int x, int y, const std::array<int, 50>& dacs) : mX(x), mY(y) {
	std::copy(std::begin(dacs), std::end(dacs), std::begin(mDacs));
	mCondition = calculateThreshold();
}

TThreshold::~TThreshold() { }

ThrCondition TThreshold::calculateThreshold() {
	if ( *std::begin(mDacs) > 25 ) {
		mThr = 0;
		mErr = 0;
		return ThrCondition::bad_too_low;
	} else if ( *(std::end(mDacs) - 1) < 25 ) {
		mThr = 100;
		mErr = 100;
		return ThrCondition::bad_too_high;
	} else {
		std::array<int, 50> adcs;
		std::iota(std::begin(adcs), std::end(adcs), 1);
		thresholdGraph.reset(new TGraph(50, std::begin(adcs), std::begin(mDacs)));
		fitFunction.reset(new TF1("fitFunction", "[0]*TMath::Erf((x-[1])/[2])+[3]", 0., 50.));
		bool quality = false;
		int count = 0;
		while ( !quality ) {
			fitFunction->SetParameters(25, 10 * count + 10, 10, 25);
			fitFunction->SetParLimits(0, 24.99, 25.11);
			fitFunction->SetParLimits(3, 24.99, 25.11);
			thresholdGraph->Fit("fitFunction", "q");
			mThr = fitFunction->GetParameter(1);
			mErr = fitFunction->GetParameter(2);
			mQualityFactor = fitFunction->GetChisquare() / fitFunction->GetNDF();
			if ( mQualityFactor < 100. ) {
				quality = true;
			}
			if ( count > 4 ) {
				mThr = -1;
				mErr = -1;
				return ThrCondition::bad_undefine;
			}
			count++;
		}
		if ( mThr > 0 && mThr < 50 ) {
			return ThrCondition::good;
		} else {
			return ThrCondition::bad_undefine;
		}
	}

	findLocalMaximum();
}

const void TThreshold::savePlot(const std::string& path) const {
	std::unique_ptr<TCanvas> can(new TCanvas("can", "can", 500, 500));
	thresholdGraph->SetTitle(static_cast<TString>("Threshold Graph at " + std::to_string(mX) + ", " + std::to_string(mY) + "; ADC; DAC"));
	thresholdGraph->Draw();
	std::filesystem::path savePath = path;
	std::filesystem::create_directories(savePath);
	savePath /= (std::to_string(mX) + "_" + std::to_string(mY) + "+" + std::to_string(mQualityFactor)
				 + ".png");
	can->SaveAs(static_cast<TString>(savePath));
}

void TThreshold::findLocalMaximum() {
	int adc = 0;
	int preDac = 0;
	int prePreDac = 0;
	for ( const int dac : mDacs ) {
		if ( dac < preDac && prePreDac < preDac ) {
			mLocalMaximum.push_back(adc);
		}
		prePreDac = preDac;
		preDac = dac;
		adc++;
	}
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

const ThrCondition TThreshold::getCondition() const {
	return mCondition;
}

const int TThreshold::getNLocalMaximumPoint() const {
	return mLocalMaximum.size();
}