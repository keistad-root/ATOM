#ifndef __TGEANTINFOSET__
#define __TGEANTINFOSET__

#include<vector>
#include<string>

#include "TGeantInfo.h"

class TGeantInfoSet {
public:
	TGeantInfoSet();
	~TGeantInfoSet();
private:
	std::vector<TGeantInfo> mGeantSet;
public:
	const std::vector<TGeantInfo>& getGeantSet() const;
	const TGeantInfo& getGeantInfo(const std::string& tag) const;
};

#endif