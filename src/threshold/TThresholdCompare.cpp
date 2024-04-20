#define __TTHRESHOLDCOMPARE_HEADER__
#include "TThresholdCompare.h"

TThresholdCompare::TThresholdCompare() { }
TThresholdCompare::TThresholdCompare(std::vector<std::unique_ptr<TThreshold*>> thresholds) {
	thresholds_ = std::move(thresholds);
}

