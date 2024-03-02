#ifndef __TANALYSER__
#define __TANALYSER__

#include <vector>

#include "TChip.h"

class TAnalyser {
private:
    std::vector<TChip> chips;
public:
    TAnalyser();
    void addChip(const TChip& chip);

};


#endif