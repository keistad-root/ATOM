#ifndef __TEXPERIMENTINFOSET__
#define __TEXPERIMENTINFOSET__

#include "TExperimentInfo.h"
#include <vector>

class TExperimentInfoSet {
public:
	TExperimentInfoSet();
	~TExperimentInfoSet();
private:
	std::vector<TExperimentInfo> mExperimentSet;
};

#endif