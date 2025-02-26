#include <iostream>
#include <filesystem>
#include <fstream>
#include <numeric>
#include <cmath>

#include "TCanvas.h"
#include "TH2.h"
#include "TStyle.h"
#include "TText.h"
#include "TPaveText.h"
#include "cppargs.h"
#include "alpide_decoder.h"
#include "pitch_clock.h"

ArgumentParser set_parse(int argc, char** argv) {
    ArgumentParser parser = ArgumentParser(argc,argv).setDescription("Draw hitmap and calculate hitrate of an ALPIDE");
    parser.add_argument("rawdata").metavar("FILENAME").help("raw data file to be processed").add_finish();
    parser.add_argument("--bins").add_minor_argument("-b").type("int").add_domain({"1", "2", "4", "8", "16", "32"}).help("bin size").set_default("1").add_finish();
    parser.add_argument("--max").add_minor_argument("-M").type("int").help("color scale limit").add_finish();
    parser.add_argument("--path").help("Output plots path").set_default(".").add_finish();
    parser.add_argument("--dump-raw-hits").help("Dump hit pixel addresses for each event to file").set_default("false").add_finish();
    parser.add_argument("--dump-acc-hits").help("Dump hit pixel addresses sorted by frequency to file").set_default("false").add_finish();
    parser.parse_args();
    return parser;
}

std::ifstream open_file(ArgumentParser parser) {
    std::filesystem::path outfilepath(parser.get_value<std::string>("rawdata"));
    std::string outfilename = outfilepath.stem();

    if (!std::filesystem::exists(outfilepath)) {
        std::cerr << "File not found: " << outfilepath << std::endl;
    }
    std::ifstream outfile(outfilepath,std::ios::binary);
    return outfile;
}

std::vector<uint8_t> read_file(std::ifstream& outfile, int& nev) {
    char buf[sizeof(uint8_t)];
    std::vector<uint8_t> values;
    
    while (outfile.read(buf, sizeof(buf))) {
        values.push_back(0);
        memcpy(&values[nev], buf, sizeof(values[nev]));
        nev++;
    }
    return values;
}

std::vector<ALPIDE::TEvent> decode_event(std::vector<uint8_t>& values) {
    int i = 0;
    std::vector<ALPIDE::TEvent> events;
    ALPIDE::TDecoder* decoder = new ALPIDE::TDecoder();
    while (i < values.size()) {
        events.push_back(decoder->alpide_decode_event(values,i));
    }
    return events;
}

TCanvas* draw_hitmap(std::vector<ALPIDE::TEvent>& events) {
    TCanvas* c1 = new TCanvas("c1","c1",2000,775);
    TH2I* hm = new TH2I("hm","Hitmap; Column; Row",1024,0,1024,512,0,512);
    for(ALPIDE::TEvent event : events) {
        for(std::array<int,2> coord : event.getCoords()) {
            hm->Fill(coord[0],coord[1]);
        }
    }

    hm->Draw("colz");
    hm->GetXaxis()->SetNdivisions(4,4,4,kFALSE);
    hm->GetXaxis()->SetTickLength(-0.035);
    hm->GetXaxis()->SetLabelOffset(0.03);
    hm->GetXaxis()->CenterTitle();
    hm->GetXaxis()->SetTitleOffset(1.4);

    hm->GetYaxis()->SetNdivisions(2,4,4,kFALSE);
    hm->GetYaxis()->SetTickLength(-0.018);
    hm->GetYaxis()->SetLabelOffset(0.017);
    hm->GetYaxis()->CenterTitle();
    hm->GetYaxis()->SetTitleOffset(0.9);

    hm->GetZaxis()->SetMaxDigits(2);
    hm->GetZaxis()->SetTitle("dE/d[Row]d[Column]");
    hm->GetZaxis()->SetTitleOffset(0.6);

    gStyle->SetTitleX(0.38);
    gStyle->SetTitleY(0.98);
    c1->SetTopMargin(0.085);
    c1->SetBottomMargin(0.115);
    c1->SetLeftMargin(0.065);
    c1->SetRightMargin(0.315);

    std::vector<int> hitrate={0};
    for (int i = 0; i < 1024; i++) {
        for (int j = 0; j < 1024; j++) {
            hitrate.push_back(hm->GetBinContent(i,j));
        }
    }

    sort(hitrate.rbegin(),hitrate.rend());
    TPaveText *pt = new TPaveText(1170,0,1515,256);
    double total = (events.size()*1024.*512.);
    pt->AddText("Hit rate    0 masked:");
    TString str = Form("    %.2e per pixel per event",accumulate(hitrate.begin(), hitrate.end(), 0)/total);
    pt->AddText(str);
    pt->AddText("Hit rate   10 masked:");
    str = Form("    %.2e per pixel per event",accumulate(hitrate.begin()+10, hitrate.end(), 0)/total);
    pt->AddText(str);
    pt->AddText("Hit rate  100 masked:");
    str = Form("    %.2e per pixel per event",accumulate(hitrate.begin()+100, hitrate.end(), 0)/total);
    pt->AddText(str);
    pt->AddText("Hit rate 1000 masked:");
    str = Form("    %.2e per pixel per event",accumulate(hitrate.begin()+1000, hitrate.end(), 0)/total);
    pt->AddText(str);
    c1->Update();
    pt->SetTextAlign(12);
    pt->Draw("SAME");

    return c1;
}

void save_canvas(ArgumentParser& parser, TCanvas*& c1) {
    std::filesystem::path outfilepath(parser.get_value<std::string>("rawdata"));
    std::string outfilename = outfilepath.stem();
    std::filesystem::path outputfile = parser.get_value<std::string>("path");
    outputfile.append(outfilename+".pdf");
    std::cout << outputfile << std::endl;
    c1->SaveAs((TString) outputfile);
}

TCanvas* draw_histogram(std::vector<ALPIDE::TEvent>& events) {
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleY(1);
    TCanvas* c2 = new TCanvas("c2","c2",2000,1000);
    c2->Divide(2,1);
    c2->cd(1);
    TH1I* eventHist = new TH1I("h1","eventHist",150,0,events.back().getEventNum()+1);
    std::cout << (long int) events[0].getTime() << std::endl;
    TH1I* timeHist = new TH1I("h2","timeHist",150,events[0].getTime(),events.back().getTime());
    for (ALPIDE::TEvent event : events) {
        for (std::array<int,2> coord : event.getCoords()) {
            eventHist->Fill(event.getEventNum());
            timeHist->Fill(event.getTime());
        }
    }
    eventHist->Draw();
    c2->cd(2);
    timeHist->Draw();

    c2->SaveAs("data/histogram.png");

    return c2;
}
int main(int argc, char** argv) {
    TClock* clock = new TClock();
    std::cout << "ALPIDE Decoding..." << std::endl;
    
    ArgumentParser parser = set_parse(argc,argv);
    
    std::ifstream outfile = open_file(parser);

    int nev = 0;
    std::vector<uint8_t> values = read_file(outfile,nev);

    std::vector<ALPIDE::TEvent> events = decode_event(values);

    TCanvas* c1 = draw_hitmap(events);
    
    save_canvas(parser, c1);

    draw_histogram(events);

    clock->EndProgram();
    return 0;
}
