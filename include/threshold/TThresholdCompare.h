#ifndef __TTHRESHOLDCOMPARE__
#define __TTHRESHOLDCOMPARE__

#include <vector>
#include "TThreshold.h"

class TThresholdCompare {
private:
    std::vector<std::unique_ptr<TThreshold>> thresholds_;
public:
    TThresholdCompare();
    TThresholdCompare(std::vector<std::unique_ptr<TThreshold>> thresholds);

    
};

#endif