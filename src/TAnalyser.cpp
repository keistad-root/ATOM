#include "TAnalyser.h"

TAnalyser::TAnalyser() {
}

void TAnalyser::addChip(const TEvent& chip) {
    chips.push_back(chip);
}