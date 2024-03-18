#include "TThreshold.h"

TThreshold::TThreshold(std::ifstream& thrFile) {
    std::iota(std::begin(defRegion), std::end(defRegion), 1);
    fitFunc.reset(new TF1("fitFunc","[0]*TMath::Erf((x-[1])/[2])+[3]",0.,50.));
    openThrFile(thrFile);
}

void TThreshold::inputConfig(int vcasn, int ithr) {
    this->vcasn = vcasn;
    this->ithr = ithr;
}

void TThreshold::getThresholds() {
    for (const std::unique_ptr<ThrFormat>& data : datas) {
        calculateThr(data);
    }
}

void TThreshold::openThrFile(std::ifstream& thrFile) {
    std::string line;
    std::string value;
    std::unique_ptr<ThrFormat> data;

    int iLine = 0;
    while (thrFile) {
        getline(thrFile, line);
        std::stringstream values(line);

        int iData = 0;
        if (iLine%50 == 0) {
            data.reset(new ThrFormat());
        }
        while (getline(values, value, ' ')) {
            if (iData == 0) {
                data->y = stoi(value);
            } else if (iData == 1) {
                data->x = stoi(value);
            } else if (iData == 3) {
                data->dac[iLine%50] = stoi(value);
            }
            iData++;
        }
        if (iLine%50 == 49) {
            datas.push_back(std::move(data));
            data.release();
        }
        iLine++;
    }
}

void TThreshold::drawPlot(std::string savePoint) {
    std::unique_ptr<TCanvas> can(new TCanvas("can","threshold",1800,1800));
    std::unique_ptr<TPad> pad1(new TPad("pad1","Threshold",0.,0.,.5,.5));
    std::unique_ptr<TPad> pad2(new TPad("pad2","Error",.5,0.,1.,.5));
    std::unique_ptr<TPad> pad3(new TPad("pad3","Map",0.,.5,1.,1.));
    pad1->Draw();
    pad2->Draw();
    pad3->Draw();

    ThrPlot plot;
    plot.threshold.reset(new TH1D("h1","threshold",51,-0.5,50.5));
    plot.error.reset(new TH1D("h2","error",51,-0.5,50.5));
    plot.map.reset(new TH2D("h3","map",1024,-0.5,1023.5,512,-0.5,511.5));
    for (const std::unique_ptr<ThrFormat>& data : datas) {
        if (data->thr != 0 && data->thr != 50) {
            plot.threshold->Fill(data->thr);
            plot.error->Fill(data->err);
            plot.map->Fill(data->x,data->y,data->thr);
        }
    }
    pad1->cd();
    plot.threshold->Draw();

    pad2->cd();
    plot.error->Draw();

    pad3->cd();
    plot.map->SetMinimum(0);
    plot.map->SetMaximum(50);
    plot.map->Draw("colz");

    can->SaveAs((TString) savePoint);
}

void TThreshold::setVcasn(const int vcasn) {
    this->vcasn = vcasn;
}

void TThreshold::setIthr(const int ithr) {
    this->ithr = ithr;
}

const int TThreshold::getVcasn() const {
    return vcasn;
}

const int TThreshold::getIthr() const {
    return ithr;
}

void TThreshold::calculateThr(const std::unique_ptr<ThrFormat>& data) {
    if (data->dac[0] > 10) {
        std::cerr << "No Threshold in " << data->x << ", " << data->y << std::endl;
        data->thr = 0;
        data->err = 0;
    } else if (data->dac[49] < 45) {
        std::cerr << "Max Threshold in " << data->x << ", " << data->y << std::endl;
        data->thr = 50;
        data->err = 50;
    } else {
        graph.reset(new TGraph(50, defRegion, data->dac));
        fitFunc->SetParLimits(0,20,30);
        fitFunc->SetParLimits(1,5,40);
        fitFunc->SetParLimits(2,2,20);
        fitFunc->SetParLimits(3,20,30);
        graph->Fit("fitFunc","q");
        data->thr = fitFunc->GetParameter(1);
        data->err = fitFunc->GetParameter(2);
    }
}
