#include "TClustering.h"

TClustering::TClustering() {}

TClustering::TClustering(TFile* file) : mFile(file) {
    openRootFile();
    storeRawEvents();
}   

TClustering::TClustering(std::unique_ptr<TFile> file) : mFile(std::move(file)) {
    openRootFile();
    storeRawEvents();
}   

TClustering::TClustering(std::vector<std::unique_ptr<TALPIDEEvent>>& events) {
    for (auto& event : events) {
        mEvents.push_back(std::move(event));
    }
}

TClustering::~TClustering() {
    std::cout << "Object for clustering is terminated" << std::endl;
}

void TClustering::refineData() {
    getCluster();
    if (mHitmap == nullptr) mHitmap.reset(new TH2D("hitmap","Hitmap",1024, 0, 1024, 512, 0, 512));
    for(const std::unique_ptr<TALPIDEEvent>& event : mEvents) {
        for(std::array<int,2>& pixel : event->getData()) {
            mHitmap->Fill(pixel[0],pixel[1]);
        }
    }
    if (mClustermap == nullptr) mClustermap.reset(new TH2D("clustermap","clustermap",1024, 0, 1024, 512, 0, 512));
    for(std::unique_ptr<TCluster>& cluster : mClusters) {
        mClustermap->Fill(cluster->getCenter()[0],cluster->getCenter()[1]);
    }
    if (mClusterSize == nullptr) mClusterSize.reset(new TH1D("clusterSize","clusterSize",80,0.5,80.5));
    for(std::unique_ptr<TCluster>& cluster : mClusters) {
        mClusterSize->Fill(cluster->getClusterSize());
    }
    if (mIsMask) {
        masking();
        if (mMaskedHitmap == nullptr) mMaskedHitmap.reset(new TH2D("maskedHitmap","Hitmap",1024, 0, 1024, 512, 0, 512));
        for(const std::unique_ptr<TALPIDEEvent>& maskedEvent : mMaskedEvents) {
            for(std::array<int,2>& pixel : maskedEvent->getData()) {
                mMaskedHitmap->Fill(pixel[0],pixel[1]);
            }
        }
        getCluster(true);
        if (mMaskedClustermap == nullptr) mMaskedClustermap.reset(new TH2D("maskedClustermap","clustermap",1024, 0, 1024, 512, 0, 512));
        for(std::unique_ptr<TCluster>& maskedCluster : mMaskedClusters) {
            mMaskedClustermap->Fill(maskedCluster->getCenter()[0],maskedCluster->getCenter()[1]);
        }
        if (mMaskedClusterSize == nullptr) mMaskedClusterSize.reset(new TH1D("maskedClusterSize","clusterSize",80,0.5,80.5));
        for(std::unique_ptr<TCluster>& MaskedCluster : mMaskedClusters) {
            mMaskedClusterSize->Fill(MaskedCluster->getClusterSize());
        }
    }
}

void TClustering::setMask(int hot) {
    mIsMask = true;
    mMaskOver = hot;
}

void TClustering::masking() {
    if (mHitmap == nullptr) drawHitmap();
    for (int x = 0; x < 1024; x++) {
        for (int y = 0; y < 512; y++) {
            if (mHitmap->GetBinContent(x, y) > mMaskOver) mHotPixels.push_back({x - 1,y - 1});
        }
    }
    for (std::unique_ptr<TALPIDEEvent>& mEvent : mEvents) {
        std::unique_ptr<TALPIDEEvent> tempEvent(new TALPIDEEvent());
        tempEvent->setEvent(mEvent->getEvent());
        tempEvent->setTime(mEvent->getTime());
        for (const std::array<int, 2>& pixel : mEvent->getData()) {
            if (!isHot(pixel)) {
                tempEvent->pushData(pixel);
            }
        }
        mMaskedEvents.push_back(std::move(tempEvent));
    }
}

void TClustering::saveMaskingFile(std::string title) {
    std::ofstream maskFile(title);
    for (const std::array<int,2>& hotPixel : mHotPixels) {
        maskFile << hotPixel[0] << " " << hotPixel[1] << std::endl;
    }
    maskFile.close();
}

void TClustering::saveHitmap(std::string_view title) {
    std::unique_ptr<TCanvas> can(new TCanvas("cmap","Hitmap; rows; columns", 1000, 500));
    mHitmap->Draw();
    can->SaveAs(static_cast<const TString>(title));
}
void TClustering::saveClustermap(std::string_view title) {
    std::unique_ptr<TCanvas> can(new TCanvas("cmap","Clustermap; rows; columns", 1000, 500));
    mClustermap->Draw();
    can->SaveAs(static_cast<const TString>(title));
}
void TClustering::saveClusterSize(std::string_view title) {
    std::unique_ptr<TCanvas> can(new TCanvas("cmap","Cluster Size; Cluster Size; d[Entry] / d[Clsuter Size]",500,500));
    mClusterSize->Draw();
    can->SaveAs(static_cast<const TString>(title));
}
void TClustering::saveMaskedHitmap(std::string_view title) {
    std::unique_ptr<TCanvas> can(new TCanvas("cmap","Cluster map; rows; columns", 1000, 500));
    mMaskedHitmap->Draw();
    can->SaveAs(static_cast<const TString>(title));
}
void TClustering::saveMaskedClustermap(std::string_view title) {
    std::unique_ptr<TCanvas> can(new TCanvas("cmap","Cluster map; rows; columns", 1000, 500));
    mMaskedClustermap->Draw();
    can->SaveAs(static_cast<const TString>(title));
}
void TClustering::saveMaskedClusterSize(std::string_view title) {
    std::unique_ptr<TCanvas> can(new TCanvas("cmap","Cluster map; Cluster Size; d[Entry] / d[Clsuter Size]",500,500));
    mMaskedClusterSize->Draw();
    can->SaveAs(static_cast<const TString>(title));
}


void TClustering::openRootFile() {
    mTree.reset(static_cast<TTree*>(mFile->Get("hit;1")));

    mTree->SetBranchAddress("ChipID", &mInput.chipid);
    mTree->SetBranchAddress("TimeStamp", &mInput.timeStamp);
    mTree->SetBranchAddress("X", &mInput.x);
    mTree->SetBranchAddress("Y", &mInput.y);
}

void TClustering::storeRawEvents() {
    UInt_t preTime = 0;

    std::unique_ptr<TALPIDEEvent> tempALPIDE(new TALPIDEEvent());
    tempALPIDE->setEvent(0);
    tempALPIDE->setTime(0);

    for (int entry = 0; entry < mTree->GetEntries(); entry++) {
        mTree->GetEntry(entry);
        if (mInput.timeStamp != preTime){
            tempALPIDE->removeDuplication();
            tempALPIDE->sortPixel();
            mEvents.push_back(std::move(tempALPIDE));
            // Recording previous event
            tempALPIDE.release();
            tempALPIDE.reset(new TALPIDEEvent());
            tempALPIDE->setEvent(mInput.timeStamp);
            tempALPIDE->setTime(mInput.timeStamp);
            tempALPIDE->pushData({mInput.x,mInput.y});
            preTime = mInput.timeStamp;
        } else {
            tempALPIDE->pushData({mInput.x, mInput.y});
        }
    }
    tempALPIDE = nullptr;
}

void TClustering::removeConsecutionPixel() {
    std::vector<std::array<int,2>> preDeletedPixel;
    std::vector<std::array<int,2>> currentDeletedPixel;
    for (int iEvent = 1; iEvent < mEvents.size(); iEvent++) {
        for (int iPixel = mEvents[iEvent]->getData().size()-1; iPixel > -1; iPixel--) { 
            if(find(mEvents[iEvent-1]->getData().begin(), mEvents[iEvent-1]->getData().end(), mEvents[iEvent]->getData()[iPixel]) != mEvents[iEvent-1]->getData().end() || (!preDeletedPixel.empty() && find(preDeletedPixel.begin(), preDeletedPixel.end(), mEvents[iEvent]->getData()[iPixel]) != preDeletedPixel.end())) {
                currentDeletedPixel.push_back(mEvents[iEvent]->getData()[iPixel]);
                mEvents[iEvent]->getData().erase(mEvents[iEvent]->getData().begin() + iPixel);
            }
        }
        preDeletedPixel.clear();
        preDeletedPixel = std::move(currentDeletedPixel);
        currentDeletedPixel.clear();   
    }
}

bool TClustering::isHot(const std::array<int,2>& pixel) {
    if (find(mHotPixels.begin(),mHotPixels.end(),pixel) == mHotPixels.end()) {
        return false;
    } else {
        return true;
    }
}

void TClustering::obtainClusterFromEvent(const std::unique_ptr<TALPIDEEvent>& event, bool mask) {
    std::vector<std::unique_ptr<TCluster>> suspiciousClusters;
    bool isFirst = true;
    for (const std::array<int,2>& pixel : event->getData()) {
        if (mask && isHot(pixel)) continue;
        if (isFirst) {
            std::unique_ptr<TCluster> cluster(new TCluster(event->getEvent(),event->getTime()));
            cluster->AddPixel(pixel);
            suspiciousClusters.push_back(std::move(cluster));
            isFirst = false;
            continue;
        } // Set first cluster in event and push it to cluster array.

        if (!(suspiciousClusters[0]->getMaxY() + 2 < pixel[1])) {
            int cut = 0;
            for (std::unique_ptr<TCluster>& cluster : suspiciousClusters) {
                if (cluster->getMaxY() + 2 < pixel[1]) {
                    cut++;
                } else break;
            }
            mClusters.insert(mClusters.end(), std::make_move_iterator(suspiciousClusters.begin()), std::make_move_iterator(suspiciousClusters.begin()) + cut);
            suspiciousClusters.erase(suspiciousClusters.begin(), suspiciousClusters.end());
        } // Send the irrelevant clusters with new pixels to storing array.

        bool clusterExist = false;
        int iCluster = 0;
        int iOrigin = 0;
        for (std::unique_ptr<TCluster>& clustered : suspiciousClusters) { // Determine a pixel is belong to one of formed clusters.
            if (clustered->isNeighbour(pixel)) {
                if (clusterExist == false) {
                    clustered->AddPixel(pixel);
                    clusterExist = true;
                    iOrigin = iCluster;
                } else {
                    suspiciousClusters[iOrigin]->AddCluster(*clustered);
                    suspiciousClusters.erase(suspiciousClusters.begin() + iCluster);
                    break;
                }
            }
            iCluster++;
        }

        if (!clusterExist) {
            std::unique_ptr<TCluster> cluster(new TCluster(event->getEvent(),event->getTime()));
            cluster->AddPixel(pixel);
            suspiciousClusters.push_back(std::move(cluster));
        }
    }
    if (!suspiciousClusters.empty()) {
        mClusters.insert(mClusters.end(), std::make_move_iterator(suspiciousClusters.begin()), std::make_move_iterator(suspiciousClusters.end()));
    } // Sen the remain clusters in suspicious to storing array.
}

void TClustering::getCluster(bool mask) {
    if (!mask) {
        for (const std::unique_ptr<TALPIDEEvent>& mEvent : mEvents) {
            obtainClusterFromEvent(mEvent);
        }
    } else {
        for (const std::unique_ptr<TALPIDEEvent>& mMaskedEvent : mMaskedEvents) {
            obtainClusterFromEvent(mMaskedEvent,true);
        }
    }

}

std::unique_ptr<TH2D> TClustering::getHitmap() {
    return std::move(mHitmap);
}

std::unique_ptr<TH2D> TClustering::getClustermap() {
    return std::move(mClustermap);
}

std::unique_ptr<TH1D> TClustering::getClusterSize() {
    return std::move(mClusterSize);
}

std::unique_ptr<TH2D> TClustering::getMaskedHitmap() {
    return std::move(mMaskedHitmap);
}

std::unique_ptr<TH2D> TClustering::getMaskedClustermap() {
    return std::move(mMaskedClustermap);
}

std::unique_ptr<TH1D> TClustering::getMaskedClusterSize() {
    return std::move(mMaskedClusterSize);
}