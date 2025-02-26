#ifndef __TEXPERIMENTINFOSET__
#define __TEXPERIMENTINFOSET__

#include "TExperimentInfo.h"
#include <vector>

class TExperimentInfoSet {
public:
	TExperimentInfoSet();
private:
	std::vector<TExperimentInfo> mExperimentSet;

public:
	const std::vector<TExperimentInfo>& getExperimentSet() const;
	const TExperimentInfo& getExperimentInfo(const std::string& tag) const;
};

#endif