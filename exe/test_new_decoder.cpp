#include "TALPIDEEvent.h"
#include "TALPIDEDecoder.h"
#include "TClustering.h"

#include "TCanvas.h"
#include "TH2.h"

int main() {
    std::vector<std::string> dates = {"20240213_143743","20240213_152139","20240213_160526","20240213_164743","20240213_172735","20240213_181120","20240213_185457","20240213_193422","20240213_201854","20240213_210250","20240213_213447","20240213_220648","20240213_220648","20240213_225210","20240213_233637","20240214_002148","20240214_010536","20240214_015050","20240214_023613","20240214_031822","20240214_035828"};
    std::array<std::unique_ptr<TALPIDEDecoder>,2> decoder;
    std::vector<std::unique_ptr<TALPIDEEvent>> correlated;

    std::string date = dates[1];
    // for (const std::string& date : dates) {
    std::filesystem::path inputPath1("data/multi_chip/fhrscan-DAQ-0009010E08931F25-" + date + ".raw");
    std::filesystem::path inputPath2("data/multi_chip/fhrscan-DAQ-0009010E08933825-" + date + ".raw");
    decoder[0].reset(new TALPIDEDecoder(inputPath1));
    decoder[0]->readFile();
    decoder[0]->decode();
    decoder[1].reset(new TALPIDEDecoder(inputPath2));
    decoder[1]->readFile();
    decoder[1]->decode();

    std::vector<std::unique_ptr<TALPIDEEvent>> alpides[2];
    alpides[0] = decoder[0]->getData();
    alpides[1] = decoder[1]->getData();

    for (int iEvent = 0; iEvent < alpides[0].size(); iEvent++) {
        if (!alpides[1][iEvent]->getData().empty()) {
            for (std::unique_ptr<TALPIDEEvent>& data : alpides[0]) {
                correlated.push_back(std::move(data));
            }
        }
    }
    // }

    TClustering* clusterAna = new TClustering(correlated);
    clusterAna->exec();
    clusterAna->drawHitmap();
    TCanvas* c1 = new TCanvas("c1","c1",1800,900);
    TH2D hitmap = *clusterAna->getHitmap();
    hitmap.Draw("LEGO0");
    c1->SaveAs("data/multi_chip/multi_hitmap.pdf");

    clusterAna->getCluster();
    clusterAna->drawClustermap();
    TCanvas* c2 = new TCanvas("c2","c2",1800,900);
    TH2D* clustermap = clusterAna->getClustermap();
    clustermap->Draw("LEGO0");
    c2->SaveAs("data/multi_chip/multi_clustermap.pdf");

    TCanvas* c3 = new TCanvas("c3","c3",1800,1800);
    clusterAna->drawClusterSize();
    TH1D* clusterSize = clusterAna->getClusterSize();
    clusterSize->Draw();
    c3->SetLogy();
    c3->SaveAs("data/multi_chip/multi_cluster_size.pdf");

    delete clusterAna;
}