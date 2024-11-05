#ifndef __TSIMULCOLLIMATOR__
#define __TSIMULCOLLIMATOR__

#include<array>

#include "TSimulObject.h"
#include "TTrack.h"

class TSimulCollimator : public TSimulObject {
private:
	std::array<int, 3> mUpperCenter;
	std::array<int, 3> mLowerCenter;
	int mUpperRadius;
	int mLowerRadius;
public:
	TSimulCollimator(double uX, double uY, double uZ, double uRadius, double lX, double lY, double lZ, double lRadius);

	bool isInclude(TTrack* track);
};

#endif