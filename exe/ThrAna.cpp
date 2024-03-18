#include <iostream>
#include <string>

#include "TThreshold.h"

int main(int argc, char** argv) {
    std::ifstream thrFile(argv[1]);

    TThreshold threshold(thrFile);
    threshold.getThresholds();
    threshold.drawPlot(argv[2]);
}