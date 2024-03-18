#include "TClustering.h"

TClustering::TClustering() {}

TClustering::TClustering(TFile* file) : inFile(file) {}

TClustering::TClustering(std::vector<std::unique_ptr<TALPIDEEvent>>& events) {
    for (auto& event : events) {
        alpideEvents.push_back(std::move(event));
    }
}

TClustering::~TClustering() {
    delete hitmap;
}

int getTerminalWidth() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_col;
}

void TClustering::exec() {
    openRootFile();
    storeRawEvents();
    removeConsecutionPixel();
}

void TClustering::openRootFile() {
    inTree.reset(static_cast<TTree*>(inFile->Get("hit;1")));

    inTree->SetBranchAddress("ChipID", &input.chipid);
    inTree->SetBranchAddress("TimeStamp", &input.timeStamp);
    inTree->SetBranchAddress("X", &input.x);
    inTree->SetBranchAddress("Y", &input.y);
}

void TClustering::storeRawEvents() {
    UInt_t preTime = 0;

    std::unique_ptr<TALPIDEEvent> tempALPIDE(new TALPIDEEvent());
    tempALPIDE->setEvent(0);
    tempALPIDE->setTime(0);

    for (int entry = 0; entry < inTree->GetEntries(); entry++) {
        inTree->GetEntry(entry);
        if (input.timeStamp != preTime){
            tempALPIDE->removeDuplication();
            tempALPIDE->sortPixel();
            alpideEvents.push_back(std::move(tempALPIDE));
            // Recording previous event
            tempALPIDE.release();
            tempALPIDE.reset(new TALPIDEEvent());
            tempALPIDE->setEvent(input.timeStamp);
            tempALPIDE->setTime(input.timeStamp);
            tempALPIDE->pushData({input.x,input.y});
            preTime = input.timeStamp;
        } else {
            tempALPIDE->pushData({input.x, input.y});
        }
    }
    tempALPIDE = nullptr;
}

void TClustering::removeConsecutionPixel() {
    std::vector<std::array<int,2>> preDeletedPixel;
    std::vector<std::array<int,2>> currentDeletedPixel;
    for (int iEvent = 1; iEvent < alpideEvents.size(); iEvent++) {
        for (int iPixel = alpideEvents[iEvent]->getData().size()-1; iPixel > -1; iPixel--) { 
            if(find(alpideEvents[iEvent-1]->getData().begin(), alpideEvents[iEvent-1]->getData().end(), alpideEvents[iEvent]->getData()[iPixel]) != alpideEvents[iEvent-1]->getData().end() || (!preDeletedPixel.empty() && find(preDeletedPixel.begin(), preDeletedPixel.end(), alpideEvents[iEvent]->getData()[iPixel]) != preDeletedPixel.end())) {
                currentDeletedPixel.push_back(alpideEvents[iEvent]->getData()[iPixel]);
                alpideEvents[iEvent]->getData().erase(alpideEvents[iEvent]->getData().begin() + iPixel);
            }
        }
        preDeletedPixel.clear();
        preDeletedPixel = std::move(currentDeletedPixel);
        currentDeletedPixel.clear();   
    }
}

void TClustering::obtainClusterFromEvent(const std::unique_ptr<TALPIDEEvent>& event) {
    std::vector<std::unique_ptr<TCluster>> suspiciousClusters;
    for (int nPixel = 0; nPixel < event->getData().size(); nPixel++) {
        if (nPixel == 0) { 
            std::unique_ptr<TCluster> cluster(new TCluster(event->getEvent(),event->getTime()));
            cluster->AddPixel(event->getData()[nPixel]);
            suspiciousClusters.push_back(std::move(cluster));
            continue;
        } // Set first cluster in event and push it to cluster array.

        if (!(suspiciousClusters[0]->getMaxY() + 2 < event->getData()[nPixel][1])) {
            int cut = 0;
            for (std::unique_ptr<TCluster>& cluster : suspiciousClusters) {
                if (cluster->getMaxY() + 2 < event->getData()[nPixel][1]) {
                    cut++;
                } else break;
            }
            clusteredData.insert(clusteredData.end(), std::make_move_iterator(suspiciousClusters.begin()), std::make_move_iterator(suspiciousClusters.begin()) + cut);
            suspiciousClusters.erase(suspiciousClusters.begin(), suspiciousClusters.end());
        } // Send the irrelevant clusters with new pixels to storing array.

        bool clusterExist = false;
        int iCluster = 0;
        int iOrigin = 0;
        for (std::unique_ptr<TCluster>& clustered : suspiciousClusters) { // Determine a pixel is belong to one of formed clusters.
            if (clustered->isNeighbour(event->getData()[nPixel])) {
                if (clusterExist == false) {
                    clustered->AddPixel(event->getData()[nPixel]);
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
            cluster->AddPixel(event->getData()[nPixel]);
            suspiciousClusters.push_back(std::move(cluster));
        }
    }
    if (!suspiciousClusters.empty()) {
        clusteredData.insert(clusteredData.end(), std::make_move_iterator(suspiciousClusters.begin()), std::make_move_iterator(suspiciousClusters.end()));
    } // Sen the remain clusters in suspicious to storing array.
}

void TClustering::getCluster() {
    for (const std::unique_ptr<TALPIDEEvent>& event : alpideEvents) {
        obtainClusterFromEvent(event);
    }
}

void TClustering::drawHitmap() {
    for(const std::unique_ptr<TALPIDEEvent>& event : alpideEvents) {
        for(std::array<int,2>& pixel : event->getData()) {
            hitmap->Fill(pixel[0],pixel[1]);
        }
    }
}

TH2D* TClustering::getHitmap() {
    return hitmap;
}

void TClustering::drawClustermap() {
    for(std::unique_ptr<TCluster>& cluster : clusteredData) {
        clustermap->Fill(cluster->getCenter()[0],cluster->getCenter()[1]);
    }
}

TH2D* TClustering::getClustermap() {
    return clustermap;
}

void TClustering::drawClusterSize() {
    for(std::unique_ptr<TCluster>& cluster : clusteredData) {
        clusterSize->Fill(cluster->getClusterSize());
    }
}

TH1D* TClustering::getClusterSize() {
    return clusterSize;
}