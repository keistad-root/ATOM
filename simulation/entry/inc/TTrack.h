#ifndef __TTRACK__
#define __TTRACK__

#include<array>
#include<cmath>

#include<iostream>

class TTrack {
private:
	std::array<int, 3> mPoint;
	std::array<double, 2> mAngle;
	double mWeight;
public:
	TTrack(int x, int y, int z, double theta, double phi);
	std::array<int, 2> getXYposition(const int z);

	double getWeight();
};

#endif