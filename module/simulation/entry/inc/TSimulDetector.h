#ifndef __TSIMULDETECTOR__
#define __TSIMULDETECTOR__

#include<array>

#include"TTrack.h"
#include"TSimulObject.h"

#include"TGraph.h"
#include"TCanvas.h"

class TSimulDetector : public TSimulObject {
public:
	TSimulDetector();
	TSimulDetector(double x, double y, double z, double width, double height);
private:
	std::array<int, 3> mCenter;
	std::array<int, 2> mDimension;
public:
	bool isInclude(TTrack* track);

	void saveDetector();
	void saveCoordinate();
};

#endif