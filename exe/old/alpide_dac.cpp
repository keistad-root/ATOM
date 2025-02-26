#include <iostream>
#include <fstream>
#include <filesystem>

#include "TFile.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include "TStyle.h"
#include "TMarker.h"

#include "cppargs.h"

class DACtoADC {
private:
    std::string kind_;
    std::vector<Int_t> dac_;
    std::vector<Int_t> adc_;
public:
    DACtoADC(std::string kind) : kind_(kind) {}
    void setDAC(int dac) {
        dac_.push_back(dac);
    } 
    void setADC(int adc) {
        adc_.push_back(adc);
    }
    void setDAC(std::vector<int> dac) {
        dac_.reserve(dac_.size() + dac.size());
        dac_.insert(dac_.end(), dac.begin(), dac.end());
    } 
    void setADC(std::vector<int> adc) {
        adc_.reserve(adc_.size() + adc.size());
        adc_.insert(adc_.end(), adc.begin(), adc.end());
    }
    std::string getKind() {
        return kind_;
    }
    std::vector<Int_t> getDAC() {
        return dac_;
    }
    std::vector<Int_t> getADC() {
        return adc_;
    }
};

ArgumentParser add_parser(int argc, char** argv) {
    ArgumentParser parser = ArgumentParser(argc,argv).setDescription("Show DAC scan results");
    parser.add_argument("filename").help("Name of the file to analysis").add_finish();
    parser.add_argument("--path").help("Output plots path").set_default(".").add_finish();
    parser.add_argument("--no-fit").help("Do fit?").set_default("true").add_finish();
    parser.parse_args();
    return parser;
}

void file_open(std::ifstream& inputfile, ArgumentParser parser) {
    std::filesystem::path inputfilepath(parser.get_value<std::string>("filename"));
    if (!std::filesystem::exists(inputfilepath)) {
        std::cerr << "File not found: " << inputfilepath << std::endl;
    }
    inputfile = std::ifstream(inputfilepath, std::ios::binary);
}

void input_data(std::vector<DACtoADC>& data, std::ifstream& inputfile) {
    std::string line;

    std::vector<int> dacs;
    std::vector<int> adcs;
    std::string old = "";
    std::string kind;
    std::string dac;
    std::string adc;
    
    while (getline(inputfile,line)) {
        std::istringstream iss(line);
        std::getline(iss, kind, '\t');
        if (old != kind) {
            DACtoADC temp(old);
            temp.setDAC(dacs);
            temp.setADC(adcs);
            data.push_back(temp);
            old = kind;
            dacs.clear();
            adcs.clear();
        }
        std::getline(iss, dac, '\t');
        dacs.push_back(stoi(dac));
        std::getline(iss, adc, '\t');
        adcs.push_back(stoi(adc));
    }
    data.erase(data.begin());
}

void get_graph(std::vector<TGraph>& graphs, const std::vector<DACtoADC>& datas) {
    for (DACtoADC data : datas) {
        graphs.push_back(TGraph((data.getADC()).size(),data.getDAC().data(),data.getADC().data()));
        graphs.back().SetName((TString) data.getKind());
        graphs.back().SetTitle("Scan of " + (TString) data.getKind() + " DAC; DAC Setting; ADC");
    }
    int i = 0;
    for (TGraph& graph : graphs) {
        if (i < 9) {
            graph.SetMarkerStyle(20);
            graph.SetMarkerSize(.5);   
            graph.SetMarkerColor(i+1);
        } else {
            graph.SetMarkerStyle(22);
            graph.SetMarkerSize(.5);
            graph.SetMarkerColor(i-7);
        }
        i++;
    }
}

void get_multi_graph(TMultiGraph& mg, std::vector<TGraph>& graphs) {
    for (TGraph& graph : graphs) {
        mg.Add(&graph);
    }
}

void set_legend(TLegend& legend, std::vector<TGraph>& graphs){
    legend.SetNColumns(2);

    TMarker mark;
    for (TGraph graph : graphs) {
        graph.SetMarkerSize(1.5);
        legend.AddEntry(graph.Clone(),graph.GetName(),"p");
    }
}

void make_root_file(TMultiGraph& mg, std::vector<TGraph>& graphs, TLegend& legend) {
    TFile rootFile("data/value.root","RECREATE");
    rootFile.mkdir("DAC");
    rootFile.cd("DAC");
    mg.SetName("Total");
    mg.SetTitle("Summary plot; DAC Setting; ADC");
    TCanvas mcan("Total","Merge Canvas", 1200,1000);
    mg.Draw("AP");
    legend.Draw();
    mcan.SetLeftMargin(0.12);
    mcan.Write();
    mg.Write();
    for (TGraph& graph : graphs) {
        graph.Write();
    }
    rootFile.Close();
}

int main(int argc, char** argv) {
    ArgumentParser parser = add_parser(argc,argv);

    std::ifstream inputfile;
    file_open(inputfile, parser);

    std::vector<DACtoADC> data;
    input_data(data, inputfile);

    std::vector<TGraph> graphs;
    get_graph(graphs,data);

    inputfile.close();

    TLegend legend(0.12,0.65,0.45,0.9);
    set_legend(legend,graphs);
    
    TMultiGraph mg;
    get_multi_graph(mg,graphs);

    make_root_file(mg, graphs, legend);
    return 0;
}