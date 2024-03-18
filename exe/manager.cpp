#include <iostream>
#include <fstream>

#include "TEvent.h"
#include "cppconfig.h"
#include "TClustering.h"

#include "TCanvas.h"
#include "TFile.h"

int main(int argc, char** argv) {
    TFile* file = new TFile((TString) argv[1]+".root","READ");
    
    TClustering* clusterAna = new TClustering(file);

    clusterAna->exec();
    clusterAna->drawHitmap();
    TCanvas* c1 = new TCanvas("c1","c1",1800,900);
    TH2D hitmap = *clusterAna->getHitmap();
    hitmap.Draw("LEGO0");
    c1->SaveAs((TString) argv[1] + "_hitmap.pdf");

    clusterAna->getCluster();
    clusterAna->drawClustermap();
    TCanvas* c2 = new TCanvas("c2","c2",1800,900);
    TH2D* clustermap = clusterAna->getClustermap();
    clustermap->Draw("LEGO0");
    c2->SaveAs((TString) argv[1] + "_clustermap.pdf");

    TCanvas* c3 = new TCanvas("c3","c3",1800,1800);
    clusterAna->drawClusterSize();
    TH1D* clusterSize = clusterAna->getClusterSize();
    clusterSize->Draw();
    c3->SetLogy();
    c3->SaveAs((TString) argv[1] + "_cluster_size.pdf");

    delete clusterAna;
}