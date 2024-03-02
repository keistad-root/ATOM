#include <iostream>

#include "TChip.h"
#include "cppconfig.h"

int main() {
    std::string file = "data/config/base.conf";
    Configuration conf(file);
    conf.addConfig(conf.getConfig("Detector").find("config_file"));
    TChip* alpide = new TALPIDE(conf.getConfig("ALPIDE_0"));
}