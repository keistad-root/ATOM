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
public:
	//Constructor
	TThreshold(int x, int y, const std::array<int, 50>& dacs);
	~TThreshold();
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
public:
	ThrCondition calculateThreshold();

	double getX() const;
	double getY() const;
	double getThreshold() const;
	double getError() const;
	double getQualityFactor() const;

	ThrCondition getCondition() const;

	const std::unique_ptr<TGraph>& getThresholdGraph() const;
	const std::unique_ptr<TF1>& getFitFunction() const;
};

#endif