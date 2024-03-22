#include <filesystem>

#include "SimulDrawing.h"
#include "cppargs.h"

ArgumentParser set_parse(int argc, char** argv) {
    ArgumentParser parser = ArgumentParser(argc,argv).setDescription("Draw plots for simulation");
    parser.add_argument("simulFile").metavar("FILENAME").help("simulation file").add_finish();
    parser.add_argument("--output").help("output path").set_default("default").add_finish();
    parser.parse_args();
    return parser;
}

int main(int argc, char** argv) {
    ArgumentParser parser = set_parse(argc,argv);
    std::filesystem::path input(parser.get_value<std::string>("simulFile"));
    std::filesystem::path path;

    if (parser.get_value<std::string>("output") == "default") {
        path = std::filesystem::absolute(input.parent_path());
    } else {
        path = parser.get_value<std::string>("output");
    }

    std::filesystem::path stem = input.stem();
    std::unique_ptr<TFile> file(new TFile(static_cast<TString>((input.replace_extension(".root")).string())));
    
    path.append(stem.string());
    std::filesystem::create_directory(path);
    
    SimulDrawing drawer(file);
    drawer.refineData();
    std::filesystem::path energyLossFile = "energy_loss_" + stem.string();
    drawer.saveEnergyLossDistribution((path / energyLossFile.replace_extension(".png")).string());
    std::filesystem::path clustermapFile = "clustermap_" + stem.string();
    drawer.saveClustermap((path / clustermapFile.replace_extension(".png")).string());
    std::filesystem::path doubleClusterFile = "Doubled_cluster_frequencymap_" + stem.string();
    drawer.saveDoubleClusterFrequencymap((path / doubleClusterFile.replace_extension(".png")).string());
    return 0;
}