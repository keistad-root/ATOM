#ifndef __TCLUSTERANALYSER__
#define __TCLUSTERANALYSER__

#include "Headers.h"

#include "TALPIDEEvent.h"
#include "TAnalyser.h"
#include "TCluster.h"
#include "TClusterN.h"
#include "TClusterization.h"
#include "TFileFormat.h"
#include "cppconfig.h"

class TClusterAnalyser : public TAnalyser {
private:
    TTree* mTree;
    TInputRoot mInput;

    std::vector<TALPIDEEvent*> mEvents;
    std::vector<TALPIDEEvent*> mMaskedEvents;
    std::vector<TCluster*> mClusters;
    std::vector<TCluster*> mMaskedClusters;
    std::vector<std::array<int,2>> mHotPixels;

    TH2D* mHitmap = nullptr;
    TH2D* mMaskedHitmap = nullptr;
    TH2D* mClustermap = nullptr;
    TH2D* mMaskedClustermap = nullptr;
    TH1D* mClusterSize = nullptr;
    TH1D* mMaskedClusterSize = nullptr;
    
    bool mIsMask = false;
    int mMaskOver = 0;

public:
    //Constructor
    TClusterAnalyser() = delete;
    TClusterAnalyser(TFile& file);
    TClusterAnalyser(TFile* file);
    TClusterAnalyser(const std::vector<TALPIDEEvent> events);
    ~TClusterAnalyser();

    void refineData();
    void setMask(int hot); // The cluster is made with noise-removed pixels, it this function called.

    void genHitmap();
    void genClustermap();
    void genClusterSize();
    void genMaskedHitmap();
    void genMaskedClustermap();
    void genMaskedClusterSize();

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

    void writeHitmap(TFile* file, const Configurable& option);
    void writeClustermap(TFile* file, const Configurable& option);
    void writeClusterSize(TFile* file, const Configurable& option);
    void writeMaskedHitmap(TFile* file, const Configurable& option);
    void writeMaskedClustermap(TFile* file, const Configurable& option);
    void writeMaskedClusterSize(TFile* file, const Configurable& option);
    void writeShapes(TFile* file, const Configurable& option);
    void writeMaskedShapes(TFile* file, const Configurable& option);
    void writeShapeSpecification(TFile* file, const Configurable& option);

    TH2D* getHitmap();
    TH2D* getClustermap();
    TH1D* getClusterSize();
    TH2D* getMaskedHitmap();
    TH2D* getMaskedClustermap();
    TH1D* getMaskedClusterSize();

    void getShapeOfCS(std::string_view path, int n = 0);

private:
    // In exec function
    void openRootFile();
    void storeRawEvents();
    void masking();

    bool isHot(const std::array<int,2>& pixel);
};

#endif