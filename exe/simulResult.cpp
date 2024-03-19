#include "SimulDrawing.h"

int main() {
    std::unique_ptr<TFile> file(new TFile("data/simulation/1mm_simul.root"));
    SimulDrawing drawer(file);
    drawer.getEnergyLoss();
    drawer.drawEnergyLossHistogram();
    drawer.getDensity();
}