#include "TClusterAnalyser.h"

TClusterAnalyser::TClusterAnalyser(TFile& file) : TAnalyser(file) {
    std::cout << "TClusterAnalyser object is armed." << std::endl;
    mTree = openTree("hit");
    openRootFile();
    storeRawEvents();
}

TClusterAnalyser::TClusterAnalyser(TFile* file) : TAnalyser(file) {
    std::cout << "TClusterAnalyser object is armed." << std::endl;
    mTree = openTree("hit");
    openRootFile();
    storeRawEvents();
}

TClusterAnalyser::~TClusterAnalyser() {
    for ( auto& event : mEvents) {
        delete event;
        event = nullptr;
    }
    for ( auto& event : mMaskedEvents) {
        delete event;
        event = nullptr;
    }
    for ( auto& event : mClusters) {
        delete event;
        event = nullptr;
    }
    for ( auto& event : mMaskedClusters) {
        delete event;
        event = nullptr;
    }
    if (!mHitmap->IsDestructed()) {
        delete mHitmap;
        mHitmap = nullptr;
    }
    if (!mMaskedHitmap->IsDestructed()) {
        delete mMaskedHitmap;
        mMaskedHitmap = nullptr;
    }
    if (!mClustermap->IsDestructed()) {
        delete mClustermap;
        mClustermap = nullptr;
    }
    if (!mMaskedClustermap->IsDestructed()) {
        delete mMaskedClustermap;
        mMaskedClustermap = nullptr;
    }
    if (!mClusterSize->IsDestructed()) {
        delete mClusterSize;
        mClusterSize = nullptr;
    }
    if (!mMaskedClusterSize->IsDestructed()) {
        delete mMaskedClusterSize;
        mMaskedClusterSize = nullptr;
    }
    std::cout << "TClusterAnalyser object is terminated." << std::endl;
}

void TClusterAnalyser::refineData() {
    TClusterization cluster(mEvents); 
    cluster.clusterize();
    mClusters = cluster.getClusters();
    genHitmap();
    genClustermap();
    genClusterSize();

    if (mIsMask) {
        masking();
        genMaskedHitmap();
        TClusterization maskedCluster(mMaskedEvents); 
        maskedCluster.clusterize();
        mClusters = maskedCluster.getClusters();
        genMaskedClustermap();
        genMaskedClusterSize();
    }
}

void TClusterAnalyser::genHitmap() {
    if (mHitmap == nullptr) {
        mHitmap = new TH2D("hitmap", "hitmap", 1024, 0, 1024, 512, 0, 512);
    }
    for (const TALPIDEEvent* event : mEvents) {
        for (const std::array<int, 2>& pixel : event->getData()) {
            mHitmap->Fill(pixel[0], pixel[1]);
        }
    }
}

void TClusterAnalyser::genClustermap() {
    if (mClustermap==nullptr) {
        mClustermap = new TH2D("clustermap", "clustermap", 1024, 0, 1024, 512, 0, 512);
    }
    for (const TCluster* cluster : mClusters) {
        mClustermap -> Fill(cluster->getCenter().first, cluster->getCenter().second);
    }
}

void TClusterAnalyser::genClusterSize() {
    if (mClusterSize==nullptr) {
        mClusterSize = new TH1D("clusterSize", "clusterSize", 80, .5, 80.5);
    }
    for (const TCluster* cluster : mClusters) {
        mClusterSize->Fill(cluster->getSize());
    }
}

void TClusterAnalyser::genMaskedHitmap() {
    if (mMaskedHitmap == nullptr) {
        mMaskedHitmap = new TH2D("maskedHitmap", "maskedHitmap", 1024, 0, 1024, 512, 0, 512);
    }
    for (const TALPIDEEvent* event : mMaskedEvents) {
        for (const std::array<int, 2>& pixel : event->getData()) {
            mMaskedHitmap->Fill(pixel[0], pixel[1]);
        }
    }
}

void TClusterAnalyser::genMaskedClustermap() {
    if (mMaskedClustermap == nullptr) {
        mMaskedClustermap = new TH2D("maskedClustermap", "maskedClustermap", 1024, 0, 1024, 512, 0, 512);
    }
    for (const TCluster* cluster : mMaskedClusters) {
        mMaskedClustermap->Fill(cluster->getCenter().first, cluster->getCenter().second);
    }
}

void TClusterAnalyser::genMaskedClusterSize() {
    if (mMaskedClusterSize == nullptr) {
        mMaskedClusterSize = new TH1D("maskedClusterSize", "maskedClusterSize", 80, .5, 80.5);
    }
    for (const TCluster* cluster : mMaskedClusters) {
        mMaskedClusterSize->Fill(cluster->getSize());
    }
}

void TClusterAnalyser::setMask(int hot) {
    mIsMask = true;
    mMaskOver = hot;
}

void TClusterAnalyser::masking() {
    if (mHitmap == nullptr) {
        genHitmap();
    }

    for (int x = 0; x < 1024; x++) {
        for (int y = 0; y < 512; y++) {
            if (mHitmap->GetBinContent(x, y) > mMaskOver) mHotPixels.push_back({x - 1,y - 1});
        }
    }
    for (const TALPIDEEvent* mEvent : mEvents) {
        TALPIDEEvent* tempEvent = new TALPIDEEvent();
        tempEvent->setEvent(mEvent->getEvent());
        tempEvent->setTime(mEvent->getTime());
        for (const std::array<int, 2>& pixel : mEvent->getData()) {
            if (!isHot(pixel)) {
                tempEvent->pushData(pixel);
            }
        }
        mMaskedEvents.push_back(tempEvent);
        tempEvent = nullptr;
    }
}

void TClusterAnalyser::saveMaskingFile(std::string title) {
    std::ofstream maskFile(title);
    for (const std::array<int,2>& hotPixel : mHotPixels) {
        maskFile << hotPixel[0] << " " << hotPixel[1] << std::endl;
    }
    maskFile.close();
}

void TClusterAnalyser::saveHitmap(std::string_view title) {
    TCanvas can("hitmap","Hitmap; rows; columns", 1000, 500);
    mHitmap->Draw();
    can.SaveAs(static_cast<const TString>(title));
}

void TClusterAnalyser::saveClustermap(std::string_view title) {
    TCanvas can("clustermap","Clustermap; rows; columns", 1000, 500);
    mClustermap->Draw();
    can.SaveAs(static_cast<const TString>(title));
}

void TClusterAnalyser::saveClusterSize(std::string_view title) {
    TCanvas can("clusterSize","Cluster Size; Cluster Size; d[Entry] / d[Clsuter Size]",500,500);
    mClusterSize->Draw();
    can.SaveAs(static_cast<const TString>(title));
}

void TClusterAnalyser::saveMaskedHitmap(std::string_view title) {
    TCanvas can("maskedHitmap","Cluster map; rows; columns", 1000, 500);
    mMaskedHitmap->Draw();
    can.SaveAs(static_cast<const TString>(title));
}

void TClusterAnalyser::saveMaskedClustermap(std::string_view title) {
    TCanvas can("maskedClustermap","Cluster map; rows; columns", 1000, 500);
    mMaskedClustermap->Draw();
    can.SaveAs(static_cast<const TString>(title));
}

void TClusterAnalyser::saveMaskedClusterSize(std::string_view title) {
    TCanvas can("MaskedClusterSize","Cluster map; Cluster Size; d[Entry] / d[Clsuter Size]",500,500);
    mMaskedClusterSize->Draw();
    can.SaveAs(static_cast<const TString>(title));
}

void TClusterAnalyser::writeHitmap(TFile* file, const Configurable& option) {
    file->cd();
    TDirectory* dir = file->mkdir("Hitmaps");
    dir->cd();
    TCanvas can("hitmap","Hitmap; rows; columns", 1000, 500);

    mHitmap->SetTitle(static_cast<TString>(option.find("title")));
    mHitmap->GetXaxis()->SetTitle(static_cast<TString>(option.find("x_title")));
    mHitmap->GetYaxis()->SetTitle(static_cast<TString>(option.find("y_title")));
    mHitmap->Draw("LEGO0");
    mHitmap->Write("hitmap");
    can.Write("LEGOHitmap.png");
    mHitmap->Draw("colz");
    can.Write("COLZHitmap.png");
    dir->Write();
    file->Write();
}

void TClusterAnalyser::writeClustermap(TFile* file, const Configurable& option) {
    file->cd();
    TDirectory* dir = file->mkdir("Clustermaps");
    dir->cd();
    TCanvas* can = new TCanvas("clustermap","Clustermap; rows; columns", 1000, 500);

    mClustermap->SetTitle(static_cast<TString>(option.find("title")));
    mClustermap->GetXaxis()->SetTitle(static_cast<TString>(option.find("x_title")));
    mClustermap->GetYaxis()->SetTitle(static_cast<TString>(option.find("y_title")));
    mClustermap->Draw("LEGO0");
    mClustermap->Write("clustermap");
    can->Write("LEGOClustermap.png");
    mClustermap->Draw("colz");
    can->Write("COLZClsutermap.png");
    dir->Write();
    file->Write();
}

void TClusterAnalyser::writeClusterSize(TFile* file, const Configurable& option) {
    file->cd();
    TDirectory* dir = file->mkdir("Clustersizes");
    dir->cd();
    mClusterSize->SetTitle(static_cast<TString>(option.find("title")));
    mClusterSize->GetXaxis()->SetTitle(static_cast<TString>(option.find("x_title")));
    mClusterSize->GetYaxis()->SetTitle(static_cast<TString>(option.find("y_title")));
    mClusterSize->Write("clsuterSize");
    TCanvas* can = new TCanvas("clusterSize","Clustersize; rows; columns", 1000, 500);
    mClusterSize->Draw();
    can->SetLogy();
    can->Write("LogyClusterSize.png");
    dir->Write();
    file->Write();
}

void TClusterAnalyser::writeMaskedHitmap(TFile* file, const Configurable& option) {
    file->cd();
    TDirectory* dir = file->mkdir("MaskedHitmaps");
    dir->cd();
    TCanvas can(new TCanvas("maskedHitmap","Hitmap; rows; columns", 1000, 500));

    mMaskedHitmap->SetTitle(static_cast<TString>(option.find("title")));
    mMaskedHitmap->GetXaxis()->SetTitle(static_cast<TString>(option.find("x_title")));
    mMaskedHitmap->GetYaxis()->SetTitle(static_cast<TString>(option.find("y_title")));
    mMaskedHitmap->Draw("LEGO0");
    mMaskedHitmap->Write("hitmap");
    can.Write("LEGOHitmap.png");
    mMaskedHitmap->Draw("colz");
    can.Write("COLZHitmap.png");
    dir->Write();
    file->Write();
}

void TClusterAnalyser::writeMaskedClustermap(TFile* file, const Configurable& option) {
    file->cd();
    TDirectory* dir = file->mkdir("MaskedClustermaps");
    dir->cd();
    TCanvas* can = new TCanvas("maskedClustermap","Clustermap; rows; columns", 1000, 500);

    mMaskedClustermap->SetTitle(static_cast<TString>(option.find("title")));
    mMaskedClustermap->GetXaxis()->SetTitle(static_cast<TString>(option.find("x_title")));
    mMaskedClustermap->GetYaxis()->SetTitle(static_cast<TString>(option.find("y_title")));
    mMaskedClustermap->Draw("LEGO0");
    mMaskedClustermap->Write("clustermap");
    can->Write("LEGOClustermap.png");
    mMaskedClustermap->Draw("COLZ L");
    can->Write("COLZClsutermap.png");
    dir->Write();
    file->Write();
}

void TClusterAnalyser::writeMaskedClusterSize(TFile* file, const Configurable& option) {
    file->cd();
    TDirectory* dir = file->mkdir("MaskedClustersizes");
    dir->cd();
    mMaskedClusterSize->SetTitle(static_cast<TString>(option.find("title")));
    mMaskedClusterSize->GetXaxis()->SetTitle(static_cast<TString>(option.find("x_title")));
    mMaskedClusterSize->GetYaxis()->SetTitle(static_cast<TString>(option.find("y_title")));
    mMaskedClusterSize->Write("clsuterSize");
    TCanvas* can = new TCanvas("maskedClusterSize","Clustersize; rows; columns", 1000, 500);
    mMaskedClusterSize->Draw();
    can->SetLogy();
    can->Write("LogyClusterSize.png");
    dir->Write();
    file->Write();
}

void TClusterAnalyser::writeShapes(TFile* file, const Configurable& option) {
    file->cd();
    TDirectory* mainDir = file->mkdir("Shapes");
    mainDir->cd();

    for(int clusterSize = stoi(option.find("min_cut")); clusterSize < stoi(option.find("max_cut")); clusterSize++) {
        if (mClusterSize->GetBinContent(clusterSize)) {
            TClusterN* clusterN = new TClusterN(mClusters, clusterSize);
            clusterN->calShapeSpecies();
            int iShape = 0;
            TString dirName = Form("ClusterShapes(size=%d)", clusterSize);
            TDirectory* dir = mainDir->mkdir(dirName);
            dir->cd();
            for (const auto& clusterWithShape : clusterN->getClustersWithShape()) {
                TString shapeMapName = Form("shapeMap%d", iShape);
                TString shapeMapTitle = Form("Cluster Shape with cluster size %d(#: %d)", iShape, clusterWithShape.second);
                TMatrix2D<int> matrix = clusterWithShape.first->getShape();
                int nRow = matrix.getNRow();
                int nColumn = matrix.getNColumn();
                TH2I shapeMap(shapeMapName, shapeMapTitle, nRow+2, 0, nRow+2, nColumn+2, 0, nColumn+2);
                for (int iRow = 0; iRow < nRow; iRow++) {
                    for (int iColumn = 0; iColumn < nColumn; iColumn++) {
                        if (matrix.getElement(iRow, iColumn) == 1) {
                            shapeMap.Fill(iRow+1, iColumn+1);
                        }
                    }
                }
                TString canvasName = Form("can%d_%d",iShape, clusterSize);
                TString canvasTitle = Form("%dth Cluster Shape(size = %d)", iShape, clusterSize);
                TCanvas can(new TCanvas(canvasName, canvasTitle, 100 * (nRow + 2), 100 * (nColumn + 2)));
                shapeMap.SetStats(0);
                shapeMap.Draw("COLZ L");
                TString fileTitle = Form("/Shape_%d", iShape);
                can.Write(canvasTitle+".png");
                iShape++;
            }
            dir->Write();
            mainDir->cd();
            dir = nullptr;
        }
    }
    file->Write();
    mainDir->SaveAs("/Users/ychoi/source/ATOM/build/data/experiemnt/1phi_air/shape");
}

void TClusterAnalyser::writeShapeSpecification(TFile* file, const Configurable& option) {
    // std::vector<TCluster*> shapes;
    // std::vector<int> iShape;
    // for (const TCluster* cluster : mClusters) {
    //     int i = 0;
    //     for (const TCluster* shape : shapes) {
    //         if (cluster == shape) {
    //             iShape[i]++;
    //         }
    //         i ++;
    //     }
    //     shapes.push_back(cluster);
    //     iShape.emplace_back(1);
    // }
    // for (int i : iShape) {
    //     std::cout << i << " " << std::endl;
    // }
}

void TClusterAnalyser::openRootFile() {
    mTree->SetBranchAddress("ChipID", &mInput.chipid);
    mTree->SetBranchAddress("TimeStamp", &mInput.timeStamp);
    mTree->SetBranchAddress("X", &mInput.x);
    mTree->SetBranchAddress("Y", &mInput.y);
}

void TClusterAnalyser::storeRawEvents() {
    UInt_t preTime = 0;

    TALPIDEEvent* tempALPIDE = new TALPIDEEvent();
    tempALPIDE->setEvent(0);
    tempALPIDE->setTime((long int) 0);
    for (int entry = 0; entry < mTree->GetEntries(); entry++) {
        mTree->GetEntry(entry);
        if (mInput.timeStamp != preTime){
            tempALPIDE->removeDuplication();
            tempALPIDE->sortPixel();
            mEvents.push_back(tempALPIDE);
            tempALPIDE = nullptr;
            // Recording previous event

            tempALPIDE = new TALPIDEEvent();
            tempALPIDE->setEvent(mInput.timeStamp);
            tempALPIDE->setTime(mInput.timeStamp);
            tempALPIDE->pushData({mInput.x, mInput.y});
            preTime = mInput.timeStamp;
        } else {
            tempALPIDE->pushData({mInput.x, mInput.y});
        }
    }
}

bool TClusterAnalyser::isHot(const std::array<int,2>& pixel) {
    if (find(mHotPixels.begin(),mHotPixels.end(),pixel) == mHotPixels.end()) {
        return false;
    } else {
        return true;
    }
}

TH2D* TClusterAnalyser::getHitmap() {
    return mHitmap;
}

TH2D* TClusterAnalyser::getClustermap() {
    return mClustermap;
}

TH1D* TClusterAnalyser::getClusterSize() {
    return mClusterSize;
}

TH2D* TClusterAnalyser::getMaskedHitmap() {
    return mMaskedHitmap;
}

TH2D* TClusterAnalyser::getMaskedClustermap() {
    return mMaskedClustermap;
}

TH1D* TClusterAnalyser::getMaskedClusterSize() {
    return mMaskedClusterSize;
}

void TClusterAnalyser::getShapeOfCS(std::string_view path, const int n) {
    int i = 0;
    for (const TCluster* cluster : mMaskedClusters) {
        if (cluster->getSize() == n) {
            // TH2I shape = *(cluster->getShape());
            // TString canvasName = Form("can%d", i);
            // TString canvasTitle = Form("Cluster Shape %d of %d",i,n);
            // TCanvas can(canvasName, canvasTitle, 500, 500);
            // shape.Draw("COLZ");
            // TString fileTitle = Form("/Shape %d", i);
            // can.SaveAs(std::string(path) + fileTitle + ".png");
            // i++;
        }
    }
}