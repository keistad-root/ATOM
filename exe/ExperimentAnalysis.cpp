#include <iostream>
#include <fstream>
#include <filesystem>

#include "TEvent.h"
#include "cppconfig.h"
#include "TClustering.h"
#include "cppargs.h"

#include "TCanvas.h"
#include "TFile.h"

ArgumentParser set_parse(int argc, char** argv) {
    ArgumentParser parser = ArgumentParser(argc,argv).setDescription("Draw plots for analysis data");
    parser.add_argument("expFile").metavar("FILENAME").help("simulation file").add_finish();
    parser.add_argument("--output").help("output path").set_default("default").add_finish();
    parser.add_argument("--draw_chip_info").set_default("false").help("Set true if you want to draw each plots and save it. The new directory in which plots are saved has same name with analysis file.").add_finish();
    parser.add_argument("--union").set_default("false").help("Set true if you want to collect each plots seperately. This programe makes several directories for saving plots").add_finish();
    parser.add_argument("--masking").set_default("0").help("Set masking criteria. If it is default or set as 0, then this program doesn't mask any pixels.").add_finish();
    parser.parse_args();
    return parser;
}

int main(int argc, char** argv) {
    ArgumentParser parser = set_parse(argc,argv);

    std::filesystem::path input(parser.get_value<std::string>("expFile"));
    std::filesystem::path path;

    if (parser.get_value<std::string>("output") == "default") {
        path = std::filesystem::absolute(input.parent_path());
    } else {
        path = parser.get_value<std::string>("output");
    }

    std::filesystem::path stem = input.stem();

    std::unique_ptr<TFile> file(new TFile(static_cast<TString>((input.replace_extension(".root")).string()), "READ"));
    
    if (parser.get_value<bool>("draw_chip_info")!=false) {
        std::filesystem::create_directory(path / stem);
    }
    TClustering clusterAna(std::move(file));
    bool masking = false;
    if (parser.get_value<int>("masking") != 0) {
        masking = true;
        clusterAna.setMask(parser.get_value<int>("masking"));
    }
    clusterAna.refineData();

    if (parser.get_value<int>("masking") != 0) {
        if (parser.get_value<bool>("union")) {
            std::filesystem::create_directory(path / "masking");
            clusterAna.saveMaskingFile(path / "masking" / (stem.string() + "_masking.txt"));
        }
        if (parser.get_value<bool>("draw_chip_info")!=false) {
            clusterAna.saveMaskingFile(path / stem / "masking.txt");
        }
    }
    
    if (parser.get_value<bool>("union")) {
        std::filesystem::create_directory(path / "hitmap");
        clusterAna.saveHitmap(static_cast<std::string>(path / "hitmap" / (stem.string() + "_hitmap.png")));
        clusterAna.saveClustermap(static_cast<std::string>(path / "hitmap" / (stem.string() + "_clustermap.png")));
        clusterAna.saveClusterSize(static_cast<std::string>(path / "hitmap" / (stem.string() + "_cluster_size.png")));
        if (masking) {
            clusterAna.saveMaskedHitmap(static_cast<std::string>(path / "hitmap" / (stem.string() + "_masked_hitmap.png")));
            clusterAna.saveMaskedClustermap(static_cast<std::string>(path / "hitmap" / (stem.string() + "_masked_clustermap.png")));
            clusterAna.saveMaskedClusterSize(static_cast<std::string>(path / "hitmap" / (stem.string() + "_masked_cluster_size.png")));
        }
    }

    if (parser.get_value<bool>("draw_chip_info")!=false) {
        clusterAna.saveHitmap(static_cast<std::string>(path / stem / "_hitmap.png"));
        clusterAna.saveClustermap(static_cast<std::string>(path / stem / "_clustermap.png"));
        clusterAna.saveClusterSize(static_cast<std::string>(path / stem / "_cluster_size.png"));
        if (masking) {
            clusterAna.saveMaskedHitmap(static_cast<std::string>(path / stem / "_masked_hitmap.png"));
            clusterAna.saveMaskedClustermap(static_cast<std::string>(path / stem / "_masked_clustermap.png"));
            clusterAna.saveMaskedClusterSize(static_cast<std::string>(path / stem / "_masked_cluster_size.png"));
        }
    }
    return 0;
}