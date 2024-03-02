#include "TAnalyser.h"

TAnalyser::TAnalyser() {
}

void TAnalyser::addChip(const TChip& chip) {
    chips.push_back(chip);
}