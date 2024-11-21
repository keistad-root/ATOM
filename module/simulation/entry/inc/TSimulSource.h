#ifndef __TSIMULSOURCE__
#define __TSIMULSOURCE__

#include<array>
#include<vector>
#include<algorithm>
#include<iostream>
#include<cmath>

#include"TGraph.h"
#include"TCanvas.h"

#include "TSimulObject.h"

class TSimulSource : public TSimulObject {
public:
	TSimulSource();
	TSimulSource(double x, double y, double z, double radius, int step);
private:
	std::array<int, 3> mCenter = {0, 0, 0};
	int mRadius;
	int mStep = 100;

	std::vector<std::pair<int, int>> mCoordinate;
public:
	void extractCoordinate();
	const std::vector<std::pair<int, int>>& getSourceCoordinate() const;

	void saveCoordinate();
};


#endif