#ifndef __TTHRESHOLD__
#define __TTHRESHOLD__

#ifdef __TTHRESHOLD_HEADER__
#include<numeric>
#include "TCanvas.h"
#include "TGraph.h"
#include "TF1.h"
#endif

#include<array>
#include<filesystem>
#include<algorithm>

class TGraph;
class TF1;

enum ThrCondition {
	good,
	bad_too_low,
	bad_too_high,
	bad_undefine
};

class TThreshold {
private:
	int mX;
	int mY;
	std::array<int, 50> mDacs;
	double mThr;
	double mErr;
	double mQualityFactor;

	ThrCondition mCondition;

	std::unique_ptr<TGraph> thresholdGraph;
	std::unique_ptr<TF1> fitFunction;
	std::vector<int> mLocalMaximum;
public:
	//Constructor
	TThreshold();
	TThreshold(int x, int y, const std::array<int, 50>& dacs);
	~TThreshold();

	ThrCondition calculateThreshold();
	const void savePlot(const std::string& savePath) const;

	void findLocalMaximum();
	const double getX() const;
	const double getY() const;
	const double getThreshold() const;
	const double getError() const;
	const double getQualityFactor() const;
	const int getNLocalMaximumPoint() const;

	const ThrCondition getCondition() const;
};

#endif