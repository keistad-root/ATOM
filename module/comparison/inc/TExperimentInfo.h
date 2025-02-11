#ifndef __TEXPERIMENTINFO__
#define __TEXPERIMENTINFO__

#include<string>
#include<array>

class TExperimentInfo {
public:
	TExperimentInfo();
	~TExperimentInfo();
private:
	std::string mTag;
	int mLength;
	double mCollimatorLength;
	double mCollimatorArea;
	int mMinute;
	std::array<double, 60> mEntry;
	std::array<double, 60> mError;
};

#endif