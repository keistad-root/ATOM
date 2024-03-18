#ifndef __TANALYSER__
#define __TANALYSER__

#include <vector>

#include "TEvent.h"

class TAnalyser {
private:
    std::vector<TEvent> chips;
public:
    TAnalyser();
    void addChip(const TEvent& chip);

};


#endif