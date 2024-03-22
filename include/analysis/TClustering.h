#ifndef __TCLUSTERING__
#define __TCLUSTERING__

#include <vector>

#include "TALPIDEEvent.h"
#include "TCluster.h"
#include "TFileFormat.h"

class TClustering {
private:
    std::unique_ptr<TFile> mFile;
    std::unique_ptr<TTree> mTree;
    TInputRoot mInput;

    std::vector<std::unique_ptr<TALPIDEEvent>> mEvents;
    std::vector<std::unique_ptr<TALPIDEEvent>> mMaskedEvents;
    std::vector<std::unique_ptr<TCluster>> mClusters;
    std::vector<std::unique_ptr<TCluster>> mMaskedClusters;
    std::vector<std::array<int,2>> mHotPixels;

    std::unique_ptr<TH2D> mHitmap;
    std::unique_ptr<TH2D> mMaskedHitmap;
    std::unique_ptr<TH2D> mClustermap;
    std::unique_ptr<TH2D> mMaskedClustermap;
    std::unique_ptr<TH1D> mClusterSize;
    std::unique_ptr<TH1D> mMaskedClusterSize;
    
    bool mIsMask = false;
    int mMaskOver = 0;
public:
    //Constructor
    TClustering();
    TClustering(TFile* file);
    TClustering(std::unique_ptr<TFile> file);
    TClustering(std::vector<std::unique_ptr<TALPIDEEvent>>& events);
    ~TClustering();

    void refineData();
    void setMask(int hot); // The cluster is made with noise-removed pixels, it this function called.


    void getCluster(bool mask = false);
    void drawHitmap();
    void drawClustermap();
    void drawClusterSize();

    void saveMaskingFile(std::string title);
    void saveHitmap(std::string_view title);
    void saveClustermap(std::string_view title);
    void saveClusterSize(std::string_view title);
    void saveMaskedHitmap(std::string_view title);
    void saveMaskedClustermap(std::string_view title);
    void saveMaskedClusterSize(std::string_view title);

    std::unique_ptr<TH2D> getHitmap();
    std::unique_ptr<TH2D> getClustermap();
    std::unique_ptr<TH1D> getClusterSize();
    std::unique_ptr<TH2D> getMaskedHitmap();
    std::unique_ptr<TH2D> getMaskedClustermap();
    std::unique_ptr<TH1D> getMaskedClusterSize();

private:
    // In exec function
    void openRootFile();
    void storeRawEvents();
    void masking();

    void removeConsecutionPixel();

    void obtainClusterFromEvent(const std::unique_ptr<TALPIDEEvent>& event, bool mask = false);

    bool isHot(const std::array<int,2>& pixel);
};

#endif