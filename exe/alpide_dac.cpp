#include <iostream>
#include <fstream>
#include <filesystem>

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


int main(int argc, char** argv) {
    ArgumentParser parser = ArgumentParser(argc,argv).setDescription("Show DAC scan results");
    parser.add_argument("filename").help("Name of the file to analysis").add_finish();
    parser.add_argument("--path").help("Output plots path").set_default(".").add_finish();
    parser.add_argument("--no-fit").help("Do fit?").set_default("true").add_finish();
    parser.parse_args();

    std::filesystem::path inputfilepath(parser.get_value<std::string>("filename"));

    if (!std::filesystem::exists(inputfilepath)) {
        std::cerr << "File not found: " << inputfilepath << std::endl;
    }

    std::ifstream inputfile(inputfilepath, std::ios::binary);

    std::string line;

    std::vector<DACtoADC> dactoadc;
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
            DACtoADC* temp = new DACtoADC(old);
            temp->setDAC(dacs);
            temp->setADC(adcs);
            dactoadc.push_back(*temp);
            old = kind;
            delete temp;
            temp = nullptr;
            dacs.clear();
            adcs.clear();
        }
        std::getline(iss, dac, '\t');
        dacs.push_back(stoi(dac));
        std::getline(iss, adc, '\t');
        adcs.push_back(stoi(adc));
    }
    dactoadc.erase(dactoadc.begin());
    inputfile.close();
    int num_of_kind = dactoadc.size();
    TMultiGraph* mg = new TMultiGraph();
    TLegend* legend = new TLegend(0.12,0.65,0.45,0.9);
    legend->SetHeader("DAC scan value","C");
    for (int i = 0; i < num_of_kind; i++) {
        TString canvasName = Form("can%d",i);
        TCanvas* can = new TCanvas(canvasName,canvasName,1200,1000);
        TGraph* graph = new TGraph((dactoadc[i].getADC()).size(),dactoadc[i].getDAC().data(),dactoadc[i].getADC().data());
        TMarker* mark = new TMarker();
        graph->SetTitle("Scan of " + (TString) dactoadc[i].getKind() + " DAC; DAC Setting; ADC");
        
        if (i < 9) {
            graph->SetMarkerStyle(20);
            graph->SetMarkerSize(.5);   
            graph->SetMarkerColor(i+1);
            mark->SetMarkerStyle(20);
            mark->SetMarkerColor(i+1);
        } else {
            graph->SetMarkerStyle(22);
            graph->SetMarkerSize(.5);
            graph->SetMarkerColor(i-7);
            mark->SetMarkerStyle(22);
            mark->SetMarkerColor(i-7);
        }
        mark->SetMarkerSize(2);
        graph->Draw("AP");
        mg->Add(graph);
        legend->AddEntry(mark,(TString) dactoadc[i].getKind(),"p");
        TString output = Form("data/dac_result_%d.pdf",i);
        can->SetLeftMargin(0.12);
        can->SaveAs(output);
        delete can;
        can=nullptr;
    }

    TCanvas* mcan = new TCanvas("mcan","Merge Canvas", 1200,1000);
    mg->SetTitle("Summary plot; DAC Setting; ADC");
    mg->Draw("AP");
    legend->SetNColumns(2);
    legend->Draw();
    mcan->SetLeftMargin(0.12);

    
    mcan->SaveAs("data/dac_total.pdf");
    return 0;
}