#ifndef __TTHRESHOLD__
#define __TTHRESHOLD__

#ifdef __TTHRESHOLD_HEADER__
#include<numeric>
#include "TCanvas.h"
#include "TGraph.h"
#include "TF1.h"
#endif

#include<array>
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
public:
	//Constructor
	TThreshold();
	TThreshold(int x, int y);
	TThreshold(const std::array<int, 2>& coordinate);
	TThreshold(int x, int y, const std::array<int, 50>& dacs);
	TThreshold(const std::array<int, 2>& coodrdinate, const std::array<int, 50>& dacs);
	TThreshold(int x, int y, std::array<int, 50>&& dacs);
	TThreshold(const std::array<int, 2>& coodrdinate, std::array<int, 50>&& dacs);
	//Copy Constructor
	TThreshold(const TThreshold& copy);
	//Copy Assignment
	TThreshold& operator=(const TThreshold& copy);
	//Move Constructor
	TThreshold(TThreshold&& move);
	//Move Assignment
	TThreshold& operator=(TThreshold&& move);
	//Destructor
	~TThreshold();

	ThrCondition calculateThreshold();
	void savePlot();

	const double getX() const;
	const double getY() const;
	const double getThreshold() const;
	const double getError() const;
	const double getQualityFactor() const;

	const ThrCondition getCondition() const;
};

#endif