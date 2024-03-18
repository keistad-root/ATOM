#ifndef __TCLUSTERING__
#define __TCLUSTERING__

#include <vector>
#include <sys/ioctl.h>
#include <unistd.h>

#include "TFile.h"
#include "TTree.h"
#include "TH2.h"
#include "TH1.h"

#include "TALPIDEEvent.h"
#include "TCluster.h"
#include "TFileFormat.h"

class TClustering {
private:
    std::unique_ptr<TFile> inFile;
    std::unique_ptr<TTree> inTree;
    TInputRoot input;

    std::vector<std::unique_ptr<TALPIDEEvent>> alpideEvents;
    std::vector<std::unique_ptr<TCluster>> clusteredData;

    TH2D* hitmap = new TH2D("hitmap","Hitmap",1024, 0, 1024, 512, 0, 512);
    TH2D* clustermap = new TH2D("clustermap","clustermap",1024, 0, 1024, 512, 0, 512);
    TH1D* clusterSize = new TH1D("clusterSize","clusterSize",80,0.5,80.5);
public:
    TClustering();
    TClustering(TFile* file);
    TClustering(std::vector<std::unique_ptr<TALPIDEEvent>>& events);
    ~TClustering();

    void exec();
    void getCluster();

    void drawHitmap();
    void drawClustermap();
    void drawClusterSize();
    
    TH2D* getHitmap();
    TH2D* getClustermap();
    TH1D* getClusterSize();
private:
    // In exec function
    void openRootFile();
    void storeRawEvents();
    void removeConsecutionPixel();

    void obtainClusterFromEvent(const std::unique_ptr<TALPIDEEvent>& event);
};

#endif