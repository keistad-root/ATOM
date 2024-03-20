#include "SimulDrawing.h"
#include "cppargs.h"

ArgumentParser set_parse(int argc, char** argv) {
    ArgumentParser parser = ArgumentParser(argc,argv).setDescription("Draw plots for simulation");
    parser.add_argument("simulFile").metavar("FILENAME").help("simulation file").add_finish();
    parser.parse_args();
    return parser;
}

int main(int argc, char** argv) {
    ArgumentParser parser = set_parse(argc,argv);
    std::unique_ptr<TFile> file(new TFile((TString) parser.get_value<std::string>("simulFile")));
    SimulDrawing drawer(file);
    drawer.getEnergyLoss();
    drawer.drawEnergyLossHistogram();
    drawer.getDensity();
}