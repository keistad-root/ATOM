#include <iostream>
#include <fstream>
#include <filesystem>

#include "TEvent.h"
#include "cppconfig.h"
#include "TClusterAnalyser.h"
#include "cppargs.h"

#include "TCanvas.h"
#include "TFile.h"

ArgumentParser set_parse(int argc, char** argv) {
    ArgumentParser parser = ArgumentParser(argc,argv).setDescription("Draw plots for analysis data");
    parser.add_argument("config").help("Config file").set_default("default").add_finish();
    parser.parse_args();
    return parser;
}

int main(int argc, char** argv) {
    ArgumentParser parser = set_parse(argc,argv);

    Configuration config(parser.get_value<std::string>("config"));

    std::filesystem::path input(config.getConfig("File").value().find("input_file"));
    std::string extension;
    if (config.getConfig("File").value().find("input_type") == "root") {
        extension = "root";
    }
    TFile* iFile = new TFile(static_cast<TString>(input),"READ");
    
    if (config.getConfig("File").value().find("output_type") == "root") {
        std::filesystem::path output(config.getConfig("File").value().find("output_file"));
        output.replace_extension("root");

        TClusterAnalyser* clusterAna = new TClusterAnalyser(iFile);

        if (config.getConfig("Masking").has_value()) {
            clusterAna->setMask(stoi(config.getConfig("Masking").value().find("cut")));
        }

        clusterAna->refineData();
        
        TFile* oFile = new TFile(static_cast<TString>(output),"RECREATE");
        if (config.getConfig("Hitmap").has_value()) {
            clusterAna->writeHitmap(oFile, config.getConfig("Hitmap").value());
        }
        if (config.getConfig("Clustermap").has_value()) {
            clusterAna->writeClustermap(oFile, config.getConfig("Clustermap").value());
        }
        if (config.getConfig("Clustersize").has_value()) {
            clusterAna->writeClusterSize(oFile, config.getConfig("Clustersize").value());
        }
        if (config.getConfig("Shape").has_value()) {
            clusterAna->writeShapes(oFile, config.getConfig("Shape").value());
        }
        if (config.getConfig("ShapeSpecification").has_value()) {
            clusterAna->writeShapeSpecification(oFile, config.getConfig("ShapeSpecification").value());
        }
        if (config.getConfig("Masking").has_value()) {
            if (config.getConfig("Hitmap").has_value()) {
            clusterAna->writeMaskedHitmap(oFile, config.getConfig("Hitmap").value());
            }
            if (config.getConfig("Clustermap").has_value()) {
            clusterAna->writeMaskedClustermap(oFile, config.getConfig("Clustermap").value());
            }
            if (config.getConfig("Clustersize").has_value()) {
                clusterAna->writeMaskedClusterSize(oFile, config.getConfig("Clustersize").value());
            }
        }
        oFile->Write();

        if (!oFile->IsDestructed()) {
            oFile->Close();
        }
        delete clusterAna;
    }
    if (!iFile->IsDestructed()) {
        iFile->Close();
    }
    
    return 0;
}