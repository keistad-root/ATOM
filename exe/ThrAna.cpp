#include <iostream>
#include <string>

#include "TThreshold.h"
#include "cppargs.h"

ArgumentParser set_parse(int argc, char** argv) {
    ArgumentParser parser = ArgumentParser(argc,argv).setDescription("Draw hitmap and calculate hitrate of an ALPIDE");
    parser.add_argument("rawdata").metavar("FILENAME").help("raw data file to be processed").add_finish();
    return parser;
}


int main(int argc, char** argv) {
    std::ifstream thrFile(argv[1]);

    TThreshold threshold(thrFile);
    threshold.getThresholds();
    threshold.drawPlot(argv[2]);
}